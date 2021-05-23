/* some notes:
 * 
 */

#include "Rio.h"
#include "LIDARLite.h"

#ifndef Rio_Sensors_h
#define Rio_Sensors_h

#if defined(__AVR_ATmega328P__)
    #include <SoftwareSerial.h>
    extern SoftwareSerial DebugSerial;
#endif

uint8_t readLidarLite(int16_t*, uint8_t, uint8_t, Stream &stream);

#endif
