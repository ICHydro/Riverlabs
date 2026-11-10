# Setup Guide: step by step

This page takes you from a logger in its box to one that is measuring in the field. Do the steps in order. Each one says **what** to do, **why**, and **how to know it worked**. You do not need to know electronics or coding.

!!! abstract "The short version"
    1. [Find the parts, put the batteries in and check the LED](#1-check-you-have-everything) (steps 1 to 4).
    2. [Decide if you need new code](#5-do-you-need-to-upload-code) (step 5). If not, [jump to step 12](#12-what-the-logger-does-all-day).
    3. [Pick the sketch, download it and change the settings](#6-pick-the-right-sketch) (steps 6 to 8).
    4. [Set up the computer once](#9-set-up-the-computer-one-time-only), then [upload](#10-upload-the-code) (steps 9 and 10). SD loggers may also need [the clock set](#11-set-the-clock-sd-loggers-only) (step 11).
    5. [Learn what the logger does, take the SD card out, and test on the bench](#12-what-the-logger-does-all-day) (steps 12 to 14).

    The step list is also in the menu on the left, so you can jump to any step.

??? info "New words? Open this list"
    | Word | What it means |
    |---|---|
    | **Board** | The green circuit board inside the box. |
    | **Sketch** | A program for the logger. Each logger model has its own sketch, for example `wari_3G_v2`. |
    | **Upload** | Sending a sketch from the computer into the logger. |
    | **Arduino IDE** | The free program on the computer that you use to change a sketch and upload it. |
    | **Compile** | When the computer turns a sketch into code the logger's chip can run. The Arduino IDE does this by itself when you click Upload. |
    | **Setting** | A line near the top of the sketch that you can change, such as how often to measure. |
    | **FTDI cable** | A cable with USB at one end and a 6-pin plug at the other. It connects the computer to the board. |
    | **Port** | The name the computer gives the cable, such as `/dev/cu.usbserial-A1B2C3` on a Mac or `COM3` on Windows. |
    | **Serial Monitor** | A window in the Arduino program that shows the messages the logger prints. |
    | **Baud** | How fast the cable talks. The logger's messages use **115200**. |
    | **XBee** | The plug-in mobile phone modem used by cellular loggers. |
    | **APN** | A short name your SIM card company gives you, so the modem can reach the internet. |
    | **Access token** | A secret password-like code that tells the server which logger is sending data. |
    | **UTC** | World time with no time zone and no summer time. The loggers keep time in UTC. |

---

## 1. Check you have everything

[![What you need](../../assets/images/setup/01-what-you-need.svg)](../../assets/images/setup/01-what-you-need.svg){ title="Click to open full size" }

- **The logger**, with its board inside the box.
- **The sensor.** Ultrasound loggers use a Maxbotix MB7389, which has a white plug. Lidar loggers use a Garmin LIDAR-Lite.
- **A main battery.** A rechargeable 3.7 V lithium-ion cell, size 18650. Charge it with a lithium-ion charger, not a Ni-MH one. Some models take other sizes: see the [Battery and Power Guide](../hardware/battery-power-guide.md).
- **A CR1220 coin cell** on the back of the board. It keeps the clock running when the main battery is out. It is usually already fitted.
- **Cellular loggers:** an XBee modem, a SIM card with a data plan, and an antenna.
- **SD loggers:** a microSD card formatted as FAT32.
- **Only if you will upload code:** a 3.3 V FTDI cable (for example the TTL-232R-3V3) and a computer.

---

## 2. Find the parts on the board

<!-- TODO: replace this drawing with a labelled photo of a real board -->
[![The logger board](../../assets/images/setup/03-board-map.svg)](../../assets/images/setup/03-board-map.svg){ title="Click to open full size" }

| Part | What it does |
|---|---|
| **FTDI header** | 6 pins on the edge. The FTDI cable plugs in here. On most boards the ends are labelled **BLK** and **GRN**. If yours has no labels, the black wire goes on the side nearest the SD card slot. |
| **Power switch** | Labelled **ON (batt)** and **OFF (ftdi)**. See below. |
| **RESET button** | Restarts the code. It does **not** delete data or settings. |
| **Red LED** | Tells you what the logger is doing (step 4). |
| **Sensor plug** | Where the white sensor plug goes. |
| **Battery holder** | For the main battery. The + and - ends are marked. |
| **XBee socket** | Cellular loggers only. The modem plugs in here. |
| **microSD slot** | Used by SD loggers. Cellular boards may have one too, but their code does not use it. |

### The power switch

[![The power switch](../../assets/images/setup/04-power-switch.svg)](../../assets/images/setup/04-power-switch.svg){ title="Click to open full size" }

- **ON (batt):** the battery powers the board. Use this in the field.
- **OFF (ftdi):** the battery is disconnected. If the FTDI cable is plugged in, the computer powers the board. With no cable, OFF means no power at all.

!!! warning "Golden rule: switch OFF before you plug or unplug anything"
    Always switch OFF (and unplug the FTDI cable) before you take out or put in the modem, the sensor plug or the SD card.

---

## 3. Put in the batteries and parts

!!! danger "Check the + and - before you push the battery in"
    Loggers without solar charging are protected against a battery put in backwards: they just do not work. Loggers **with** solar charging are not protected. A battery put in backwards damages them for good and can start a fire.

1. Switch **OFF**.
2. Put the main battery in, matching **+** to **+**.
3. Check the CR1220 coin cell is in its holder on the back. Without it the logger still works, but it forgets the time when the main battery is taken out.
4. **SD loggers:** push the microSD card gently into its slot until it is fully in, with its contacts facing outwards (away from the battery).
5. **Cellular loggers:** put the SIM card in the XBee, plug the XBee into its socket (match the shape of the socket), and connect the antenna.
6. Plug the sensor in.
7. Switch **ON** and watch the LED (the next step explains what you see).

---

## 4. Read the LED

[![What the red LED is telling you](../../assets/images/setup/08-led-codes.svg)](../../assets/images/setup/08-led-codes.svg){ title="Click to open full size" }

An error pattern is quick flashes, a 1 second pause, repeated 5 times. So "3 flashes" lasts about 8 seconds in total.

| What you see | What it means | What to do |
|---|---|---|
| **SD loggers:** the LED stays on for a while right after start-up | Saved readings are being copied to the SD card. The more readings, the longer it takes. | Wait until it goes dark. |
| **Cellular loggers:** one 1-second flash, usually 20 to 40 seconds after start-up | The logger connected to the internet and set its clock. | Nothing. |
| A short light at each reading (about 2 seconds on cellular ultrasound loggers) | The sensor is measuring. | Nothing. |
| 3 quick flashes, 5 times | SD: no card or a bad card. Cellular: the modem is not answering. | Switch OFF, reseat the card or modem, switch ON. |
| 2 quick flashes, 5 times (cellular) | The logger could not get the time from the internet. It keeps trying for 3 to 6 minutes first, so this pattern comes late. | Check the SIM, APN, antenna and signal. |
| The pattern comes back every 8 seconds or so | The logger keeps restarting. On `wari_4G` and `wari_lidar_cellular` this happens when the modem is missing. | Switch OFF and fit the modem (or the SD card). |
| Nothing at all | No power. | Check the battery and the switch. |

Press **RESET** to restart the logger and see the start-up again. It does not delete data or settings.

---

## 5. Do you need to upload code?

| Your situation | Upload code? | Which steps |
|---|---|---|
| The logger came programmed and you are happy with the settings | **No** | [Go to step 12](#12-what-the-logger-does-all-day) |
| You want to measure more or less often | Yes | [Steps 6 to 10](#6-pick-the-right-sketch) |
| You want to send data more or less often (cellular) | Yes | [Steps 6 to 10](#6-pick-the-right-sketch) |
| You use a different server, SIM card or modem | Yes | [Steps 6 to 10](#6-pick-the-right-sketch) |
| The logger is new and has no code | Yes | [Steps 6 to 10](#6-pick-the-right-sketch) |
| An SD logger's clock is wrong, or its coin cell was taken out | Yes | [Steps 9 to 11](#9-set-up-the-computer-one-time-only) |

---

## 6. Pick the right sketch

The serial number starts with `RL` and is printed on the logger.

[![Which sketch do I upload?](../../assets/images/setup/02-which-sketch.svg)](../../assets/images/setup/02-which-sketch.svg){ title="Click to open full size" }

**3G or 4G modem?** Read the label on the XBee. "3G" means `wari_3G` or `wari_3G_v2`. "LTE-M" or "NB-IoT" means `wari_4G`.

!!! warning "`wari_4G` is set up for NB-IoT only"
    The sketch tells the modem to use **NB-IoT only**, on frequency bands 8 and 20 (common in Europe). If your SIM or network only offers LTE-M, or uses other bands, it will not connect as it is. Ask Riverlabs before you upload. Many countries, including the UK, have switched off their 3G networks, so a 3G modem may no longer connect at all.

!!! warning "Fitting a 4G modem to a `wari_3G_v2` board?"
    The `wari_4G` sketch switches the sensor on in a different way. On our `wari_3G_v2` test board, `wari_4G` read the distance as **-1** every time. If you see this, ask Riverlabs before you go further.

---

## 7. Get the code

1. Go to the Riverlabs repository on GitHub: `https://github.com/ICHydro/Riverlabs`.
2. Click the green **Code** button, then **Download ZIP**.
3. Unzip it. On Windows, right-click the ZIP file and choose **Extract All**; do not open the sketch from inside the ZIP. On a Mac, double-click the ZIP file. You get a folder with one folder per sketch, for example `wari_3G_v2`.
4. Open that folder and double-click the file with the same name ending in `.ino` (for example `wari_3G_v2.ino`). It opens in the Arduino program.

!!! warning "Keep the folder together"
    Most sketches have a `src` folder next to the `.ino` file, and they need it. Do not move or copy the `.ino` file on its own, and do not rename the folder.

---

## 8. Change the settings

Settings are near the top of the sketch, in lines that start with `#define`:

```cpp
#define READ_INTERVAL 5        // Interval for sensor readings, in minutes
```

- `READ_INTERVAL` is the **name**. Never change it.
- `5` is the **value**. This is what you change.
- Everything after `//` is a note for people. The logger ignores it.
- Text goes inside quote marks, like `"RL000726"`.
- A line that starts with `//` is switched **off**. Delete the `//` to switch it on.

### 8.1 Cellular loggers: change these four lines

Find these four lines and put in your own values:

```cpp
#define HOST "riverflow.io"              // the server. riverflow.io is the Riverlabs platform
#define ACCESSTOKEN "paste-token-here"   // this logger's token, copied from its device page
#define LOGGERID "RL000726"              // the serial number printed on the logger
#define APN "your.apn.here"              // the APN for your SIM card
```

| Line | Where the value comes from |
|---|---|
| `HOST` | `riverflow.io` for the Riverlabs platform (you need an account from Riverlabs). For your own ThingsBoard, its address, without `https://`. |
| `ACCESSTOKEN` | Add a device for this logger on the server, open it and copy its access token. **Each logger has its own.** |
| `LOGGERID` | The serial number on the logger. **Each logger has its own.** |
| `APN` | Your SIM company's website ("APN settings"), or Riverlabs if the SIM came from them. |

!!! warning "Keep your access token secret"
    Anyone with the token can send data as your logger. Do not share the sketch with your real token in it, and never put it on GitHub.

### 8.2 SD loggers: nothing to change

SD loggers work with the settings as they are. Some SD sketches have a `LOGGERID` line. You can put the serial number in it, like `#define LOGGERID "RL000331"`, but it is only shown in the start-up messages (and `wari_v2.1` does not use it at all).

### 8.3 Optional: change how often

| I want to... | Change | Allowed values |
|---|---|---|
| Measure more or less often | `READ_INTERVAL` or `INTERVAL` (your sketch has one of them) | Minutes that divide 60 evenly: 1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30, 60 |
| Send more or less often (cellular) | `SEND_INTERVAL` | Hours that divide 24 evenly: 1, 2, 3, 4, 6, 8, 12, 24 |
| Keep SD saving once a day after changing the interval (SD) | `FLUSHAFTER` | 1440 ÷ your interval. For example 15 min → `96`. The sketch copies to the card after this many readings, so the default `288` is once a day only at 5 minutes. |

### 8.4 Leave everything else alone

- **LTE-M or NB-IoT modem?** Use the `wari_4G` sketch (NB-IoT only, see [step 6](#6-pick-the-right-sketch)). Do not switch on `LTE` in `wari_3G_v2` (at the moment that stops it compiling). In `wari_lidar_cellular`, switch on `XBEE4G` instead.
- **Do not change** `NREADINGS`, `TIMEOUT`, `MQTT`, `NTP`, `FLASH`, `OPTIBOOT` or `DEBUG`, or anything below the settings block.
- **Brand-new XBee modem** (not supplied with the logger)? It needs a one-time setup with Digi's XCTU program: API mode 2, plus a few other settings listed in the sketch's `Readme.md` file. See the [XBee Setup Walkthrough](../../telemetry/xbee-setup-walkthrough.md).

!!! tip "Not sure about a setting?"
    [Settings Explained and Common Questions](settings-explained.md) explains every setting and answers questions like "What should the APN be?", "Why does nothing appear on riverflow.io?" and "Is h the water level?".

---

## 9. Set up the computer (one time only)

1. **Install the Arduino IDE** from the Arduino website.
2. **Choose the board:** **Tools → Board → Arduino AVR Boards → Arduino Pro or Pro Mini**, then **Tools → Processor → ATmega328P (3.3V, 8 MHz)**.
3. **Install the libraries:** **Tools → Manage Libraries**, then search for and install each of these:
    - **Rtc** by Makuna
    - **SdFat** by Bill Greiman
    - **Low-Power** by Rocket Scream
    - **AltSoftSerial** by Paul Stoffregen
    - **LIDAR-Lite** by Garmin
    - **SPIMemory** by Prajwal Bhattaram
    - **RadioHead** by Mike McCauley (only for `wari_lidar_lora`)
4. **Driver:** Mac and Linux usually need nothing. On Windows, install the FTDI VCP driver if no COM port appears in step 10.

**How to know it worked:** click the **✓ Verify** button. After a minute the bottom of the window says "Done compiling". If it says a file `.h` is missing, a library from the list is not installed yet.

More detail: [Arduino Setup](../../programming/arduino-setup.md). If you prefer the command line, see [PlatformIO, Backup and Restore](../../programming/advanced.md).

---

## 10. Upload the code

[![Uploading code, one step at a time](../../assets/images/setup/07-upload-steps.svg)](../../assets/images/setup/07-upload-steps.svg){ title="Click to open full size" }

Before you start, make sure you have:

- The logger, with its lid off so you can see the board.
- The FTDI cable (USB at one end, a black 6-hole plug with coloured wires at the other).
- A computer with the Arduino IDE set up as in [step 9](#9-set-up-the-computer-one-time-only).
- The sketch, with your settings changed and saved, from [steps 7 and 8](#7-get-the-code).
- **Cellular loggers:** a charged 18650 battery in the holder. You need it in step 10.9.

Allow about 15 minutes the first time. Do the steps in order and do not skip any.

### 10.1 Switch OFF and unplug the sensor

1. Find the power switch ([step 2](#2-find-the-parts-on-the-board)) and slide it to **OFF (ftdi)**.
2. Take a photo of the board on your phone. It helps you put everything back the same way at the end.
3. Find the white plug where the sensor cable joins the board. Hold the plug itself, not the wires, and pull it gently straight out.

If the sensor stays plugged in, the upload fails with a message like `not in sync`, `out of sync` or `programmer is not responding`.

??? question "Why?"
    The sensor and the cable share one wire into the chip, so the chip hears both at once.

    [![Why the Maxbotix must be unplugged](../../assets/images/setup/06-why-unplug-sensor.svg)](../../assets/images/setup/06-why-unplug-sensor.svg){ title="Click to open full size" }

### 10.2 Plug the FTDI cable into the board

Do this **before** you plug the USB end into the computer.

1. Find the row of 6 pins at the edge of the board, with **BLK** printed at one end and **GRN** at the other ([step 2](#2-find-the-parts-on-the-board)). If your board has no labels, the BLK end is the end nearer the SD card slot.
2. Hold the black plug so the **black wire** is at the **BLK** end and the **green wire** is at the **GRN** end.
3. Push the plug straight onto all 6 pins until it stops. No pin should be left showing.

<!-- TODO: add a photo of the FTDI cable plugged into a real board -->
[![Plug in the FTDI cable](../../assets/images/setup/05-ftdi-wiring.svg)](../../assets/images/setup/05-ftdi-wiring.svg){ title="Click to open full size" }

The plug fits both ways round, so look again: black to BLK, green to GRN. If something does not work later, check this first.

### 10.3 Plug the USB end into the computer

1. Leave the switch on **OFF**. In this position the computer powers the board through the cable, with or without a battery fitted.
2. Plug the USB end into the computer.

The LED may flash as the logger starts up. That is fine.

### 10.4 Choose the port

The *port* is the name the computer gives the cable. The Arduino IDE needs to know it.

1. Open the Arduino IDE if it is not open yet.
2. Click **Tools → Port**.
3. Pick the cable from the list. It is usually called:
    - **Mac:** `/dev/cu.usbserial-` followed by letters and numbers, for example `/dev/cu.usbserial-A10KJ3HZ`
    - **Windows:** `COM` and a number, for example `COM5`
    - **Linux:** `/dev/ttyUSB0`
4. If the list has more than one port and you are not sure which one is the cable: unplug the USB end, open **Tools → Port** and remember the list. Plug the cable back in and open the list again. The new one is the cable.

If no new port appears, try another USB socket. On Windows, install the FTDI VCP driver ([Arduino Setup](../../programming/arduino-setup.md#the-ftdi-driver)).

If the Serial Monitor is open, close it now. Only one program at a time can use the port.

### 10.5 First time only: back up

Before the new code goes in, save a copy of the old code on the logger, so it can be put back if something goes wrong. If you skip this, the old code is gone for good.

This step means typing commands into a terminal (a text window for giving the computer commands), so it is harder than the others. If you have not used a terminal before, ask an adult or someone who has to do it with you, using [Backup and Restore](../../programming/advanced.md#back-up-a-logger-before-uploading). It only has to be done once for each logger.

### 10.6 Open your sketch

If the sketch from [step 8](#8-change-the-settings) is still open, go to the next step. If not:

1. Click **File → Open**.
2. Go to the folder you unzipped in [step 7](#7-get-the-code), then into the folder for your sketch, for example `wari_3G_v2`.
3. Choose the file with the same name ending in `.ino`, for example `wari_3G_v2.ino`, and click **Open**.

Then check:

- The tab at the top of the code has the name of your sketch.
- Your settings are still there. Scroll to the `#define` lines near the top and read them again, for example `LOGGERID` and `ACCESSTOKEN` on a cellular logger.
- The changes are saved: click **File → Save** (or press Ctrl+S, or Cmd+S on a Mac).

### 10.7 Check the board and port

Click **Tools** and read three lines in the menu. They should say:

| Line in the Tools menu | Should say |
|---|---|
| Board | Arduino Pro or Pro Mini |
| Processor | ATmega328P (3.3V, 8 MHz) |
| Port | The port you chose in step 10.4 |

If one is wrong, click it and choose the right one. The **Processor** line only appears after you choose the board. The 5V, 16 MHz choice is wrong for these loggers.

### 10.8 Click Upload

1. Click the **Upload** button. It is the round button with an arrow pointing right (→), at the top left, next to the tick (✓).
2. Wait, and do not touch the cable or the switch. Three things happen:
    - **Compiling.** The computer turns the sketch into code for the chip. The first time, this can take a minute or more.
    - **Uploading.** The code goes down the cable to the logger.
    - **Done.** A message saying **Done uploading** appears near the bottom of the window.
**It worked** if you see **Done uploading**. Go to step 10.9.

**It did not work** if the text at the bottom turns orange or red and you do not see Done uploading. Find the first line with the word `error` in it, then look for that message in the table [The upload failed?](#the-upload-failed) at the end of this step. Lines with the word `warning` are not a problem on their own.

??? info "What is the line that says \"Sketch uses ... bytes\"?"
    At the bottom you may see a line like `Sketch uses ... bytes (...%) of program storage space. Maximum is 30720 bytes.` It tells you how much of the chip's memory the code fills. This is normal. When we built the sketches with PlatformIO, `wari_v2.1` filled about 74%, `wari_3G_v2` about 76%, and `wari_4G` and `wari_lidar_cellular` about 87%. The Arduino IDE may show slightly different numbers.

### 10.9 Open the Serial Monitor

The Serial Monitor shows the messages the logger writes while it runs. You use it to check that the new code works.

1. **Cellular loggers only:** slide the switch to **ON (batt)** now, with a charged battery in. The Riverlabs instructions say the cable alone does not give enough power for the modem. Leave the cable plugged in.
2. Click the **Serial Monitor** button. It is the magnifying glass at the top right of the window. A Serial Monitor panel opens (in older versions, a separate window).
3. Find the box that shows a speed, for example `9600 baud`. In version 2 it is at the right-hand end of the Serial Monitor panel; in older versions it is at the bottom right of the window. Change it to **115200 baud**.

Opening the Serial Monitor normally restarts the logger, so the messages start from the beginning.

- **Nothing appears after 10 seconds?** Press the **RESET** button on the board once.
- **Strange characters?** The speed is not set to 115200 baud.
- **Still nothing?** Check the plug direction (black to BLK) and, for cellular loggers, that the switch is ON with a charged battery.

### 10.10 Check the messages

You do not need to understand every line. Look for these things:

| Look for | Cellular logger | SD logger |
|---|---|---|
| **1. The first line** starts with "This is Riverlabs" and says "compiled on" with **today's date** | Yes | Yes (except `wari_v2.0`) |
| **2. "Logger ID"** shows the serial number you typed in `LOGGERID` | Yes | Only the lidar sketches |
| **3. "AI status = 0"** appears within about a minute. This means the modem is connected to the mobile network. | Yes | (not shown) |
| **4. "NTP received"** appears soon after. This means the logger got the time from the internet. | Yes | (not shown) |
| **5. "EEPROM flushed."** appears. This means the logger can write to the SD card. | (not shown) | Yes |

If you see all the things in your column, the upload worked. Go to step 10.11.

If the "compiled on" date is **not today**, the upload did not work and the logger is still running its old code. Go back to step 10.8.

The name on the first line is not always the sketch name: `wari_lidar_cellular` says just "Wari", and `wari_lidar_lora` says "WMOnode". `wari_v2.0` does not show a date, and writes "GMT" instead of "UTC" (they are the same thing).

??? example "What a cellular logger prints"
    This is from our test logger. Your logger ID, times and numbers will be different.

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
    AI status = 22
    AI status = 23
    Callback - Modem Status Response: 2
    AI status = 0
    Callback - IP Address: AC E8 9D 1B
    NTP received. Clock is set to: 11/06/2026 19:57:51
    ```

    - "AI status" lines appear every 3 seconds while the modem connects. They usually go `FF`, then `22`, then `0`, sometimes with `23` before `0`. On our test logger, `0` came 20 to 40 seconds after start-up. If it stays on `FF` or `22` for several minutes, see [Questions about the Serial Monitor](settings-explained.md#questions-about-the-serial-monitor).
    - "Current time is ..." may be wrong at first. The logger corrects it when "NTP received" appears. The time is in UTC (`wari_4G` writes "GMT", which is the same thing).

??? example "What an SD logger prints"
    We wrote this example from the `wari_v2.1` code. It was not copied from a real logger.

    ```text
    This is Riverlabs Wari v2.1 (optiboot), compiled on Nov 10 2026
    Current time is 2026/11/10 14:02:11 UTC
    Flushing EEPROM... EEPROM flushed.
    Sleeping
    Waking up!
    Sleeping
    ```

    - "Current time is ..." should be the right time in UTC. To find the time in UTC now, search the internet for "UTC time now". If the logger's time is wrong, set the clock in [step 11](#11-set-the-clock-sd-loggers-only).
    - If you see "Failed to flush EEPROM. SD card missing?" instead of "EEPROM flushed.", the logger cannot use the SD card. Switch OFF and check the card ([step 13](#13-take-the-sd-card-out-safely)).
    - "Sleeping" and "Waking up!" appear about once a minute. That is normal.

    `wari_v1` prints nothing at all here. It sends its messages to a different connector instead.

??? info "If you keep watching: readings and sending"
    You do not have to wait for these. The logger takes a reading when the minutes on the clock can be divided exactly by the interval. With a 5 minute interval, that is 10:00, 10:05, 10:10 and so on.

    - At each reading it prints the date, the battery voltage, the temperature and the distance. Because the sensor is unplugged, the distance is not real. Cellular sketches show `-1`. That is expected.
    - Cellular sketches may print letters such as `SWSWSW` just before the date. [That is normal](settings-explained.md#questions-about-the-serial-monitor).
    - Cellular loggers send their data at the start of each hour, UTC (for example 11:00). You then see `CONNACK received: accepted` and `PUBACK received`, which means the server got the data. Before that, nothing appears on the server, even when everything is working.

### 10.11 Unplug the cable and plug the sensor back in

1. Slide the switch to **OFF**.
2. Unplug the USB end from the computer.
3. Pull the FTDI plug straight off the pins. Do not bend it sideways.
4. Plug the white sensor plug back in, the same way round as in your photo from step 10.1. Push it in gently until it stops.

### 10.12 Switch ON

1. Check the battery is in.
2. Slide the switch to **ON (batt)**.
3. Watch the LED. It should show the start-up pattern in [step 4](#4-read-the-led), not an error pattern.

The logger is now running your new code. Before you take it to the field, test it on the bench ([step 14](#14-test-on-the-bench-before-going-to-the-field)).

### The upload failed?

| Message | What it means | Fix |
|---|---|---|
| `not in sync`, `out of sync` or `programmer is not responding` | The computer cannot talk to the logger. | Unplug the sensor (10.1). Check black to BLK and green to GRN (10.2). Check Tools → Board, Processor and Port (10.7). Then try again. |
| `Resource busy`, `port is open` or `Access denied` | Another program is using the port. | Close the Serial Monitor and any other program that uses the cable. |
| `No such file or directory` for a file ending in `.h` | A library is not installed. | Install the library from [step 9](#9-set-up-the-computer-one-time-only). The file name hints which one, for example `SdFat.h` is SdFat. |
| An error from `LowPower.h`, such as `Processor architecture is not supported` | The wrong board is chosen. | Choose Arduino Pro or Pro Mini, ATmega328P (3.3V, 8 MHz) (10.7). |
| `Sketch too big` | The code is larger than the chip. | Undo any changes you made to the code apart from the settings. |
| No port in the list | The computer cannot see the cable. | Try another USB socket or cable. Windows: install the FTDI driver. |

If it still fails after these checks, see [Uploading Code → Troubleshooting](../../programming/uploading-code.md#troubleshooting).

---

## 11. Set the clock (SD loggers only)

**Only needed when:** an SD logger is new, its clock is wrong, or its CR1220 coin cell was taken out. Cellular loggers set their own clock from the internet every time they start, so skip this step for them.

Connect the logger as in steps 10.1 to 10.4, then:

1. Open `set_clock/set_clock.ino`.
2. Change `TZ` to your **computer's** difference from UTC in hours: `0` for UK winter time, `1` for UK summer time or West Africa Time, `-5` for US Eastern winter time.
3. **Open the Serial Monitor first** (115200 baud), then click Upload. Do it in this order because the sketch sets the clock every time it starts, and opening the Serial Monitor restarts it.
4. Check the time it prints is correct UTC.
5. **Straight away**, upload the logger sketch again (steps 10.6 to 10.12). Otherwise a restart would set the clock back to the old time.

---

## 12. What the logger does all day

[![What the logger does all day](../../assets/images/setup/09-how-it-works.svg)](../../assets/images/setup/09-how-it-works.svg){ title="Click to open full size" }

The clock wakes the logger every minute. On the minutes that match your interval, it switches the sensor on and takes readings. The readings are kept in a memory chip. SD loggers copy them to the card after `FLUSHAFTER` readings (once a day with the default settings and a 5-minute interval) and every time you press RESET. Cellular loggers send them at the top of the hour, if the battery is above 3.5 V. In between, the logger sleeps and uses almost no power.

---

## 13. Take the SD card out safely

[![Taking the SD card out without losing data](../../assets/images/setup/10-sd-card.svg)](../../assets/images/setup/10-sd-card.svg){ title="Click to open full size" }

1. Press **RESET**. This writes any waiting readings to the card.
2. Wait until the LED goes dark. This takes longer when there are more readings to copy.
3. Switch **OFF** and pull the card out.
4. Copy (do not move) the `YYYYMMDD.CSV` files to your computer. Put the card back and switch **ON**.

See [Data Download](data-download.md) for what is inside the files.

---

## 14. Test on the bench before going to the field

Leave the logger running with the sensor plugged in and the switch ON for at least 30 minutes.

- [ ] After RESET there is no error pattern on the LED.
- [ ] With the FTDI cable and Serial Monitor connected (switch ON), point the sensor at a wall 1 m away. Cellular ultrasound sketches print about `Distance (ultrasound) = 1000` (in mm). SD sketches print a line with the date and the readings, and lidar sketches print `Distance (lidar)`. A reading of `5000` means nothing is in range, and `-1` means the sensor is not plugged in or the sketch does not match the board.
- [ ] **SD loggers:** after the first readings and a RESET, there is a file named after today's date (UTC) on the card.
- [ ] **Cellular loggers:** after the top of the hour, the Serial Monitor shows `CONNACK received: accepted` and `PUBACK received`, and the data appears on the server.
- [ ] The battery is above 3.5 V. Cellular sketches print it in mV (`VBatt = 3815` means 3.815 V). SD sketches print it in volts at the end of each reading line. Below 3.5 V, cellular loggers stop sending.
- [ ] The FTDI cable is unplugged, the sensor is plugged in, the switch is ON, and the box is closed and sealed.

Then follow the [First Deployment Checklist](first-deployment-checklist.md) and the [Mounting Guide](../../installation/mounting-guide.md).

---

??? info "Editing the drawings"
    Each drawing has an editable source file next to it in `overrides/assets/images/setup/`, with the same name ending in `.excalidraw`. Open it at excalidraw.com (menu → Open), change it, and export it as SVG under the same name.
