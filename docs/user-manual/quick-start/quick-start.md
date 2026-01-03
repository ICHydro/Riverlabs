# Quick Start Guide

!!! tip "New to Riverlabs Loggers?"
    This guide will get you up and running with your Riverlabs environmental logger in minutes. For detailed technical information, see the [Developer Manual](../../arduino.md).

## What You'll Need

Before you begin, make sure you have:

- Your Riverlabs logger (Ultrasonic or Lidar)
- Main Battery (a 14500 or 18650 depening on type).
- CR1220 coin battery (for RTC backup)
- Micro SD card 
- FTDI cable or adapter (for programming)
- Arduino IDE installed on your computer

!!! warning "Battery Safety"
    **Always check battery polarity before insertion.** Incorrect polarity can cause permanent damage and fire risk, especially on loggers with solar charging circuits.

## Quick Setup Steps

### 1. Identify Your Logger

First, identify which logger model you have. See the [Logger Identification Guide](logger-identification.md) for detailed comparisons.

=== "Wari Ultrasound"

    ![Wari logger](../../images/Wari_v1.jpg){ width="300" }
    
    **Key Features:**
    
    - Maxbotix MB7389 ultrasound sensor
    - 0.3m - 5m range
    - Best for water level monitoring
    - Wide beam angle (requires clearance)

=== "Wari Lidar"

    ![Lidar logger](../../images/WMOnode.jpg){ width="300" }
    
    **Key Features:**
    
    - Garmin Lidarlite v3HP sensor
    - 0.05m - 35m range
    - Can measure at angles up to 40°
    - Narrow beam (precise measurements)

### 2. Insert Batteries

!!! danger "Check Polarity First!"
    Double-check battery orientation before insertion. Match the **+** and **-** markings on the battery with the markings in the battery compartment.

0. Switch off the board

**RTC Battery (CR1220):**

!!! tip "Coin cell may already be installed, only replace if needed to preserve programmed date install the main battery first and switch on.

1. Locate the small coin battery slot on the PCB (sometimes on the revere)
2. Insert the CR1220 battery (+ side up, typically)
3. Press firmly until it clicks into place

!!! tip "Battery Life"
    A freshly charged 18650 lipo battery should show ~4.2V. The logger will function down to ~3.3V. See the [Battery & Power Guide](../hardware/battery-power-guide.md) for detailed voltage interpretation.

4. if applicable screw the circuit board back into the enclosure.

**Main Battery (18650 or 14500):**

2. Verify polarity markings
3. Insert the 18650 or 14500 battery (typically flat end first)


### 3. Prepare SD Card

Your logger needs a micro SD card to store data locally.

2. Insert the card into the logger's micro SD slot
3. Ensure it clicks into place - you should be able to feel this



### 4. Set the Clock

!!! warning "Important First Step"
    The clock must be set before first use, and should be set to **UTC time zone** if using telemetry.

**Before Programming:**

!!! danger "Disconnect Sensor First - CRITICAL"
    **For Wari Ultrasonic models:** Disconnect the Maxbotix sensor (white connector) from the logger before programming. The sensor and FTDI cable use the same serial port, causing interference that prevents successful code upload. Reconnect the sensor after programming is complete.

**Clock Setting Steps:**

1. **Disconnect sensor** (white connector) if using Wari Ultrasonic
2. Connect the FTDI cable to your logger
   - Align green wire with "GRN" marking
   - Align black wire with "BLK" marking
3. Set power switch to OFF position (powered by FTDI)
4. Open Arduino IDE
5. Set board to **Arduino Pro or Pro Mini**
6. Set processor to **ATmega328P (3.3V, 8MHz External)**
7. Load the `set_clock.ino` sketch (in repository root)
8. Upload to the logger
9. Open Serial Monitor (baud rate: 57600) to verify time

**The clock only needs to be set once** (unless both batteries are removed).


### 5. Upload Main Script

Now upload the main logging script:

!!! warning "Sensor Must Be Disconnected"
    Keep the sensor disconnected during this step as well (Wari Ultrasonic only).

1. Open the appropriate script for your logger model:
   - `wari.ino` - Wari Ultrasonic
   - `wari_lidar.ino` - Wari Lidar
   - `wari_3G.ino` or `wari_4G.ino` - Cellular telemetry models
   - etc.
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
    Higher logging frequency = shorter battery life. Start with conservative settings (10-15 minute intervals) for deployments.

### 6. Test Before Deployment

Before taking your logger to the field, perform a bench test:

- **Sensor reconnected** (white connector plugged in)
- Power switch in ON position
- Verify LED flashes during measurements
- Check that data is being written to SD card
- Confirm sensor readings are reasonable
- Test for at least 30 minutes to ensure stable operation

See the [First Deployment Checklist](first-deployment-checklist.md) for comprehensive pre-deployment testing.

### 7. Install in Field

Your logger is now ready for field deployment!

See the [Mounting Guide](../installation/mounting-guide.md) for detailed installation instructions including:

- Zip tie mounting methods
- Adhesive mounting techniques
- Sensor positioning and orientation
- Environmental considerations

## Next Steps

- [Logger Identification](logger-identification.md) - Compare Wari vs Lidar in detail
- [First Deployment Checklist](first-deployment-checklist.md) - Complete pre-deployment verification
- [Telemetry Setup](../telemetry/setup-guide.md) - Configure cellular data transmission
- [Battery & Power Guide](../hardware/battery-power-guide.md) - Understand battery life and voltage

## Need Help?

- Check [Common Issues](../troubleshooting/common-issues.md) for quick solutions
- Review [Diagnostic Flowcharts](../troubleshooting/diagnostic-flowcharts.md) for systematic troubleshooting
- Contact support: info@riverlabs.uk

---

!!! success "Ready to Deploy?"
    Once you've completed these steps and verified operation, your logger is ready for field deployment. Remember to document your installation location and take photos for future reference!
