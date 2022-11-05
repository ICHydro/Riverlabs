
//* functions developed by Isabela Mapstone to control the XBee 3G */

#include <Arduino.h>
#include <Wire.h>
#include "XBee_dev.h"
#include "Rio_COAP.h"
#include "Rio_MQTT.h"
#include <RtcDS3231.h>
#if defined(__AVR_ATmega328P__)
    #include <SoftwareSerial.h>
#endif

#define DEBUGSERIAL Serial

#ifndef Rio_xbee_h
#define Rio_xbee_h

/* class definitions */

// IP request sequence statuses
// This is used to keep track of the Xbee status
class CellularStatus {
    public:
    void reset() {
        isAwake = false;
        isRegistered = false;
        aiRequested = false;
        isConnected = false;
        dnsLookupRequested = false;
        hostIPResolved = false;
        ipRequestSent = false;
        ipRequestSentOk = false;
        ipResponseReceived = false;
        xbcErrorOccurred = false;
        gotStatusResponse = false;
        dontSleep = false;
        CoapSentAcknowledged = false;
        MessageSent = false;
        MessageConfirmed = false;
        MqttConnected = false;
        MqttConnack = false;
        MqttPuback = false;
    };
    bool isAwake = false;
    bool isRegistered = false;
    bool aiRequested = false;
    bool isConnected = false;
    bool dnsLookupRequested = false;
    bool hostIPResolved = false;
    bool ipRequestSent = false;
    bool ipRequestSentOk = false;
    bool ipResponseReceived = false;
    bool xbcErrorOccurred = false;
    bool gotStatusResponse = false;
    bool dontSleep = false;
    bool MessageSent = false;
    bool MessageConfirmed = false;      // Telemetry server confirmed successful receipt of message (2.01 or 2.03 for COAP)
    // MQTT specific status flags:
    bool MqttConnected = false;
    bool MqttConnack = false;
    bool MqttPuback = false;
    // COAP specific status flags:
    bool CoapSentAcknowledged = false;     // Recipient acknowledges arrival
};

//class CoapTransaction {
//    public:
//		CoapPacket packet;
//};


/* variable and function declarations */

extern uint8_t assocCmd[];
extern ModemStatusResponse modemStatusResponse;
extern AtCommandResponse atResponse;
extern IPTxRequest ipRequest;
extern TxStatusResponse txResponse;
extern IPRxResponse ipResponse;
extern CellularStatus MyXBeeStatus;
extern uint32_t IP;
extern uint16_t Port;
extern uint8_t protocol;
extern RtcDS3231<TwoWire> Rtc;

extern XBeeWithCallbacks xbc;
extern uint32_t IPaddr;

void checkForRegistrationMessage();
void getAIResponse();
bool getAIStatus(Stream &stream, uint8_t*);
bool getDBStatus(Stream &stream, uint8_t*);
void sendDNSLookupCommand(char*, int);
void getLAResponse();
void tcpSend(uint32_t, uint16_t, uint8_t, uint8_t*, uint16_t);
void getTcpSendResponse();
void readIPResponse();
void printUnexpectedMessage();
void printIPRX(IPRxResponse& IPresponse, Stream &stream);
void sendAtCommand(AtCommandRequest);

// callback functions:
void zbModemStatusCb(ModemStatusResponse& mx, uintptr_t);
void zbIPResponseCb(IPRxResponse& ipResponse, uintptr_t);           // generic callback
void zbIPResponseCb_COAP(IPRxResponse& ipResponse, uintptr_t);      // specific for COAP
void zbIPResponseCb_NTP(IPRxResponse& ipResponse, uintptr_t);       // specific for NTP
void zbIPResponseCb_MQTT(IPRxResponse& ipResponse, uintptr_t);      // specific for MQTT
void zbTcpSendResponseCb(TxStatusResponse& txr, uintptr_t);
void zbAtResponseCb(AtCommandResponse& atr, uintptr_t);

void sendXbeeMessage(uint8_t*, uint16_t, char *host, uint8_t);
void sendXbeeMessage(uint16_t, char *host, uint8_t);

bool setclock_ntc();

#endif
