# Programming the Riverlabs loggers

## Introduction

!!! tip "The open hardware principle"
    The Riverlabs loggers are built around the open hardware principle. This means that the hardware design, and the software that runs on it, are fully open source. You can therefore adapt the software (and hardware, if you are bit more adventurous) completely to your own needs. One of the most popular open hardware initiatives is [Arduino](https://www.arduino.cc). The Arduino "ecosystem" consists of many different hardware components (boards, shields, and other peripherals) and a wide range of software tools including an Integrated Development Environment (IDE). The Riverlabs hardware is fully compatible with the Arduino IDE, and indeed we recommend the use of Arduino to program the loggers because of its ease of use.

For most users, we recommend using the Arduino IDE to program Riverlabs loggers. [Arduino](https://www.arduino.cc/) is a fantastic [open source hardware](https://en.wikipedia.org/wiki/Open-source_hardware) ecosystem, and the [Arduino IDE](https://www.arduino.cc/en/software) is a user-friendly software development environment to write and compile computer code that runs on embedded processors such as those that power the Riverlabs loggers. 

If you do not like the Arduino IDE or find it too limited, then we recommend using [PlatformIO](https://platformio.org/), which is compatible with many advanced IDEs such as VSCode, Atom, and even Emacs. Just make sure that you install [MiniCore for PlatformIO](https://github.com/MCUdude/MiniCore/blob/master/PlatformIO.md), as this is the bootloader and Arduino core that the Riverlabs loggers use.

We hope to provide more details on how to use PlatformIO in this guide, but for now the documentation follows the use of the Arduino IDE.

## Hardware requirements

One of the main advantages of urboot and similar bootloaders is that you do not need an expensive hardware programmer to program the CPU. Instead, you can connect the board directly to an USB port. That is, if the CPU has built-in USB functionality. That is not the case for the Atmega328 CPU that the Riverlabs logger use, and therefore you will still need one piece of hardware, which is a USB to Serial converter, also known as an FTDI cable:

![FTDI Cable Connection](../../assets/images/FTDICable.png)<br>
*FTDI cable showing the 6-pin connector with colour-coded wires*

Some examples of cables and breakout boards are the following:

- **FTDI Cable**: Direct USB connection (e.g., [Sparkfun FTDI Cable](https://www.sparkfun.com/products/9717))
- **FTDI Breakout Board**: Small board requiring micro-USB cable (e.g., [Sparkfun FTDI Basic](https://www.sparkfun.com/products/9873))

FTDI cables/boards come in **3.3 V** or **5 V** versions. Riverlabs loggers are compatible with both, but **3.3 V is recommended**.

!!! tip "Installing FTDI Drivers"
    Follow [these instructions](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers) to install the FTDI drivers on your computer.

The FTDI cable (or similar breakout boards) connect to the 6-pin serial connector on the Riverlabs boards. Mind the direction: the "GRN" side of the connector should align with the green wire on the cable, and the "BLK" side of the connector should align with the black wire.

!!! warning "Power considerations" 
    The logger has a power switch, which is labelled "ON (Batt) / OFF (ftdi)". This means that the logger is supplied with power from the battery when on (as you would expect). In the off state, the logger will receive power from the ftdi cable, if one is connected, and otherwise will not have any power.
    This makes it possible to program the logger without a battery (as the logger will get power from the ftdi cable), which is convenient. But you can also program the logger when a battery is inserted, either in the OFF position (when the battery is disconnected, and the logger gets power from the ftdi cable) and in the ON position (when the logger will get power from the battery).
    This design also makes it impossible to connect the ftdi power and the battery power at the same time, which may damage the battery (as it may be forced with 5 V power from the USB port via the ftdi cable).
