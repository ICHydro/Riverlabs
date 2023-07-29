/****************************************** 
 * wari.ino: Arduino script for the Riverlabs Wari water level sensor
 * Wouter Buytaert
 * 2020/08/02
 * 
 * Upload by selecting the board "Arduino Pro or Pro Mini"
 * and processor "Atmega 328 P (3.3V, 8 MHz)". 
 * 
 * By default, the logger integrates precipitation tips over 15 minutes.
 * This value written to the internal EEPROM chip,
 * which is flushed to the SD card every 24 hours. 
 * Edit the #define statements below to alter any of these parameters.
 * 
 * (c) Riverlabs UK, except for any code copied from other sources.
 * 
 * Distributed under an MIT licence
 ******************************************/


/******** OPERATING SPECIFICATIONS *******/

#define NREADINGS 1            // number of sensor readings taken per measurement
#define SENSOR PREC_TB         // PREC_TB = tipping bucket rain gauge
#define READ_INTERVAL 5       // Interval for precipitation readings, in minutes
#define DEBUG 1
#define FLUSHAFTER 4         // Number of readings before EEPROM is flushed to SD = (FLUSHAFTER x INTERVAL) minutes.
#define LOGGERID ""            // Logger ID. Set to whatever you like
#define TIPINTERVAL 300        // minimum time between precipitation tips, to filter out bounces


/******** DESIGN SPECIFICATIONS *******/

#define ErrorLED A2
#define WriteLED A2
#define SDpowerPin A0
#define CS 10
#define INTERRUPTPIN 2
#define VBATPIN A7
#define EEPROM_ADDR 0x51       
#define EEPromPageSize 32
#define EEPromHeaderSize 8
#define MAXPAGENUMBER 2048

#define countof(a) (sizeof(a) / sizeof(a[0]))

/* INCLUDES - make sure that they are installed! */

#include <LowPower.h>
#include <Wire.h>  
#include <RtcDS3231.h>
#include <SdFat.h>
#include <avr/power.h>
#include <RH_RF95.h>


/******** variable declarations **********/

const char LoggerID[] = LOGGERID;              // unique logger ID, to be used for data transmission and writing files.
uint32_t readstart = 0;
int16_t readings[NREADINGS];
uint8_t nread;
uint8_t n;
int8_t alarmcount = 0;
uint16_t i, j;

// variables needed in interrupt should be of type volatile.

volatile bool interruptFlag = false;
#if SENSOR == PREC_TB
    volatile uint16_t BucketTips = 0;
    volatile uint32_t TipTime = 0;
    volatile uint32_t PreviousTipTime = 0;
#endif

//Clock variables

RtcDS3231<TwoWire> Rtc(Wire);
RtcDateTime now;
uint32_t SecondsSince2000;
uint32_t day = 40;
uint32_t newday;
char datestring[20];
DS3231AlarmFlag flag;

//sd card related variables:

SdFat SD;
SdFile dataFile;
char filename[] = "00000000.CSV";
boolean SDcardOn;
byte keep_SPCR;
uint8_t status;
boolean fileopen = false;

//EEPROM variables

byte EEPromPage[EEPromPageSize]; // byte array to read a page from eeprom;
uint16_t eeaddress = 0;          // page address, starts after header
boolean flusheeprom = false;

/************** functions ****************/

// interrupt function. Should be as short as possible

void InterruptServiceRoutine() {
    interruptFlag = true;
}

// too much for an interruptfunction?

void count() {
    TipTime = millis();
    if((unsigned long) (TipTime - PreviousTipTime) > TIPINTERVAL) BucketTips++;
    PreviousTipTime = TipTime;
}

// eeprom write functions from the arduino website:
// https://playground.arduino.cc/Code/I2CEEPROM/

void i2c_eeprom_write_byte( int deviceaddress, uint16_t eeaddress, byte data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write(eeaddress >> 8); // MSB
    Wire.write(eeaddress & 0xFF); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
    delay(5);
}
  
