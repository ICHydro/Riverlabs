
//* functions developed by Isabela Mapstone to control the XBee 3G */

//#include "Rio.h"
#include <Arduino.h>
#include "XBee_dev.h"
#include "Rio_COAP.h"
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
        //tryagain = false;
        dontSleep = false;
        CoapSentAcknowledged = false;
        CoapSent203Received = false;
        Coap203Confirmed = false;
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
    uint8_t tryagain = 0;
    bool dontSleep = false;

    // COAP stuff
    bool CoapSentAcknowledged = false;     // Recipient acknowledges arrival
    bool CoapSent203Received = false;      // Recipient confirms successful parsing (via confirmable 2.03 message)
    bool Coap203Confirmed = false;         // 2.03 message confirmation sent

};

class CoapTransaction {
    public:
		CoapPacket packet;
};


/* variable and function declarations */

extern uint8_t assocCmd[];
extern ModemStatusResponse modemStatusResponse;
extern AtCommandResponse atResponse;
extern IPTxRequest ipRequest;
extern TxStatusResponse txResponse;
extern IPRxResponse ipResponse;
extern CellularStatus seqStatus;
extern uint32_t IP;
extern const uint16_t Port;
extern uint8_t protocol;

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
void zbIPResponseCb(IPRxResponse& ipResponse, uintptr_t);        // generic callback
void zbIPResponseCb_COAP(IPRxResponse& ipResponse, uintptr_t);        // specific for COAP
void zbTcpSendResponseCb(TxStatusResponse& txr, uintptr_t);
void zbAtResponseCb(AtCommandResponse& atr, uintptr_t);

void sendXbeeMessage(uint8_t*, uint16_t, char *host, uint8_t);
void sendXbeeMessage(uint16_t, char *host, uint8_t);

#endif
