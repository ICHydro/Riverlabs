
#include "Rio_Sensors.h"

int16_t readMaxBotix(uint8_t serialPin, uint8_t powerPin, uint8_t nreadings, bool debug) {

    int16_t readings[nreadings];

    for (int i = 0; i < nreadings; i++){
      readings[i] = -1;
    }

    // select the type of serial on the basis of the serialPin

    if(serialPin == 0) {
        Serial.begin(9600);
    }

    digitalWrite(WriteLED, HIGH);
    digitalWrite(MBONPIN, LOW);
    delay(160);   // wait 160ms for startup and boot message to pass
    
    uint32_t readstart = millis();
    uint8_t n = 0;
         
    while ((millis() - readstart) <= 1800) {
        readings[n] = EZread(Serial);
        if(readings[n] > -2) {                   // Returning -2 indicates an error
            n++;
            if(n >= nreadings) {n = 0; }             //avoid overflow
        }
    }

    Serial.end();

    digitalWrite(MBONPIN, HIGH);
    digitalWrite(WriteLED, LOW);

    // Note: if more than half of the array is not filled because of read errors
    // then the median will be -1. Fine for now, because this just means that
    // not enough readings could be taken to return a reliable measurement.
            
    int16_t distance = median(readings, nreadings);

    return distance;
}

/* function copied from the TTL_ArduinoCode_Parsing example on the Arduino forum */

int EZread(Stream &stream) {
  
    int result;
    char inData[6];                             //char array to read data into
    int index = 0;
    unsigned long timer;
    boolean stringComplete = false;

    stream.flush();                           // Clear cache ready for next reading
    timer = millis();                           // use timer to time out after 1 sec.

    while ((stringComplete == false) && ((millis() - timer) < 1000)) {
        if (stream.available()) {    
            char rByte = stream.read();       //read serial input for "R" to mark start of data
            if(rByte == 'R') {
                while (index < 5) {             //read next 4 characters for range from sensor
                    if (stream.available()) {
                        inData[index] = stream.read();               
                        index++;                // Increment where to write next
                    }
                }
                inData[index] = 0x00;           //add a padding byte at end for atoi() function
            }
            rByte = 0;                          //reset the rByte ready for next reading
            index = 0;                          // Reset index ready for next reading
            stringComplete = true;              // Set completion of read to true
            result = atoi(inData);              // Changes string data into an integer for use
        }
    }
    if(stringComplete == false) result = -2;
    return result;
}

uint8_t readLidarLite(int16_t* readings, uint8_t nreadings, uint8_t debug, Stream &DebugSerial) {

    LIDARLite_v3HP myLidarLite;
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

    myLidarLite.configure(0);
    myLidarLite.waitForBusy();
    // 2. Trigger range measurement.
    myLidarLite.takeRange();
    // 3. Wait for busyFlag to indicate device is idle. This should be
    //    done before reading the distance data that was triggered above.
    myLidarLite.waitForBusy();
    // 4. Read new distance data from device registers
    reading = myLidarLite.readDistance();
    myLidarLite.takeRange(); // already start next reading

    i++;
    if(reading > 0) {
        readings[j] = reading;
        j++;
    }
    while((j < nreadings) && (i < 50)) {           // Maximum 50 attempts
        myLidarLite.waitForBusy();
        myLidarLite.takeRange();
        reading = myLidarLite.readDistance();
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

