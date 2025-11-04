/*
  Rio_SD.h - Library for riverflow.io sensors: functions that deal with SD card writing
  Created by Wouter Buytaert
*/

#include "Rio_EEPROM.h"

#ifndef SD_h
#define SD_h

#define countof(a) (sizeof(a) / sizeof(a[0]))

extern SdFat sd;
extern char filename[];
extern char datestring[20];
extern boolean SDcardOn;
extern byte keep_SPCR;


void turnOnSDcard();
void turnOffSDcard();
uint8_t writeEEPROMline(uint16_t);
uint8_t dumpEEPROM2();

#endif
