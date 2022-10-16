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

#define MQTT                                      // Telemetry protocol. Choose either COAP or MQTT
#define READ_INTERVAL 5                           // Interval for sensor readings, in minutes
#define SEND_INTERVAL 1                           // telemetry interval, in hours
#define NREADINGS 9                               // number of readings taken per measurement (excluding 0 values)
#define HOST "demo.thingsboard.io"                // internet address of the IoT server to report to
#define ACCESSTOKEN ""                            // Thingsboard access token
#define LOGGERID ""                               // Logger ID. Set to whatever you like
#define APN ""                                    // APN of the cellular network
#define TIMEOUT 600                               // cellular timeout in seconds, per attempt
#define NTC                                       // set the clock at startup by querying an ntc server
#define OPTIBOOT                                  // set ONLY if your device uses the optiboot bootloader

/* INCLUDES */

#include "src/Rio.h"                              // includes everything else
#include <avr/wdt.h>

/********** variable declarations **********/

char LoggerID[] = LOGGERID;                 // unique logger ID, to be used for data transmission and writing files.
char accesstoken[] = ACCESSTOKEN;
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

CellularStatus MyXBeeStatus;

AltSoftSerial XBeeSerial;
uint8_t resb[100];                                // XBee's responsebuffer
uint8_t buffer[150];
XBeeWithCallbacks xbc = XBeeWithCallbacks(resb, sizeof(resb));  // needs to be done this way, so we can delete the object, see https://forum.arduino.cc/index.php?topic=376860.0
const char host[] = HOST;
uint32_t IP = 0;
#ifdef COAP
    const uint16_t Port = 0x1633;                 // 5683 (COAP)
    uint8_t protocol = 0;                         // 0 for UDP, 1 for TCP, 4 for SSL over TCP
    CoapPacket packet;
    char token[] = "tk";                          // to be randomised
    char MsgLength = 0;
    char Option0[] = "api";
    char Option1[] = "v1";
    char Option2[] = ACCESSTOKEN;
    char Option3[] = "telemetry";
#endif
#ifdef MQTT
    const uint16_t Port = 0x75B;                  // 1833
    uint8_t protocol = 1;                         // 0 for UDP, 1 for TCP, 4 for SSL over TCP
    byte m[] = {0xE0, 0x0};
#endif

