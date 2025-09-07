/*******************************************************************************
 * Code for the Riverlabs feather lidar lora logger
 * 
 * Adapted by Riverlabs (Wouter Buytaert)
 * from the ttn-otaa-feather-us915.ino example of the MCCI LoraWAN LMIC library
 *
 * Learn Guide: https://learn.adafruit.com/the-things-network-for-feather
 *
 * Copyright (c) 2015 Thomas Telkamp and Matthijs Kooijman
 * Copyright (c) 2018 Terry Moore, MCCI
 * Copyright (c) 2018 Brent Rubell, Adafruit Industries
 *
 * Permission is hereby granted, free of charge, to anyone
 * obtaining a copy of this document and accompanying files,
 * to do whatever they want with them without any restriction,
 * including, but not limited to, copying, modification and redistribution.
 * NO WARRANTY OF ANY KIND IS PROVIDED.
 *******************************************************************************/

#define DEBUG 1
#define DebugSerial Serial

// Select here the sensor on the node:
#define LIDARLITE
#define Boost5V_on 6
#define LIDARONPIN 5
#define SWITCH5V A3
#define WriteLED 13
#define NREADINGS 9
#define VBATPIN A7


// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 60*15;

#include <arduino_lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <Wire.h>
//#include <ArduinoLowPower.h>
#include "LIDARLite.h"

/******** variable declarations **********/

int16_t readings[NREADINGS];
uint32_t readstart = 0;
uint8_t nread;
uint8_t n;
bool next = false;
int16_t distance = -9999;
int16_t measuredvbat;

// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70.
static const u1_t PROGMEM APPEUI[8]= {  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

// This should also be in little endian format, see above.
static const u1_t PROGMEM DEVEUI[8]= { 0xec, 0x10, 0x12, 0x00, 0x00, 0xB6, 0x76, 0x98 };
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from the TTN console can be copied as-is.
static const u1_t PROGMEM APPKEY[16] = { 0xB3, 0xBA, 0xB9, 0x4B, 0x49, 0x7C, 0xF4, 0x11, 0x86, 0xEF, 0xA6, 0x4D, 0x18, 0xA5, 0xDB, 0xE1 };
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}

// payload to send to TTN gateway
static uint8_t payload[5];
static osjob_t sendjob;


// Pin mapping for Adafruit Feather M0 LoRa
// /!\ By default Adafruit Feather M0's pin 6 and DIO1 are not connected.
// Please ensure they are connected.
const lmic_pinmap lmic_pins = {
    .nss = 8,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 4,
    .dio = {3, 11, LMIC_UNUSED_PIN},
    .rxtx_rx_active = 0,
    .rssi_cal = 8,              // LBT cal for the Adafruit Feather M0 LoRa, in dB
    .spi_freq = 8000000,
};


void bubbleSort(int16_t A[],int len) {
  unsigned long newn;
  unsigned long n=len;
  int16_t temp=0;
  do {
    newn=1;
    for(int p=1;p<len;p++){
      if(A[p-1]>A[p]){
        temp=A[p];           //swap places in array
        A[p]=A[p-1];
        A[p-1]=temp;
        newn=p;
      } //end if
    } //end for
    n=newn;
  } while(n>1);
}

int16_t median(int16_t samples[],int m) //calculate the median
{
  //First bubble sort the values: https://en.wikipedia.org/wiki/Bubble_sort
  int16_t sorted[m];   //Define and initialize sorted array.
  int16_t temp=0.0;      //Temporary float for swapping elements

  for(int i=0;i<m;i++){
    sorted[i]=samples[i];
  }
  bubbleSort(sorted,m);  // Sort the values

  if (bitRead(m,0)==1) {  //If the last bit of a number is 1, it's odd. This is equivalent to "TRUE". Also use if m%2!=0.
    return sorted[m/2]; //If the number of data points is odd, return middle number.
  } else {    
    return (sorted[(m/2)-1]+sorted[m/2])/2; //If the number of data points is even, return avg of the middle two numbers.
  }
}

