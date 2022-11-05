/* some notes:
 * 
 */

#include "Rio.h"

extern byte keep_SPCR;
extern bool SDcardOn;

#ifndef Rio_SD_h
#define Rio_SD_h

void turnOnSDcard();
void turnOffSDcard();

#endif