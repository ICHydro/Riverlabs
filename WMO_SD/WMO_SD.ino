/**************************************
 * Arduino code for the Riverlabs sensor node with following functionality:
 * - LidarLite sensor
 * - Digi Cellular Xbee 3G transmission
 * - Data buffering in EEPROM
 * 
 * (c) Riverlabs UK except where indicated
 * 
 * Notes:
 * - By default, debug messages are printed to the Serial terminal.
 *   To change the level of debug output, change the following line in line Rio.h:
 *   # define DEBUG 2
 *   The following levels can be used:
 *     0: no output
 *     1: sparse output
 *     2: full output
 */

/************* User settings **************/

#define READ_INTERVAL 5                          // Interval for sensor readings, in minutes
#define FLUSHAFTER 288                            // Number of readings before EEPROM is flushed to SD = (FLUSHAFTER x INTERVAL) minutes.
#define NREADINGS 9                               // number of readings taken per measurement (excluding 0 values)
#define LOGGERID "RL000393"                               // Logger ID. Set to whatever you like
#define FLASH                                     // write to flash chip

/* INCLUDES */

#include "src/Rio.h"                                  // includes everything else

/********** variable declarations **********/

uint32_t readstart = 0;
int16_t readings[NREADINGS];
uint8_t nread;
uint8_t n;
bool TakeMeasurement = 0;
uint16_t i, j;
int16_t distance = -9999;

volatile bool interruptFlag = false;              // variables needed in interrupt should be of type volatile.

//Clock stuff

RtcDS3231<TwoWire> Rtc(Wire);
RtcDateTime now;
uint32_t SecondsSince2000;
uint32_t day = 40;
uint32_t newday;
char datestring[20];
DS3231AlarmFlag flag;

// other variables

uint16_t arraycounter = 0;
uint16_t DeltaT;
uint16_t measuredvbat;
int16_t temp;
uint8_t MaxContentLength;
bool timeout = false;

RioLogger myLogger = RioLogger();

//EEPROM variables

byte EEPromPage[(EEPromPageSize > 30) ? 30 : EEPromPageSize]; 
uint16_t eeaddress = 0;                           // page address, starts after header
boolean flusheeprom = false;

// SD card variables

SdFat SD;
SdFile dataFile;
char filename[] = "00000000.CSV";
boolean SDcardOn;
byte keep_SPCR;
uint8_t status;
boolean fileopen = false;

// Flash variables

uint32_t flashStart;
SPIFlash flash = SPIFlash(FLASH_CS);
byte buffer[FLASHPAGESIZE];

/*************** setup ***************/

void setup () {
    
    #if DEBUG > 0
        Serial.begin(115200);
    #endif

    /* set the pins */
    
    pinMode(WriteLED, OUTPUT);
    pinMode(ErrorLED, OUTPUT);
    digitalWrite(WriteLED, LOW);
    digitalWrite(ErrorLED, LOW);

    pinMode(VBATPIN, INPUT);

    pinMode(FLASHPOWERPIN, OUTPUT);
    pinMode(SDpowerPin, OUTPUT);

    #ifdef Boost5V_on
        pinMode(Boost5V_on, OUTPUT);
        pinMode(SWITCH5V, OUTPUT);
        digitalWrite(Boost5V_on, LOW);
        digitalWrite(SWITCH5V, LOW);
    #endif

    //RH_RF95 radio(A1, 9);
    //if (!radio.init()){}
    //radio.sleep();

//    #ifdef XBEE_SLEEPPIN
//        pinMode(XBEE_SLEEPPIN, INPUT);   // do not set high but keep floating
//    #endif


    /* Start clock */
    
    Rtc.Begin();    
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmTwo); 

    // Alarm 2 set to trigger at the top of the minute
    
    DS3231AlarmTwo alarm2(0, 0, 0, DS3231AlarmTwoControl_OncePerMinute);
    Rtc.SetAlarmTwo(alarm2);

    // alarm 1 set for transmission

    // TODO

    // throw away any old alarm state
    Rtc.LatchAlarmsTriggeredFlags();

    now = Rtc.GetDateTime();

    #ifdef DEBUG > 0
        Serial.println("");
        Serial.print(F("This is Riverlabs WMOnode, compiled on "));
        Serial.println(__DATE__);
        Serial.print(F("Logger ID: "));
        Serial.println(LOGGERID);
        Serial.print(F("Current time is "));
        formatDateTime(now);
        Serial.print(datestring);
        Serial.println(F(" GMT"));
        Serial.println(F("Measuring the following variables:"));
        Serial.println(F("- Distance (Lidarlite sensor)"));
        Serial.print(F("Measurement interval (minutes): "));
        Serial.println(READ_INTERVAL);
    #endif

    /* set interrupts */

    pinMode(INTERRUPTPIN, INPUT);
    attachInterrupt(interruptNo, InterruptServiceRoutine, FALLING);

    // Start wire for i2c communication (EEPROM) (note: this does not seem necessary for atmel, but it is for SAMD21)

    Wire.begin();

    #ifdef DEBUG
        Serial.println(F("Flushing EEPROM. This will also test SD card"));
    #endif

    turnOnSDcard();

    if(dumpEEPROM2()) {
        resetEEPromHeader(EEPROM_ADDR);
        #ifdef DEBUG
            Serial.println(F("EEPROM flushed."));
        #endif
    } else {
        #ifdef DEBUG
            Serial.println(F("Failed to flush EEPROM. SD card missing? Continuing anyway."));
            error(3, ErrorLED);
        #endif     
    }

    #ifdef FLASH
        flashStart = getFlashStart();
        #if DEBUG > 0
            Serial.print(F("Flash memory starting at position: "));
            Serial.println(flashStart);
        #endif
    #endif

    turnOffSPI();

    #ifdef DEBUG
        Serial.println(F("Powering off SD card and Flash chip"));
    #endif

    keep_SPCR=SPCR;
    pinMode(FLASH_CS, INPUT_PULLUP);
    turnOffSDcard();

}

