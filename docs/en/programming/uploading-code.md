# Programming the Riverlabs loggers

!!! Abstract "Overview"
    The previous section explains how to install the Arduino IDE, choose the board and install the libraries. This section covers connecting the logger, uploading the code and checking that it works.

The whole process at a glance:

[![Uploading code, one step at a time](../assets/images/setup/07-upload-steps.svg)](../assets/images/setup/07-upload-steps.svg){ title="Click to open full size" }

## Connect the logger to your computer

### 1. Switch off and unplug the sensor

Set the power switch to **OFF**, then pull out the white Maxbotix plug.

[![Why the Maxbotix must be unplugged](../assets/images/setup/06-why-unplug-sensor.svg)](../assets/images/setup/06-why-unplug-sensor.svg){ title="Click to open full size" }

The sensor and the FTDI cable share the chip's only hardware serial input (the RX pin). While the sensor is plugged in, it keeps sending distance readings down that wire, so the bootloader hears a mix of your code and the sensor's readings. The upload then fails with `stk500_getsync(): not in sync`. This is the most common upload problem.

!!! warning "Only plug or unplug parts when the power is off"
    The same goes for the XBee modem and the SD card. Switch OFF and unplug the FTDI cable before you take them out or put them in.

### 2. Plug in the FTDI cable the right way round

The Riverlabs loggers do not have a USB port. Instead, an FTDI cable connects to the 6-pin header on the side of the board.

<!-- TODO: add a photo of the FTDI cable plugged into a real board -->
[![Plug in the FTDI cable](../assets/images/setup/05-ftdi-wiring.svg)](../assets/images/setup/05-ftdi-wiring.svg){ title="Click to open full size" }

Line up the **green wire** with **GRN** and the **black wire** with **BLK**, then push the plug straight onto the pins.

| Wire | Signal | What it does |
|---|---|---|
| Black | GND | 0 V, the shared ground |
| Brown | CTS | Not used |
| Red | VCC | Power from USB (5 V). The board only uses it when the switch is OFF. |
| Orange | TX | The computer talks to the board |
| Yellow | RX | The board talks to the computer |
| Green | RTS or DTR (the name depends on the cable) | Resets the board, so the bootloader starts at the right moment |

!!! danger "Check the direction before you plug in the USB end"
    The plug fits both ways round, and the wrong way puts the wires on the wrong pins. If the board does not respond, unplug the USB end, check that green meets GRN, and try again.

### 3. Power the board

[![The power switch](../assets/images/setup/04-power-switch.svg)](../assets/images/setup/04-power-switch.svg){ title="Click to open full size" }

- **OFF (ftdi):** the USB port powers the board through the cable. This works with or without a battery fitted, because the switch disconnects the battery.
- **ON (batt):** the battery powers the board and the cable only carries data. Use this only if the battery is charged.

The switch never connects the battery and the USB power together, so the battery cannot be damaged by the USB port.

### 4. Select the port

Plug the USB end into the computer. The port names vary by operating system:

- **macOS:** `/dev/cu.usbserial-XXXXXXXX`
- **Linux:** `/dev/ttyUSB0`
- **Windows:** `COM3`, `COM4`, etc.

Choose it in **Tools → Port**. If several ports show up, unplug the FTDI cable and check the list, then plug it in and check again. The port that disappears and comes back is the logger. On a Mac you can also run `ls /dev/cu.usbserial*` in Terminal.

!!! tip "Close anything that is using the port"
    Only one program can use the serial port at a time. Close the Serial Monitor, and any other terminal program, before you upload.

## Upload the logger code

### 1. Open the right sketch

Each logger model has its own sketch. Use the serial number (starting with `RL`) printed on the logger to choose:

[![Which sketch do I upload?](../assets/images/setup/02-which-sketch.svg)](../assets/images/setup/02-which-sketch.svg){ title="Click to open full size" }

