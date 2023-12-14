# Firmware for the Riverlabs suite of sensors

## Overview

This is the code repository for the [Riverlabs](https://riverlabs.uk) suite of sensors. The sensors use an Arduino-compatible bootloader, and the recommended programming environment is therefore the Arduino IDE. To get started with Arduino, we refer to the excellent [Arduino documentation](https://www.arduino.cc/en/Guide/HomePage).

Full documentation and instructions can be found on our [Github Pages site](https://ichydro.github.io/Riverlabs/).

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


## Acknowledgements

Our code is based on numerous libraries, examples, and discussion posts from the Arduino community. We do our best to acknowledge and reference all sources of external code and specific solutions. For any improvements, corrections, and other comments, do not hesitate to [get in touch](https://www.imperial.ac.uk/people/w.buytaert). 