/*************** Main routine ***************/

void loop () 
{

    /* At the start of the loop, the logger can be in the following states:
     *  
     *  - Alarm has gone off while doing something else -> check what action to take
     *  - Logger was waken up by clock                  -> check what action to take
     *  - Telemetry event ongoing. or timeout           -> continue telemetry operation.
     *  If none of the above applies, the logger goes to sleep
     */

    if(interruptFlag) {

        cli();                                                              // See https://www.pjrc.com/teensy/interrupts.html
        interruptFlag = false;
        sei();

        // Only switch the alarm off *after* we have set the interruptFlag to zero.
        // This is to avoid that we end up in a state where the alarm may go off before
        // the interrupflag is set to false, in which case the alarm is never switched off and
        // the logger never wakes up again.
        
        flag = Rtc.LatchAlarmsTriggeredFlags();                             // Switches off alarm
        now = Rtc.GetDateTime();

        // Check whether it is time for a measurement

        if((now.Minute() % READ_INTERVAL) == 0) {
            TakeMeasurement = true;
        }

    }

    // if nothing needs to be done, then we can safely sleep until the next alarm.
    // the timeout variable allows sleeping briefly between telemetry attemps
    // (XBee stays awake)
    
    if(!TakeMeasurement) {

        #ifdef NOSLEEP
            while(!interruptFlag) {}                                        // wait for alarm if not sleeping
        #else 
            #ifdef DEBUG > 0
                Serial.print(F("S"));
                Serial.flush();
            #endif
            #if defined(__MKL26Z64__)
                Snooze.hibernate( config_digital );
            #endif
            #if defined(__AVR_ATmega328P__)
                if(!interruptFlag) {                                        // check again, just in case alarm went off right before sleeping
                    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);    // SLEEP_FOREVER
                }
            #endif
        #endif

        // when woken up:

        #ifdef DEBUG > 0
            Serial.print(F("W"));
            Serial.flush();
        #endif

    }
         
    /* if it is time for a measurement then do so */

    if(TakeMeasurement) {

        TakeMeasurement = false;

        measuredvbat = analogRead(VBATPIN) * 2 * 3.3 / 1.024;     // Battery voltage
        temp = Rtc.GetTemperature().AsCentiDegC();                // Clock temperature
        readLidarLite(readings, NREADINGS, DEBUG, Serial);            // Lidar
        distance = median(readings, NREADINGS);

        #ifdef DEBUG > 0
            formatDateTime(now);
            Serial.println(datestring);
            Serial.print(F("VBatt = "));
            Serial.println(measuredvbat);
            Serial.print(F("T = "));
            Serial.println(temp);
            Serial.print(F("Distance (lidar) = "));
            Serial.println(distance);
        #endif

        /*********** store values in EEPROM ***********/
        
        SecondsSince2000 = now.TotalSeconds();

        // prepare EEPromPage. Note that we use a 16 byte page here

        i = 0;

        EEPromPage[i++] = SecondsSince2000;
        EEPromPage[i++] = SecondsSince2000 >> 8;
        EEPromPage[i++] = SecondsSince2000 >> 16;
        EEPromPage[i++] = SecondsSince2000 >> 24; 
                  
        for(j = 0; j < 2; j++){
            EEPromPage[i++] = ((byte *)&measuredvbat)[j];
        }
        for(j = 0; j < 2; j++){
            EEPromPage[i++] = ((byte *)&temp)[j];
        }
        EEPromPage[i++] = distance;
        EEPromPage[i++] = distance >> 8;

        // plenty of space for more payload here!

        while (i < sizeof(EEPromPage)){    // fill rest of page with zeros
            EEPromPage[i++] = 0;
        }

        myLogger.write2EEPROM(EEPromPage, sizeof(EEPromPage));

        /*********** store values in FLASH ***********/

        #ifdef FLASH

            pinMode(FLASH_CS, OUTPUT);

            turnOnSDcard();

            #ifdef FLASH
                write2Flash(EEPromPage, sizeof(EEPromPage), flashStart++);
            #endif

            keep_SPCR=SPCR;
            pinMode(FLASH_CS, INPUT_PULLUP);

            turnOffSDcard();
        #endif

        /******** reset readings *****/    
          
        for (i = 0; i < NREADINGS; i++){
            readings[i] = -1;
        }

        Serial.println(eeaddress);


        /********* flush EEPROM to SD card when full **********/
            
        if(myLogger.eePageAddress >= FLUSHAFTER) {
          flusheeprom = true;
        }

        if(flusheeprom) {

            //digitalWrite(SDpowerPin, HIGH);
            //delay(6);                      // let FLASH power settle
            //turnOnSPI();
            turnOnSDcard();

            if(dumpEEPROM2()) {
                resetEEPromHeader(EEPROM_ADDR);
                myLogger.eePageAddress = 0;
                flusheeprom = false;
            }

            turnOffSDcard();

            //turnOffSPI();
            //digitalWrite(SDpowerPin, LOW);

        }

        // avoid memory overflow - just cycle memory.
        // NOTE: redundant: already part of the write2EEPROM function.

        if(myLogger.eePageAddress > (maxpagenumber - EEPromHeaderSize)) {
            myLogger.eePageAddress = 0;
        }
    }
}