void i2c_eeprom_write_page( int deviceaddress, unsigned int eeaddresspage, byte* data, byte length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < length; c++)
      Wire.write(data[c]);
    Wire.endTransmission();
}

byte i2c_eeprom_read_byte( int deviceaddress, uint16_t eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
}

void i2c_eeprom_read_buffer( int deviceaddress, uint16_t eeaddress, byte *buffer, int length ) {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,length);
    int c = 0;
    for ( c = 0; c < length; c++ )
        if (Wire.available()) buffer[c] = Wire.read();
}


// Error code function from Adafruit

void error(uint8_t errno, uint8_t errpin) {
    uint8_t j;
    for(j=0; j<3; j++) {              // repeat 3 times
        uint8_t i;
        for (i=0; i<errno; i++) {
            digitalWrite(errpin, HIGH);
            delay(100);
            digitalWrite(errpin, LOW);
            delay(100);
        }
        for (i=errno; i<10; i++) {
            delay(200);
        }
    }
}

// Create file name 

void getFilename(const RtcDateTime& Rtc) {
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

uint8_t dumpEEPROM() {

    uint16_t i, j;
    boolean readmore = true;
    boolean writefailure = false;
    byte headerbyte;
    digitalWrite(WriteLED, HIGH);

    turnOnSDcard();
    
    if (!SD.begin(CS, SPI_FULL_SPEED)) {
        #ifdef DEBUG   
            Serial.println("Card failed, or not present");
        #endif
        return 0;
    }

    #ifdef DEBUG   
        Serial.println("SD card found.");
    #endif

    delay(100);  // probably not needed

    i = 0;

    while(readmore) {
      
        headerbyte = i2c_eeprom_read_byte(EEPROM_ADDR, i);
        
        for(j = 0; j < 8; j++) {
            if ((headerbyte >> j) & 0x1) {
                writefailure = !writeEEPROMline(i * 8 + j);
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
    #ifdef DEBUG
        Serial.println(F("Powering off SD card"));
    #endif
    keep_SPCR=SPCR;
    turnOffSDcard(); 
    digitalWrite(WriteLED, LOW);
    return (writefailure) ? 0 : 1;
}

uint8_t writeEEPROMline(uint16_t n) {
    byte EEPromPage2[28];   // byte array to read a page from eeprom;
    RtcDateTime timestamp;  // pointer to convert bytes to time;
    int16_t *dataout;       // pointer to convert bytes back to integers
    
    i2c_eeprom_read_buffer(EEPROM_ADDR, (n + EEPromHeaderSize) * EEPromPageSize, EEPromPage2, EEPromPageSize);
    
    timestamp = RtcDateTime(((uint32_t *)EEPromPage2)[0]);
    dataout = (int16_t *)EEPromPage2;

    // open new file if needed
    if(!fileopen) {
        getFilename(timestamp);
        fileopen = dataFile.open(filename, O_CREAT | O_WRITE | O_APPEND);
        day = timestamp.Day();
    } else {
        if(day != timestamp.Day()) {
            dataFile.close();
            getFilename(timestamp);
            fileopen = dataFile.open(filename, O_CREAT | O_WRITE | O_APPEND);
        }
    }

    // write in file: 
    if (!fileopen) {
        #ifdef DEBUG
            Serial.println("Error: could not open datafile");
        #endif
        error(3, ErrorLED); // TODO: FAIL GRACIOUSLY -> TRY AGAIN LATER INSTEAD OF LOCKING UP
        return 0;
     } else {
        formatDateTime(timestamp);
        dataFile.print(datestring);
         #ifdef DEBUG
            Serial.print(datestring);
        #endif
        for(n = 0; n < (NREADINGS + 2); n++) {
            dataFile.print(F(", "));
            dataFile.print(dataout[n+2]);
             #ifdef DEBUG
                 Serial.print(F(", "));
                 Serial.print(dataout[n+2]);
             #endif
        }
        dataFile.println("");
        #ifdef DEBUG
            Serial.println("");
        #endif
        return 1;
    }        
}

// date time formatting from Rrc by Makuna

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

// SD card power management as copied from the Cave Pearl project

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

void resetEEPromHeader(int deviceaddress){
    // note: this is probably faster with a page write, but we can only write up to 30 bytes at the time with
    // the default wire library buffer, so we'd need two page writes per page to do this.
  
    for(uint16_t i = 0; i < (EEPromHeaderSize * EEPromPageSize); i++) {
        i2c_eeprom_write_byte(deviceaddress, i, 0x00);
    }
}

//************* setup *************//

void setup() 
{ 
    // reset readings    
            
    for (int i=0; i < NREADINGS; i++){
        readings[i] = -1;
    }
  
    // set the pins
    pinMode(INTERRUPTPIN, INPUT);
    pinMode(3, INPUT_PULLUP);
    pinMode(ErrorLED, OUTPUT);
    pinMode(WriteLED, OUTPUT);
    digitalWrite(WriteLED, LOW);
    digitalWrite(ErrorLED, LOW);

 
    // start wire for the EEPROM
    Wire.begin();
  
    // start clock. Note: clock initialization is done in separate script!
  
    Rtc.Begin();
    now = Rtc.GetDateTime();
      
    Rtc.Enable32kHzPin(false);
    Rtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmBoth); 
  
    // Alarm 2 set to trigger at the top of the minute
    // (Using alarm 2 with minute resolution - not sure why this is different from alarm 1)
    DS3231AlarmTwo alarm2(0,
                          0,
                          0, 
                          DS3231AlarmTwoControl_OncePerMinute);
    Rtc.SetAlarmTwo(alarm2);
  
    // throw away any old alarm state
    Rtc.LatchAlarmsTriggeredFlags();
  
    // setup external interupt 
    attachInterrupt(digitalPinToInterrupt(INTERRUPTPIN), InterruptServiceRoutine, FALLING);

    attachInterrupt(digitalPinToInterrupt(3), count, FALLING);
    
    
    #if DEBUG > 0
        Serial.begin(115200);
        Serial.println("");
        Serial.print(F("This is Riverlabs Wari"));
        #ifdef OPTIBOOT
            Serial.print(F(" (optiboot)"));
        #endif
        Serial.print(F(", compiled on "));
        Serial.println(__DATE__);
        Serial.print(F("Logger ID: "));
        Serial.println(LoggerID);
        Serial.print(F("Current time is "));
        formatDateTime(now);
        Serial.print(datestring);
        Serial.println(F(" GMT"));
        Serial.println(F("Measuring the following variables:"));
        #if SENSOR == PREC_TB
            Serial.println(F("- Precipitation (tipping bucket)"));
        #else
            Serial.print(F("Measurement interval (minutes): "));
            Serial.println(READ_INTERVAL);
        #endif
        Serial.println(F("Flushing EEPROM. This will also test SD card"));
    #endif

    RH_RF95 rf95(A1, 9);
    rf95.init();    // even if it fails, tt is still needed!!
    rf95.sleep();   // do not init (fails anyway on pin 9) but do put to sleep!

    pinMode(A1, OUTPUT);
    digitalWrite(A1, HIGH); // needed to avoid some ghost current through this pin, CS high disables the device anyway.

    digitalWrite(WriteLED, HIGH);

    if(dumpEEPROM()) {
        resetEEPromHeader(EEPROM_ADDR);
        #ifdef DEBUG
            Serial.println(F("EEPROM flushed."));
        #endif
    } else {
        #ifdef DEBUG
            Serial.println(F("Failed to flush EEPROM. SD card missing? Continuing anyway."));
        #endif
        error(3, ErrorLED);
    }

    digitalWrite(WriteLED, LOW);


}

//******** main routine **********//

void loop() {
  
    // Do not sleep within TIPINTERVAL ms of the previous bucket tip

    if(!interruptFlag && ((unsigned long)(millis() - PreviousTipTime) > TIPINTERVAL)) {
    
        #ifdef DEBUG
            Serial.print(F("S"));
            Serial.flush();           
        #endif

        // Check if alarm went off while we were doing something else,
        // just to avoid that we missed it and never wake up again...

        if(Rtc.LatchAlarmsTriggeredFlags()) {
            interruptFlag = true;
        } else {
            LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
        }

        /* if interrupt wakes us up, then we take action: */
    }

    if(interruptFlag) {                            // should always be true
        #ifdef DEBUG
            Serial.print(F("W"));
        #endif 

        cli();     
        interruptFlag = false;                     // reset the flag
        sei();

        flag = Rtc.LatchAlarmsTriggeredFlags();    // Switch off the alarm

        now = Rtc.GetDateTime();                   // get the current time from the clock

        if(now.Minute() % READ_INTERVAL == 0) {         // only take measurement if interval threshold is exceeded:         
        
            /************ take a reading **********/
      
            nread = 0;

            int16_t measuredvbat = analogRead(VBATPIN) * 2 * 3.3 / 1.024;
            int16_t temp = Rtc.GetTemperature().AsCentiDegC(); // take the temperature
            readings[0] = BucketTips;
            cli();
                BucketTips = 0;
            sei();

            #if DEBUG > 0
                formatDateTime(now);
                Serial.print(datestring);
                Serial.print(", ");
                n = 0;
                while(n < NREADINGS) {        
                    Serial.print(readings[n]);
                    Serial.print(F(", "));
                    n++;
                }
                Serial.print(((float) measuredvbat)/1000);
                Serial.print(F(", "));
                Serial.println(temp);
            #endif


            /*********** store values in EEPROM ***********/

            SecondsSince2000 = now.TotalSeconds();

            // prepare EEPromPage

            for(i = 0; i < 4; i++){
                EEPromPage[i] = ((byte *)&SecondsSince2000)[i];
            }
            for(i = 0; i < (NREADINGS * 2); i++) {
                EEPromPage[i+4] = ((byte *)readings)[i];
            }
            for(i = 0; i < 2; i++){
                EEPromPage[i+4+(NREADINGS*2)] = ((byte *)&measuredvbat)[i];
            }
            for(i = 0; i < 2; i++){
                EEPromPage[i+6+(NREADINGS*2)] = ((byte *)&temp)[i];
            }
            for(i = (8 + (NREADINGS*2)); i < EEPromPageSize; i++){    // fill rest of page with zeros
                EEPromPage[i] = 0;
            }

            // Write page. Note: only write 30 bits because the last 2 bits seem to be used by Wire library
                
            i2c_eeprom_write_page(EEPROM_ADDR, (eeaddress + EEPromHeaderSize) * EEPromPageSize, EEPromPage, 30);

            delay(50);  // to get rid of some issues with writing to EEPROM (first entry is sometimes wrong)
            
            // mark page as written (and not flushed) in EEPromHeader

            byte EEPROMbyte = i2c_eeprom_read_byte(EEPROM_ADDR, (uint16_t) eeaddress / 8); // read the relevant byte 
            bitWrite(EEPROMbyte, eeaddress % 8, 1);                                        // toggle relevant bit
            i2c_eeprom_write_byte(EEPROM_ADDR, (uint16_t) eeaddress / 8, EEPROMbyte);      // write relevant byte
            
            eeaddress++;
            
            delay(5);

            /******** reset readings *****/    
          
            for (i = 0; i < NREADINGS; i++){
                readings[i] = -1;
            }

            /********* flush EEPROM to SD card when full **********/
            
            if(eeaddress >= FLUSHAFTER) {
              flusheeprom = true;
            }

            if(flusheeprom) {
                if(dumpEEPROM()) {
                    resetEEPromHeader(EEPROM_ADDR);
                    eeaddress = 0;
                    flusheeprom = false;
                }
            }

            // avoid memory overflow - just cycle memory

            if(eeaddress > (MAXPAGENUMBER - EEPromHeaderSize)) {
                eeaddress = 0;
            }
        }                                     // end of threshold if
    }                                         // end interruptflag
}                                             // end loop()
