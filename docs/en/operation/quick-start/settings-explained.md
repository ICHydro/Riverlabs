# Settings Explained and Common Questions

This page explains every setting at the top of the logger sketches, and answers the questions people ask most when they set up a logger for the first time. No question here is too basic.

!!! abstract "Three things to know first"
    1. **Settings are built into the code.** To change a setting, you change the sketch and upload it again. There is no way to change settings over the internet or with a button.
    2. **Each logger gets its own upload.** Every logger needs its own `LOGGERID` and (for cellular loggers) its own `ACCESSTOKEN`, so you change those two lines before each upload.
    3. **Most settings should be left alone.** For a cellular logger you normally change four lines: `HOST`, `ACCESSTOKEN`, `LOGGERID` and `APN`.

---

## The four settings you change on a cellular logger

| Setting | What to put | Example |
|---|---|---|
| `HOST` | The address of the server that receives the data. For the Riverlabs data platform this is `riverflow.io`. No `https://`, no `/` at the end. | `#define HOST "riverflow.io"` |
| `ACCESSTOKEN` | This logger's access token, copied from its device page in ThingsBoard (riverflow.io runs ThingsBoard). | `#define ACCESSTOKEN "Ab12Cd34Ef56Gh78"` |
| `LOGGERID` | A name for the logger. Use its serial number. | `#define LOGGERID "RL000726"` |
| `APN` | The APN of the SIM card in the modem. It depends on the SIM company. If you got the SIM from Riverlabs, use the APN they gave you with it. | `#define APN "your.apn.here"` |

The values that come in the sketch are only placeholders: `wari_3G_v2` has `HOST "demo.thingsboard.io"` and `ACCESSTOKEN "A1_TEST_TOKEN"`, `wari_3G` has a misspelt `"demo.thingsbooard.io"`, and `wari_4G` and `wari_lidar_cellular` have both empty. If you upload them unchanged, your data will not arrive anywhere useful.

---

## Sending data to the Riverlabs platform (riverflow.io)

1. **Get an account** on riverflow.io from Riverlabs.
2. **Add a device** for the logger. Name it after the serial number, for example `RL000726`. Each logger needs its own device.
3. **Copy the device's access token.** Open the device and use the button to copy its access token.
4. **In the sketch, set:**
    ```cpp
    #define HOST "riverflow.io"
    #define ACCESSTOKEN "paste-the-token-here"
    #define LOGGERID "RL000726"
    #define APN "the-APN-for-your-SIM"
    ```
