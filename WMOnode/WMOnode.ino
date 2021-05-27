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

#define COAP                                      // Do not change
#define READ_INTERVAL 5                           // Interval for sensor readings, in minutes
#define SEND_INTERVAL 3                           // telemetry interval, in hours
#define NREADINGS 9                               // number of readings taken per measurement (excluding 0 values)
#define HOST "riverflow.io"                     // internet address of the IoT server to report to
#define ACCESSTOKEN "qcoGouZ9Blq8rZwtTRVz"               // COAP access token
#define LOGGERID "RL000200"                       // Logger ID. Set to whatever you like
#define TIMEOUT 180                               // cellular timeout in seconds, per attempt
#define DONOTUSEEEPROMSENDBUFFER
#define NTC                                       // set the clock at startup by querying an ntc server

/* INCLUDES */

#include "Rio.h"                                  // includes everything else

/********** variable declarations **********/

const char LoggerID[] = LOGGERID;              // unique logger ID, to be used for data transmission and writing files.
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
uint32_t newday;
char datestring[20];
DS3231AlarmFlag flag;

// Cellular xbee variables

CellularStatus seqStatus;

AltSoftSerial XBeeSerial;
uint8_t resb[100];                            // XBee's responsebuffer
uint8_t buffer[150];
XBeeWithCallbacks xbc = XBeeWithCallbacks(resb, sizeof(resb));  // needs to be done this way, so we can delete the object, see https://forum.arduino.cc/index.php?topic=376860.0
const char host[] = HOST;
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
uint8_t DB;
uint8_t pagecount;
char token[] = "tk";                           // to be randomised
char MsgLength = 0;
char Option0[] = "api";
char Option1[] = "v1";
char Option2[] = ACCESSTOKEN;
char Option3[] = "telemetry";
CoapPacket packet; 
bool SendBufferCreated = 0;
byte Eeprom3Gmask[2 + MAXFIT / 8];  
uint8_t assocCmd[] = {'A','I'};
AtCommandRequest AIRequest(assocCmd);           



// other variables

uint16_t arraycounter = 0;
uint16_t DeltaT;
uint16_t measuredvbat;
int16_t temp;
uint8_t MaxContentLength;
bool timeout = false;

RioLogger myLogger = RioLogger();

//EEPROM stuff

byte EEPromPage[(EEPromPageSize > 30) ? 30 : EEPromPageSize]; 
boolean flusheeprom = false;

// internal EEPROM is used to create the telemetry buffer

uint16_t EEPROM_payload_start = 0;

// SD card stuff

//SdFat SD;

/*************** setup ***************/

