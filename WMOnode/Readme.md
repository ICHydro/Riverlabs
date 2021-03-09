# WMOnode Lidar-based Water Level Sensor

This is the Arduino code for the Riverlabs WMOnode water level sensor.

To upload the code, use the procedure outlined below.

## Uploading the code

### 1. Get an FTDI cable

To minimize component cost, the WMOnode logger does not have a USB connection. Instead it uses a serial interface that can be programmed with a USB to Serial (TTL level) converter. The most common tools are based on the FTDI chip and consist of either a small breakout board (such as the [Sparkfun board](https://www.sparkfun.com/products/9873)), which can be connected to your computer with a micro-USB cable, or an [FTDI cable](https://www.sparkfun.com/products/9717) that can be connected directly to your computer. FTDI boards and cables come in either 3.3V and 5V versions. WMOnode is compatible with both, but we recommend the 3.3V version.

Follow [these instructions](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers) to install the drivers of the FTDI chip on your computer.

### 2. Get the Arduino IDE 

You can use either the desktop application or the web editor. Instructions to install the desktop app for various operation systems can be found on the [Arduino website](https://www.arduino.cc/en/Guide/HomePage).

### 3. Get the required libraries

The code uses the following external libraries that need to be installed separately:

* RTC by Makuna
* Rocketscream LowPower

The first library can be installed via the Arduino app, following the [instructions](https://www.arduino.cc/en/Guide/Libraries) on the Arduino website.

The Rocketscream LowPower library is not available via the Arduino app. You will need to download the code from the [Github page](https://github.com/rocketscream/Low-Power) and put it manually in your Arduino libraries folder (see Manual Installation on the [Arduino instructions page](https://www.arduino.cc/en/Guide/Libraries))

### 4. Power considerations

The logger has a power switch, which is labelled "ON (Batt) / OFF (ftdi)". This means that the logger is supplied with power from the battery when on (as you would expect). In the off state, the logger will receive power from the ftdi cable, if one is connected, and otherwise will not have any power.

This makes it possible to program the logger without a battery (as the logger will get power from the ftdi cable), which is convenient. But you can also program the logger when a battery is inserted, either in the OFF position (when the battery is disconnected, and the logger gets power from the ftdi cable) and in the ON position (when the logger will get power from the battery).

This design also makes it impossible to connect the ftdi power and the battery power at the same time, which may damage the battery (as it may be forced with 5V power from the USB port via the ftdi cable).

### 5. Connect the WMOnode to your computer with the FTDI cable or breakout board

Plug the FTDI cable or breakout board onto the FTDI pins of the WMOnode board. The black pin (GND) is on the side of the SD card slot.

### 6. Set the board in the Arduino interface

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

Download and open the "WMOnode.ino" script, and hit the "upload" button. 

## Debugging

For simple debugging, you can use the debug serial pinouts on the board and the FTDI cable. Connect the "GND" pin of the DBG pins on the WMOnode to the black wire of the FTDI cable, and the other pin to the yellow (serial in) wire of the FTDI cable using breadboard jumper wires. Open the Serial Monitor in the Arduino app, set the baud rate to 115200 and reset the board by hitting the reset button. You should now see the debugging information appearing on the monitor.

## Troubleshooting


> when compiling, I get the error "This LowPower library only works on AVR processors"

Make sure that you select the right board, i.e. "Arduino Pro or Pro Mini"

> When uploading the code, I get the error "programmer is not responding"

Make sure that:
* you have selected the right board in the Arduino IDE;
* the FTDI cable is properly connected to the board. Mind the pin layout: the green wire should be on the outside and the black wire should be on the side of the SD card slot.





