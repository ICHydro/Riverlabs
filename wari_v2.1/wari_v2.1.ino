/******************************************
 * wari.ino: Arduino script for the Riverlabs Wari water level sensor
 * Wouter Buytaert
 * 2020/08/02
 *
 * Upload by selecting the board "Arduino Pro or Pro Mini"
 * and processor "Atmega 328 P (3.3V, 8 MHz)".
 *
 * By default, the logger takes 10 repeated distance measurements 
 * at an interval of 5 minutes (starting at the hour).
 * These are written to the internal EEPROM chip,
 * which is flushed to the SD card every 24 hours. 
 * Edit the #define statements below to alter any of these parameters.
 * 
 * (c) Riverlabs UK, except for any code copied from other sources.
 * 
 * Distributed under an MIT licence
 ******************************************/


/******** OPERATING SPECIFICATIONS *******/

#define INTERVAL 1             // Measurement interval in minutes.
#define NREADINGS 10           // number of sensor readings taken per measurement
#define DEBUG 2
#define FLUSHAFTER 288         // Number of readings before EEPROM is flushed to SD = (FLUSHAFTER x INTERVAL) minutes.
#define LOGGERID "RL009998"
//#define FLASH
#define OPTIBOOT

#include "src/Rio.h"                                  // includes everything else

/******** variable declarations **********/

uint32_t readstart = 0;
int16_t readings[NREADINGS];
uint8_t nread;
uint8_t n;
int8_t alarmcount = 0;
uint16_t i, j;
RioLogger myLogger = RioLogger();

// variables needed in interrupt should be of type volatile.

volatile uint16_t interruptCount = 0;
volatile bool interruptFlag = false;

//Clock variables

RtcDS3231<TwoWire> Rtc(Wire);
RtcDateTime now;
uint32_t SecondsSince2000;
uint32_t day = 40;
uint32_t newday;
char datestring[20];
DS3231AlarmFlag flag;

//sd card related variables:

SdFat SD;
SdFile dataFile;
char filename[] = "00000000.CSV";
boolean SDcardOn;
byte keep_SPCR;
uint8_t status;
boolean fileopen = false;

//EEPROM variables

byte EEPromPage[EEPromPageSize]; // byte array to read a page from eeprom;
boolean flusheeprom = false;

// Flash variables
#ifdef FLASH
    uint32_t flashStart;
    SPIFlash flash = SPIFlash(FLASH_CS);
    //byte buffer[FLASHPAGESIZE];
#endif

/* Maxbotix readout from the TTL_ArduinoCode_Parsing example on the Arduino forum */

int EZread() {
  
    int result;
    char inData[6];                             //char array to read data into
    int index = 0;
    unsigned long timer;
    boolean stringComplete = false;

    MBSerial.flush();                           // Clear cache ready for next reading
    timer = millis();                           // use timer to time out after 1 sec.

    while ((stringComplete == false) && ((millis() - timer) < 1000)) {
        if (MBSerial.available()) {    
            char rByte = MBSerial.read();       // read serial input for "R" to mark start of data
            //Serial.print(rByte);
            if(rByte == 'R') {
                while (index < 5) {             // read next 4 characters for range from sensor
                    if (MBSerial.available()) {
                        inData[index] = MBSerial.read();               
                        index++;                // Increment where to write next
                    }
                }
                inData[index] = 0x00;           // add a padding byte at end for atoi() function
            }
            rByte = 0;                          // reset the rByte ready for next reading
            index = 0;                          // Reset index ready for next reading
            stringComplete = true;              // Set completion of read to true
            result = atoi(inData);              // Changes string data into an integer for use
        }
    }
    if(stringComplete == false) result = -2;
    return result;
}

//************* setup *************//