5. **Upload** (see the [Setup Guide](setup-guide.md#10-upload-the-code)).
6. **Wait for the top of the hour** (in UTC). The logger only sends at the top of the hour, so nothing appears on riverflow.io before then, even if the logger is working perfectly.
7. **Check the device's latest telemetry.** You should see three values, explained below under "What are h, v and t?".

### Using your own ThingsBoard server instead

Everything is the same, with `HOST` set to your server's address. Two things to check on your server:

- The logger sends with **MQTT on port 1883**, without encryption. Your server must accept MQTT connections on port 1883 from the internet.
- Each logger needs its own **device**, and its token goes into `ACCESSTOKEN`.

---

## Every setting, one by one

### Cellular sketches (`wari_3G`, `wari_3G_v2`, `wari_4G`, `wari_lidar_cellular`)

| Setting | What it does | Default | Change it? |
|---|---|---|---|
| `MQTT` | Chooses how data is sent: MQTT or COAP. ThingsBoard and riverflow.io use MQTT. | on | **No.** |
| `LTE` | In `wari_3G_v2` only: sets the modem up for an LTE modem instead of 3G. | off | **No.** At the moment, switching it on makes the sketch fail to compile (`'atRequest6' was not declared`). For an LTE-M or NB-IoT modem, use the `wari_4G` sketch instead. |
| `XBEE4G` | Sets the sketch up for a 4G (LTE-M or NB-IoT) modem. | on in `wari_4G`; off in `wari_lidar_cellular` and `wari_3G` | In `wari_lidar_cellular`, switch it on if your lidar logger has a 4G modem. Leave it alone in the others. |
| `READ_INTERVAL` | Minutes between readings. | 5 | If you want. Use a number that divides 60 evenly: 1, 2, 3, 4, 5, 6, 10, 12, 15, 20, 30 or 60. The logger measures when the minute on the clock divides evenly by this number, so a value like 7 gives uneven gaps. |
| `SEND_INTERVAL` | Hours between sends. Sending always happens at minute 0 of the hour. | 1 | If you want. Use a number that divides 24 evenly: 1, 2, 3, 4, 6, 8, 12 or 24. Sending less often saves battery. |
| `NREADINGS` | How many readings the sensor takes each time. The logger keeps the **middle value** (the median), which ignores odd one-off readings. | 9 | **No.** |
| `HOST` | The server address. | a placeholder (see above) | **Yes.** |
| `ACCESSTOKEN` | The device's access token on the server. | a placeholder (see above) | **Yes.** |
| `LOGGERID` | The logger's name. The comment says "set to whatever you like", but it is also used as the logger's connection name on the server, so each logger should have a **different** one. | empty | **Yes.** Use the serial number. |
| `APN` | The SIM card's APN. | empty | **Yes**, if your SIM needs one. |
| `TIMEOUT` | How many seconds each send attempt may take. The logger makes up to 5 attempts each send time. | 180 | Usually no. |
| `DONOTUSEEEPROMSENDBUFFER` | In `wari_3G` and `wari_3G_v2` only. The current code does not use it, so it has no effect. | on | **No.** |
| `NTP` | Sets the clock from the internet every time the logger starts. | on | **No.** Without it, a cellular logger's clock is never corrected. |
| `FLASH` | In `wari_4G` and `wari_lidar_cellular`: also keeps a backup copy of the readings in the flash memory chip. | on | **No.** |
| `OPTIBOOT` | Turns on the watchdog, which restarts the logger if it ever freezes. | on | **No.** It needs the Optiboot bootloader. Our test logger had it, but if you are unsure about yours, ask Riverlabs (see the questions below). |

### SD sketches (`wari_v1`, `wari_v2.0`, `wari_v2.1`, `wari_lidar`, `wari_lidar_lora`)

| Setting | What it does | Default | Change it? |
|---|---|---|---|
| `INTERVAL` or `READ_INTERVAL` | Minutes between readings. The name depends on the sketch. | 5 (lidar: 15) | If you want. Use a number that divides 60 evenly. |
| `NREADINGS` | Readings taken each time. | 10 (lidar: 9) | **No.** |
| `FLUSHAFTER` | How many readings are kept in the memory chip before they are copied to the SD card. | 288 | Only if you change the interval. To copy about once a day, use 1440 divided by your interval (15 min → `96`). The chip holds about 1,950 readings or more, so values up to a few hundred are safe. |
| `LOGGERID` | The logger's name. Only in `wari_v2.1`, `wari_lidar` and `wari_lidar_lora`, and only shown in the start-up messages (`wari_v2.1` does not use it). | empty (`wari_lidar_lora`: `mylogger`) | Optional. |
| `DEBUG` | Switches on the messages the logger prints to the Serial Monitor. | on | **No.** |
| `FLASH` | Keeps a backup copy in the flash memory chip, on boards that have one. Not in every sketch. | off | **No.** |
| `OPTIBOOT` | Turns on the watchdog. Not in `wari_v1`, `wari_v2.0` or `wari_lidar_lora`. | on | **No.** |

---

## Questions about the settings

??? question "Are HOST, ACCESSTOKEN, LOGGERID and APN really the only settings I need to change?"
    For a cellular logger, yes, in most cases. You may also want to change `READ_INTERVAL` or `SEND_INTERVAL`. Leave everything else as it is.

??? question "What should the APN be?"
    It depends on the SIM card, not on the logger. The SIM company publishes it, usually on a page called "APN settings". If Riverlabs gave you the SIM, use the APN they gave you with it.

??? question "Can I leave the APN empty?"
    Sometimes. With an empty APN, some SIM cards still connect. If the logger never connects (the "AI status" never reaches 0), put in the APN from your SIM company.

??? question "I set the APN in Digi's XCTU program. Why did it change?"
    The logger writes its own `APN` setting into the modem every time it starts, even when the setting is empty. So the value in the sketch always wins. Set the APN in the sketch, not in XCTU.

??? question "The comment says LOGGERID can be 'whatever you like'. Does it matter?"
    It matters a little. The logger uses it as its connection name when it talks to the server. Two loggers with the same name on the same server can keep pushing each other off. Use the serial number and you will never have a clash.

??? question "Can two loggers share an access token?"
    No. The token tells the server which device the data belongs to. Two loggers with the same token would mix their data into one device. Make a separate device, with its own token, for each logger.

??? question "My modem is LTE-M or NB-IoT. Do I switch on LTE?"
    No. The `LTE` switch in `wari_3G_v2` currently stops the sketch compiling. Use the `wari_4G` sketch instead, but note that it tells the modem to use **NB-IoT only**, on frequency bands 8 and 20 (common in Europe). If your SIM or network only offers LTE-M, or uses other bands, ask Riverlabs first. For a lidar logger, use `wari_lidar_cellular` and switch on `XBEE4G`.

??? question "How do I know if my logger uses the Optiboot bootloader?"
    There is no simple test from the Serial Monitor. The word `(optiboot)` in the first start-up line, for example `This is Riverlabs Wari_3G_v2 (optiboot)`, only means the sketch was built with the `OPTIBOOT` setting on. It does not prove which bootloader is on the chip. Our test logger had Optiboot 8.0. If you are unsure, ask Riverlabs.

??? question "Can I change a setting without the cable, for example from riverflow.io?"
    No. Settings are part of the code, so they can only be changed by uploading again with the FTDI cable.

??? question "Will uploading new code delete the readings that have not been sent yet?"
    The readings are kept in a separate memory chip, which an upload does not erase. On our test logger, readings taken before a restart were sent at the next hour as normal. To be safe, upload just after the top of the hour, when everything has just been sent. Always [back up](../../programming/advanced.md#back-up-a-logger-before-uploading) a logger before its first upload.

## Questions about the data

??? question "I uploaded the code, but nothing appears on riverflow.io. Is it broken?"
    Probably not. Check these in order:

    1. **Wait for the top of the hour (UTC).** The logger only sends at minute 0 of the hour, or every few hours if `SEND_INTERVAL` is more than 1.
    2. **Check the battery.** Cellular loggers only send when the battery is at 3.5 V (`VBatt = 3500`) or more.
    3. **Watch the Serial Monitor at the top of the hour.** `CONNACK received: accepted` means the server accepted the logger. `PUBACK received` means it accepted a reading.
    4. **`CONNACK received` without `: accepted`, or no CONNACK at all?** The server did not accept the logger. Check `HOST` and `ACCESSTOKEN` (a copied token with a missing letter will not work), and check that the device exists on the server.
    5. **"AI status" never reaches 0?** The logger never got onto the internet. See "AI status" below.

??? question "The logger sends once an hour, but riverflow.io shows a point every 5 minutes. How?"
    Each reading is stored with the time it was taken. At the top of the hour the logger sends all the readings since the last send, each with its own time, so the server puts every point in the right place.

??? question "What are h, v and t?"
    These are the names the logger gives its values on the server:

    | Name | What it is | Unit | Example |
    |---|---|---|---|
    | `h` | The distance from the sensor to the surface below it | millimetres (lidar loggers: centimetres) | `1076` = 1.076 m |
    | `v` | The main battery voltage | millivolts | `3815` = 3.815 V |
    | `t` | The temperature on the board | hundredths of a degree C | `2400` = 24.00 °C |

??? question "Is h the water level?"
    No, and this catches many people out. `h` is the **distance from the sensor down to the water**. When the water rises, `h` gets **smaller**. To get the water level, measure the height of the sensor above a fixed point (for example the river bed or a gauge zero) when you install it, then:

    **water level = sensor height − h**

    For example, a sensor 2500 mm above the river bed that reads `h = 1076` means the water is 2500 − 1076 = 1424 mm deep.

??? question "The times look an hour (or more) off."
    The logger keeps time in UTC. The server shows times in your browser's time zone, which may differ from UTC, especially in summer. The data is right, it is only displayed differently.

??? question "What happens if there is no signal for a while?"
    The readings stay in the memory chip and are sent at the next successful send. The chip holds about 4,000 readings, which is about 13 days at a 5 minute interval. If the logger cannot send for longer than that, it starts writing over the readings at the start of the chip. After a long gap, catching up can take several send times, because each send time is limited to 5 attempts of `TIMEOUT` seconds.

??? question "The distance shows 5000, -1, or 300."
    These are for the Maxbotix ultrasound sensor. The 5000 and 300 values come from the sensor itself (see the MaxBotix datasheet).

    - `5000`: nothing was detected within range (the sensor reaches up to 5 m). The sensor may be pointing at open space, at a target further than 5 m away, or at a surface that does not reflect sound well.
    - `-1`: fewer than half the readings came back from the sensor. It is unplugged, not powered, or the sketch does not match the board (see the [Setup Guide](setup-guide.md#6-pick-the-right-sketch)).
    - `300`: the target is 300 mm or closer. The sensor cannot measure closer than that.

    On lidar loggers, a failed reading shows as `0` instead of `-1`.

## Questions about the hardware

??? question "Does the SIM card need anything special?"
    - It must be **activated** with a data plan. Some IoT SIMs must be activated in the company's online portal first.
    - It must have **no PIN lock**. If it has one, put it in a phone and turn the PIN off.
    - It must work on the network type of your modem (3G, LTE-M or NB-IoT) in the country where you install the logger.

??? question "My 3G logger will not connect any more."
    Many countries, including the UK, have switched off their 3G networks. A 3G modem cannot connect where there is no 3G network. You need an LTE-M or NB-IoT modem and the `wari_4G` sketch.

??? question "Does the antenna matter?"
    Yes. Connect the antenna **before** you switch on, and make sure it is pushed fully onto its connector. A modem without an antenna will not get a signal.

??? question "Will sending more often flatten the battery?"
    Sending switches on the modem, which uses much more power than taking a reading. So the less often the logger sends, the longer the battery lasts. A larger `SEND_INTERVAL` (for example 6 hours) helps. We have not measured battery life, so check the battery voltage (`v`) on the server for the first weeks.

??? question "Can I leave the FTDI cable plugged in when the logger is in the field?"
    There is no need, and it should be unplugged. The cable is only for uploading and watching messages.

## Questions about the Serial Monitor

??? question "What do all these letters mean? (SWSWSW...)"
    `S` is printed just before the logger goes to sleep, and `W` when it wakes up. The clock wakes it every minute, so you see `SW` once a minute. This is normal.

??? question "It keeps showing \"AI status = FF\". What do I do?"
    `FF` means the modem is still starting up. On our test logger, `FF` showed for the first 15 to 30 seconds, and then it connected normally. The logger checks every 3 seconds for up to 3 minutes. `wari_3G` and `wari_3G_v2` then print "Trying again." and try for another 3 minutes.

    If it stays on `FF`:

    1. **Use the battery.** Run the logger on a charged battery with the switch on **ON**. The Riverlabs instructions say the FTDI cable does not give enough power for sending. You can keep the cable plugged in to watch the messages.
    2. **Check the SIM and antenna.** Switch OFF, then check the SIM is pushed fully into the XBee the right way round, and the antenna is connected.
    3. **Check the modem type.** If the label on the XBee says LTE-M or NB-IoT, use the `wari_4G` sketch, not `wari_3G_v2`.
    4. **Check that the modem answers.** If an error line appears before the "AI status" lines, the logger could not talk to the modem at all, and `FF` is just the starting value. Switch OFF and push the XBee firmly into its socket.

    If the "This is Riverlabs..." start-up message keeps coming back, the logger is restarting. Check the power first (point 1) and that the modem is fitted (point 4).

??? question "What do the 'AI status' numbers mean?"
    The modem reports its progress while connecting. The logger prints this as "AI status". These are the most common values for Digi XBee cellular modems:

    | AI status | Meaning |
    |---|---|
    | `FF` | The modem is starting up. |
    | `22` | Looking for and joining the mobile network. |
    | `23` | Joined the network, now connecting to the internet. |
    | `0` | Connected to the internet. |
    | `24` | A problem inside the modem (Digi says it may be faulty or need new firmware). Switch off and on again. If it stays, contact Riverlabs. |
    | `25` | The network refused the SIM. Check the SIM is activated and allowed on this network. |

    The numbers are hexadecimal, as Digi writes them. On our test logger it usually went from `FF` to `22` to `0` (sometimes with `23` in between), reaching `0` 20 to 40 seconds after start-up. If it stays on `22`, the modem cannot get onto the network (signal, SIM or antenna). If it stays on `23`, check the APN.

??? question "I see nothing at all in the Serial Monitor."
    Check that the baud rate is 115200, that the FTDI plug has the green wire on GRN, and that the logger has power (switch OFF with the cable in, or ON with a charged battery). Then press RESET.

??? question "I see strange characters."
    The baud rate is wrong. Set it to 115200.

---

Still stuck? See the [Setup Guide](setup-guide.md), the [upload troubleshooting](../../programming/uploading-code.md#troubleshooting), or contact info@riverlabs.uk.
