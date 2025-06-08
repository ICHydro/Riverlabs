/**************************************
 * Arduino code for the Riverlabs sensor node with following functionality:
 * - LidarLite sensor
 * - Digi Cellular Xbee 3G/4G transmission
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

#define MQTT                                      // Set to either MQTT or COAP
//#define XBEE4G                                    // set if you are using a 4G modem (LTE-M or NB-IoT)
#define READ_INTERVAL 5                           // Interval for sensor readings, in minutes
#define SEND_INTERVAL 1                           // telemetry interval, in hours
#define NREADINGS 9                               // number of readings taken per measurement (excluding 0 values)
#define HOST ""                                   // internet address of the IoT server to report to
#define ACCESSTOKEN ""                            // Thingsboard access token
#define LOGGERID ""                               // Logger ID. Set to whatever you like
#define APN ""                                    // APN of the cellular network
#define TIMEOUT 180                               // cellular timeout in seconds, per attempt
#define NTP                                       // set the clock at startup by querying an ntp server
#define FLASH                                     // using flash backup storage?
#define OPTIBOOT                                  // set ONLY if your device uses the optiboot bootloader. Enables the watchdog timer

/*************** includes ******************/

#include "src/Rio.h"                                  // includes everything else

extern unsigned int __bss_end;
extern unsigned int __heap_start;
extern void *__brkval;

uint16_t getFreeSram() {
  uint8_t newVariable;
  // heap is empty, use bss as start memory address
  if ((uint16_t)__brkval == 0)
    return (((uint16_t)&newVariable) - ((uint16_t)&__bss_end));
  // use heap end as the start of the memory address
  else
    return (((uint16_t)&newVariable) - ((uint16_t)__brkval));
};

/********** variable declarations **********/

char LoggerID[] = LOGGERID;              // unique logger ID, to be used for data transmission and writing files.
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
uint8_t resb[XBEEBUFFERSIZE];                            // XBee's responsebuffer
//uint8_t *resb = new uint8_t[XBEEBUFFERSIZE];    // XBee's responsebuffer
//uint8_t *resb;
XBeeWithCallbacks xbc = XBeeWithCallbacks(resb, XBEEBUFFERSIZE);  // needs to be done this way, so we can delete the object, see https://forum.arduino.cc/index.php?topic=376860.0
char host[] = HOST;
uint32_t IP = 0;

#ifdef COAP
    uint16_t Port = 0x1633;                       // 5683 (COAP)
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
    uint16_t Port = 0x75B;                        // 1833
    uint8_t protocol = 1;                         // 0 for UDP, 1 for TCP, 4 for SSL over TCP
    byte m[] = {0xE0, 0x00};
    char topic[] = "v1/devices/me/telemetry";
#endif

uint8_t TelemetryAttempts = 0;
uint32_t XbeeWakeUpTime;
uint32_t timeInMillis = 0;
uint32_t lastTimeInMillis = 0;
uint32_t waitingMessageTime = 0;
int32_t startposition = -1;
uint8_t AIstatus;
uint8_t DB;
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

// SD card stuff

boolean SDcardOn;
byte keep_SPCR;
boolean fileopen = false;
uint32_t day = 40;
//SdFat SD;

// Flash stuff
#ifdef FLASH
    uint32_t FlashStart = 0;
    SPIFlash flash(FLASH_CS);
#endif

/*************** setup ***************/

