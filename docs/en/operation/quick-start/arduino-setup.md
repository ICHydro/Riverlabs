# Arduino Settings

## Introduction

!!! tip "The open hardware principle"
    The Riverlabs loggers are built around the open hardware principle. This means that the hardware design, and the software that runs on it, are fully open source. You can therefore adapt the software (and hardware, if you are bit more adventurous) completely to your own needs. One of the most popular open hardware initiatives is [Arduino](https://www.arduino.cc). The Arduino "ecosystem" consists of many different hardware components (boards, shields, and other peripherals) and a wide range of software tools including an Integrated Development Environment (IDE). The Riverlabs hardware is fully compatible with the Arduino IDE, and indeed we recommend the use of Arduino to program the loggers becuase of its userfriendliness. However, many other options exist, which are provided in our full [Programming manual](../../../programming/arduino-setup/).

This section provides a quick overview of the main Arduino settings, and assumes that you are already (somewhat) familiar with Arduino. A more complete walk-through of the Arduino IDE and toolkit is provided in the [Programming manual](../../../programming/arduino-setup/).

Note that Riverlabs loggers don't have a USB port, but use a serial interface instead. You'll need a USB to Serial (TTL level) converter, commonly called an FTDI cable or FTDI board. Different types and models exist, but some popular options include the [original FTDI Cable](https://www.sparkfun.com/products/9717), or breakout boards such as the [Sparkfun FTDI Basic breakout](https://www.sparkfun.com/products/9873), which you can use in combination with a regular USB cable. FTDI cables and boards come in 3.3V or 5V versions. Riverlabs loggers work with both, but we recommend 3.3V as this is the internal voltage of the CPU.


## Required Libraries

The Riverlabs logger requires the following external libraries:

- **RTC by Makuna** by Michael C. Miller: Real-time clock control
- **SdFat** by Bill Greiman: SD card file system
- **AltSoftSerial** by Paul Stoffregen: Software serial (for cellular models)
- **LowPower_LowPowerLab** by LowPowerLab: sleep functions
- **LIDAR-Lite** by Garmin: Lidar sensor functions
- **SPIMemory** by Prajwal Bhattaram: FLASH memory functions

All of them can installed through the Library Manager in the Arduino IDE.

!!! note "SdFat Version"
    Make sure to install the original **SdFat** library authored by **Bill Greiman**.

## Required board definitions

Riverlabs loggers use the **MiniCore** board definitions, which provides optimized support for ATmega328 microcontrollers:

1. In the "Additional Boards Manager URLs" field, add:
   ```
   https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json
   ```
2. Search and install **MiniCore by MCUdude** in the Bard Manager of the Arduino IDE.

For more information, visit the [MiniCore GitHub repository](https://github.com/MCUdude/MiniCore).

## Arduino IDE configuration

Before uploading code, you must configure the Arduino IDE with the correct board settings:

### Board Settings

1. Go to **Tools → Board → MiniCore** and select **ATmega328**
2. Configure the following settings in the **Tools** menu:
    - **Clock:** External 8 MHz
    - **BOD:** BOD 2.7 V
    - **EEPROM:** EEPROM retained
    - **Compiler LTO:** LTO Disabled
    - **Variant:** 328P / 328PA
    - **Bootloader:** Yes (UART0)

!!! warning "Critical Settings"
    The board MUST be set to **MiniCore → ATmega328** with **Clock: External 8 MHz**. Using the wrong settings will cause upload failures or runtime issues.

## Resources

- [Arduino Official Documentation](https://www.arduino.cc/en/Guide/HomePage)
- [Sparkfun FTDI Tutorial](https://learn.sparkfun.com/tutorials/serial-communication)
- [Common Issues](../troubleshooting/common-issues.md): Programming troubleshooting




