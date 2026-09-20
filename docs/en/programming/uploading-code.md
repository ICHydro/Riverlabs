
# Programming the Riverlabs loggers

!!! Abstract "Overview"
    The previous section of this chapter explains how to install the Arduino IDE, the board definitions, and the required libraries. Once you have installed everything, you are ready to connect the logger to a PC and compile and upload the code. This is covered in this section.

## Connect the Logger to Your Computer

!!! warning "Disconnect Maxbotix Sensor First"
    Disconnect the Maxbotix ultrasonic sensor from the logger before programming. The sensor and FTDI cable use the same serial port, which will cause interference and prevent successful code upload. Unplug the white connector.

As explained in the previous section the wari loggers do not have an USB port, and will require an FTDI cable or board to connect to the 6-pin header on the side of the PCB. The PCB has markings to indicate the correct orientation: the pin labeled with "GRN" should align with the green wire of the cable, while the pin labeled "BLK" should align with the black wire of the cable.

<!-- insert picture -->

!!! danger "Check Orientation"
    Incorrect FTDI orientation can prevent programming or potentially damage components. Always verify the GRN/BLK markings before connecting.

Now connect the cable to an USB port on your computer. If your device recognizes the FTDI chip correctly, it should show up in the Arduino IDE under the menu **Tools → Port**. The port names vary by operating system:

- **macOS:** `/dev/cu.usbserial-XXXXXXXX`
- **Linux:** `/dev/ttyUSB0` or `/dev/ttyACM0`
- **Windows:** `COM3`, `COM4`, etc.

If no port appears, then there are a couple of things you can check:

- Verify FTDI drivers are installed (only for Windows)
- Try a different USB port on your computer

For detailed troubleshooting, the [Sparkfun FTDI Guide](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers) provides an in-depth overview of the use of an FTDI cable.

## Select the port and set the board in the Arduino IDE

Make sure the correct port is selected in the menu **Tools → Port**. If other devices are connected to your PC, then several ports may show up, for example 'COM3' and 'COM4'. To identify the correct port, you can unplug the FTDI cable, check the list of ports in the menu, and then plug it in and check again. The COM port to which the FTDI cable is assigned, should disappear and reappear.

Next, the correct board definitions need to be selected in the **Tools** menu:


1. **Board:** MiniCore → ATmega328
2. **Clock:** External 8 MHz
3. **BOD:** BOD 2.7V
4. **Variant:** 328P / 328PA
5. **Bootloader:** Yes (UART0)

If these settings do not appear in your menu, then you may need to install the Minicore board definitions via the boards manager.

!!! warning "Board Settings are Critical"
    The board MUST be set to **MiniCore → ATmega328** with **Clock: External 8 MHz**. Using the wrong settings can cause upload failures or runtime issues.

## Uploading the Riverlabs code

### Set the clock

This step is only needed if you start with a brand-new board, or when a new CR1220 coin battery is placed or the battery has been removed. The clock will retain the time as long as the coin battery is in place, even if new code is uploaded.

The clock can be set using the "set_clock.ino" sketch in the Riverlabs repository. Open the serial monitor **before* uploading the sketch to verify that the clock is set correctly. If nothing shows up in the serial monitor, then verify that the baud rate has been set to **115200 baud**. A nice tutorial of how to use the Serial Monitor in Arduino can be found on [Instructables](https://www.instructables.com/id/HOW-TO-use-the-ARDUINO-SERIAL-MONITOR/).

!!! info "Dealing with different time zones"
     If you set the clock using this sketch then it will be set to your computer's time zone. You can change this by altering the "TZ' variable in the sketch. Set `TZ` with the time offset from UTC in hours (e.g., `-5` for EST, `+1` for CET). In our research, we have adopted the procedure of always setting the clock in UCT (GMT). We operate loggers in different time zones and this minimizes the risk of confusion. If you have a preprogrammed Riverlabs logger, then it is very likely that the clock will be set in UTC time.

!!! tip "Compensation for Upload Delay"
    The sketch hardcode you PC's into the code during compilation, and this time is then set to the clock when the code is subsequently run on the logger. There is approximately a **10-second delay** between these steps. This time delay is accounted for in the sketch. As the delay depends on the specific PC, you can finetune the value if you like:

Adjusting the time delay compensation can be done in this line of the sketch:
    
    ```cpp
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__) - TZ * 3600 + 10;
    ```

### Uploading the Logger Code

1. **Download the appropriate sketch** for your logger model. We are in the process of unifying our logger code, but at this moment, each logger model has its own code. Please refer to the README in the repository to identify the correct code for your model.

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

## Troubleshooting with the Hardware Serial

When operating, the logger sends messages to the serial port during operation. This is useful to check its operation and address any issues. To view these messages, leave the FTDI cable connected to the logger, and open the serial monitoring. Set the baud rate to 115200 baud. When you open the serial monitor, the Arduino IDE will reset the logger, and you will see the debugging output from the start. This is normal behaviour. You will see information including:

- Boot messages
- Sensor readings
- RTC timestamps
- SD card status
- Telemetry transmission logs
- Error messages

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
    - Processor: ATmega328P (3.3 V, 8 MHz)
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

### Port Not Showing Up in the tools menu

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
    - Must be **3.3 V, 8 MHz** not 5 V, 16 MHz
    - Re-select correct processor and re-upload

2. **Battery voltage too low**
    - Check battery voltage with multimeter
    - Should be >3.5 V for reliable operation

3. **Forgot to reconnect sensor**
    - Plug sensor white connector back into board

---

### Need More Help?

- Check the [Common Issues](../operation/troubleshooting/common-issues.md) guide
- Review the [Diagnostic Flowcharts](../operation/troubleshooting/diagnostic-flowcharts.md)
- Contact support: info@riverlabs.uk