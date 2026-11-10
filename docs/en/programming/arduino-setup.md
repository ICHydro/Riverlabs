# Programming the Riverlabs loggers
 
## The Arduino IDE

!!! tip "New to Arduino?"
    If you want to learn more about Arduino, the [Arduino website](https://www.arduino.cc/en/Guide/Environment) has excellent tutorials and documentation. Although you can implement many of the examples from the Arduino website on the Riverlabs loggers, they do not always use the standard designs, for example for the LED and communications. Therefore, it may be useful to buy one of the many Arduino boards from suppliers such as [Arduino.cc](https://www.arduino.cc/en/Main/Products), Sparkfun, or Adafruit for learning and experimentation.

To install the Arduino IDE, follow the [instructions](https://www.arduino.cc/en/software/#ide) on the Arduino website.

The user interface of the Arduino IDE consists of two main windows. At the top is the editor, in which you can write and edit the code before uploading. Arduino uses a computer language that is very similar to C++. The screenshot below shows one of the many examples that comes with the installation, Blink.ino, which is a short sketch to control an LED on an Arduino board. This will not work out of the box on the Riverlabs boards because the LED on the board is connected to a different pin. To try it out, replace "LED_BUILTIN" to "A2".

Below the editor sits the information window, which displays the output of the compilation and uploading steps, including any errors that may occur during this process.

![Arduino IDE overview](../assets/images/arduino1.jpg "Arduino IDE Overview")

**Important toolbar buttons**

- ✓ **Verify**: Compiles your code to check for errors
- → **Upload**: Compiles and uploads code to your logger
- **Serial Monitor**: Shows real-time serial output from your logger

Of the menus, the "tool" menu is the most important. This is where you set the correct USB port to which the board is attached (menu item "board"), and you will also need to select the correct type of board, and sometimes the correct type of processor.

![Board settings in Arduino](../assets/images/arduino2.jpg "Arduino IDE Settings")

## Installing MiniCore Board Support

Riverlabs loggers use the **MiniCore** bootloader and board definitions, which provides optimized support for ATmega328 microcontrollers. The minicore board definitions are not installed by default with the Arduino IDE, but they can be added conveniently via the boards manager:

1. Open the Arduino IDE
2. Go to **File → Preferences**
3. In the "Additional Boards Manager URLs" field, add:
   ```
   https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json
   ```
4. Click **OK**
5. Go to **Tools → Board → Boards Manager**
6. Search for **MiniCore**
7. Click **Install** on the MiniCore entry by MCUdude
8. Close the Boards Manager

For more information, visit the [MiniCore GitHub repository](https://github.com/MCUdude/MiniCore).

## Installing additional libraries

The Riverlabs code relies on several external libraries, all of which can be installed via the libraries manager of the Arduino IDE:

1. Open the Arduino IDE
2. Go to **Tools -> Manage Libraries** or click on the libraries icon in the left toolbar.
3. Install the following external libraries:
    - **RTC by Makuna** by Michael C. Miller: Real-time clock control
    - **SdFat** by Bill Greiman: SD card file system
    - **AltSoftSerial** by Paul Stoffregen: Software serial (for cellular models)
    - **LowPower_LowPowerLab** by LowPowerLab: sleep functions
    - **LIDAR-Lite** by Garmin: Lidar sensor functions
    - **SPIMemory** by Prajwal Bhattaram: FLASH memory functions


!!! note "SdFat Version"
    Make sure to install the original **SdFat** library authored by **Bill Greiman**.

## Next steps

Now you are ready to [compile and upload code](uploading-code.md)!
