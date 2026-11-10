# Arduino Settings

## Introduction

!!! tip "The open hardware principle"
    The Riverlabs loggers are built around the open hardware principle. This means that the hardware design, and the software that runs on it, are fully open source. You can therefore adapt the software (and hardware, if you are bit more adventurous) completely to your own needs. One of the most popular open hardware initiatives is [Arduino](https://www.arduino.cc). The Arduino "ecosystem" consists of many different hardware components (boards, shields, and other peripherals) and a wide range of software tools including an Integrated Development Environment (IDE). The Riverlabs hardware is fully compatible with the Arduino IDE, and indeed we recommend the use of Arduino to program the loggers becuase of its userfriendliness. However, many other options exist, which are provided in our full [Programming manual](../../programming/arduino-setup.md).

This section provides a quick overview of the main Arduino settings, and assumes that you are already (somewhat) familiar with Arduino. A more complete walk-through of the Arduino IDE and toolkit is provided in the [Programming manual](../../programming/arduino-setup.md).

Note that Riverlabs loggers don't have a USB port, but use a serial interface instead. You'll need a USB to Serial (TTL level) converter, commonly called an FTDI cable or FTDI board. Different types and models exist, but some popular options include the [original FTDI Cable](https://www.sparkfun.com/products/9717), or breakout boards such as the [Sparkfun FTDI Basic breakout](https://www.sparkfun.com/products/9873), which you can use in combination with a regular USB cable. FTDI cables and boards come in 3.3V or 5V versions. Riverlabs loggers work with both, but we recommend 3.3V as this is the internal voltage of the CPU.


## Required Libraries

Install these through **Tools → Manage Libraries**. They are the same libraries listed in the repository's `platformio.ini`.

- **Rtc** by Makuna (Michael C. Miller): real-time clock
- **SdFat** by Bill Greiman: SD card file system (make sure it is Bill Greiman's original version)
- **Low-Power** by Rocket Scream: sleep functions
- **AltSoftSerial** by Paul Stoffregen: an extra serial port (cellular sketches, `wari_v2.1` and `wari_lidar`)
- **LIDAR-Lite** by Garmin: lidar sensor
- **SPIMemory** by Prajwal Bhattaram: flash memory
- **RadioHead** by Mike McCauley: LoRa radio (only `wari_lidar_lora`)

Not every sketch needs every library, but installing them all does no harm.

## Board settings

The loggers use an ATmega328P at 8 MHz and 3.3 V, with a bootloader that uploads at 57600 baud. Choose:

1. **Tools → Board → Arduino AVR Boards → Arduino Pro or Pro Mini**
2. **Tools → Processor → ATmega328P (3.3V, 8 MHz)**

This board comes with the Arduino IDE and matches the repository's PlatformIO setting (`pro8MHzatmega328`). MiniCore (ATmega328, External 8 MHz) also works if its bootloader and upload speed settings match your logger. See [Arduino Setup](../../programming/arduino-setup.md) for details.

!!! warning "Before every upload"
    Unplug the Maxbotix sensor, and plug the FTDI cable in with the green wire on **GRN** and the black wire on **BLK**. See [Uploading Code](../../programming/uploading-code.md).

## Resources

- [Arduino Official Documentation](https://www.arduino.cc/en/Guide/HomePage)
- [Sparkfun FTDI Tutorial](https://learn.sparkfun.com/tutorials/serial-communication)
- [Common Issues](../troubleshooting/common-issues.md): Programming troubleshooting
