/**************************************
 * Arduino code for the Riverlabs sensor node with following functionality:"
 * - LidarLite sensor
 * - Digi Cellular Xbee 3G transmission
 * - Data buffering in EEPROM
 * 
 * (c) Riverlabs UK except where indicated
 */

/********** includes and defines **********/


#define DEBUG 1                                    // needs to be defined in Rio.h as well.
#define LIDARLITE

#define USE_EEPROM_BUFFER
#define TRANSMIT_3G
#define COAP

#define READ_INTERVAL 5                           // read interval, in minutes
#define SEND_INTERVAL 1                           // telemetry interval, in hours
#define NREADINGS 10                              // number of readings taken per measurement


/* INCLUDES */

#include "Rio.h"                                  // includes everything else

/********** variable declarations **********/

const char LoggerID[] = "MyLogger1";              // unique logger ID, to be used for data transmission and writing files.

uint32_t readstart = 0;
int16_t readings[NREADINGS];
uint8_t nread;
uint8_t n;
int8_t alarmcount = 0;
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

// Cellular xbee variables

CellularStatus seqStatus;

#ifdef TRANSMIT_3G
    
    AltSoftSerial XBeeSerial;
    uint8_t resb[100];                            // XBee's responsebuffer
    XBeeWithCallbacks xbc(resb, sizeof(resb)); 
    const char host[] = "demo.thingsboard.io";         // set to your COAP server
    uint32_t IP = 0;
    const uint16_t Port = 0x1633;                 // 0x50 = 80; 0x1BB = 443, 0x1633 = 5683 (COAP)
    uint8_t protocol = 0;                         // 0 for UDP, 1 for TCP, 4 for SSL over TCP
    uint16_t bufferSize;
    uint32_t XbeeWakeUpTime;
    uint32_t timeInMillis = 0;
    uint32_t lastTimeInMillis = 0;
    uint32_t waitingMessageTime = 0;
    uint32_t startposition = 0;
    uint8_t AIstatus;
    uint8_t pagecount;
    char token[] = "test";                         // to be replaced by loggerID
    char MsgLength = 0;
    char Option0[] = "api";
    char Option1[] = "v1";
    char Option2[] = "A1_TEST_TOKEN";
    char Option3[] = "telemetry";
    CoapPacket packet; 
    bool EepromBufferCreated = 0;
    byte Eeprom3Gmask[2 + MAXFIT / 8];             
#endif


// other variables

uint16_t arraycounter = 0;
uint16_t DeltaT;
uint16_t measuredvbat;
int16_t temp;
uint8_t MaxContentLength;

RioLogger myLogger = RioLogger();

#ifdef DS18S20
    OneWire oneWire(DS18S20PIN);
    DallasTemperature sensors(&oneWire);
    DeviceAddress tempDeviceAddress; 
#endif

//sd card related stuff:
#if defined(WRITE_SD_DIRECT) || defined(WRITE_EEPROM_TO_SD)
    SdFat SD;
    SdFile dataFile;
    char filename[] = "00000000.CSV";
    boolean SDcardOn;
    byte keep_SPCR;
    uint8_t status;
    boolean fileopen = false;
#endif

//EEPROM stuff
#ifdef USE_EEPROM_BUFFER
    byte EEPromPage[(EEPromPageSize > 30) ? 30 : EEPromPageSize]; 
    boolean flusheeprom = false;
#endif

// internal EEPROM is used to create the telemetry buffer
uint16_t EEPROM_payload_start = 0;

/*************** setup ***************/

