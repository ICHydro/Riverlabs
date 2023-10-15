
#include "Rio_Flash.h"

void turnOnFlash() {
    pinMode(FLASHPOWERPIN, OUTPUT);
    digitalWrite(FLASHPOWERPIN, HIGH);          //turn on the SD ground line
    delay(6);                                // let the card settle
    // some cards will fail on power-up unless SS is pulled up  ( &  D0/MISO as well? )
    DDRB = DDRB | (1<<DDB5) | (1<<DDB3) | (1<<DDB2); // set SCLK(D13), MOSI(D11) & SS(D10) as OUTPUT
    // Note: | is an OR operation so  the other pins stay as they were.                (MISO stays as INPUT) 
    PORTB = PORTB & ~(1<<DDB5);  // disable pin 13 SCLK pull-up – leave pull-up in place on the other 3 lines
    power_spi_enable();                      // enable the SPI clock 
    SPCR=keep_SPCR;                          // enable SPI peripheral
    delay(10);
}

void turnOffFlash() {
    delay(6);
    keep_SPCR=SPCR;
    SPCR = 0;                                         // disable SPI
    power_spi_disable();                     // disable SPI clock
    DDRB &= ~((1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB2));   // set All SPI pins to INPUT
    PORTB |= ((1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB2));     // set ALL SPI pins HIGH (~30k pullup)
    // Note: you must disconnect the LED on pin 13 or you’ll bleed current through the limit resistor
    LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF); // wait 1 second before pulling the plug!
    //delay(6);
    pinMode(FLASHPOWERPIN, OUTPUT); digitalWrite(FLASHPOWERPIN, LOW);
    delay(6);
} 

void write2Flash(byte array[], uint16_t size, uint32_t start) {
    turnOnFlash();
    flash.begin();
    flash.powerUp();
    
    flash.writeByteArray(start * PAGESIZE, buffer, size);
    Serial.print(F("Written to Flash page "));
    Serial.print(start);
    Serial.print(F(": "));
    flash.readByteArray(start * PAGESIZE, buffer, size);

    for(uint8_t i = 0; i < size; i++) {
        Serial.print(buffer[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    Serial.flush();

    flash.powerDown();
    turnOffFlash();
}

uint32_t getFlashStart() {
    uint32_t i = 0;
    turnOnSDcard();
    SPIFlash flash(6);
    flash.begin();
    //flash.eraseChip();
    uint32_t size = flash.getCapacity() / EEPromPageSize;        // size in pages
    while((flash.readByte(i * EEPromPageSize) != 255) && (i++ < size)) {
        //Serial.println(flash.readByte(i * EEPromPageSize));
    };
    flash.powerDown();
    turnOffSDcard();
    if(i == size) {
        return 0;
    } else {
        return i;
    }
}

uint8_t dumpEEPROM2FLASH() {

    uint16_t i, j;
    boolean readmore = true;
    boolean writefailure = false;
    byte headerbyte;

    SPIFlash flash = SPIFlash(6);

    digitalWrite(WriteLED, HIGH);

    turnOnSDcard();
    delay(10);  // let flash settle
    flash.begin();
    flash.powerUp();

    i = 0;

    while(readmore) {

        headerbyte = i2c_eeprom_read_byte(EEPROM_ADDR, i);

        for(j = 0; j < 8; j++) {
            if ((headerbyte >> j) & 0x1) {
              // TODO!!!!!!!!!!
                //writefailure = !writeEEPROMline(i * 8 + j);
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

    #if DEBUG > 0
        Serial.println(F("Powering off SD card"));
    #endif
    keep_SPCR=SPCR;
    turnOffSDcard();
    digitalWrite(WriteLED, LOW);
    return (writefailure) ? 0 : 1;
}