uint16_t bufferSize;
uint32_t XbeeWakeUpTime;
uint32_t timeInMillis = 0;
uint32_t lastTimeInMillis = 0;
uint32_t waitingMessageTime = 0;
int32_t startposition = -1;                   
uint8_t AIstatus;
uint8_t DB;
uint8_t pagecount;
uint16_t messageid = 1;

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

    pinMode(MBONPIN, OUTPUT);
    digitalWrite(MBONPIN, HIGH);
 

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
        Serial.print(F("This is Riverlabs Wari_4G (optiboot), compiled on "));
        Serial.println(__DATE__);
        Serial.print(F("Logger ID: "));
        Serial.println(LoggerID);
        Serial.print(F("Current time is "));
        formatDateTime(now);
        Serial.print(datestring);
        Serial.println(F(" GMT"));
        Serial.println(F("Measuring the following variables:"));
        Serial.println(F("- Distance (Maxbotix ultrasound sensor)"));
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
    MyXBeeStatus.reset();   
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
            Serial.println(F("Cellular XBee detected. Setting APN"));
        #endif
        uint8_t laCmd1[] = {'A','N'};
        uint8_t laCmd4[] = {'D','O'};
        uint8_t laCmd5[] = {'C','P'};                   // carrier profile
        uint8_t laCmd6[] = {'B','N'};                   // band mask IoT
        uint8_t laCmd7[] = {'N','#'};                   // Network technology
        uint8_t laCmd2[] = {'W','R'};
        uint8_t laCmd3[] = {'A','C'};
        
        char APNstring[] = APN;
        uint8_t CarrierProfile = 0;
        byte bandmask[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x80};
        uint8_t nettech = 2;
        uint8_t DOvalue = 1;
        
        AtCommandRequest atRequest1(laCmd1, (uint8_t*) APNstring, sizeof(APNstring) - 1);
        AtCommandRequest atRequest4(laCmd4, &DOvalue, 1);
        AtCommandRequest atRequest5(laCmd5, &CarrierProfile, 1);
        AtCommandRequest atRequest6(laCmd6, bandmask, 16);
        AtCommandRequest atRequest7(laCmd7, &nettech, 1);
        AtCommandRequest atRequest2(laCmd2);
        AtCommandRequest atRequest3(laCmd3);
      
        uint8_t status = xbc.sendAndWait(atRequest1, 150);
        status += xbc.sendAndWait(atRequest4, 150);
        status += xbc.sendAndWait(atRequest5, 150);
        status += xbc.sendAndWait(atRequest6, 150);
        status += xbc.sendAndWait(atRequest7, 150);
        status += xbc.sendAndWait(atRequest2, 150);
        status += xbc.sendAndWait(atRequest3, 150);

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
    }
    
    pinMode(XBEE_SLEEPPIN, INPUT);                        // sleeping XBee. Deassert instead of setting high - see above

    #ifdef COAP
        packet.type = COAP_CON;                               // 0 = confirmable
        packet.code = 2;                                      // 0.02 = post method
        packet.tokenlen = sizeof(token) - 1;
        memcpy(packet.token, token, sizeof(token) - 1);
        packet.addOption(11, sizeof(Option0) - 1, (uint8_t*) Option0);   // note: first argument is option number according to Table 7 in spec.
        packet.addOption(11, sizeof(Option1) - 1, (uint8_t*) Option1);
        packet.addOption(11, sizeof(Option2) - 1, (uint8_t*) Option2);
        packet.addOption(11, sizeof(Option3) - 1, (uint8_t*) Option3);
        xbc.onIPRxResponse(zbIPResponseCb_COAP);
    #endif

    #ifdef MQTT
        xbc.onIPRxResponse(zbIPResponseCb_MQTT);
    #endif
    
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

    #ifdef OPTIBOOT
        wdt_reset();                                                           // Reset the watchdog every cycle
    #endif

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
        
        if (((now.Hour() % SEND_INTERVAL) == 0) && (now.Minute() == 0 )) {   // only on the hour itself!
            MyXBeeStatus.tryagain = 5;                                         // maximum number of tries
            pinMode(XBEE_SLEEPPIN, OUTPUT);
            digitalWrite(XBEE_SLEEPPIN, LOW);
            XbeeWakeUpTime = millis();                                      // used for timeout
            timeInMillis = 0;
            lastTimeInMillis = 0;
            waitingMessageTime = 0;
        }
    
    }

    // if nothing needs to be done, then we can safely sleep until the next alarm.
    // The timeout variable allows sleeping briefly between telemetry attemps
    // (XBee stays awake)
    
    if((!TakeMeasurement) && (MyXBeeStatus.tryagain == 0 || timeout)) {

        #ifdef NOSLEEP
            while(!interruptFlag) {}                                        // wait for alarm if not sleeping
        #else 
            #ifdef DEBUG > 0
                Serial.print(F("S"));
                Serial.flush();
            #endif
            #ifdef OPTIBOOT
                wdt_disable();
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

        #ifdef OPTIBOOT
            // enable watchdog timer. Set at 8 seconds 
            wdt_enable(WDTO_8S);
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
        Serial.end();
        distance = readMaxBotix(MBSERIALPIN, maxbotixPin, NREADINGS, 0); // distance
        Serial.begin(115200); 

        #ifdef DEBUG > 0
            formatDateTime(now);
            Serial.println(datestring);
            Serial.print(F("VBatt = "));
            Serial.println(measuredvbat);
            Serial.print(F("T = "));
            Serial.println(temp);
            Serial.print(F("Distance (ultrasound) = "));
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
          
        for (i = 0; i < NREADINGS; i++){
            readings[i] = -1;
        }
    }

    /******************* Telemetry *********************/

    if (MyXBeeStatus.tryagain > 0) {                         // start or continue the telemetry action;

        xbc.loop();                                       // Check for any messages from the Xbee.

        // First, check if there are any data to be sent. Don't create the sendbuffer yet
        // but locate the position of the first data in the EEPROM.
        // Note: this is slow because this means that EEPROM buffer mask is read every time a data packet is sent.
        // It may be faster to just read the mask in memory once, and then loop over that.
        // (EEPROM3Gmask can be used for this)
        // If the startposition is -1 then that means that the buffer is empty and we can go back
        // to sleep.

        if(startposition < 0) {
          
            startposition = getBufferStartPosition();     // will return -1 if the buffer is empty

        }

        // If there is nothing to send, then we can finish the telemetry session.

        if (startposition < 0) {

            #ifdef MQTT
                if(MyXBeeStatus.MqttConnected) {
                    tcpSend(IP, Port, protocol, m, 2);
                    MyXBeeStatus.MqttConnected = false;
                    timeInMillis = millis();              // should be ok to reuse here
                    // local loop ok here; this should be quick because
                    // we are just waiting for the xbee to confirm sending.
                    while(!MyXBeeStatus.ipRequestSentOk && ((millis() - timeInMillis) < 1000)) {
                        xbc.loop();
                    }
                }
            
            #endif

            #ifdef DEBUG > 0
                Serial.println(F("All data sent. Sleeping XBee."));
            #endif
            
            pinMode(XBEE_SLEEPPIN, INPUT);
            MyXBeeStatus.tryagain = 0;
            MyXBeeStatus.reset();
            
            // Reset the logger's writing position when we get to the end of the EEPROM              
            // Note that this is a stopgap until proper cycling is implemented.
            
            if(myLogger.eePageAddress >= (maxpagenumber - EEPromHeaderSize - 300)) {
                myLogger.eePageAddress = 0;
            }

        // if there is anything to send then first wait until the modem is connected
        
        } else if(!MyXBeeStatus.isConnected) {
          
            if (waitingMessageTime > 5000) {             // don't check once connection established to avoid interference between xbee replies.
                xbc.send(AIRequest);                      // main xbee.loop() takes care of the response.
                waitingMessageTime = 0;
            } else {
                  waitingMessageTime += timeInMillis - lastTimeInMillis;
            }
            lastTimeInMillis = timeInMillis;

        // If the modem is connected then we can start sending the data. For now, we do things in sequence, i.e., wait for each transaction
        // to finish before starting the next one. This can be made faster by sending messages without waiting for a reply, but this
        // needs a system to keep track of all sent messages and their status.
        // A transaction is finished if ipRequestSent is reset to false. This happens in th callback functions,
        // but may need to be done manually for tcp messages that do not require a reply (e.g., MQTT disconnect).
        
        } else if(!MyXBeeStatus.ipRequestSent) {

            // first resolve the IP address of the server. This is only done once per session

            if(!MyXBeeStatus.hostIPResolved) {
              
                sendDNSLookupCommand((char*) host, sizeof(host) - 1);
                MyXBeeStatus.ipRequestSent = true;
                
            } else {

                // Note that we need to keep a copy of the relevant part of the EEPROM3Gmask until succesfully sending
                // to keep track of what records have been sent, because new records may be created
                // between creating the buffer and sending it.
                
                #ifdef MQTT
                    if(!MyXBeeStatus.MqttConnected) {
                        MQTT_connect(buffer, LoggerID, sizeof(LoggerID), accesstoken, sizeof(accesstoken));
                        messageid = 1;
                        // TODO: deal with timeout and errors
                    } else {
                        bufferSize = CreateMqttHeader(buffer, messageid);
                        // TODO: remove pagecount. Not needed any more
                        Serial.println(bufferSize);
                        pagecount = CreateSendBuffer(startposition, Eeprom3Gmask, buffer);
                        messageid++;
                        Serial.println(bufferSize);
                        tcpSend(IP, Port, protocol, buffer, bufferSize);
                    }
                #endif
                
                #ifdef COAP
                    // send one data message at a time and wait until it is entirely processed
                    // before sending a new one.
                    if(!MyXBeeStatus.MessageSent) {
                        packet.messageid = rand();                    // rand() returns int16_t, random() returns int_32
                        bufferSize = packet.createMessageHeader(buffer);
                        pagecount = CreateSendBuffer(startposition, Eeprom3Gmask, buffer);
                        tcpSend(IP, Port, protocol, buffer, bufferSize);
                        MyXBeeStatus.MessageSent = true;
                    }
                #endif
            } 
        
        }

        // if an IP response is received and processed, then we are ready to send the next packet
        
        if(MyXBeeStatus.ipResponseReceived) {
          
            MyXBeeStatus.ipRequestSent = false;
            MyXBeeStatus.ipRequestSentOk = false;
            MyXBeeStatus.ipResponseReceived = false;
            
        }

        // Once we receive an acknowledgement of successful processing of a data message, 
        // the 3G mask can be erased and the startposition reset.
        // this starts the telemetry sequence again from the beginning.

        if(MyXBeeStatus.MessageConfirmed) {
          
            MyXBeeStatus.MessageConfirmed = false;
            Reset3GBuffer(startposition);
            startposition = -1;
            MyXBeeStatus.MessageSent = false;
            
        }

        // Lastly, handle potential timeouts and errors
        // TODO: deal with IP communication issues, e.g. lost packages. We probably need more than 1 timer

        timeInMillis = millis() - XbeeWakeUpTime;
        
        if(((timeInMillis/1000) > TIMEOUT) || MyXBeeStatus.xbcErrorOccurred) {
            MyXBeeStatus.tryagain--;
            MyXBeeStatus.reset();
            if(MyXBeeStatus.tryagain > 0) {
                #ifdef DEBUG > 0
                    Serial.println(F("Timeout or error. Trying again next wakeup."));
                #endif
                timeout = true;
            } else {
                pinMode(XBEE_SLEEPPIN, INPUT);
                #ifdef DEBUG > 0;
                    Serial.println(F("All attempts failed. Sleeping xbee modem."));
                #endif
                startposition = -1;
            }
        }
    }
}
