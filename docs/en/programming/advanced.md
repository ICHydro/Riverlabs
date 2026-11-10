# PlatformIO, Backup and Restore

This page is for people who are comfortable with a terminal. It covers building and uploading with PlatformIO, backing up the code that is already on a logger, and putting it back safely.

## PlatformIO

[PlatformIO](https://platformio.org/) builds and uploads from the command line, or from editors such as VS Code. The repository already contains a `platformio.ini` with one *environment* (env) for each logger sketch, and PlatformIO downloads the libraries by itself. The configuration uses the `pro8MHzatmega328` board, which uploads at 57600 baud. We tested it unchanged on a real `wari_3G_v2` logger.

### Install

```bash
git clone https://github.com/ICHydro/Riverlabs.git
cd Riverlabs
python3 -m venv .venv
.venv/bin/pip install platformio
```

### Build, upload and watch

```bash
.venv/bin/pio run -e wari_3G_v2                                           # build only
.venv/bin/pio run -e wari_3G_v2 -t upload --upload-port /dev/cu.usbserial-XXXX
.venv/bin/pio device monitor -b 115200 -p /dev/cu.usbserial-XXXX          # Ctrl+C to quit
```

A successful upload ends with **SUCCESS**. The same hardware rules apply as with the Arduino IDE: unplug the sensor, and plug the FTDI cable in with green to GRN (see [Uploading Code](uploading-code.md)).

| Sketch folder | PlatformIO env |
|---|---|
| `wari_v1` | `wari_v1` |
| `wari_v2.0` | `wari_v20` |
| `wari_v2.1` | `wari_v21` |
| `wari_3G`, `wari_3G_v2`, `wari_4G` | same name |
| `wari_lidar`, `wari_lidar_cellular`, `wari_lidar_lora` | same name |

There is no env for `set_clock`. Use the Arduino IDE for that sketch.

### Static checks

```bash
.venv/bin/pio check -e wari_3G_v2
```

This runs `clang-tidy` with the repository's `.clang-tidy` settings.

## Back up a logger before uploading

Before you upload new code to a logger for the first time, save a copy of the code that is on it now. If something goes wrong later, you can put the original code back. You only need to do this once for each logger.

[![Back up first, and restore the safe way](../assets/images/setup/11-backup-restore.svg)](../assets/images/setup/11-backup-restore.svg){ title="Click to open full size" }

The chip has 32 KB of flash memory. The program (the sketch) uses the space from address `0x0000` up to `0x7DFF`. The **bootloader**, which is what lets the FTDI cable upload code, sits in the last 512 bytes from `0x7E00`.

### What you need

- The logger, its FTDI cable and a computer.
- **PlatformIO**, installed as shown in [Install](#install) above. The backup uses **avrdude**, a program that comes with PlatformIO. We tested every command on this page with PlatformIO's avrdude **version 8.1** on a Mac. Older versions (such as 6.3) cannot do step 8.
- About 15 minutes.

!!! note "Only have the Arduino IDE?"
    The Arduino IDE also contains an avrdude, but it can be an older version, and it is stored in a different place on each computer. The simplest way to get the tested version is to install PlatformIO as shown above, even if you upload with the Arduino IDE.

### 1. Make sure PlatformIO has downloaded avrdude

PlatformIO only downloads avrdude the first time you build or upload a sketch. In the `Riverlabs` folder, run a build once:

```bash
.venv/bin/pio run -e wari_3G_v2
```

On Windows, the command is `.venv\Scripts\pio run -e wari_3G_v2`.

The last line should say **SUCCESS**. This only builds the code on your computer. It does not touch the logger.

### 2. Make a folder for the backups

Make one folder for each logger, named after the logger, **outside** the `Riverlabs` code folder. For example:

=== "macOS and Linux"

    ```bash
    mkdir -p ~/Riverlabs-backups/RL000XXX
    cd ~/Riverlabs-backups/RL000XXX
    ```

=== "Windows (PowerShell)"

    ```powershell
    mkdir $HOME\Riverlabs-backups\RL000XXX
    cd $HOME\Riverlabs-backups\RL000XXX
    ```

Replace `RL000XXX` with your logger's serial number. Every command below saves its files into the folder you are in.

!!! warning "Why outside the code folder?"
    A cellular logger's backup contains its server access token. Git sees any new file inside the `Riverlabs` folder, and a `git add .` would add the backup to the next commit. Keep the backup folder private: do not put it in a shared or public folder, and never upload it to GitHub.

### 3. Get the logger ready

This is the same as for an upload:

1. Set the power switch to **OFF**.
2. Unplug the white sensor plug.
3. Plug in the FTDI cable: green wire to **GRN**, black wire to **BLK**.
4. Plug the USB end into the computer.
5. Close the Arduino IDE's Serial Monitor, and any other program that might be using the cable.

### 4. Find the port

=== "macOS"

    ```bash
    ls /dev/cu.usbserial*
    ```

    It prints a name like `/dev/cu.usbserial-A10KJ3HZ`.

=== "Linux"

    ```bash
    ls /dev/ttyUSB*
    ```

    It prints a name like `/dev/ttyUSB0`.

=== "Windows"

    Open **Device Manager** and expand **Ports (COM & LPT)**. Look for "USB Serial Port (COM5)" or similar. The port is the `COM` part.

If more than one port shows up, unplug the cable, look again, then plug it back in. The one that disappears and comes back is the logger.

### 5. Set two shortcuts

These save you typing the long avrdude path and the port name in every command. Replace the port with the one you found in step 4.

=== "macOS and Linux"

    ```bash
    AVRDUDE=~/.platformio/packages/tool-avrdude/avrdude
    PORT=/dev/cu.usbserial-XXXX
    $AVRDUDE -v
    ```

=== "Windows (PowerShell)"

    ```powershell
    $AVRDUDE = "$HOME\.platformio\packages\tool-avrdude\avrdude.exe"
    $PORT = "COM5"
    & $AVRDUDE -v
    ```

    On Windows, start every avrdude command below with `& $AVRDUDE` instead of `$AVRDUDE`. We have not tested these commands on Windows yet.

The last command should print `Avrdude version 8.1` or a higher number. If it says "no such file" or "not found", go back to step 1.

The shortcuts only last while this terminal window is open. If you open a new window, set them again.

!!! tip "Use the right avrdude"
    PlatformIO can keep an older avrdude in a folder next to it called `tool-avrdude@1.60300...`. That one is version 6.3. Use the path shown above, without the `@`.

### 6. Check that the logger answers

This only reads from the logger. It changes nothing.

```bash
$AVRDUDE -c arduino -p atmega328p -P $PORT -b 57600 -n
```

It should end with `Avrdude done.  Thank you.` and have no lines starting with `Error`. If you see `not in sync` or `programmer is not responding`, go through the checks in [Uploading Code → Troubleshooting](uploading-code.md#troubleshooting) and try again.

### 7. Save the code and the stored readings

Put the logger's serial number and today's date in the file names, so you can tell backups apart later:

```bash
$AVRDUDE -c arduino -p atmega328p -P $PORT -b 57600 \
  -U flash:r:RL000XXX_2026-11-09_flash.hex:i \
  -U eeprom:r:RL000XXX_2026-11-09_eeprom.hex:i
```

On Windows, write the command on one line without the `\` at the ends of lines.

It should print these two lines, then `Avrdude done.  Thank you.`:

```text
Writing 32768 bytes to output file RL000XXX_2026-11-09_flash.hex
Writing 1024 bytes to output file RL000XXX_2026-11-09_eeprom.hex
```

Then check that the saved file matches the logger. This also only reads:

```bash
$AVRDUDE -c arduino -p atmega328p -P $PORT -b 57600 \
  -U flash:v:RL000XXX_2026-11-09_flash.hex:i
```

It should print `32768 bytes of flash verified`.

### 8. Make the app-only copy

This is the copy you restore from later. It keeps the program and leaves out the bootloader. It is made from the file you just saved, so the logger does not need to be connected:

```bash
$AVRDUDE -c dryrun -p atmega328p \
  -U flash:w:RL000XXX_2026-11-09_flash.hex:i \
  -T "save flash 0 0x7E00 RL000XXX_2026-11-09_app_only.hex:i"
```

`-c dryrun` tells avrdude to use a pretend chip in the computer's memory instead of a real logger. `save flash 0 0x7E00` saves the first `0x7E00` bytes (32,256 bytes), which is everything except the bootloader.

It should end with `Avrdude done.  Thank you.` If it says `Can't find programmer id "dryrun"`, your avrdude is too old. Go back to step 5 and check the version.

### What you end up with

| File | What is in it | When to use it |
|---|---|---|
| `..._app_only.hex` | The program, without the bootloader | **To restore over the FTDI cable** |
| `..._flash.hex` | The program and the bootloader (all 32 KB) | Only with an ISP programmer, to [repair a damaged bootloader](#if-the-bootloader-is-damaged). Never over the FTDI cable. |
| `..._eeprom.hex` | Readings stored on the logger | Rarely needed |

!!! warning "Keep backups private"
    A cellular logger's backup contains its server access token. Do not share the `.hex` files and never commit them to Git.

## Restore the original code

Get the logger ready as in [step 3](#3-get-the-logger-ready) and set the shortcuts as in [step 5](#5-set-two-shortcuts). Go to the logger's backup folder, then write the **app-only** file:

```bash
$AVRDUDE -c arduino -p atmega328p -P $PORT -b 57600 \
  -U flash:w:RL000XXX_2026-11-09_app_only.hex:i
```

It should end with a line ending in `bytes of flash verified`, then `Avrdude done.  Thank you.` Afterwards, open the Serial Monitor at 115200 baud and check that the start-up messages show the logger ID you expect.

A normal upload from the Arduino IDE or PlatformIO writes this same part of the memory, which is why this is the safe way to restore. We have not yet restored an app-only backup on a real logger, so please tell us how it goes.

!!! danger "Never write the full 32 KB backup over the FTDI cable"
    The `_flash.hex` file includes the bootloader. Writing it over the cable asks the bootloader to overwrite itself while it is running. The write fails near 99% with a protocol error, and after that the cable can no longer upload code. The program may still run, so this is easy to miss. We did this by mistake on a test logger. Always restore the `_app_only.hex` file over the cable.

You do not usually need to restore the EEPROM. It only holds readings, and those have normally already been saved or sent.

## If the bootloader is damaged

You will know because the logger runs its program normally, but every upload fails with `not in sync`, even with the sensor unplugged, the cable the right way round and the right board setting.

The repair needs an **ISP programmer**, for example the Pololu USB AVR Programmer v2.1, connected to the board's 6-pin ISP header. In outline:

1. Set the programmer to 3.3 V and do not let it power the board. Power the board from its battery with the switch ON, with the FTDI cable and the sensor unplugged.
2. Read and write down the chip signature, the fuses (lfuse, hfuse, efuse) and the lock bits. **Do not change the fuses.**
3. Back up the flash and EEPROM over ISP.
4. Write the full original backup (program and bootloader) over ISP, and verify it. Over ISP this is safe, because the programmer erases and writes the chip directly instead of going through the bootloader.
5. Put back the lock bits and the EEPROM if the erase changed them.
6. Check the serial bootloader answers again with `avrdude -c arduino -p atmega328p -P $PORT -b 57600 -n`.

With the Pololu programmer, use avrdude's `-c stk500v2` option with the programmer's own port.
