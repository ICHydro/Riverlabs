### Overview

The Wari logger platform is compatible with many sensors, however the most common implementations use distance sensors for non-contact measurement of water levels in rivers and lakes. We use ultrasonic sensors (mostly the Maxbotix MB7389) for short range measuremnts (up to 5m) and lidar (Garmin Lidarlite) for longer (up to 40m) measurements. The lidar sensor has the added advantage that it can be installed at an angle from the vertical, but it is less accurate. 

We have successfully deployed wari loggers with other sensors, such as soil moisture sensors, water quality sensors, and tipping bucket rain gauges. Some of those require a bit of thinkering, or additional electronic modules. They are not explicitly covered in this guide, but if you are interested in those, then please get in touch. As our designs mature, we aim to add them here.

Here is an overview of the main characteristics of the Ultrasonic and Lidar models:

## Detailed Specifications

| Specification         | Ultrasound           | Lidar                       |
|-----------------------|----------------------|-----------------------------|
| **Sensor Model**      | Maxbotix MB7389 HRXL | Garmin Lidarlite v3HP       |
| **Measurement Range** | 0.3 m–5 m            | 0.05 m–35 m                 |
| **Resolution**        | 1 mm                 | 1 cm                        |
| **Resolution (water level)** | 1 mm          | 1 cm                        |
| **Resolution (temperature)** | 0.25°C        | 0.25°C                      |
| **Resolution (voltage)**     | 0.01 V        | 0.01 V                      |
| **Accuracy (water level)**   | ±5mm          | ± 1%                        |
| **Beam Width**        | ~15° cone            | 8 milliradians              |
| **Mounting Angle**    | Vertical preferred   | Up to 40° from the vertical |
| **Target Surface**    | Water, solid objects | Most surfaces               |
| **Temperature Range** | -40°C to +65°C       | -20°C to +60°C              |
| **CPU**               | Atmel Atmega328      | Atmel Atmega328             |


## Physical Identification

### Finding the Model Number

The model designation is typically found on a label on the side of the enclosure, but can also be on the inside.

**Look for:**

- **Serial Number:** "RL0XXX" format (e.g., RL000123)
- **Sensor Type:** Ultrasound (Maxbotix) or lidar (Garmin)
- **Telemetry:** Check for XBee cellular modem or LoRa radio

### Firmware Selection by Serial Number

| Serial Number Range | Sensor     | Telemetry         | Firmware File              |
|---------------------|------------|-------------------|----------------------------|
| RL000001–RL000277   | Ultrasound | None              | `wari_v1.ino`              |
| RL000001–RL000277   | Ultrasound | 3G Cellular       | `wari_3G.ino`              |
| RL000278–RL000330   | Ultrasound | None              | `wari_v2.0.ino`            |
| RL000278+           | Ultrasound | 3G Cellular       | `wari_3G_v2.ino`           |
| RL000331+           | Ultrasound | None              | `wari_v2.1.ino`            |
| Any                 | Ultrasound | 4G LTE-M/NB-IoT   | `wari_4G.ino`              |
| Any                 | Lidar      | None              | `wari_lidar.ino`           |
| Any                 | Lidar      | 3G or 4G Cellular | `wari_lidar_cellular.ino`* |
| Any                 | Lidar      | LoRa Radio        | `wari_lidar_lora.ino`      |

**\*** Set correct modem type (3G or LTE-M) in compiler definition at top of code

!!! info "Legacy Names"
    If you have been using our Github repository for some time, you may have noticed some name changes. To avoid confusionL
    - `wari_lidar.ino` was formerly `WMO_SD.ino`
    - `wari_lidar_cellular.ino` was formerly `WMOnode.ino`
    - `wari_lidar_lora.ino` was formerly `WMO_SD_lora.ino`
