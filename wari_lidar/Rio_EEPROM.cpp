
#include "Rio_EEPROM.h"

void RioLogger::write2EEPROM(byte *EEPromPage, uint8_t length) {

    uint16_t offset[3];
    offset[0] = 1 * EEPromPageSize;
    offset[1] = offset[0] + EEPromSDMaskSize * EEPromPageSize;
    offset[2] = offset[1] + EEProm3GMaskSize * EEPromPageSize;

    // Write page. Note: only write 30 bits because the last 2 bits seem to be used by Wire library

    i2c_eeprom_write_page(EEPROM_ADDR, (eePageAddress + EEPromHeaderSize) * EEPromPageSize, EEPromPage, length);

    delay(50); // to get rid of some issues with writing to EEPROM (first entry sometimes wrong)

    // mark page as 1 in EEPromSDMask (offset 1 page), EEProm3GMask (offset 33 pages), and EEPromMask (offset 65 pages)

    for(uint8_t i = 0; i < 3; i++) {
        byte EEPROMbyte = i2c_eeprom_read_byte(EEPROM_ADDR, offset[i] + (uint16_t) eePageAddress / 8);  // read the relevant byte
        bitWrite(EEPROMbyte, eePageAddress % 8, 1);                                         // toggle relevant bit
        i2c_eeprom_write_byte(EEPROM_ADDR, offset[i] + (uint16_t) eePageAddress / 8, EEPROMbyte);       // write relevant byte
    }

    // wrap around if needed:
    eePageAddress = (eePageAddress < (maxpagenumber - EEPromHeaderSize)) ?  eePageAddress + 1 : 0;

    delay(5);
}

// Functions below copied from Arduino fora. Proper referencing needed.


void i2c_eeprom_write_byte(int deviceaddress, uint16_t eeaddress, byte data)
{
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write(eeaddress >> 8);   // MSB
  Wire.write(eeaddress & 0xFF); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
  delay(5); // TODO: check how long this needs to be. 3 seems too small
}

// Note that data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
// and this includes the two address bytes

void i2c_eeprom_write_page(int deviceaddress, unsigned int eeaddresspage, byte *data, byte length)
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddresspage >> 8));   // MSB
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB
  byte c;
  for (c = 0; c < length; c++)
    Wire.write(data[c]);
  Wire.endTransmission();
}

byte i2c_eeprom_read_byte(int deviceaddress, uint16_t eeaddress)
{
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available())
    rdata = Wire.read();
  return rdata;
}

void i2c_eeprom_read_buffer(int deviceaddress, uint16_t eeaddress, byte *buffer, int length)
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8));   // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, length);
  int c = 0;
  for (c = 0; c < length; c++)
    if (Wire.available())
      buffer[c] = Wire.read();
}