void setup () 
{
    
    #if DEBUG == 1
        Serial.begin(115200);
    #endif

    /* set the pins */
    
    pinMode(WriteLED, OUTPUT);
    pinMode(ErrorLED, OUTPUT);
    digitalWrite(WriteLED, LOW);
    digitalWrite(ErrorLED, LOW);

    pinMode(VBATPIN, INPUT);

    #ifdef Boost5V_on
        pinMode(Boost5V_on, OUTPUT);    // do them outside the ifdef in case ultrasound logger has the hardware
        pinMode(SWITCH5V, OUTPUT);
        digitalWrite(Boost5V_on, LOW);
        digitalWrite(SWITCH5V, LOW);
    #endif  

    #ifdef FLASH_SPI_PIN
        pinMode(FLASH_SPI_PIN, OUTPUT);
        digitalWrite(FLASH_SPI_PIN, LOW);
    #endif

    #ifdef XBEE_SLEEPPIN
        pinMode(XBEE_SLEEPPIN, OUTPUT);
        digitalWrite(XBEE_SLEEPPIN, HIGH);
    #endif
    
    #if defined(__MKL26Z64__)
        pinMode(17, OUTPUT);   // needed? May reduce power consumption during hibernate for TeensyLC
    #endif

    #ifdef LIDARLITE
        //myLogger.HasLidar = 1;
        pinMode(LIDARONPIN, OUTPUT);
        digitalWrite(LIDARONPIN, LOW);
    #endif
 

    /* Start clock */
    
    Rtc.Begin();    
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth); 

    // Alarm 2 set to trigger at the top of the minute
    
    DS3231AlarmTwo alarm2(0, 0, 0, DS3231AlarmTwoControl_OncePerMinute);
    Rtc.SetAlarmTwo(alarm2);

    // alarm 1 set for transmission

    // TODO

    // throw away any old alarm state
    Rtc.LatchAlarmsTriggeredFlags();

    now = Rtc.GetDateTime();
    day = now.Day();

    #ifdef DEBUG
        Serial.println("");
        Serial.print(F("This is Rio node_v1, compiled on "));
        Serial.println(__DATE__);
        Serial.print(F("Logger ID: "));
        Serial.println(LoggerID);
        Serial.print(F("Current time is "));
        formatDateTime(now);
        Serial.print(datestring);
        Serial.println(F(" GMT"));
        Serial.println(F("Measuring the following variables:"));
        #ifdef LIDARLITE
            Serial.println(F("- Distance (Lidarlite sensor)"));
        #endif
        Serial.print(F("Measurement interval (minutes): "));
        Serial.println(READ_INTERVAL);
    #endif

    /* set interrupts */

    pinMode(interruptPin, INPUT);
    attachInterrupt(interruptNo, InterruptServiceRoutine, FALLING);
    
    /* Set up cellular xbee */
    /* XBee needs to be in mode: API with escapes */

    #ifdef TRANSMIT_3G

        // TODO: determine how many measurements are still stored in EEPROM3Gmask after reset to avoid overwriting them.
    
        XBeeSerial.begin(9600);
        pinMode(XBEE_SLEEPPIN, OUTPUT);
        digitalWrite(XBEE_SLEEPPIN, LOW);                     // allow starting up
        pinMode(XBEE_RESETPIN, INPUT);                        // set by default on input. We should never set this high to avoid a potential short circuit in case xbee sets it low.
        //

        xbc.setSerial(XBeeSerial);
        seqStatus.reset();   
        xbc.onModemStatusResponse(zbModemStatusCb);
        xbc.onAtCommandResponse(zbLAResponseCb);
        xbc.onTxStatusResponse(zbTcpSendResponseCb);
        xbc.onIPRxResponse(zbIPResponseCb_COAP);
    
        // wait a bit for power to settle...
        delay(1000);
        
        // check whether we can connect to the XBee:
        if(!getAIStatus(Serial, &AIstatus)) {
          
            Serial.println(F("Error communicating with Xbee. Resetting"));
            pinMode(XBEE_RESETPIN, OUTPUT); 
            digitalWrite(XBEE_RESETPIN, LOW);
            delay(500);
            digitalWrite(XBEE_RESETPIN, INPUT);               // do not set high - if xbee pulls low for some reason we have a short circuit, and deasserting is sufficient.
            delay(1000);
            if(!getAIStatus(Serial, &AIstatus)){
                error(3, ErrorLED);
                Serial.println(F("Unable to reset XBee"));
            }
        }
        // sleeping XBee
        
        digitalWrite(XBEE_SLEEPPIN, HIGH);

        packet.type = COAP_CON;                               // 0 = confirmable
        packet.code = 2;                                      // 0.02 = post method
        packet.tokenlen = sizeof(token) - 1;
        memcpy(packet.token, token, sizeof(token) - 1);
        packet.addOption(11, sizeof(Option0) - 1, Option0);   // note: first argument is option number according to Table 7 in spec.
        packet.addOption(11, sizeof(Option1) - 1, Option1);
        packet.addOption(11, sizeof(Option2) - 1, Option2);
        packet.addOption(11, sizeof(Option3) - 1, Option3);
        
    #endif

    Serial.flush();

}

