/* some notes:
 * 
 */

#include <SPIMemory.h>

#ifndef Rio_Flash_h
#define Rio_Flash_h

void writeToFlash(byte*, uint16_t, uint32_t);
uint32_t getFlashStart();

#endif