void printHex2(unsigned v) {
    v &= 0xff;
    if (v < 16)
        DebugSerial.print('0');
    DebugSerial.print(v, HEX);
}

void onEvent (ev_t ev) {
    DebugSerial.print(os_getTime());
    DebugSerial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            DebugSerial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            DebugSerial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            DebugSerial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            DebugSerial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            DebugSerial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            DebugSerial.println(F("EV_JOINED"));
            {
              u4_t netid = 0;
              devaddr_t devaddr = 0;
              u1_t nwkKey[16];
              u1_t artKey[16];
              LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
              DebugSerial.print("netid: ");
              DebugSerial.println(netid, DEC);
              DebugSerial.print("devaddr: ");
              DebugSerial.println(devaddr, HEX);
              DebugSerial.print("AppSKey: ");
              for (size_t i=0; i<sizeof(artKey); ++i) {
                if (i != 0)
                  DebugSerial.print("-");
                printHex2(artKey[i]);
              }
              DebugSerial.println("");
              DebugSerial.print("NwkSKey: ");
              for (size_t i=0; i<sizeof(nwkKey); ++i) {
                      if (i != 0)
                              DebugSerial.print("-");
                      printHex2(nwkKey[i]);
              }
              DebugSerial.println();
            }
            // Disable link check validation (automatically enabled
            // during join, but because slow data rates change max TX
      // size, we don't use it in this example.
            LMIC_setLinkCheckMode(0);
            break;
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_RFU1:
        ||     Serial.println(F("EV_RFU1"));
        ||     break;
        */
        case EV_JOIN_FAILED:
            DebugSerial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            DebugSerial.println(F("EV_REJOIN_FAILED"));
            break;
            break;
        case EV_TXCOMPLETE:
            DebugSerial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              DebugSerial.println(F("Received ack"));
            if (LMIC.dataLen) {
              DebugSerial.println(F("Received "));
              DebugSerial.println(LMIC.dataLen);
              DebugSerial.println(F(" bytes of payload"));
            }
            // Schedule next transmission
            //os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            next = true;
            break;
        case EV_LOST_TSYNC:
            DebugSerial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            DebugSerial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            DebugSerial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            DebugSerial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            DebugSerial.println(F("EV_LINK_ALIVE"));
            break;
        /*
        || This event is defined but not used in the code. No
        || point in wasting codespace on it.
        ||
        || case EV_SCAN_FOUND:
        ||    Serial.println(F("EV_SCAN_FOUND"));
        ||    break;
        */
        case EV_TXSTART:
            DebugSerial.println(F("EV_TXSTART"));
            break;
        case EV_TXCANCELED:
            DebugSerial.println(F("EV_TXCANCELED"));
            break;
        case EV_RXSTART:
            /* do not print anything -- it wrecks timing */
            break;
        case EV_JOIN_TXCOMPLETE:
            DebugSerial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
            break;

        default:
            DebugSerial.print(F("Unknown event: "));
            DebugSerial.println((unsigned) ev);
            break;
    }
}

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        DebugSerial.println(F("OP_TXRXPEND, not sending"));
    } else {

        #if DEBUG > 0
            DebugSerial.println(F("Sleep complete"));
        #endif

        readLidarLite(readings, NREADINGS, DEBUG);
        distance = median(readings, NREADINGS);
        measuredvbat = analogRead(VBATPIN) * 2 * 3.3 / 1.024;
        #if DEBUG > 1
            DebugSerial.println(distance);
            DebugSerial.println(measuredvbat);
            DebugSerial.println(distance, HEX);
            DebugSerial.println(measuredvbat, HEX);
        #endif

        // place the bytes into the payload
        payload[0] = lowByte(distance);
        payload[1] = highByte(distance);
        payload[2] = lowByte(measuredvbat);
        payload[3] = highByte(measuredvbat);

        // prepare upstream data transmission at the next possible time.
        // transmit on port 1 (the first parameter); you can use any value from 1 to 223 (others are reserved).
        // don't request an ack (the last parameter, if not zero, requests an ack from the network).
        // Remember, acks consume a lot of network resources; don't ask for an ack unless you really need it.
        LMIC_setTxData2(1, payload, sizeof(payload)-1, 0);
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

/************** functions ****************/

uint8_t readLidarLite(int16_t* readings, uint8_t nreadings, uint8_t debug) {

    LIDARLite myLidarLite;
    uint16_t reading;
    uint8_t i = 0;                     // number of attempts
    uint8_t j = 0;                     // number of successful readings (> 0)

    // initialize with 0
    memset(readings, 0, nreadings * 2); // readings is an int16_t array, so two bytes per position

    digitalWrite(WriteLED, HIGH);
    digitalWrite(SWITCH5V, HIGH);  
    delay(5);
    digitalWrite(Boost5V_on, HIGH);    // capacitor needs only 10ms to charge but values below 30 seem to affect lidar precision.
    delay(30);
    digitalWrite(LIDARONPIN, HIGH);
    delay(30);
    myLidarLite.begin(0, false);    
    reading = myLidarLite.distance();
    i++;
    if(reading > 0) {
        readings[j] = reading;
        j++;
    }
    while((j < nreadings) && (i < 50)) {           // Maximum 50 attempts
        reading = myLidarLite.distance(false);
        i++;
        if(reading > 0) {
            readings[j++] = reading;
        }
    }
    digitalWrite(LIDARONPIN, LOW);
    delay(5);
    digitalWrite(SWITCH5V, LOW);
    delay(5);
    digitalWrite(Boost5V_on, LOW);
    delay(5);
    digitalWrite(WriteLED, LOW);

    if(debug > 0) {
        DebugSerial.println();
        for(i = 0; i < nreadings; i++) {
            DebugSerial.print(readings[i]);
            DebugSerial.print(" ");
        }
        DebugSerial.println();
    }
    return 1;   // success.
}

void setup() {
    delay(1000);
    while (! DebugSerial);
    DebugSerial.begin(115200);
    DebugSerial.println(F("Starting"));

    pinMode(LIDARONPIN, OUTPUT);
    pinMode(Boost5V_on, OUTPUT);
    pinMode(SWITCH5V, OUTPUT);
    digitalWrite(SWITCH5V, LOW);
    digitalWrite(LIDARONPIN, LOW);
    digitalWrite(Boost5V_on, LOW);

    pinMode(WriteLED, OUTPUT);
    digitalWrite(WriteLED, LOW);
    pinMode(VBATPIN, INPUT);

    // LMIC init
    os_init();

    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
    // Disable link-check mode and ADR, because ADR tends to complicate testing.
    LMIC_setLinkCheckMode(0);
    // Set the data rate to Spreading Factor 7.  This is the fastest supported rate for 125 kHz channels, and it
    // minimizes air time and battery power. Set the transmission power to 14 dBi (25 mW).
    LMIC_setDrTxpow(DR_SF7,14);

    // Start job (sending automatically starts OTAA too)
    do_send(&sendjob);
}


void loop() {

  if (next == false) {

      os_runloop_once();

  } else {

    int sleepcycles = TX_INTERVAL / 8;  // calculate the number of sleepcycles (8s) given the TX_INTERVAL
    #if DEBUG > 1
        DebugSerial.prin t(F("Enter sleeping for "));
        DebugSerial.print(sleepcycles);
        DebugSerial.println(F(" cycles of 8 seconds"));
        DebugSerial.flush(); // give the serial print chance to complete
    #endif
    // Note: somehow the timer is off. Does it take the board 1s to fall asleep?
    for (int i=0; i<sleepcycles; i++) {
        //LowPower.sleep(7000);
        DebugSerial.print(".");
    }

    next = false;
    // start job
    do_send(&sendjob);
    #if DEBUG > 1
        Serial.println(LMIC.seqnoUp);
        Serial.flush();
    #endif
  }
}
