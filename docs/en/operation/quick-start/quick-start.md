# Quick Start Guide

## What You'll Need

Before you begin, make sure you have:

- Your Riverlabs logger (Ultrasonic or Lidar)
- Main battery (a 14500 or 18650 depending on type)
- CR1220 coin battery (for RTC backup)
- MicroSD card 
- FTDI cable or adapter (for programming)
- Arduino IDE installed on your computer

## 1. Identify Your Logger

First, identify which logger model you have. See the [Logger Identification Guide](logger-identification.md) for detailed comparisons.

<div class="grid cards logger-cards" markdown>

-    **Wari Ultrasound Logger**

    ---

    **Ultrasound Distance Sensor**

    ![Wari logger](../../assets/images/Wari_v1.jpg){ width="250" }

    **Sensor:** Maxbotix MB7389  
    **Range:** 0.3 m–5 m  
    **Resolution:** 1 mm  
    **Beam angle:** Wide (~15°)  
    **Best for:**

    - Water level monitoring
    - Budget-conscious projects
    - Shorter-range applications
    - Vertical mounting positions

-   :material-laser-pointer:{ .lg .middle } **Lidar Logger**

    ---

    **Wari Lidar Logger**

    ![Lidar logger](../../assets/images/WMOnode.jpg){ width="250" }

    **Sensor:** Garmin Lidarlite v3HP  
    **Range:** 0.05 m–35 m  
    **Resolution:** 1 cm  
    **Beam angle:** Very narrow (~0.5°)  
    **Best for:**
    
    - Long-range measurements
    - Angled installations (up to 40°)
    - High-precision applications
    - Difficult mounting situations

</div>

## 2. Insert Batteries

!!! danger "Check Polarity First!"
    Double-check battery orientation before insertion. Match the **+** and **-** markings on the battery with the markings in the battery compartment.

**RTC Battery (CR1220):**

!!! tip "Date Preservation" 
    The coin cell may already be installed. Only replace it if needed to preserve the programmed date. Install the main battery first and switch on.

1. Switch off the board
2. Locate the small coin battery slot on the PCB (sometimes on the reverse)
3. Insert the CR1220 battery (typically **+** side up)
4. Press firmly until it clicks into place
5. If applicable, screw the circuit board back into the enclosure

**Main Battery (ususally a 18650 or 14500):**