void setup () 
{

    #ifdef OPTIBOOT
        // enable watchdog timer. Set at 8 seconds
        // TODO: may searching flash starting point take longer than 8s? 
        wdt_enable(WDTO_8S);
    #endif

    #if DEBUG > 0
        Serial.begin(115200);
    #endif

    /* set the pins */
    
    pinMode(WriteLED, OUTPUT);
    pinMode(ErrorLED, OUTPUT);
    digitalWrite(WriteLED, LOW);
    digitalWrite(ErrorLED, LOW);

    pinMode(VBATPIN, INPUT);

    pinMode(Boost5V_on, OUTPUT);
    pinMode(SWITCH5V, OUTPUT);
    digitalWrite(Boost5V_on, LOW);
    digitalWrite(SWITCH5V, LOW);


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

    #if DEBUG > 0
        Serial.println("");
        Serial.print(F("This is Riverlabs WMOnode"));
        #ifdef OPTIBOOT
            Serial.print(F(" (optiboot)"));
        #endif
        Serial.print(F(", compiled on "));
        Serial.println(__DATE__);
        Serial.print(F("Logger ID: "));
        Serial.println(LoggerID);
        Serial.print(F("Current time is "));
        formatDateTime(now);
        Serial.print(datestring);
        Serial.println(F(" GMT"));
        Serial.println(F("Variables:"));
        Serial.println(F("- Distance (Lidarlite)"));
        Serial.print(F("Measurement interval (minutes): "));
        Serial.println(READ_INTERVAL);
    #endif

    /* set interrupts */

    pinMode(interruptPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(interruptPin), InterruptServiceRoutine, FALLING);

    myLogger.eePageAddress = getBufferEndPosition();      // do not overwrite any previous measurements that have not yet been transmitted
    Serial.print(F("Buffer starts at position: "));
    Serial.println(myLogger.eePageAddress);

    //dumpEEPROM();
    //Serial.println(".");

    /* Set up cellular xbee */
    /* XBee needs to be in mode: API with escapes */

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
        #if DEBUG > 0
            Serial.println(F("Cannot find Xbee. Resetting"));
        #endif
        pinMode(XBEE_RESETPIN, OUTPUT); 
        digitalWrite(XBEE_RESETPIN, LOW);
        delay(500);
        pinMode(XBEE_RESETPIN, INPUT);
        delay(2000);                                      // XBee needs 2s to restart
        if(!getAIStatus(Serial, &AIstatus)){
            error(3, ErrorLED);
            #if DEBUG > 0
                Serial.println(F("Unable to reset XBee"));
            #endif
        }
    } else {

        #if DEBUG > 0
            Serial.println(F("XBee found. Configuring."));
        #endif
        
        ConfigureXBee();

        #ifdef NTP
            Serial.println(F("Getting network time"));
            MyXBeeStatus.reset();
            bool ntpstatus = setclock_ntp();
            if(!ntpstatus) ntpstatus = setclock_ntp();      // try once more, just in case
            if(ntpstatus) {
                Serial.print(F("NTP: "));
                formatDateTime(Rtc.GetDateTime());
                Serial.println(datestring);
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
    #endif

    #ifdef MQTT
        xbc.onIPRxResponse(zbIPResponseCb_MQTT);
    #endif
    #ifdef COAP
        xbc.onIPRxResponse(zbIPResponseCb_COAP);
    #endif

    #if DEBUG > 1
        Serial.println(getFreeSram());
    #endif

    #ifdef FLASH
    turnOnSDcard();
        #ifdef OPTIBOOT
            wdt_reset();                                                           // Reset the watchdog every cycle
        #endif
        FlashStart = getFlashStart();
        #if DEBUG > 0
            Serial.print(F("Flash starts at: "));
            Serial.println(FlashStart);
        #endif
    #endif

    turnOffSDcard();
    pinMode(FLASH_CS, INPUT_PULLUP);
    digitalWrite(FLASHPOWERPIN, LOW);
    digitalWrite(WriteLED, LOW);
    
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
        
        if (((now.Hour() % SEND_INTERVAL) == 0) && (now.Minute() == 0)) {   // only on the hour itself!
            measuredvbat = analogRead(VBATPIN) * 2 * 3.3 / 1.024;
            startposition = getBufferStartPosition();                       // will return -1 if the buffer is empty
            if((measuredvbat >= 3500) && (startposition >= 0)) {
                //dumpEEPROM();
                TelemetryAttempts = 5;                                      // maximum number of tries
                pinMode(XBEE_SLEEPPIN, OUTPUT);
                digitalWrite(XBEE_SLEEPPIN, LOW);
                // check whether the xbee responds
                delay(500);                                                 // How much time does the xbee need to wake up?
                if(!getAIStatus(Serial, &AIstatus)) {
                    #if DEBUG > 0
                        Serial.println(F("Can't find Xbee. Resetting"));
                    #endif
                    pinMode(XBEE_RESETPIN, OUTPUT);
                    digitalWrite(XBEE_RESETPIN, LOW);
                    delay(100);
                    pinMode(XBEE_RESETPIN, INPUT);
                }
                // TODO: deal with case that reset is not effective
                XbeeWakeUpTime = millis();                                  // used for timeout
                timeInMillis = 0;
                lastTimeInMillis = 0;
                waitingMessageTime = 0;
            }
        }
    }

    // if nothing needs to be done, then we can safely sleep until the next alarm.
    // the timeout variable allows sleeping briefly between telemetry attemps
    // (XBee stays awake)
    
    if((!TakeMeasurement) && (TelemetryAttempts == 0 || timeout)) {

        #ifdef NOSLEEP
            while(!interruptFlag) {}                                        // wait for alarm if not sleeping
        #else 
            #if DEBUG > 0
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

        #ifdef OPTIBOOT
            // enable watchdog timer. Set at 8 seconds 
            wdt_enable(WDTO_8S);
        #endif

        #if DEBUG > 0
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

        #if DEBUG > 0        
            formatDateTime(now);
            Serial.println();
            Serial.println(datestring);
            Serial.print(F("VBatt = "));
            Serial.println(measuredvbat);
            Serial.print(F("T = "));
            Serial.println(temp);
            Serial.print(F("Distance (lidar) = "));
            Serial.println(distance);
        #endif
        #if DEBUG > 1
            Serial.print(F("Free memory (bytes): "));
            Serial.println(getFreeSram());
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

        //for(i = 0; i < sizeof(EEPromPage); i++) {
        //    Serial.print(EEPromPage[i], HEX);
        //    Serial.print(" ");
        //}
        //Serial.println();

        myLogger.write2EEPROM(EEPromPage, sizeof(EEPromPage));

        /*********** store values in FLASH ***********/

        #ifdef FLASH

            digitalWrite(FLASHPOWERPIN, HIGH);
            pinMode(FLASH_CS, OUTPUT);
            turnOnSDcard();

            #ifdef FLASH
                write2Flash(EEPromPage, 30, FlashStart++);
            #endif

            pinMode(FLASH_CS, INPUT_PULLUP);
            keep_SPCR=SPCR;
            turnOffSDcard();
            digitalWrite(FLASHPOWERPIN, LOW);
        #endif

        /******** reset readings *****/    
          
        for (i = 0; i < NREADINGS; i++){
            readings[i] = -1;
        }
    }

    /******************* Telemetry *********************/

    if (TelemetryAttempts > 0) {                         // start or continue the telemetry action;

        xbc.loop();                                       // Check for any messages from the Xbee.

        // TODO: can we speed this up by reading the EEPROM mask in memory once, and then loop over that.
        // (EEPROM3Gmask can be used for this) instead of reading EEPROM every time?

        // If startposition is -1, then the buffer is empty and we can finish the telemetry session.

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

            #if DEBUG > 0
                Serial.println(F("Sleeping XBee."));
            #endif
            
            pinMode(XBEE_SLEEPPIN, INPUT);
            TelemetryAttempts = 0;
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
                        MQTT_connect(LoggerID, sizeof(LoggerID), accesstoken, sizeof(accesstoken));
                        messageid = 1;
                        // TODO: deal with timeout and errors
                    } else {
                        MQTT_send(messageid++);
                    }
                #endif
                
                #ifdef COAP
                    if(!MyXBeeStatus.MessageSent) {
                        COAP_send(packet);
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
            startposition = getBufferStartPosition();
            MyXBeeStatus.MessageSent = false;
            
        }

        // Lastly, handle potential timeouts and errors
        // TODO: deal with IP communication issues, e.g. lost packages. We probably need more than 1 timer

        timeInMillis = millis() - XbeeWakeUpTime;
        
        if(((timeInMillis/1000) > TIMEOUT) || MyXBeeStatus.xbcErrorOccurred) {
            TelemetryAttempts--;
            MyXBeeStatus.reset();
            if(TelemetryAttempts > 0) {
                #if DEBUG > 0
                    Serial.println(F("Timeout or error. Trying again next wakeup."));
                #endif
                timeout = true;
            } else {
                pinMode(XBEE_SLEEPPIN, INPUT);
                #if DEBUG > 0
                    Serial.println(F("All attempts failed. Sleeping xbee modem."));
                #endif
                startposition = -1;
            }
        }
    }
}

