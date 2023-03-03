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

// date time formatting from Rtc by Makuna

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

uint8_t CreateEepromSendBuffer(uint16_t start, byte *mask) {

    byte EEPromPage[EEPromPageSize];
    uint16_t i, j, k, starth, startv, startt, starttime, index;
    char timestamp[] = "0000000000";
    char value[] = "00000";
    boolean readmore = true;
    uint8_t count = 0;
    int16_t *dataout;
  

    // read a block of bytes from the EEPROM3Gmask that encompasses MAXBIT fits,

   // TODO: cycle if we come to the end of the EEPROM

    for(i = 0; i < (2 + MAXFIT / 8); i++) {
        mask[i] = i2c_eeprom_read_byte(EEPROM_ADDR, OFFSET3GMASK + start/8 + i);
    }
    Serial.println("");
    
    // count how may records we'll copy. Note: this can probably be done faster, but works for now.
    // as avr boards do not support the C++ bitset, one potential alternative is to use a uint64_t as bit array and do byteshifting.

    for(i = 0; i < MAXFIT; i++) {
        index = (i + start % 8) / 8;                    // index of the mask byte
        j = (i + start) % 8;                            // index of the bit within the mask byte (0 - 7)
        if ((mask[index] >> j) & 0x1) { count++;}
    }

    if(count > MAXFIT) {count = MAXFIT;}                // should not happen

    if(count == 0) { return count; }                    // if there aren't any data to send, we can stop here.

    sprintf(timestamp, "%10lu", now + 946684800);       // note: long!
    EEPROM.write(bufferSize++, 0xFF);
    EEPROM.put(bufferSize, "{\"ts\": ");
    starttime = bufferSize + 7;
    bufferSize += 17;
    EEPROM.put(bufferSize, "000,\"values\":{\"h\":[");
    bufferSize += 19;
    starth = bufferSize;
    bufferSize += (count * 6 - 1);                      // skip the space in which h will be printed
    EEPROM.put(bufferSize, "],\"v\":[");
    bufferSize += 7;
    startv = bufferSize;
    bufferSize += (count * 6 - 1);                      // skip the space in which v will be printed
    EEPROM.put(bufferSize, "],\"t\":[");
    bufferSize += 7;
    startt = bufferSize;
    bufferSize += (count * 6 - 1);                      // skip the space in which t will be printed
    EEPROM.put(bufferSize, "]}}");
    bufferSize += 3;

    // write the data
    bool first = 1;

    Serial.print(F("Reading pages "));

    for(i = 0; i < MAXFIT; i++) {
        index = (i + start % 8) / 8;                    // index of the mask byte
        j = (i + start) % 8;                            // index of the bit within the mask byte (0 - 7)
        if ((mask[index] >> j) & 0x1) {

            uint16_t readsize = (EEPromPageSize > 30) ? 30 : EEPromPageSize;
            i2c_eeprom_read_buffer(EEPROM_ADDR, (i + start + EEPromHeaderSize) * EEPromPageSize, EEPromPage, readsize);
            if(first) { 
                sprintf(timestamp, "%10lu", ((uint32_t *)EEPromPage)[0] + 946684800);
                for(k = 0; k < 10; k++) {
                    EEPROM.write(starttime++, timestamp[k]);  // use write instead of put to avoid writing the null character
                }
            }
            dataout = (int16_t *)EEPromPage;
            sprintf(value, "%5d", dataout[4]);
            if(!first) {EEPROM.put(starth++, ",");}
            for(k = 0; k < 5; k++) {
                EEPROM.write(starth++, value[k]);       // use write instead of put to avoid writing the null character
            }
            sprintf(value, "%5d", dataout[2]);
            if(!first) {EEPROM.put(startv++, ",");}
            for(k = 0; k < 5; k++) {
                EEPROM.write(startv++, value[k]);       // use write instead of put to avoid writing the null character
            }
            sprintf(value, "%5d", dataout[3]);
            if(!first) {EEPROM.put(startt++, ",");}
            for(k = 0; k < 5; k++) {
                EEPROM.write(startt++, value[k]);       // use write instead of put to avoid writing the null character
            }
            first = 0;
        }
    }

    Serial.println("");

    return(count);                                      // number of pages that has been copied into the EEPROM send buffer

}


/*** non EEPROM version, sending one page at the time ****/

