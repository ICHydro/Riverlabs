
#include <Arduino.h>
#include <Wire.h>
#include <RtcDS3231.h>
#include "Rio.h"

#define countof(a) (sizeof(a) / sizeof(a[0]))

/* RioLogger constructor */

RioLogger::RioLogger() {
  //PayloadHeader1Length = 0;
  //PayloadHeader2Length = 0;
  eePageAddress = 0;      // starts after header
  //HasLidar = 0;
  //HasUltrasound = 0;
  //HasDS18S20 = 0;
}


/********** function definitions **********/

void InterruptServiceRoutine()
{
    interruptFlag = true;
}


/* Error code function  - adapted from Adafruit */

void error(uint8_t errno, uint8_t errpin) {
  uint16_t j = 0;
  while(j < 5) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(errpin, HIGH);
      delay(100);
      digitalWrite(errpin, LOW);
      delay(100);
    }
    delay(1000);
    j++;
  }
}

/* from https://playground.arduino.cc/Main/QuickStats */

void bubbleSort(int16_t A[],int len)
{
  unsigned long newn;
  unsigned long n=len;
  int16_t temp=0;
  do {
    newn=1;
    for(int p=1;p<len;p++){
      if(A[p-1]>A[p]){
        temp=A[p];           //swap places in array
        A[p]=A[p-1];
        A[p-1]=temp;
        newn=p;
      } //end if
    } //end for
    n=newn;
  } while(n>1);
}

int16_t median(int16_t samples[],int m) //calculate the median
{
  //First bubble sort the values: https://en.wikipedia.org/wiki/Bubble_sort
  int16_t sorted[m];   //Define and initialize sorted array.
  int16_t temp=0.0;      //Temporary float for swapping elements

  for(int i=0;i<m;i++){
    sorted[i]=samples[i];
  }
  bubbleSort(sorted,m);  // Sort the values

  if (bitRead(m,0)==1) {  //If the last bit of a number is 1, it's odd. This is equivalent to "TRUE". Also use if m%2!=0.
    return sorted[m/2]; //If the number of data points is odd, return middle number.
  } else {    
    return (sorted[(m/2)-1]+sorted[m/2])/2; //If the number of data points is even, return avg of the middle two numbers.
  }
}


void resetEEPromHeader(int deviceaddress){
    // todo: this can be sped up with a page write (16 pages, as library can do max 30

    for(uint16_t i = 0; i < (EEPromHeaderSize * EEPromPageSize); i++) {
        i2c_eeprom_write_byte(deviceaddress, i, 0x00);
    }
}

void resetEEPromSDMask(int deviceaddress){
    // todo: this can be sped up with a page write (16 pages, as library can do max 30
  
    for(uint16_t i = 0; i < (EEPromSDMaskSize * EEPromPageSize); i++) {
        uint8_t offset = 1 * EEPromPageSize;
        i2c_eeprom_write_byte(deviceaddress, offset + i, 0x00);
    }
}


void formatDateTime(const RtcDateTime& dt) {
    snprintf_P(datestring, 
    countof(datestring),
    PSTR("%04u/%02u/%02u %02u:%02u:%02u"),
    dt.Year(),
    dt.Month(), 
    dt.Day(),
    dt.Hour(),
    dt.Minute(),
    dt.Second() );
}

void printDateTime(const RtcDateTime& dt)
{
  char datestring[20];

  snprintf_P(datestring, 
      countof(datestring),
      PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
      dt.Month(),
      dt.Day(),
      dt.Year(),
      dt.Hour(),
      dt.Minute(),
      dt.Second() );
    Serial.print(datestring);
}