/*************** Main routine ***************/

void loop () 
{

    /* At the start of the loop, the logger can be in the following states:
     *  
     *  - Alarm has gone off while doing something else -> check first what to do
     *  - Logger was waken up by clock -> check if it is time for measurement or telemetry
     *  - Logger is waiting for modem (seqStatus.tryagain > 0) -> continue telemetry operation until finished or timed out.
     *  - If none of the above conditions applies, the modem can safely go to sleep.
     *  
     *  Note: may be shortened; all checks done upfront in the if(interruptflag) {} loop
     */

    if(interruptFlag) {                               // alarm occured while executing code, so take action

        flag = Rtc.LatchAlarmsTriggeredFlags();       // returns zero if no alarm
        
        #ifdef DEBUG
            Serial.println(F("Alarm went off during code execution..."));
            Serial.flush();
        #endif

        now = Rtc.GetDateTime();

        if(now.Minute() % READ_INTERVAL != 0) {    // only measure every READ_INTERVAL minutes
            interruptFlag = false;
        }
        
    } else if(seqStatus.tryagain == 0) {               // sleep until the next alarm

        #ifdef NOSLEEP                            // still useful?
            while(!interruptFlag) {}              // wait for interrupt if not sleeping
        #else 
            #ifdef DEBUG
                Serial.print(F("S"));
                Serial.flush();
            #endif
            #if defined(__MKL26Z64__)
                Snooze.hibernate( config_digital );
            #endif
            #if defined(__AVR_ATmega328P__)
                LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); // SLEEP_FOREVER
            #endif
        #endif

        // when woken up...

        #ifdef DEBUG
            Serial.print(F("W"));
            Serial.flush();
        #endif

        flag = Rtc.LatchAlarmsTriggeredFlags();   // switch off the alarm
        interruptFlag = true;                     // should always be the case after being woken up?

        // determine what we need to do: taking a reading, transmit.
        
        now = Rtc.GetDateTime();
    
        if(now.Minute() % READ_INTERVAL != 0) {    // only measure every READ_INTERVAL minutes
            interruptFlag = false;
        }
   
        #ifdef TRANSMIT_3G
    
            if (((now.Hour() % SEND_INTERVAL) == 0) && (now.Minute() == 0)) {   // only on the hour itself!
                seqStatus.tryagain = 5;                                         // maximum number of tries
                // awake the xbee already so that it can start connecting while we do other stuff.
                digitalWrite(XBEE_SLEEPPIN, LOW);
                XbeeWakeUpTime = millis();                                                 // used for timeout
            }
        
        #endif
        
    }
         
    /* if it is time for a measurement then do so */
    
    if(interruptFlag) {

        interruptFlag = false;

        #ifdef DEBUG
            formatDateTime(now);
            Serial.println(datestring);
        #endif

        // Voltage

        measuredvbat = analogRead(VBATPIN) * 2 * 3.3 / 1.024;

        #ifdef DEBUG
            Serial.print(F("VBatt = "));
            Serial.println(measuredvbat);
        #endif

        // Temperature from clock

        temp = Rtc.GetTemperature().AsCentiDegC();

        #ifdef DEBUG
            Serial.print(F("T = "));
            Serial.println(temp);
        #endif
        
        // measure Lidar

        #ifdef LIDARLITE
            readLidarLite(readings, NREADINGS, 1, Serial);
            distance = median(readings, NREADINGS);
        
            #ifdef DEBUG
                Serial.print(F("Distance (lidar) = "));
                Serial.println(distance);
            #endif
        #endif

        /*********** store values in EEPROM ***********/

        #ifdef USE_EEPROM_BUFFER
        
            SecondsSince2000 = uint32_t(now);

            // prepare EEPromPage. Note that we use a 16 byte page here, contrary to the 32 byte page used for the SD loggers.
            // This is to save space and 3G transmission.

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

            // plenty of space for more payload here. Fill with raw measurements

            for(j = 0; (j < (NREADINGS * 2)) && (i < EEPromPageSize); j++) {
                EEPromPage[i++] = ((byte *)readings)[j];
            }
 
            while (i < sizeof(EEPromPage)){    // fill rest of page with zeros
                EEPromPage[i++] = 0;
            }

            myLogger.write2EEPROM(EEPromPage, sizeof(EEPromPage));
 
        #endif

      /******** reset readings *****/    
          
      for (int i = 0; i < 10; i++){
          readings[i] = -1;
      }

    }

    /******************* Telemetry *********************/

    #ifdef TRANSMIT_3G
    
        if (seqStatus.tryagain > 0) {               // start or continue the telemetry action;

            // Check for any messages from the Xbee.

            xbc.loop();

            // create a new buffer if there isn't one
            
            // Note that we need copy the relevant bit of the EEPROM3Gmask and stored until succesfully sending
            // to keep track of what records have been sent, even if new records are created
            // between creating the buffer and sending it.
 
            if(!EepromBufferCreated) {
                // TODO: read buffer and obtain starting position. Perhaps also calculate the pagecount here.
                startposition = getBufferStartPosition();
                Serial.print("Startposition: ");
                Serial.println(startposition);
                packet.messageid = rand();
                bufferSize = packet.createMessageHeader(EEPROM);
                pagecount = CreateEepromSendBuffer(startposition, Eeprom3Gmask);
                if(pagecount > 0) {                          // if pagecount is zero then there is nothing to send
                    EepromBufferCreated = true;
                }           
            }

            // If pagecount is zero then that means that there is nothing to send. Otherwise we may either
            // be waiting for the buffer to be sent, or for the COAP server to respond.

            if(EepromBufferCreated && !seqStatus.ipRequestSent) {

                if(seqStatus.isRegistered) {
                   // Send COAP message. Wait for direct confirmation from COAP server, but not for 2.03 response.
                   sendXbeeMessage(EEPROM, bufferSize, host, sizeof(host) - 1); // do not include "\0"
                } else {
                    // query and print the AI status every now and then, although this is not strictly necessary
                    // because the callback function will set this automatically.
                    
                    if (waitingMessageTime > 5000) {
                        getAIStatus(Serial, &AIstatus);
                        waitingMessageTime = 0;
                    } else {
                        waitingMessageTime += timeInMillis - lastTimeInMillis;
                    }
                    lastTimeInMillis = timeInMillis;
                    timeInMillis = millis() - XbeeWakeUpTime;
                }  
            }

            // if we receive an acknowledgement, then EepromBufferCreated can be reset, and the 3G mask erased.
            // A new EepromBuffer will be created in the next round. If this results in pagecount = 0
            // then this means that the mask is empty and we can close the telemetry process.

            if(seqStatus.CoapSent203Received) {
                EepromBufferCreated = false;
                seqStatus.ipRequestSent = false;
                seqStatus.ipRequestSentOk = false;
                seqStatus.ipResponseReceived = false;
                seqStatus.CoapSent203Received = false;
                Reset3GBuffer(startposition, Eeprom3Gmask);
            }

            // Close things off, and handle potential errors
            
            if (pagecount == 0) {
              
                Serial.println(F("All data sent. Sleeping."));
                digitalWrite(XBEE_SLEEPPIN, HIGH);
                seqStatus.tryagain = 0;
                seqStatus.reset();
                
                // Reset the logger's writing position when we get to the end of the EEPROM              
                // Note that this is a stopgap until proper cycling is implemented.
                
                if(myLogger.eePageAddress >= (maxpagenumber - EEPromHeaderSize - 150)) {
                    myLogger.eePageAddress = 0;
                }
                
            } else if(timeInMillis > 120000) {
              
                seqStatus.tryagain--;
                seqStatus.reset();                 // TODO: this will reset seqStatus.isconnected, even though the Xbee will still be connected
                if(seqStatus.tryagain > 0) {
                    Serial.println(F("Error. Trying again next wakeup."));
                } else {
                    digitalWrite(XBEE_SLEEPPIN, HIGH);
                    Serial.println(F("All attempts failed. Sleeping xbee modem."));
                }
            }
        }
    #endif
}
