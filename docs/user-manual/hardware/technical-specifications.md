## Wari logger

### Overview

The Wari logger is based around the Maxbotix MB7389 Ultrasound distance sensor. Due to the short range of ultrasound sensors (5&nbsp;m in the case of the MB7389) ultrasound loggers are mostly suitable for water level sensing in small rivers and reservoirs. The Wari does not have telemetry functionality, and is therefore only suited for offline data collection with local storage onto an SD card. It is engineered for ultra low power consumption, with a battery life of over 5 years under typical use.

## Detailed Specifications

### Wari (Ultrasound)

| Specification | Value | Notes |
|--------------|-------|-------|
| **Sensor Type** | Ultrasonic | Maxbotix MB7389 HRXL |
| **Measurement Range** | 0.3m - 5m | Practical range ~4.5m |
| **Resolution** | 1mm | Under ideal conditions |
| **Accuracy** | ±1% | Temperature dependent |
| **Beam Width** | ~15° cone | Requires obstacle clearance |
| **Mounting Angle** | Vertical preferred | Angled mounting reduces accuracy |
| **Target Surface** | Water, solid objects | Good water surface reflectivity |
| **Temperature Range** | -40°C to +65°C | Sensor operating range |
| **Power Consumption** | ~50mA active | Low power between readings |
| **CPU** | Atmel Atmega328 | Arduino Pro Mini compatible |


### Lidar

| Specification | Value | Notes |
|--------------|-------|-------|
| **Sensor Type** | Laser rangefinder | Garmin Lidarlite v3HP |
| **Measurement Range** | 0.05m - 35m | Depends on target reflectivity |
| **Resolution** | 1cm | Consistent across range |
| **Accuracy** | ±2.5cm | Up to 40m |
| **Beam Divergence** | 8 milliradians | Very narrow, <1° effective |
| **Mounting Angle** | Up to 40° | Minimal accuracy loss |
| **Target Surface** | Most surfaces | Best with rough/turbid water |
| **Temperature Range** | -20°C to +60°C | Sensor operating range |
| **Power Consumption** | ~100mA active | Higher but infrequent |
| **CPU** | Atmel Atmega328 | Arduino Pro Mini compatible |


## Physical Identification

### Finding the Model Number

!!! tip "Model Number Location"
    The model designation is typically found on a label on the **back of the enclosure** or **inside the battery compartment**.

**Look for:**

- **Serial Number:** "RL0XXX" format (e.g., RL000123)
- **Sensor Type:** Ultrasound (Maxbotix) or Lidar (Garmin)
- **Telemetry:** Check for XBee cellular modem or LoRa radio

### Firmware Selection by Serial Number

| Serial Number Range | Sensor | Telemetry | Firmware File |
|---------------------|--------|-----------|---------------|
| RL000001 - RL000277 | Ultrasound | None | `wari_v1.ino` |
| RL000001 - RL000277 | Ultrasound | 3G Cellular | `wari_3G.ino` |
| RL000278 - RL000330 | Ultrasound | None | `wari_v2.0.ino` |
| RL000278+ | Ultrasound | 3G Cellular | `wari_3G_v2.ino` |
| RL000331+ | Ultrasound | None | `wari_v2.1.ino` |
| Any | Ultrasound | 4G LTE-M/NB-IoT | `wari_4G.ino` |
| Any | Lidar | None | `wari_lidar.ino` |
| Any | Lidar | 3G or 4G Cellular | `wari_lidar_cellular.ino` * |
| Any | Lidar | LoRa Radio | `wari_lidar_lora.ino` |

**\*** Set correct modem type (3G or LTE-M) in compiler definition at top of code

!!! info "Legacy Names"
    - `wari_lidar.ino` was formerly `WMO_SD.ino`
    - `wari_lidar_cellular.ino` was formerly `WMOnode.ino`
    - `wari_lidar_lora.ino` was formerly `WMO_SD_lora.ino`



### Technical specifications

| Specification | Value |
|---------------|-------|
| **Range** | 300 mm - 5000 mm |
| **Resolution (water level)** | 1 mm |
| **Resolution (temperature)** | 0.25°C |
| **Resolution (voltage)** | 0.01 V |
| **Accuracy (water level)** | ~ 5 mm |
| **Power supply** | 1 x 3.6 V Li-ion battery (18650) |
| **Water resistance** | IP67 |

### PCB layout

![PCB layout of the Wari](images/wari_layout.jpg "Wari PCB layout")

### Power considerations

## WMOnode

### Overview

The WMOnode was developed as part of the WMO funded Innovation Hub project. It is based around a Lidarlite v3HP lidar distance sensor with a range of up to 35 m, and has a slot for a telemetry module (XBee form factor). We typically combine it with a DIGI 3G Cellular modem for telemetry.

### Technical specifications

| Specification | Value |
|---------------|-------|
| **Range** | 0.05 m - 35 m |
| **Resolution (water level)** | 1 cm |
| **Resolution (temperature)** | 0.25°C |
| **Resolution (voltage)** | 0.01 V |
| **Accuracy (water level)** | ~ 5 - 10 cm |
| **Power supply** | 1 x 3.6 V Li-ion battery (18650) |
| **Water resistance** | IP67 |

### PCB layout

### Power considerations