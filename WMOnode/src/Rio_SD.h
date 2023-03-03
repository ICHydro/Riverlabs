/* some notes:
 * 
 */

#include "Rio.h"

#ifndef Rio_SD_h
#define Rio_SD_h

extern boolean SDcardOn;
extern byte keep_SPCR;
extern uint8_t status;
extern bool fileopen;
extern uint32_t day;
extern SdFat SD;

void getFilename(const RtcDateTime&, char*);
void printDateTime(const RtcDateTime&);
uint8_t dumpEEPROM();
uint8_t writeEEPROMline(uint16_t n, SdFile);
void turnOnSDcard();
void turnOffSDcard();

#endif