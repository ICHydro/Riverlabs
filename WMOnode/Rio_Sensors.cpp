
#include "Rio_Sensors.h"

uint8_t readLidarLite(int16_t* readings, uint8_t nreadings, bool debug, Stream &DebugSerial) {

    LIDARLite myLidarLite;

    digitalWrite(WriteLED, HIGH);
    digitalWrite(SWITCH5V, HIGH);  
    delay(5);
    digitalWrite(Boost5V_on, HIGH);    // capacitor needs only 10ms to charge but values below 30 seem to affect lidar precision.
    delay(30);
    digitalWrite(LIDARONPIN, HIGH);
    delay(30);
    myLidarLite.begin(0, false);    
    readings[0] = myLidarLite.distance();
    if(debug) {
        DebugSerial.print(readings[0]);
    }
    for (uint8_t i=1; i < nreadings; i++) {
        readings[i] = myLidarLite.distance(false);
        if(debug) {
            DebugSerial.print(", ");
            DebugSerial.print(readings[i]);
        }
    }
    if(debug) {
        DebugSerial.println();
    }
    digitalWrite(LIDARONPIN, LOW);
    delay(5);
    digitalWrite(SWITCH5V, LOW);
    delay(5);
    digitalWrite(Boost5V_on, LOW);
    delay(5);
    digitalWrite(WriteLED, LOW);
    return 1;   // success.
}
