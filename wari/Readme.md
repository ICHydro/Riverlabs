# Wari Water Level Sensor

This is the Arduino code for the Riverlabs Wari water level sensor.

To upload the code, use the procedure outlined below.

## Uploading the code

### 1. Get an FTDI cable

To minimize component cost, the Wari logger does not have an USB connection. Instead it uses a serial interface that can be programmed with a USB to Serial (TTL level) converter. The most common tools are based on the FTDI chip and consist of either a small breakout board (such as the [Sparkfun board](https://www.sparkfun.com/products/9873)), which can be connected to your computer with a micro-USB cable, or an [FTDI cable](https://www.sparkfun.com/products/9717) that can be connected directly to your computer. FTDI boards and cables come in either 3.3V and 5V versions. The Wari is compatible with both, but we recommend the 3.3V version.

Follow [these instructions](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers) to install the drivers of the FTDI chip on your computer.

### 2. Get the Arduino IDE 

You can use either the desktop application or the web editor. Instructions to install the desktop app for various operation systems can be found on the [Arduino website](https://www.arduino.cc/en/Guide/HomePage).

### 3. Get the required libraries

The code uses the following external libraries that need to be installed separately:

* RTC by Makuna
* SdFat (we use the original version by Bill Greiman)
* Rocketscream LowPower

These libraries can be installed via the Arduino app, following the [instructions](https://www.arduino.cc/en/Guide/Libraries) on the Arduino website.

The Rocketscream LowPower libraru is not available via the Arduino app. You will need to download the code from the [Github page](https://github.com/rocketscream/Low-Power) and put it manually in your Arduino libraries folder (see Manual Installation on the [Arduino instructions page](https://www.arduino.cc/en/Guide/Libraries))

### 4. Connect the Wari to your computer with the FTDI cable or breakout board

* Set the power switch to "Off" or remove the battery from the board. The FTDI cable (and most breakout boards) supply power directly from the USB port to the Wari, which will interfere with the battery.
* Disconnect the sensor from the Wari by unplugging the white connector. The serial communication of the FTDI cable uses the same port, which causes interference.
* Plug the FTDI cable or breakout board onto the FTDI pins of the Wari board. Mind the direction: the board has indications "GRN" and "BLK" to show which side the green and black wires should go.

### 5. Set the board in the Arduino interface

Choose the following board settings in the Arduino IDE (under the "tools" menu):
* Board: Arduino Pro or Pro Mini
* Processor: Atmega328P (3.3V, 8MHz)

Select the right port. If the FTDI cable or breakout board is properly connected and the driver is installed, it should show up in the "ports" menu. The name depends on the operating system, for instance "/dev/ttyUSB0" under linux. For a more detailed guide and troubleshooting, see the [Sparkfun guide](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers).

### 6. Set the clock

This step is only needed when a new CR1220 coin battery is placed or the battery has been removed. The clock will retain the time as long as the coin battery is in place, even if new code is uploaded.

The clock can be set using the example script provided by the RTC library. In the Arduino IDE go to File -> Examples -> Rtc by Makuna -> RS3231_Simple.ino. Open the script. Open a Serial Monitor, set the baud rate to 57600, and hit the "upload" button. If all goes well, the monitor should show the correct time every 10 seconds.

A nice tutorial of how to use the Serial Monitor in Arduino can be found on [Instructables](https://www.instructables.com/id/HOW-TO-use-the-ARDUINO-SERIAL-MONITOR/).

IMPORTANT NOTE: the Wari loggers are originally programmed in the UTC (GMT) time zone. If you set the clock again, then the clock will be set in the time zone of your computer. 

LESS IMPORTANT NOTE: There is about a 10 second delay between the moment that the code is compiled on your computer, and the moment that your microcontroller will run it. This means that the clock on the logger will also be delayed with about 10s. To avoid this, you can change the code of the DS3231_Simple script to:

`RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__) + 10;`

You can also adjust this line if you want to program in a different time zone:

`RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__) - TZ * 3600 + 10;`

in which you replace 'TZ' by the time offset (in hours) between your computer's time and the desired time.


### 7. Upload the logger code

Download and open the "wari.ino" script, and hit the "upload" button. If the upload finishes successfully, the LED light will light up for about a second, and the logger is good to go.

Do not forget to reconnect the sensor to the board after finishing the upload.

## Debugging

For simple debugging, you can use the debug serial pinouts on the board and the FTDI cable. Connect the "GND" pin of the DBG pins on the Wari board to the black wire of the FTDI cable, and the other pin to the yellow (serial in) wire of the FTDI cable using breadboard jumper wires. Open the Serial Monitor in the Arduino app, set the baud rate to 115200 and reset the board by hitting the reset button. You should now see the debugging information appearing on the monitor.

## Troubleshooting

> when compiling the code, I get the error: "SdFat.h: No such file or directory"

The SdFat library is not properly installed. This may also happen for other libraries such as LowPower.h (Rocketscream library) or RtcDS3231.h (Rtc library).

> when compiling, I get the error "This LowPower library only works on AVR processors"

Make sure that you select the right board, i.e. "Arduino Pro or Pro Mini"

> When uploading the code, I get the error "programmer is not responding"

Make sure that:
* the Maxbotix sensor is disconnected. The sensor and the FTDI cable use the same serial port, and the sensor will interfere with the serial communication between the computer and the Wari if it is connected;
* you have selected the right board in the Arduino IDE;
* the FTDI cable is properly connected to the board. Mind the pin layout: the green wire should be on the side that says "GRN", and the black wire should be on the side that says "BLK".