void setup() 
{ 
    // reset readings    
            
    for (int i=0; i < NREADINGS; i++){
        readings[i] = -1;
    }
  
    // set the pins
    pinMode(INTERRUPTPIN, INPUT);
    pinMode(MBONPIN, OUTPUT);
    pinMode(ErrorLED, OUTPUT);
    pinMode(WriteLED, OUTPUT);
    digitalWrite(WriteLED, LOW);
    digitalWrite(ErrorLED, LOW);
    digitalWrite(MBONPIN, LOW);            // Low side switching but through mosfet

    pinMode(FLASHPOWERPIN, OUTPUT);
    digitalWrite(FLASHPOWERPIN, LOW);      // start in off mode
    pinMode(SD_CS_PIN, OUTPUT);
    digitalWrite(SD_CS_PIN, HIGH);         // high = deactivated. Needed to avoid interference with flash comms
    pinMode(FLASH_CS, INPUT_PULLUP);
    pinMode(SDpowerPin, OUTPUT);
    digitalWrite(SDpowerPin, LOW);         // start in off mode

    pinMode(Boost5V_on, OUTPUT);
    pinMode(SWITCH5V, OUTPUT);
    digitalWrite(Boost5V_on, LOW);
    digitalWrite(SWITCH5V, LOW);
  
    // start wire for the EEPROM
    Wire.begin();
  
    // start clock. Note: clock initialization is done in separate script!
  
    Rtc.Begin();
    now = Rtc.GetDateTime();
      
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth); 
  
    // Alarm 2 set to trigger at the top of the minute
    // (Using alarm 2 with minute resolution - not sure why this is different from alarm 1)
    DS3231AlarmTwo alarm2(0,
                          0,
                          0, 
                          DS3231AlarmTwoControl_OncePerMinute);
    Rtc.SetAlarmTwo(alarm2);
  
    // throw away any old alarm state
    Rtc.LatchAlarmsTriggeredFlags();
  
    // setup external interupt 
    attachInterrupt(digitalPinToInterrupt(INTERRUPTPIN), InterruptServiceRoutine, FALLING);
    
    #ifdef DEBUG
        Serial.begin(115200);
        Serial.print(F("This is Riverlabs Wari v2.1"));
        #ifdef OPTIBOOT
            Serial.print(F(" (optiboot)"));
        #endif
        Serial.print(F(", compiled on "));
        Serial.println(__DATE__); 
        Serial.print(F("Current time is "));
        formatDateTime(now);
        Serial.print(datestring);
        Serial.println(F(" GMT"));
        Serial.print(F("Flushing EEPROM... "));
    #endif

    digitalWrite(WriteLED, HIGH);

    #ifdef FLASH
        pinMode(FLASH_CS, OUTPUT);
        digitalWrite(FLASHPOWERPIN, HIGH);           // switch flash on;
        delay(100);                                  // no need for turnonSPI(). Already on.
        flashStart = getFlashStart();
        #if DEBUG > 0
            Serial.print(F("Flash memory starting at position: "));
            Serial.println(flashStart);
        #endif
        pinMode(FLASH_CS, INPUT_PULLUP);
        digitalWrite(FLASHPOWERPIN, LOW);
    #endif

    // enable watchdog timer. Set at 8 seconds

    #ifdef OPTIBOOT
        wdt_enable(WDTO_8S);
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
        #endif
        error(3, ErrorLED);
    }

    turnOffSDcard();
}

//******** main routine **********//

