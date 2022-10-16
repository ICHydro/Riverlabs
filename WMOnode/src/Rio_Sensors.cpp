
#include "Rio_Sensors.h"

uint8_t readLidarLite(int16_t* readings, uint8_t nreadings, uint8_t debug, Stream &DebugSerial) {

    LIDARLite myLidarLite;
    uint16_t reading;
    uint8_t i = 0;                     // number of attempts
    uint8_t j = 0;                     // number of successful readings (> 0)

    // initialize with 0
    memset(readings, 0, nreadings * 2); // readings is an int16_t array, so two bytes per position

    digitalWrite(WriteLED, HIGH);
    digitalWrite(SWITCH5V, HIGH);  
    delay(5);
    digitalWrite(Boost5V_on, HIGH);    // capacitor needs only 10ms to charge but values below 30 seem to affect lidar precision.
    delay(30);
    digitalWrite(LIDARONPIN, HIGH);
    delay(30);
    myLidarLite.begin(0, false);    
    reading = myLidarLite.distance();
    i++;
    if(reading > 0) {
        readings[j] = reading;
        j++;
    }
    while((j < nreadings) && (i < 50)) {           // Maximum 50 attempts
        reading = myLidarLite.distance(false);
        i++;
        if(reading > 0) {
            readings[j++] = reading;
        }
    }
    digitalWrite(LIDARONPIN, LOW);
    delay(5);
    digitalWrite(SWITCH5V, LOW);
    delay(5);
    digitalWrite(Boost5V_on, LOW);
    delay(5);
    digitalWrite(WriteLED, LOW);

    if(debug > 1) {
        Serial.println();
        for(i = 0; i < nreadings; i++) {
            Serial.print(readings[i]);
            Serial.print(" ");
        }
        Serial.println();
    }
    return 1;   // success.
}
