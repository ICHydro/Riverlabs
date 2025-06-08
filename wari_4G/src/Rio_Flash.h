/* some notes:
 * 
 */

#include <SPIMemory.h>
#include "Rio.h"

#ifndef Rio_Flash_h
#define Rio_Flash_h

extern SPIFlash flash;
extern byte keep_SPCR;

void write2Flash(byte*, uint16_t, uint32_t);
uint32_t getFlashStart();
void turnOnSPI();
void turnOffSPI();

#endif
