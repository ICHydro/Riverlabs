# Programming the Riverlabs loggers

## The Arduino IDE

!!! tip "New to Arduino?"
    If you want to learn more about Arduino, the [Arduino website](https://www.arduino.cc/en/Guide/Environment) has excellent tutorials and documentation. Although you can implement many of the examples from the Arduino website on the Riverlabs loggers, they do not always use the standard designs, for example for the LED and communications. Therefore, it may be useful to buy one of the many Arduino boards from suppliers such as [Arduino.cc](https://www.arduino.cc/en/Main/Products), Sparkfun, or Adafruit for learning and experimentation.

To install the Arduino IDE, follow the [instructions](https://www.arduino.cc/en/software/#ide) on the Arduino website.

The user interface of the Arduino IDE consists of two main windows. At the top is the editor, in which you can write and edit the code before uploading. Arduino uses a computer language that is very similar to C++. The screenshot below shows one of the many examples that comes with the installation, Blink.ino, which is a short sketch to control an LED on an Arduino board. This will not work out of the box on the Riverlabs boards because the LED on the board is connected to a different pin. To try it out, replace "LED_BUILTIN" with "A2".

Below the editor sits the information window, which displays the output of the compilation and uploading steps, including any errors that may occur during this process.

![Arduino IDE overview](../assets/images/arduino1.jpg "Arduino IDE Overview")

**Important toolbar buttons**

- ✓ **Verify**: Compiles your code to check for errors
- → **Upload**: Compiles and uploads code to your logger
- **Serial Monitor**: Shows real-time serial output from your logger

Of the menus, the **Tools** menu is the most important. This is where you choose the type of board and the port the FTDI cable is plugged into.

![Board settings in Arduino](../assets/images/arduino2.jpg "Arduino IDE Settings")

## The FTDI driver

- **macOS:** nothing to install. macOS already includes the FTDI driver. The port appears as `/dev/cu.usbserial-XXXX`. Always use the `cu.` port, not the `tty.` one.
- **Linux:** nothing to install. The port appears as `/dev/ttyUSB0`.
- **Windows:** if no COM port appears when the cable is plugged in, install the [FTDI VCP driver](https://ftdichip.com/drivers/vcp-drivers/).

## Choosing the board

The Riverlabs loggers use an ATmega328P running at 8 MHz and 3.3 V, with a bootloader that accepts uploads at **57600 baud**. The simplest board setting that matches this comes with the Arduino IDE itself, so you do not need to install anything extra:

1. Go to **Tools → Board → Arduino AVR Boards → Arduino Pro or Pro Mini**
2. Go to **Tools → Processor → ATmega328P (3.3V, 8 MHz)**

This is also the board that the repository's PlatformIO configuration uses (`pro8MHzatmega328`). We have tested uploads at this speed on one `wari_3G_v2` logger, which had the Optiboot 8.0 bootloader.

!!! note "Using MiniCore instead"
    Earlier versions of this guide used the **MiniCore** board package (Tools → Board → MiniCore → ATmega328, Clock: External 8 MHz, Variant: 328P / 328PA, Bootloader: Yes (UART0)). MiniCore works as long as its bootloader and upload speed settings match the bootloader on your logger. If an upload fails with `not in sync` even though the sensor is unplugged and the cable is the right way round, switch to the Pro Mini setting above.

    To install MiniCore, add `https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json` under **File → Preferences → Additional Boards Manager URLs**, then install **MiniCore by MCUdude** in **Tools → Board → Boards Manager**.

!!! danger "Do not use Burn Bootloader"
    The **Tools → Burn Bootloader** command needs a separate ISP programmer and rewrites the chip's fuses (its basic hardware settings). It is never needed for normal uploads.

## Installing the libraries

The Riverlabs code relies on several external libraries, all of which can be installed through the Library Manager:

1. Go to **Tools → Manage Libraries**, or click the libraries icon in the left toolbar.
2. Search for and install each of these:
    - **Rtc** by Makuna (Michael C. Miller): the real-time clock
    - **SdFat** by Bill Greiman: the SD card file system (make sure it is Bill Greiman's original version)
    - **Low-Power** by Rocket Scream: the sleep functions
    - **AltSoftSerial** by Paul Stoffregen: an extra serial port, used by the cellular sketches, `wari_v2.1` and `wari_lidar`
    - **LIDAR-Lite** by Garmin: the lidar sensor
    - **SPIMemory** by Prajwal Bhattaram: the flash memory chip
    - **RadioHead** by Mike McCauley: the LoRa radio, only needed for `wari_lidar_lora`

These are the same libraries that the repository's `platformio.ini` lists, so the Arduino IDE and PlatformIO build the same code. Not every sketch needs every library (`wari_v1`, `wari_v2.0` and `set_clock` need only the first three), but installing them all does no harm.

## Next steps

Now you are ready to [compile and upload code](uploading-code.md)! If you prefer the command line, or want to back up the code that is already on a logger, see [PlatformIO, Backup and Restore](advanced.md).
