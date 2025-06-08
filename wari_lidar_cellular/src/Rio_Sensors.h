/* some notes:
 * 
 */

#include "Rio.h"
#include <LIDARLite_v3HP.h>

#ifndef Rio_Sensors_h
#define Rio_Sensors_h

uint8_t readLidarLite(int16_t*, uint8_t, uint8_t, Stream &stream);

#endif
