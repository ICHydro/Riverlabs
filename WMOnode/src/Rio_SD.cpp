#include "Rio_SD.h"

#define countof(a) (sizeof(a) / sizeof(a[0]))

uint8_t dumpEEPROM() {

    uint16_t i, j;
    boolean readmore = true;
    boolean writefailure = false;
    byte headerbyte;

    SdFile dataFile;

    digitalWrite(WriteLED, HIGH);

    turnOnSDcard();

    if (!SD.begin(CS, SPI_FULL_SPEED)) {
        #if DEBUG > 0
            Serial.println(F("No card found"));
        #endif
        keep_SPCR=SPCR;
        turnOffSDcard();
        return 0;
    }

    #if DEBUG > 0
        Serial.println(F("SD card found"));
    #endif

    delay(100);  // probably not needed

    i = 0;

    while(readmore) {

        headerbyte = i2c_eeprom_read_byte(EEPROM_ADDR, i);

        for(j = 0; j < 8; j++) {
            if ((headerbyte >> j) & 0x1) {
                writefailure = !writeEEPROMline(i * 8 + j, dataFile);
                if(writefailure) {
                    readmore = false;
                    j = 8;
                }
            } else {
                readmore = false;
                j = 8;
            }
        }
        i++;
    }

    if(fileopen) {
        dataFile.close();   // returns 1 on success 
        fileopen = 0;       // set to 0 whatever the outcome of close() because the SD card will be powered off anyway.
    }
    #if DEBUG > 1
        Serial.println(F("Powering off SD card"));
    #endif
    keep_SPCR=SPCR;
    turnOffSDcard();
    digitalWrite(WriteLED, LOW);
    return (writefailure) ? 0 : 1;
}

uint8_t writeEEPROMline(uint16_t n, SdFile dataFile) {
    byte EEPromPage2[28];   // byte array to read a page from eeprom;
    RtcDateTime timestamp;  // pointer to convert bytes to time;
    int16_t *dataout;       // pointer to convert bytes back to integers
    char filename[] = "00000000.CSV";

    i2c_eeprom_read_buffer(EEPROM_ADDR, (n + EEPromHeaderSize) * EEPromPageSize, EEPromPage2, EEPromPageSize);

    timestamp = RtcDateTime(((uint32_t *)EEPromPage2)[0]);
    dataout = (int16_t *)EEPromPage2;

    // open new file if needed
    if(!fileopen) {
        getFilename(timestamp, filename);
        fileopen = dataFile.open(filename, O_CREAT | O_WRITE | O_APPEND);
        day = timestamp.Day();
    } else {
        if(day != timestamp.Day()) {
            dataFile.close();
            getFilename(timestamp, filename);
            fileopen = dataFile.open(filename, O_CREAT | O_WRITE | O_APPEND);
            day = timestamp.Day();
        }
    }

    // write in file:
    if (!fileopen) {
        #if DEBUG > 0
            Serial.println(F("Error: could not open datafile"));
        #endif
        error(3, ErrorLED); // TODO: FAIL GRACIOUSLY -> TRY AGAIN LATER INSTEAD OF LOCKING UP
        return 0;
     } else {
        formatDateTime(timestamp);
        dataFile.print(datestring);
        for(n = 0; n < 12; n++) {
            dataFile.print(F(", "));
            dataFile.print(dataout[n+2]);
        }
        dataFile.println("");
        return 1;
    }
}

void turnOnSDcard() {
    pinMode(SDpowerPin, OUTPUT);
    digitalWrite(SDpowerPin, HIGH);          //turn on the SD ground line
    delay(6);                                // let the card settle
    // some cards will fail on power-up unless SS is pulled up  ( &  D0/MISO as well? )
    DDRB = DDRB | (1<<DDB5) | (1<<DDB3) | (1<<DDB2); // set SCLK(D13), MOSI(D11) & SS(D10) as OUTPUT
    // Note: | is an OR operation so  the other pins stay as they were.                (MISO stays as INPUT) 
    PORTB = PORTB & ~(1<<DDB5);  // disable pin 13 SCLK pull-up – leave pull-up in place on the other 3 lines
    power_spi_enable();                      // enable the SPI clock 
    SPCR=keep_SPCR;                          // enable SPI peripheral
    delay(10);
    SDcardOn = true;       // just a flag
}

void turnOffSDcard() {
    delay(6);
    SPCR = 0;                                         // disable SPI
    power_spi_disable();                     // disable SPI clock
    DDRB &= ~((1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB2));   // set All SPI pins to INPUT
    PORTB |= ((1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB2));     // set ALL SPI pins HIGH (~30k pullup)
    // Note: you must disconnect the LED on pin 13 or you’ll bleed current through the limit resistor
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF); // wait 1 second before pulling the plug!
    delay(6);
    pinMode(SDpowerPin, OUTPUT); digitalWrite(SDpowerPin, LOW);
    delay(6);
    SDcardOn = false;
}

void getFilename(const RtcDateTime& Rtc, char* filename) {
    int year = Rtc.Year();
    int month = Rtc.Month();
    int day = Rtc.Day();
    filename[0] = '2';
    filename[1] = '0';
    filename[2] = (year-2000)/10 + '0';
    filename[3] = year%10 + '0';
    filename[4] = month/10 + '0';
    filename[5] = month%10 + '0';
    filename[6] = day/10 + '0';
    filename[7] = day%10 + '0';
    filename[8] = '.';
    filename[9] = 'C';
    filename[10] = 'S';
    filename[11] = 'V';
    return;
}