/*************** helper functions ******************/

void ConfigureXBee() {

    uint8_t laCmd1[] = {'A','N'};
    uint8_t laCmd2[] = {'W','R'};
    uint8_t laCmd3[] = {'A','C'};
    uint8_t laCmd4[] = {'D','O'};
    #ifdef XBEE4G
        uint8_t laCmd5[] = {'C','P'};                   // carrier profile
        uint8_t laCmd6[] = {'B','N'};                   // band mask IoT
        uint8_t laCmd7[] = {'N','#'};                   // Network technology
    #endif

    char APNstring[] = APN;
    uint8_t DOvalue = 0x43;

    #ifdef XBEE4G
        uint8_t CarrierProfile = 0;   // 0 = read out from SIM
        byte bandmask[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x80};
        uint8_t nettech = 3;     // 3 = NB-IoT only
        DOvalue = 1;
    #endif

    AtCommandRequest atRequest1(laCmd1, (uint8_t*) APNstring, sizeof(APNstring) - 1);
    AtCommandRequest atRequest2(laCmd2);
    AtCommandRequest atRequest3(laCmd3);
    AtCommandRequest atRequest4(laCmd4, &DOvalue, 1);

    #ifdef XBEE4G
        AtCommandRequest atRequest5(laCmd5, &CarrierProfile, 1);
        //AtCommandRequest atRequest6(laCmd6, bandmask, 16);
        AtCommandRequest atRequest7(laCmd7, &nettech, 1);
    #endif

    uint8_t status = xbc.sendAndWait(atRequest1, 150);
    status += xbc.sendAndWait(atRequest4, 150);

    #ifdef XBEE4G
        status += xbc.sendAndWait(atRequest5, 150);
        //status += xbc.sendAndWait(atRequest6, 150);
        status += xbc.sendAndWait(atRequest7, 150);
    #endif

    status += xbc.sendAndWait(atRequest2, 150);
    status += xbc.sendAndWait(atRequest3, 150);
}
