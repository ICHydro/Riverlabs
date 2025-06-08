
/* some notes:
 * - physical page size is fixed, and you can only write a sequence of several bytes into a single page.
 * However, you can use smaller "pages" that are a divisor of the physical page size (e.g., 8 and 32)
 * 
 * We use the EEPROM as follows:
 * Header consist of
 *      - 1 page to store various parameters
 *      - "EEPromSDMaskSize" pages to keep track of what data have been written to SD card
 *      - "EEProm3GMaskSize" pages to keep track of what data have been sent via XBee 
 *      - "EEPromMaskSize" pages to keep track of which pages have data stored on them
 * remaining pages are used for measurements
 * 
 */

#ifndef Rio_EEPROM_h
#define Rio_EEPROM_h

#include "Rio.h"

void i2c_eeprom_write_byte( int deviceaddress, uint16_t eeaddress, byte data );
void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length );
byte i2c_eeprom_read_byte( int deviceaddress, uint16_t eeaddress );
void i2c_eeprom_read_buffer( int deviceaddress, unsigned int eeaddress, byte *buffer, int length );

#endif