void setup () 
{
    
    #if DEBUG > 0
        Serial.begin(115200);
    #endif

    /* set the pins */
    
    pinMode(WriteLED, OUTPUT);
    pinMode(ErrorLED, OUTPUT);
    digitalWrite(WriteLED, LOW);
    digitalWrite(ErrorLED, LOW);

    pinMode(VBATPIN, INPUT);

    #ifdef Boost5V_on
        pinMode(Boost5V_on, OUTPUT);
        pinMode(SWITCH5V, OUTPUT);
        digitalWrite(Boost5V_on, LOW);
        digitalWrite(SWITCH5V, LOW);
    #endif

    #ifdef XBEE_SLEEPPIN
        pinMode(XBEE_SLEEPPIN, INPUT);   // do not set high but keep floating
    #endif

    pinMode(LIDARONPIN, OUTPUT);
    digitalWrite(LIDARONPIN, LOW);
 

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

    #ifdef DEBUG > 0
        Serial.println("");
        Serial.print(F("This is Riverlabs WMOnode, compiled on "));
        Serial.println(__DATE__);
        Serial.print(F("Logger ID: "));
        Serial.println(LoggerID);
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

    pinMode(interruptPin, INPUT);
    attachInterrupt(interruptNo, InterruptServiceRoutine, FALLING);
    
    /* Set up cellular xbee */
    /* XBee needs to be in mode: API with escapes */

    myLogger.eePageAddress = getBufferEndPosition();      // do not overwrite any previous measurements that have not yet been transmitted
    Serial.print(F("Buffer starts at position: "));
    Serial.println(myLogger.eePageAddress);

    XBeeSerial.begin(9600);
    pinMode(XBEE_SLEEPPIN, OUTPUT);
    digitalWrite(XBEE_SLEEPPIN, LOW);                     // allow starting up
    pinMode(XBEE_RESETPIN, INPUT);                        // set by default on input. We should never set this high to avoid a potential short circuit in case xbee sets it low.

    xbc.setSerial(XBeeSerial);
    seqStatus.reset();   
    xbc.onModemStatusResponse(zbModemStatusCb);
    xbc.onAtCommandResponse(zbAtResponseCb);
    xbc.onTxStatusResponse(zbTcpSendResponseCb);
    xbc.onIPRxResponse(zbIPResponseCb_NTP);

    // wait a bit for power to settle and XBee to start up.
    delay(2000);
    
    // check whether we can connect to the XBee:

    if(!getAIStatus(Serial, &AIstatus)) {
        #ifdef DEBUG > 0
            Serial.println(F("Error communicating with Xbee. Resetting"));
        #endif
        pinMode(XBEE_RESETPIN, OUTPUT); 
        digitalWrite(XBEE_RESETPIN, LOW);
        delay(500);
        pinMode(XBEE_RESETPIN, INPUT);
        delay(2000);                                      // XBee needs 2s to restart
        if(!getAIStatus(Serial, &AIstatus)){
            error(3, ErrorLED);
            #ifdef DEBUG > 0
                Serial.println(F("Unable to reset XBee"));
            #endif
        }
    } else {
      #ifdef DEBUG > 0
          Serial.print(F("AI status = "));
          Serial.println(AIstatus);
      #endif
    }

    #ifdef NTC
        if(setclock_ntc()) {
            Serial.print(F("NTP received. Clock is set to: "));
            RtcDateTime now = Rtc.GetDateTime();
            printDateTime(now);
            Serial.println();
            digitalWrite(WriteLED, HIGH);
            delay(1000);
            digitalWrite(WriteLED, LOW);
        } else {
            error(2, ErrorLED);
        }
    #endif

    xbc.onIPRxResponse(zbIPResponseCb_COAP);
    
    pinMode(XBEE_SLEEPPIN, INPUT);                        // sleeping XBee. Deassert instead of setting high - see above

    packet.type = COAP_CON;                               // 0 = confirmable
    packet.code = 2;                                      // 0.02 = post method
    packet.tokenlen = sizeof(token) - 1;
    memcpy(packet.token, token, sizeof(token) - 1);
    packet.addOption(11, sizeof(Option0) - 1, Option0);   // note: first argument is option number according to Table 7 in spec.
    packet.addOption(11, sizeof(Option1) - 1, Option1);
    packet.addOption(11, sizeof(Option2) - 1, Option2);
    packet.addOption(11, sizeof(Option3) - 1, Option3);

    Serial.flush();

    // Start wire for i2c communication (EEPROM) (note: this does not seem necessary for atmel, but it is for SAMD21)

    Wire.begin();

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

        // Check whether it is time for a telemetry event. Wake up xbee already
        // so it can start connecting while doing other things
        
        if (((now.Hour() % SEND_INTERVAL) == 0) && (now.Minute() == 0)) {   // only on the hour itself!
            seqStatus.tryagain = 5;                                         // maximum number of tries
            pinMode(XBEE_SLEEPPIN, OUTPUT);
            digitalWrite(XBEE_SLEEPPIN, LOW);
            XbeeWakeUpTime = millis();                                      // used for timeout
            timeInMillis = 0;
            lastTimeInMillis = 0;
            waitingMessageTime = 0;
        }
    
    }

    // if nothing needs to be done, then we can safely sleep until the next alarm.
    // the timeout variable allows sleeping briefly between telemetry attemps
    // (XBee stays awake)
    
    if((!TakeMeasurement) && (seqStatus.tryagain == 0 || timeout)) {

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

        // if we wake up after a timeout, reset the timer so that another telemetry attempt can be made
        if(timeout) {
            timeout = false;
            XbeeWakeUpTime = millis();
            timeInMillis = 0;
            lastTimeInMillis = 0;
            waitingMessageTime = 0;
        }
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
        
        SecondsSince2000 = uint32_t(now);

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

        /******** reset readings *****/    
          
        for (i = 0; i < 10; i++){
            readings[i] = -1;
        }
    }

    /******************* Telemetry *********************/

    if (seqStatus.tryagain > 0) {                         // start or continue the telemetry action;

        xbc.loop();                                       // Check for any messages from the Xbee.

        // create a new buffer if there isn't one
        
        // Note that we need to keep a copy the relevant part of the EEPROM3Gmask until succesfully sending
        // to keep track of what records have been sent, because new records may be created
        // between creating the buffer and sending it.

        if(!SendBufferCreated) {
            startposition = getBufferStartPosition();     // will return 0 if the buffer is empty
            #ifdef DEBUG > 0
                Serial.print(F("Startposition: "));
                Serial.println(startposition);
            #endif
            packet.messageid = rand();                    // rand() returns int16_t, random() returns int_32
            #ifdef DONOTUSEEEPROMSENDBUFFER
                bufferSize = packet.createMessageHeader(buffer);
                pagecount = CreateSendBuffer(startposition, Eeprom3Gmask, buffer);
            #else
                bufferSize = packet.createMessageHeader(EEPROM);
                pagecount = CreateEepromSendBuffer(startposition, Eeprom3Gmask);
            #endif
     
            if(pagecount > 0) {                           // if pagecount is zero then there is nothing to send
                SendBufferCreated = true;
            }           
        }

        // If pagecount is zero then that means that there is nothing to send. Otherwise we may either
        // be waiting for the buffer to be sent, or for the COAP server to respond.

        if(SendBufferCreated && !seqStatus.ipRequestSent) {

            // let's ignore the unsolicited status message for now. This sets isRegistered, but
            // this may not necessarily mean that the modem is connected to the internet, so we better
            // wait until we explicitly have an AI status of zero.
            // At worst we lose 5 seconds with this.

            if(seqStatus.isConnected) {
                //if(!seqStatus.isConnected) { 
                //    getAIStatus(Serial, &AIstatus);           // will set is.Connected
                // } else {
                  // Send COAP message. Wait for direct confirmation from COAP server, but not for 2.03 response.
                  // sendXbeeMessage(bufferSize, host, sizeof(host) - 1); // do not include "\0"
                  #ifdef DONOTUSEEEPROMSENDBUFFER
                      sendXbeeMessage(buffer, bufferSize, host, sizeof(host) - 1);
                  #else
                      sendXbeeMessage(bufferSize, host, sizeof(host) - 1);
                  #endif
                //}
            } else if (waitingMessageTime > 5000) {             // don't check once connection established to avoid interference between xbee replies.
                xbc.send(AIRequest);
                // wait for reply before sending another request:
                waitingMessageTime = millis();                // reuse waitingMessageTime for memory efficiency
                while((millis() - waitingMessageTime) < 150) {
                  xbc.loop();
                }
                //xbc.send(DBRequest);
                //getAIStatus(Serial, &AIstatus);
                //#ifdef DEBUG > 0
                //    Serial.print(F("AI status = "));
                //    Serial.println(AIstatus);
                //#endif
                //getDBStatus(Serial, &DB);
                waitingMessageTime = 0;
            } else {
                waitingMessageTime += timeInMillis - lastTimeInMillis;
            }
            lastTimeInMillis = timeInMillis;
        }

        timeInMillis = millis() - XbeeWakeUpTime;

        // If we receive an acknowledgement, then SendBufferCreated can be reset, and the 3G mask erased.
        // A new SendBuffer will be created in the next round. Once pagecount = 0,
        // the mask is empty and we can finish the telemetry process.

        if(seqStatus.CoapSent203Received) {
            SendBufferCreated = false;
            seqStatus.ipRequestSent = false;
            seqStatus.ipRequestSentOk = false;
            seqStatus.ipResponseReceived = false;
            seqStatus.CoapSent203Received = false;
            #ifdef DONOTUSEEEPROMSENDBUFFER
                Reset3GBuffer(startposition);                // in case only one page is written
            #else
                Reset3GBuffer(startposition, Eeprom3Gmask);
            #endif
        }

        // Close things off, and handle potential errors
        
        if (pagecount == 0) {

            #ifdef DEBUG > 0
                Serial.println(F("All data sent. Sleeping."));
            #endif
            pinMode(XBEE_SLEEPPIN, INPUT);
            seqStatus.tryagain = 0;
            seqStatus.reset();
            
            // Reset the logger's writing position when we get to the end of the EEPROM              
            // Note that this is a stopgap until proper cycling is implemented.
            
            if(myLogger.eePageAddress >= (maxpagenumber - EEPromHeaderSize - 300)) {
                myLogger.eePageAddress = 0;
            }
            
        } else if(((timeInMillis/1000) > TIMEOUT) || seqStatus.xbcErrorOccurred) {
            seqStatus.tryagain--;
            seqStatus.reset();
            if(seqStatus.tryagain > 0) {
                #ifdef DEBUG > 0
                    Serial.println(F("Timeout or error. Trying again next wakeup."));
                #endif
                timeout = true;
            } else {
                pinMode(XBEE_SLEEPPIN, INPUT);
                #ifdef DEBUG > 0;
                    Serial.println(F("All attempts failed. Sleeping xbee modem."));
                #endif
            }
        }
    }
}