void loop() {

    #ifdef OPTIBOOT
        wdt_reset();                                                           // Reset the watchdog every cycle
    #endif
  
    // sleep until an interrupt happens, except if the alarm went off while still doing something else...
    // (the alarm goes off every minute, so that is quite likely!)

    if(!interruptFlag) {
    
        #ifdef DEBUG
            Serial.println(F("Sleeping"));
            Serial.flush();
        #endif

        #ifdef OPTIBOOT
            wdt_disable();
        #endif

        LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 

        /* if interrupt wakes us up, then we take action: */

        #ifdef OPTIBOOT
            // enable watchdog timer. Set at 8 seconds 
            wdt_enable(WDTO_8S);
        #endif

        #ifdef DEBUG
            Serial.println(F("Waking up!"));
        #endif
    }

    if(interruptFlag) {                            // should always be true
      
        interruptFlag = false;                     // reset the flag

        flag = Rtc.LatchAlarmsTriggeredFlags();    // Switch off the alarm

        now = Rtc.GetDateTime();                   // get the current time from the clock

        if(now.Minute() % INTERVAL == 0) {         // only take measurement if interval threshold is exceeded:         
        
            /************ take a reading **********/
      
            nread = 0;
            
            Serial.end();

            MBSerial.begin(9600);

            digitalWrite(WriteLED, HIGH);
            digitalWrite(MBONPIN, HIGH);
            LowPower.powerDown(SLEEP_250MS, ADC_OFF, BOD_OFF);     // for Li-SOCl batteries to minimize power peak. 
            //delay(160);   // wait 160ms for startup and boot message to pass
            digitalWrite(WriteLED, LOW);

            // The watchdog timer seems to be disabled after period sleep, perhaps because powerDown() needs the timer?
            // In any case, reenabling it should not hurt
            #ifdef OPTIBOOT
                wdt_enable(WDTO_8S);
            #endif

            readstart = millis();
         
            while ((millis() - readstart) <= 1800) {
                readings[nread] = EZread();
                if(readings[nread] > -2) {                   // Returning -2 indicates an error
                    nread++;
                    if(nread >= NREADINGS) { nread = 0; }             //avoid overflow
                }
            }

            digitalWrite(MBONPIN, LOW);
            MBSerial.end();

            Serial.begin(115200);

            int16_t measuredvbat = analogRead(VBATPIN) * 2 * 3.3 / 1.024;
            int16_t temp = Rtc.GetTemperature().AsCentiDegC(); // take the temperature

            #ifdef DEBUG
                formatDateTime(now);
                Serial.print(datestring);
                Serial.print(",");
                n = 0;
                while(n < 10) {        
                    Serial.print(readings[n]);
                    Serial.print(F(", "));
                    n++;
                }
                Serial.println(((float) measuredvbat)/1000);
            #endif


            /*********** store values in EEPROM ***********/

            SecondsSince2000 = now.TotalSeconds();

            // prepare EEPromPage

            for(i = 0; i < 4; i++){
                EEPromPage[i] = ((byte *)&SecondsSince2000)[i];
            }
            for(i = 0; i < 20; i++) {
                EEPromPage[i+4] = ((byte *)readings)[i];
            }
            for(i = 0; i < 2; i++){
                EEPromPage[i+24] = ((byte *)&measuredvbat)[i];
            }
            for(i = 0; i < 2; i++){
                EEPromPage[i+26] = ((byte *)&temp)[i];
            }
            for(i = 28; i < EEPromPageSize; i++){    // fill rest of page with zeros
                EEPromPage[i] = 0;
            }

            // Write page. Note: only write 30 bits because the last 2 bits seem to be used by Wire library
                
            myLogger.write2EEPROM(EEPromPage, sizeof(EEPromPage));

            /*********** store values in FLASH ***********/

            #ifdef FLASH

                digitalWrite(FLASHPOWERPIN, HIGH);
                pinMode(FLASH_CS, OUTPUT);
                turnOnSPI();

                for(uint8_t i = 0; i < 30; i++) {
                    Serial.print(EEPromPage[i], HEX);
                    Serial.print(" ");
                }
                Serial.println();
                write2Flash(EEPromPage, 30, flashStart++);
                pinMode(FLASH_CS, INPUT_PULLUP);
                keep_SPCR=SPCR;
                turnOffSPI();
                digitalWrite(FLASHPOWERPIN, LOW);
            #endif

            /******** reset readings *****/    
          
            for (i = 0; i < NREADINGS; i++){
                readings[i] = -1;
            }

            /********* flush EEPROM to SD card when full **********/
            
            if(myLogger.eePageAddress >= FLUSHAFTER) {
              flusheeprom = true;
            }

            if(flusheeprom) {

                turnOnSDcard();

                if(dumpEEPROM2()) {
                    resetEEPromHeader(EEPROM_ADDR);
                    myLogger.eePageAddress = 0;
                    flusheeprom = false;
                }

                turnOffSDcard();

            }

            // avoid memory overflow - redundant because already done in RioLogger:write2EEPROM()

            if(myLogger.eePageAddress > (MAXPAGENUMBER - EEPromHeaderSize)) {
                myLogger.eePageAddress = 0;
            }
        }                                     // end of threshold if
    }                                         // end interruptflag
}                                             // end loop()
