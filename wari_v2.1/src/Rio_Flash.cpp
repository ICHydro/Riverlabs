
#include "Rio_Flash.h"

void writeToFlash(byte array[], uint16_t size, uint32_t start) {

    byte readarray[32];
    
    flash.writeByteArray(start * 32, array, size);
    Serial.print(F("Read string: "));
    flash.readByteArray(start * 32, readarray, 32);

    for(uint8_t i = 0; i < 32; i++) {
        Serial.print(readarray[i], HEX);
        Serial.print(" ");
    }
    Serial.println();
    Serial.flush();

}

uint32_t getFlashStart() {
    uint32_t i = 0;
    flash.begin();
    //flash.eraseChip();
    uint32_t size = flash.getCapacity() / 32;        // size in pages
    while((flash.readByte(i * 32) != 255) && (i++ < size)) {
        Serial.println(flash.readByte(i * 32));
    };
    flash.powerDown();
    if(i == size) {
        return 0;
    } else {
        return i;
    }
}
