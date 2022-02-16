/* some notes:
 * 
 */

#include "Rio.h"

#ifndef Rio_Sensors_h
#define Rio_Sensors_h

int16_t readMaxBotix(uint8_t, uint8_t, uint8_t, bool);

int EZread(Stream &stream);

#endif