!!! warning "The distance reads -1? Check the sketch first"
    Different board versions switch the sensor on in different ways. On a `wari_3G_v2` logger, the matching sketch read the distance fine, but the `wari_4G` sketch read **-1** every time because it switched the sensor power the other way. If you only ever see -1, make sure the sketch matches your board before you suspect the sensor.

### 2. Change the settings

The settings are at the top of each sketch, in lines that start with `#define`. Only change the value, never the name.

**SD loggers** (for example `wari_v2.1`):

```cpp
#define INTERVAL 5        // minutes between readings
#define NREADINGS 10      // readings taken each time
#define FLUSHAFTER 288    // readings kept before writing to SD (288 x 5 min = 24 h)
#define LOGGERID "RL000XXX"
#define OPTIBOOT          // leave this on: it turns on the watchdog
```

**Cellular loggers** (for example `wari_3G_v2` and `wari_4G`):

```cpp
#define READ_INTERVAL 5            // minutes between readings
#define SEND_INTERVAL 1            // hours between sends
#define HOST "your.server.address" // the ThingsBoard server
#define ACCESSTOKEN "your-token"   // this logger's ThingsBoard token (keep it secret)
#define LOGGERID "RL000XXX"
#define APN ""                     // your SIM card's APN, if it needs one
#define NTP                        // set the clock from the internet at start-up
//#define LTE                      // leave off: for LTE-M / NB-IoT modems use wari_4G
                                   // (switching LTE on in wari_3G_v2 currently fails to compile)
#define OPTIBOOT                   // leave this on
```

For the Riverlabs data platform, `HOST` is `riverflow.io`. Every setting is explained on [Settings Explained and Common Questions](../operation/quick-start/settings-explained.md).

!!! warning "Keep your access token private"
    Never share or commit a sketch with your real `ACCESSTOKEN` in it.

### 3. Upload

1. Click the **→ Upload** button.
2. Wait. First the code compiles, then "Uploading..." appears, and finally **"Done uploading"**.
3. The board restarts by itself.

### Check the start-up messages

Keep the FTDI cable plugged in and open the Serial Monitor at **115200 baud**. Opening it restarts the board through the green wire, so you see the messages from the very beginning. This is normal.

A healthy cellular logger prints something like this:

```text
This is Riverlabs Wari_3G_v2 (optiboot), compiled on Nov  6 2026
Logger ID: RL000726
Current time is 2026/11/06 19:57:23 UTC
Measuring the following variables:
- Distance (Maxbotix ultrasound sensor)
Measurement interval (minutes): 5
Buffer starts at position: 6
Cellular XBee detected. Setting APN
AI status = FF
AI status = FF
AI status = FF
AI status = FF
AI status = 22
AI status = 22
AI status = 23
Callback - Modem Status Response: 2
AI status = 0
Callback - IP Address: AC E8 9D 1B
NTP received. Clock is set to: 11/06/2026 19:57:51
```

Check that:

- The **sketch name** and **logger ID** are right.
- The **time** is correct in UTC.
- **Cellular:** the "AI status" codes change from `FF` to `22` (sometimes `23`) and then `0`, and an IP address appears. On our test logger this took 20 to 35 seconds. Then "NTP received" appears.
- **At every reading:** the battery voltage in mV (`VBatt = 3815`), the temperature in hundredths of a degree (`T = 2400` means 24.00 °C) and the distance in mm (`Distance (ultrasound) = 1076`).
- **Cellular, at the top of the hour:** `CONNACK received: accepted`, then one `PUBACK received` for each page of readings sent. That means the server received the data.

The Maxbotix MB7389 reads from 300 to 5000 mm. **5000** means nothing was detected within range. **-1** means fewer than half the readings came back from the sensor, usually because it is unplugged, not powered, or the sketch does not match the board. While the sensor is unplugged for the upload, -1 is expected. To see real readings, switch OFF, plug the sensor in, switch ON and watch again. Do not upload while the sensor is plugged in.

### 4. Finish