1. Switch off the board
2. Check you have the right battery (see the [Battery & Power Guide](../hardware/battery-power-guide.md))
3. Verify the polarity (**+**/**-**) and charge
4. Insert the battery, ensuring the metal clips touch the battery

### 3. Prepare SD Card

Your logger may need a microSD card.

- Insert the card into the logger's microSD slot
- Ensure it clicks into place (you should be able to feel this)

### 4. Set the Clock

!!! warning "Important First Step"
    The clock must be set before first use. Set the clock to **UTC time zone** if using telemetry.

**Before programming:**

!!! danger "CRITICAL: Disconnect Sensor First"
    **For Wari Ultrasonic models:** Disconnect the Maxbotix sensor (3-pin connector) from the logger before programming. The sensor and FTDI cable use the same serial port, causing interference that prevents successful code upload. Reconnect the sensor after programming is complete.

**Clock setting steps:**

1. **Disconnect sensor** (white connector) if using Wari Ultrasonic
2. Connect the FTDI cable to your logger
   - Align green wire with "GRN" marking
   - Align black wire with "BLK" marking
3. Set power switch to OFF position (powered by FTDI)
4. Open Arduino IDE
5. Set board to **MiniCore → ATmega328**
6. Set clock to **External 8 MHz**
7. Load the `set_clock.ino` sketch (in repository root)
8. Upload to the logger
9. Open Serial Monitor (baud rate: 57600) to verify time

**The clock only needs to be set once** (unless both batteries are removed).

### 5. Firmware Selection by Serial Number

| Serial Number Range | Sensor      | Telemetry         | Firmware File               |
|---------------------|-------------|-------------------|-----------------------------|
| RL000001–RL000277   | Ultrasound  | None              | `wari_v1.ino`               |
| RL000001–RL000277   | Ultrasound  | 3G Cellular       | `wari_3G.ino`               |
| RL000278–RL000330   | Ultrasound  | None              | `wari_v2.0.ino`             |
| RL000278+           | Ultrasound  | 3G Cellular       | `wari_3G_v2.ino`            |
| RL000331+           | Ultrasound  | None              | `wari_v2.1.ino`             |
| Any                 | Ultrasound  | 4G LTE-M/NB-IoT   | `wari_4G.ino`               |
| Any                 | Lidar       | None              | `wari_lidar.ino`            |
| Any                 | Lidar       | 3G or 4G Cellular | `wari_lidar_cellular.ino`*  |
| Any                 | Lidar       | LoRa Radio        | `wari_lidar_lora.ino`       |

*\*Set correct modem type (3G or LTE-M) in compiler definition at top of code*

!!! info "Legacy Names"
    - `wari_lidar.ino` was formerly `WMO_SD.ino`
    - `wari_lidar_cellular.ino` was formerly `WMOnode.ino`
    - `wari_lidar_lora.ino` was formerly `WMO_SD_lora.ino`


### 6. Upload Main Script

Now upload the main logging script:

!!! warning "Sensor Must Be Disconnected"
    Keep the sensor disconnected during this step as well (Wari Ultrasonic only).

1. Open the appropriate script in Ardunio IDE

2. Review and adjust settings at the top of the script:
   ```cpp
   #define INTERVAL 5             // Measurement interval in minutes
   #define NREADINGS 10           // Number of sensor readings per measurement
   #define FLUSHAFTER 288         // Readings before flushing to SD card
   ```
3. Upload the script to your logger
4. **Reconnect the sensor** (white connector) after upload completes
5. Verify operation via Serial Monitor (if DEBUG is enabled)
6. Disconnect FTDI cable
7. Switch power to ON position

!!! success "Upload Complete"
    When the LED flashes after upload, your logger is programmed and ready. Don't forget to reconnect the sensor!

!!! tip "Battery Considerations"
    Higher logging frequency = shorter battery life. Start with conservative settings (10–15 minute intervals) for deployments.

### 7. Test Before Deployment

Before taking your logger to the field, perform a bench test:

- **Sensor reconnected** (Matbotix 3-pin connector plugged in)
- Power switch in ON position
- Verify LED flashes during measurements
- Check that data is being written to SD card
- Confirm sensor readings are reasonable
- Test for at least 30 minutes to ensure stable operation

See the [First Deployment Checklist](first-deployment-checklist.md) for comprehensive pre-deployment testing.

### 8. Install in Field

Your logger is now ready for field deployment! See the [Mounting Guide](../../installation/mounting-guide.md) for detailed installation instructions including:

- Zip tie mounting methods
- Adhesive mounting techniques
- Sensor positioning and orientation
- Environmental considerations

## Next Steps

- [Logger Identification](logger-identification.md): Compare Wari vs Lidar in detail
- [First Deployment Checklist](first-deployment-checklist.md): Complete pre-deployment verification
- [Telemetry Setup](../../telemetry/setup-guide.md): Configure cellular data transmission
- [Battery & Power Guide](../hardware/battery-power-guide.md): Understand battery life and voltage

## Need Help?

- Check [Common Issues and Solutions](../troubleshooting/common-issues.md) for quick solutions
- Review [Diagnostic Flowcharts](../troubleshooting/diagnostic-flowcharts.md) for systematic troubleshooting
- Contact support: info@riverlabs.uk

---

!!! success "Ready to Deploy?"
    Once you've completed these steps and verified operation, your logger is ready for field deployment. Remember to document your installation location and take photos for future reference!