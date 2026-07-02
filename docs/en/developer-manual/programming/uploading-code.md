## Uploading the code

### Get the required libraries

The code uses the following external libraries that need to be installed separately:

* **RTC by Makuna** - Real-time clock control
* **SoftwareSerial** - Software-based serial communication (cellular models)
* **SdFat** - SD card file system (use original version by Bill Greiman)
* **AltSoftSerial** - Alternative software serial (cellular/lidar models only)
* **Rocketscream LowPower** - Low-power sleep modes

The first 4 libraries can be installed via the Arduino Library Manager:
1. Open Arduino IDE
2. Go to **Sketch → Include Library → Manage Libraries**
3. Search for each library by name
4. Click **Install**

!!! note "SdFat Version"
    Use the original SdFat library by Bill Greiman. If multiple versions appear, select the one authored by Bill Greiman.

The **Rocketscream LowPower** library is not available via the Library Manager. You will need to:
1. Download the code from the [Github page](https://github.com/rocketscream/Low-Power)
2. Extract the ZIP file
3. Move the folder to your Arduino libraries directory
4. Restart Arduino IDE

See [Manual Installation](https://www.arduino.cc/en/Guide/Libraries) on the Arduino website for detailed instructions.

### Power considerations

The logger has a power switch, which is labelled "ON (Batt) / OFF (ftdi)". This means that the logger is supplied with power from the battery when on (as you would expect). In the off state, the logger will receive power from the ftdi cable, if one is connected, and otherwise will not have any power.

This makes it possible to program the logger without a battery (as the logger will get power from the ftdi cable), which is convenient. But you can also program the logger when a battery is inserted, either in the OFF position (when the battery is disconnected, and the logger gets power from the ftdi cable) and in the ON position (when the logger will get power from the battery).

This design also makes it impossible to connect the ftdi power and the battery power at the same time, which may damage the battery (as it may be forced with 5V power from the USB port via the ftdi cable).

### Get an FTDI Cable

To minimize component cost, Riverlabs loggers do not have a USB connection. Instead they use a serial interface that requires a **USB to Serial (TTL level) converter**. The most common tools are:

- **FTDI Cable** - Direct USB connection (e.g., [Sparkfun FTDI Cable](https://www.sparkfun.com/products/9717))
- **FTDI Breakout Board** - Small board requiring micro-USB cable (e.g., [Sparkfun FTDI Basic](https://www.sparkfun.com/products/9873))

FTDI cables/boards come in **3.3V** or **5V** versions. Riverlabs loggers are compatible with both, but **3.3V is recommended**.

!!! tip "Installing FTDI Drivers"
    Follow [these instructions](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers) to install the FTDI drivers on your computer.

### Connect the Logger to Your Computer

!!! warning "Disconnect Sensor First"
    **CRITICAL:** Disconnect the Maxbotix ultrasonic sensor from the logger before programming. The sensor and FTDI cable use the same serial port, which will cause interference and prevent successful code upload. Unplug the white connector.

**Connection Steps:**

1. **Remove or disconnect the battery** (recommended for first-time uploads)
   - Set power switch to "OFF" position if battery is installed
   
2. **Identify FTDI pin orientation** on the logger board:
   - Look for **"GRN"** and **"BLK"** markings on the board
   - For cellular/lidar models: Black pin (GND) is on the side of the SD card slot
   
3. **Connect FTDI cable/board to FTDI pins:**
   - Green wire → "GRN" side
   - Black wire → "BLK" side
   - Yellow wire → RX
   - Orange wire → TX
   
4. **Plug FTDI cable/board into your computer's USB port**

!!! danger "Check Orientation Twice"
    Incorrect FTDI orientation can prevent programming or potentially damage components. Always verify the GRN/BLK markings before connecting.

### Set the Correct Board in the Arduino IDE

**Board Settings** (under the **Tools** menu):

1. **Board:** MiniCore → ATmega328
2. **Clock:** External 8 MHz
3. **BOD:** BOD 2.7V
4. **Variant:** 328P / 328PA
5. **Bootloader:** Yes (UART0)

!!! warning "Board Settings are Critical"
    The board MUST be set to **MiniCore → ATmega328** with **Clock: External 8 MHz**. Using the wrong settings can cause upload failures or runtime issues.

!!! info "MiniCore Installation"
    If you don't see MiniCore in your boards list, you need to install it via the Boards Manager. Add this URL in File → Preferences → Additional Boards Manager URLs:
    ```
    https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json
    ```
    Then install MiniCore from Tools → Board → Boards Manager.

**Select the Port:**

1. Go to **Tools → Port**
2. Select the port that appears after connecting the FTDI cable
3. Port names vary by operating system:
   - **macOS:** `/dev/cu.usbserial-XXXXXXXX`
   - **Linux:** `/dev/ttyUSB0` or `/dev/ttyACM0`
   - **Windows:** `COM3`, `COM4`, etc.

If no port appears:
- Verify FTDI drivers are installed
- Try a different USB port on your computer
- Check FTDI cable connection to logger

For detailed troubleshooting, see the [Sparkfun FTDI Guide](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers).

### Set the clock

This step is only needed when a new CR1220 coin battery is placed or the battery has been removed. The clock will retain the time as long as the coin battery is in place, even if new code is uploaded.

The clock can be set using the example script provided by the RTC library. In the Arduino IDE go to File -> Examples -> Rtc by Makuna -> RS3231_Simple.ino. Open the script. Open a Serial Monitor, set the baud rate to 57600, and hit the "upload" button. If all goes well, the monitor should show the correct time every 10 seconds.

A nice tutorial of how to use the Serial Monitor in Arduino can be found on [Instructables](https://www.instructables.com/id/HOW-TO-use-the-ARDUINO-SERIAL-MONITOR/).

!!! info "Time Zone Note"
    Riverlabs loggers are originally programmed in **UTC (GMT)** time zone. If you set the clock again, it will be set to your computer's time zone.

!!! tip "Compensate for Upload Delay"
    There is approximately a **10-second delay** between when code is compiled and when the microcontroller runs it. This means the clock will be 10 seconds slow.
    
    **To compensate**, modify the DS3231_Simple script:
    
    ```cpp
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__) + 10;
    ```
    
    **For different time zones**, use this formula:
    
    ```cpp
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__) - TZ * 3600 + 10;
    ```
    
    Replace `TZ` with your time offset in hours (e.g., `-5` for EST, `+1` for CET).

### Upload the Logger Code

1. **Download the appropriate sketch** for your logger model:
   - `wari.ino` - Wari Ultrasonic loggers
   - `wari_lidar.ino` - Wari Lidar loggers
   - `wari_3G.ino` - Wari with 3G cellular telemetry
   - `wari_4G.ino` - Wari with 4G cellular telemetry

2. **Open the sketch** in Arduino IDE

3. **Configure settings** in the code (if needed):
   - Measurement interval
   - Sensor calibration
   - Telemetry settings

4. **Click the Upload button** (→ arrow icon)

5. **Wait for upload to complete**
   - Progress bar will show compilation
   - Then "Uploading..." will appear
   - Success message: "Done uploading"

6. **Verify the upload:**
   - LED should light up for about 1 second
   - Logger is now programmed and ready

7. **Reconnect the sensor** (if you disconnected it earlier)

!!! success "Upload Complete"
    If the LED flashed, your logger is successfully programmed! You can now disconnect the FTDI cable and deploy your logger.

---

## Debugging with Hardware Serial

For advanced debugging, you can use the **DBG (debug) pins** on the logger board to monitor serial output without reprogramming.

### Hardware Setup

**What You Need:**
- FTDI cable or breakout board
- 2× breadboard jumper wires (female-to-female or male-to-female)

**Connection:**

1. Locate the **DBG pins** on the logger board (usually 2-pin header near edge)
2. Connect using jumper wires:
   - **DBG GND pin** → **Black wire** of FTDI cable
   - **DBG TX pin** → **Yellow wire (RX)** of FTDI cable
3. Leave FTDI cable **not connected** to main FTDI header
4. Power the logger normally (battery ON)

### Viewing Debug Output

1. Connect FTDI cable to computer
2. Open Arduino IDE **Serial Monitor** (Tools → Serial Monitor)
3. Set baud rate to **115200**
4. Reset the logger by pressing the reset button
5. Debug information will appear in the monitor

!!! tip "Debug Serial Baud Rate"
    Debug output uses **115200 baud**. Make sure the Serial Monitor is set to this rate, or you'll see garbled text.

**What You'll See:**
- Boot messages
- Sensor readings
- RTC timestamps
- SD card status
- Telemetry transmission logs
- Error messages

This is invaluable for troubleshooting issues in the field or during development.

---

## Troubleshooting Common Upload Issues

### Error: "This LowPower library only works on AVR processors"

**Cause:** Wrong board type selected in Arduino IDE.

**Solution:**
- Go to **Tools → Board → MiniCore**
- Select **ATmega328**
- Verify **Clock** is set to **External 8 MHz**

---

### Error: "programmer is not responding" or "avrdude: stk500_getsync()"

**Cause:** Communication issue between computer and logger.

**Solutions to try:**

1. **Check sensor is disconnected**
   - ⚠️ **Most common issue!** The Maxbotix sensor MUST be unplugged during upload
   - Unplug the white connector from the board

2. **Verify FTDI cable orientation**
   - Green wire → "GRN" marking
   - Black wire → "BLK" marking or SD card side
   - Re-seat the FTDI cable firmly

3. **Check board settings**
   - Board: Arduino Pro or Pro Mini
   - Processor: ATmega328P (3.3V, 8MHz)
   - Correct port selected

4. **Try different USB port** on your computer

5. **Check power**
   - Power switch in OFF position (powered by FTDI)
   - OR battery installed and switch ON
   - LED should show power when connected

---

### Error: "SdFat.h: No such file or directory"

**Cause:** Required library not installed.

**Solution:**
- Install missing library via **Sketch → Include Library → Manage Libraries**
- Search for the library name (e.g., "SdFat", "RTC by Makuna")
- Click **Install**
- Restart Arduino IDE

This error can occur for any missing library:
- `LowPower.h` → Install Rocketscream LowPower (manual installation required)
- `RtcDS3231.h` → Install RTC by Makuna
- `AltSoftSerial.h` → Install AltSoftSerial (cellular models only)

---

### Port Not Showing Up

**Cause:** FTDI drivers not installed or cable not detected.

**Solutions:**

1. **Install FTDI drivers**
   - [Download from FTDI website](https://ftdichip.com/drivers/vcp-drivers/)
   - Or follow [Sparkfun's guide](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers)

2. **Try a different FTDI cable** (cable may be faulty)

3. **Check cable is fully inserted** into USB port

4. **Restart Arduino IDE** after connecting cable

---

### Code Compiles But Doesn't Run

**Possible causes:**

1. **Wrong processor speed/voltage**
   - Must be **3.3V, 8MHz** not 5V, 16MHz
   - Re-select correct processor and re-upload

2. **Battery voltage too low**
   - Check battery voltage with multimeter
   - Should be >3.5V for reliable operation

3. **Forgot to reconnect sensor**
   - Plug sensor white connector back into board

---

### Need More Help?

- Check the [Common Issues](../../user-manual/troubleshooting/common-issues.md) guide
- Review the [Diagnostic Flowcharts](../../user-manual/troubleshooting/diagnostic-flowcharts.md)
- Contact support: info@riverlabs.uk