1. Unplug the FTDI cable.
2. With the switch still **OFF**, plug the sensor back in.
3. Switch to **ON (batt)** and watch for the start-up flash. See the [LED guide](../operation/quick-start/setup-guide.md#4-read-the-led).

## Set the clock

This step is needed for SD loggers when the board is new, or after the CR1220 coin cell was taken out. The clock keeps the time as long as the coin cell is in place, even when new code is uploaded. Cellular loggers set their own clock from the internet (NTP) at every start-up, so they do not need this.

The clock is set with the `set_clock.ino` sketch in the repository. It takes the time from your computer when the sketch is compiled, and writes it to the clock **every time the sketch starts**. That is why the order of the steps matters:

1. Open `set_clock/set_clock.ino`.
2. Set `TZ` to your **computer's** offset from UTC, in hours. For example, use `0` if your computer is on UK winter time, and `1` on UK summer time, Central European winter time or West Africa Time. The logger then keeps time in UTC, which we recommend for every logger.
3. **Open the Serial Monitor at 115200 baud before uploading.** Opening the Serial Monitor restarts the board, and every restart sets the clock again to the time of compiling. If you open it after the upload, the clock is set to a time that is already out of date.
4. Upload, and check the time that is printed.
5. **Straight away**, upload the logger sketch, so that a later restart cannot set the clock back.

!!! tip "Compensation for the upload delay"
    There are a few seconds between compiling and the sketch running on the logger. The sketch adds 12 seconds to make up for this. If your computer is faster or slower, you can change the number in this line:

    ```cpp
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__) - TZ * 3600 + 12;
    ```

---

## Troubleshooting

### `avrdude: stk500_getsync(): not in sync` or "programmer is not responding"

The computer cannot talk to the bootloader. Check, in this order:

1. **The sensor is unplugged.** This is the most common cause.
2. **The FTDI plug is the right way round:** green wire on GRN, black wire on BLK.
3. **The board has power:** switch OFF with the cable plugged in, or ON with a charged battery.
4. **The board setting:** Arduino Pro or Pro Mini, ATmega328P (3.3V, 8 MHz), and the right port.
5. **No other program has the port open.**
6. **Another USB port or cable.**

If none of this helps, the bootloader may be damaged. See [PlatformIO, Backup and Restore](advanced.md#if-the-bootloader-is-damaged).

### Error from `LowPower.h`, such as "Processor architecture is not supported"

The wrong board is selected. Choose **Arduino Pro or Pro Mini** with **ATmega328P (3.3V, 8 MHz)**.

### Error: "SdFat.h: No such file or directory"

A library is missing. Install it through **Tools → Manage Libraries**. The same goes for:

- `LowPower.h` → Low-Power by Rocket Scream
- `RtcDS3231.h` → RTC by Makuna
- `AltSoftSerial.h` → AltSoftSerial by Paul Stoffregen

### No port in the Tools menu

1. Unplug and replug the cable, and try another USB port.
2. On Windows, install the [FTDI VCP driver](https://ftdichip.com/drivers/vcp-drivers/).
3. Try another FTDI cable.
4. Restart the Arduino IDE.

### "Resource busy" or "port is open"

Another program is using the port. Close the Serial Monitor or the other terminal program.

### The Serial Monitor shows strange characters or nothing

- **Strange characters:** set the baud rate to 115200.
- **Nothing:** check the plug direction and the power, then press RESET.

### The code uploads but the logger does not work

1. **The battery is low.** It should be above 3.5 V.
2. **The sensor was not plugged back in.**
3. **The sketch does not match the board.** See the chart above.
4. **The error pattern keeps repeating and the start-up messages come back every 8 seconds or so.** The board is restarting over and over. On the `wari_4G` and `wari_lidar_cellular` sketches this happens when the XBee is missing or not answering. Switch OFF and fit the modem.

### Need more help?

- Check the [Common Issues](../operation/troubleshooting/common-issues.md) guide
- Review the [Diagnostic Flowcharts](../operation/troubleshooting/diagnostic-flowcharts.md)
- Contact support: info@riverlabs.uk