uint16_t CreateSendBuffer(uint16_t start, byte *mask2, uint8_t *buffer, uint16_t bufferSize) {

    byte EEPromPage[EEPromPageSize];
    uint16_t i, j, index;
    char string1[] = "{\"ts\":"; 
    char string2[] = "000,\"values\":{\"h\":";
    char string3[] = ",\"v\":";
    char string4[] = ",\"t\":";
    char string5[] = "}}";
    int16_t *dataout;

    Serial.print(F("Reading page "));
    Serial.println(start);
    uint16_t readsize = (EEPromPageSize > 30) ? 30 : EEPromPageSize;
    i2c_eeprom_read_buffer(EEPROM_ADDR, (start + EEPromHeaderSize) * EEPromPageSize, EEPromPage, readsize);
    dataout = (int16_t *)EEPromPage;

    memcpy(buffer + bufferSize, string1, 6);
    bufferSize += 6;
    sprintf((char*) (buffer + bufferSize), "%10lu", ((uint32_t *)EEPromPage)[0] + 946684800);   
    bufferSize += 10;
    memcpy(buffer + bufferSize, string2, 19);
    bufferSize += 18;
    sprintf((char*) (buffer + bufferSize), "%5d", dataout[4]);
    bufferSize += 5;
    memcpy(buffer + bufferSize, string3, 7);
    bufferSize += 5;
    sprintf((char*) (buffer + bufferSize), "%5d", dataout[2]);
    bufferSize += 5;
    memcpy(buffer + bufferSize, string4, 7);
    bufferSize += 5;
    sprintf((char*) (buffer + bufferSize), "%5d", dataout[3]);
    bufferSize += 5;
    memcpy(buffer + bufferSize, string5, 3);
    bufferSize += 2;
    return(bufferSize);
}

void Reset3GBuffer(uint16_t start, byte *oldmask) {

  // NOTE: make sure that this is compatible with the procedure followed in CreateEepromSendBuffer

    uint8_t count = 0; 
    uint16_t i, j, index;
    byte newmask[2 + MAXFIT / 8];                       // Note that this may be different form the old mask if more data were written in the meanwhile!

    // read the mask again, in case any bits have changed (new records created) after oldmask was created

    for(i = 0; i < (2 + MAXFIT / 8); i++) {
        newmask[i] = i2c_eeprom_read_byte(EEPROM_ADDR, OFFSET3GMASK + start/8 + i);
    }

    //toggle all bits to 0

    for(i = 0; i < MAXFIT; i++) {
        index = (i + start % 8) / 8;                    // index of the mask byte
        j = (i + start) % 8;                            // index of the bit within the mask byte (0 - 7)
        // if the value in oldmask was 1, then that means that it has been written to EEPROM and sent
        if((oldmask[index] >> j) & 0x1) {
            bitWrite(newmask[index], j, 0);
        }
    }  

    // Write buffer back to EEPROMSDMask
    for(i = 0; i < (2 + MAXFIT / 8); i++) {
        i2c_eeprom_write_byte(EEPROM_ADDR, OFFSET3GMASK + start/8 + i, newmask[i]);
    }

}

// case in which only 1 page is sent:

void Reset3GBuffer(uint16_t start) {

  // NOTE: make sure that this is compatible with the procedure followed in CreateEepromSendBuffer

    byte mask;

    // read the mask again, in case any bits have changed (new records created) after oldmask was created

    mask = i2c_eeprom_read_byte(EEPROM_ADDR, OFFSET3GMASK + start/8);
    bitWrite(mask, start % 8, 0);                       // toggle relevant bit
    i2c_eeprom_write_byte(EEPROM_ADDR, OFFSET3GMASK + start/8, mask);

}

// count bits in a byte, from https://stackoverflow.com/questions/9949935/calculate-number-of-bits-set-in-byte

uint8_t count_ones (uint8_t byte)
{
    static const uint8_t NIBBLE_LOOKUP [16] =
    {
        0, 1, 1, 2, 1, 2, 2, 3, 
        1, 2, 2, 3, 2, 3, 3, 4
    };
  return NIBBLE_LOOKUP[byte & 0x0F] + NIBBLE_LOOKUP[byte >> 4];
}


// find the first bit in the 3G mask that is not zero
// return -1 if the buffer is empty

int32_t getBufferStartPosition() {
    byte maskbyte; 
    for(uint32_t i = 0; i < (EEProm3GMaskSize * EEPromPageSize); i++) {
        maskbyte = i2c_eeprom_read_byte(EEPROM_ADDR, OFFSET3GMASK + i);
        if(maskbyte != 0) {
            for(uint8_t j = 0; j < 8; j++) {
                if((maskbyte >> j) & 0x1) {
                    return(i * 8 + j);
                }
            }
        }
    }
    return(-1);
}

// find the last bit in the 3G mask that is not zero

uint32_t getBufferEndPosition() {
    byte maskbyte; 
    for(uint32_t i = (EEProm3GMaskSize * EEPromPageSize); i-- > 0; ) {
        maskbyte = i2c_eeprom_read_byte(EEPROM_ADDR, OFFSET3GMASK + i);
        if(maskbyte != 0) {
            for(uint8_t j = 8; j-- > 0; ) {
                if((maskbyte >> j) & 0x1) {
                    return(i * 8 + j + 1);
                }
            }
        }
    }
    return(0);
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
