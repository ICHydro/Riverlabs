
#include "Rio_Flash.h"

void writeToFlash(byte array[], uint16_t size, uint32_t start) {
    turnOnSDcard();
    SPIFlash flash = SPIFlash(6);
    flash.begin();
    flash.powerUp();

    byte readarray[EEPromPageSize];
    
    flash.writeByteArray(start * EEPromPageSize, array, size);
    Serial.print(F("Read string: "));
    flash.readByteArray(start * EEPromPageSize, readarray, EEPromPageSize);

    for(uint8_t i = 0; i < EEPromPageSize; i++) {
        Serial.print(readarray[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    Serial.flush();

    flash.powerDown();
    turnOffSDcard();
}

uint32_t getFlashStart() {
    uint32_t i = 0;
    turnOnSDcard();
    SPIFlash flash(6);
    flash.begin();
    //flash.eraseChip();
    uint32_t size = flash.getCapacity() / EEPromPageSize;        // size in pages
    while((flash.readByte(i * EEPromPageSize) != 255) && (i++ < size)) {
        Serial.println(flash.readByte(i * EEPromPageSize));
    };
    flash.powerDown();
    turnOffSDcard();
    if(i == size) {
        return 0;
    } else {
        return i;
    }
}