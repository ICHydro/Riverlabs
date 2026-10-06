# Firmware for the Riverlabs suite of sensors

## Overview

This is the code repository for the [Riverlabs][riverlabs] suite of sensors. The sensors use an Arduino-compatible bootloader, and the recommended programming environment is therefore the Arduino IDE. To get started with Arduino, we refer to the excellent [Arduino documentation][arduino-documentation].

Full documentation and instructions can be found on our [Github Pages site][github-pages].

## Sketches

Currently, we use a different sketch for each logger model. We are working to integrate them into a single sketch, which will happen in the not-too-distant future. For now, make sure to use the correct sketch.

### set_clock.ino

This sketch sets the internal clock of the logger. If you make use of the backup coin battery, this only needs to be done once. The loggers can be used without backup battery but then the clock needs to be reset every time the main battery is taken out.

### wari_v1.ino

Code for the oldest generation of our Maxbotix ultrasound logger, without telemetry. Only use on loggers with serial number of RL000277 or lower.

### wari_v2.0.ino

Code for the second generation of our Maxbotix ultrasound logger, without telemetry. Only use on loggers with serial number between RL000278 and RL000330.

### wari_v2.1.ino

Code for the latest generation of our Maxbotix ultrasound logger, without telemetry. Use if the serial number is RL000331 or higher.

### wari_3G.ino

Code for the oldest generation of our Maxbotix ultrasound logger, with 3G cellular telemetry. Only use if your serial number is RL000277 or lower and has a DIGI 3G cellular modem.

### wari_3G_v2.ino

Code for the newest generation of our Maxbotix ultrasound logger, with 3G cellular telemetry. Only use if your serial number is RL000278 or higher and has a DIGI 3G cellular modem.

### wari_4G.ino

Code for our Maxbotix ultrasound logger, with 4G cellular telemetry. Only use if your logger has a DIGI LTE-M/NB-IoT cellular modem.

### wari_lidar.ino

Code for our Garmin Lidarlite logger, without telemetry. Formerly known as WMO_SD.ino.

### wari_lidar_cellular.ino

Code for our Garmin Lidarlite logger with cellular modem. Formerly known as WMOnode.ino. This works for both the 3G and LTE-M/NB-IoT modems but make sure to set the correct compiler definitation at the top of the code.

### wari_lidar_lora.ino

Code for our Garmin Lidarlite logger with lora radio. Formerly known as WMO_SD_lora.ino.

### feather_lora_lidar.ino

Example code for our Adafruit feather based loggers. This one is for the Lora feather.

## Changelog

* 2023/12/14: Updating the docs. Changing the names of the sketches to provide more consistency.

## Developer notes

### Using PlatformIO

PlatformIO is used for development work on this project. We recommend using [VS Code][vscode] with the [PlatformIO IDE extension][platformio-extension].

The `platformio.ini` file contains the configuration details for each sensor environment. To compile for all sensors, use:

```bash
pio run
```

To specify a particular sensor, use the environment name defined in `platformio.ini`, for example:

```bash
pio run -e wari_3G
```

Static analysis checks (using [`clang-tidy`][clang-tidy]) can be run similarly:

```bash
pio check -e wari_3G
```

To add a new sensor, add a new environment to the `platformio.ini` file, specifying the library dependencies, `custom_sensor_dir` (which contains the `.ino` sketch), [`check_src_filters`][check-src-filters] and [`build_src_filter`][build-src-filter]. More information regarding the available `platformio.ini` options can be found in the [documentation][platformio-documentation].

### Pre-commit

Pre-commit hooks are also available for this project. To install and update them, use:

```bash
pre-commit install
pre-commit autoupdate
```

To run the pre-commit hooks on all files, use:

```bash
pre-commit run --all-files
```

## Acknowledgements

Our code is based on numerous libraries, examples, and discussion posts from the Arduino community. We do our best to acknowledge and reference all sources of external code and specific solutions. For any improvements, corrections, and other comments, do not hesitate to [get in touch][contact].

[riverlabs]: https://riverlabs.uk
[arduino-documentation]: https://www.arduino.cc/en/Guide/HomePage
[github-pages]: https://ichydro.github.io/Riverlabs/
[contact]: https://www.imperial.ac.uk/people/w.buytaert
[vscode]: https://code.visualstudio.com/
[platformio-extension]: https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide
[clang-tidy]: https://clang.llvm.org/extra/clang-tidy/
[check-src-filters]: https://docs.platformio.org/en/latest/projectconf/sections/env/options/check/check_src_filters.html
[build-src-filter]: https://docs.platformio.org/en/latest/projectconf/sections/env/options/build/build_src_filter.html
[platformio-documentation]: https://docs.platformio.org/en/latest/projectconf/index.html
