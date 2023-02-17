/*
  Rio.h - Library for Riverlabs sensors
  Created by Wouter Buytaert
*/

#ifndef Rio_h
#define Rio_h

#define DEBUG 2

#define ErrorLED A2               
#define WriteLED A2
#define slaveSelect  10          // for SD card
#define SDpowerPin A0
#define Boost5V_on 7
#define MBONPIN 5 
#define LIDARONPIN 5
#define SWITCH5V A3           
#define CellularSleepPin A1        
#define interruptPin 2 
#define maxbotixPin 5
#define VBATPIN A7
#define DS18S20PIN A3
#define MBSERIALPIN 0
//#define DEBUGTX 4
#define XBEE_SLEEPPIN A1
#define XBEE_RESETPIN 4

#define M24512                 // type of EEPROM. M24512 
#define EEPROM_ADDR 0x51       // EEPROM I2C address: 0x57 for AT24c32 on clock; 0x51 or 81 for chip on PCB (Node_3G, SD boards)
#define EEPromPageSize 16      // max 32 bytes for the AT24c32, up to 128 bytes for M24512. Can be smaller (e.g., 8, 16)
#ifdef M24512
    #define maxpagenumber 65536 / EEPromPageSize      // maximum number of pages: 128 for AT24c32, 2048 for M24512 if page consists of 32 bytes (256 bits)
#endif
#define EEPromSDMaskSize 32     // mask to keep track of writeout to SD card in [number of pages].
                                // This should be bigger than (maxpagenumber - EEPromHeaderSize)/ (EEPromPageSize * 8)
                                // TODO: calculate this automatically
#define EEProm3GMaskSize 32     // mask to keep track of what data have been sent out via 3G XBee
#define EEPromMaskSize 32       // mask to keep track of what pages have written data on them (used for cycling)
#define EEPromHeaderSize (1 + EEPromMaskSize + EEPromSDMaskSize + EEProm3GMaskSize)   // gives 3999 useable pages for M24512 with 16 byte pages (13 days at 5 min intervals)
#define MAXFIT 50               // maximum number of records that fits in the EEPROM; will depend on format and number of variables to be transmitted.
                                // TODO: can be calculated automatically
#define OFFSET3GMASK (1 + EEPromSDMaskSize) * EEPromPageSize    // starting position of 3GMASK in EEPROM
 

/******** includes *******/

#include <Arduino.h>
#include <RtcDS3231.h>
#include <Wire.h>
#include <SdFat.h>
#include <AltSoftSerial.h>
#include <LowPower.h>
#include <avr/power.h>
#include <avr/wdt.h>

class RioLogger
{
  public:
    RioLogger();
    void write2EEPROM(byte *, uint8_t length);
    uint8_t EEPROMaddress;
    uint16_t eePageAddress;
  private:
    // int _pin;
};

#include "Rio_EEPROM.h"
#include "Rio_xbee.h"
#include "Rio_Sensors.h"
#include "Rio_SD.h"
#include "Rio_Flash.h"

/* declaration of global variables */

extern RtcDS3231<TwoWire> Rtc;
extern SdFat SD;
extern char filename[];
extern char datestring[20];
extern boolean SDcardOn;
extern SdFile dataFile;
extern uint32_t day;
extern boolean fileopen;
extern RtcDateTime now;
extern uint16_t bufferSize;
extern uint16_t bufferSize2;
extern volatile bool interruptFlag;
extern int32_t startposition;
extern byte Eeprom3Gmask[];

/* function declarations */

void InterruptServiceRoutine();
void error(uint8_t, uint8_t);
void getFilename(const RtcDateTime&);
void formatDateTime(const RtcDateTime&);
void printDateTime(const RtcDateTime&);

void resetEEPromHeader(int);
void resetEEPROMSDMask(int);
uint8_t CreateEepromSendBuffer(uint16_t, byte*);
uint16_t CreateSendBuffer(uint16_t, byte*, uint8_t*, uint16_t);
void Reset3GBuffer(uint16_t, byte*);
void Reset3GBuffer(uint16_t);
int32_t getBufferStartPosition();
uint32_t getBufferEndPosition();

/* from https://playground.arduino.cc/Main/QuickStats */
int16_t median(int16_t samples[],int m);
void bubbleSort(int16_t A[],int len);

#endif
