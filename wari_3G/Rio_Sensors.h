/* some notes:
 * 
 */

#include "Rio.h"
#include "LIDARLite.h"

#ifndef Rio_Sensors_h
#define Rio_Sensors_h

int16_t readMaxBotix(uint8_t, uint8_t, uint8_t, bool);
uint8_t readLidarLite(int16_t*, uint8_t, uint8_t, Stream &stream);

int EZread(Stream &stream);

#endif
