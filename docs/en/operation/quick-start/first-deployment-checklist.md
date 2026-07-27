# First Deployment Checklist

!!! success "Pre-Deployment Verification"
    Complete this checklist before deploying your logger in the field. Taking the time to do this now prevents failed deployments and wasted trips.

## Overview

This comprehensive checklist ensures your logger is fully tested and ready for deployment. Print this page and check off items as you complete them.

---

## Hardware Preparation

### Battery Installation

- Fully charged battery polarity verified **before** insertion
- CR1220 backup battery installed
- No loose connections or rattling

!!! tip "Battery Voltage Check"
    Use a multimeter to verify battery voltage. See [Battery & Power Guide](../hardware/battery-power-guide.md) for voltage interpretation.

### SD Card

The logger comes with a high-quality microSD card pre-installed.

!!! warning "Do Not Replace SD Card"
    The provided microSD card has been tested for reliability. **Do not replace it** unless absolutely necessary. Use the exact brand and spec as supplied.

- MicroSD card properly seated in slot

!!! info "Data Backup Options"
    - **Flash Memory:** Can be installed and used as an additional backup if required
    - **EEPROM:** Can sometimes recover lost data in case the SD card is incorrectly installed

### Physical Condition

- Enclosure intact, no cracks or damage
- Seals and gaskets in good condition
- Cable glands tight
- Sensor clean and undamaged
- Sensor lens/transducer free of debris
- All screws and fasteners tight
- Silica gel pack present and fresh (should not be off-colour)

---

## Software Configuration

### Clock Setup

- Real-time clock set using `set_clock.ino`
- Time verified in Serial Monitor
- UTC timezone used (if telemetry enabled)
- Clock verified accurate within 1 minute

### Main Script

- Correct script for logger model uploaded
    - `wari.ino` for Wari Ultrasonic loggers
    - `wari_lidar.ino` for Wari Lidar loggers
- Measurement interval configured appropriately
  ```cpp
  #define INTERVAL 5  // Your chosen interval in minutes
  ```
- Number of readings per measurement set
  ```cpp
  #define NREADINGS 10  // Typical value
  ```
- EEPROM flush interval configured
  ```cpp
  #define FLUSHAFTER 288  // Readings before SD write
  ```
- DEBUG mode enabled for initial testing
  ```cpp
  #define DEBUG  // Comment out for deployment
  ```

### Telemetry (If Applicable)

- XBee module installed and secure
- XBee configured for API mode with escapes
- Network APN settings correct for SIM provider
- ThingsBoard device credentials entered
- Telemetry interval appropriate for battery life
- SIM card installed with active data plan
- Test transmission successful

See [Telemetry Setup Guide](../../telemetry/setup-guide.md) for detailed configuration.

---

## Bench Testing

### Initial Power-Up

- Logger powers on (LED activity observed)
- No error LED patterns
- Serial output shows initialization (if DEBUG enabled)
- Clock shows correct time

### Sensor Testing

- Sensor measurements returning valid readings
- Readings consistent and reasonable
- Test at known distance to verify accuracy
- Multiple readings show low variance

**For Wari Ultrasonic:**

- Test targets at various distances (0.5 m, 1 m, 2 m, etc.)
- Verify no interference from nearby objects
- Check beam has clear path

**For Wari Lidar:**

- Test on different surface types
- Verify angle measurements (if using)
- Confirm readings from 10 cm to max expected range

### Data Logging

- Run logger for minimum 30 minutes
- LED flashes at expected intervals (when readings taken — compare to the serial output)
- SD card contains data file
- Data file readable and properly formatted
- Timestamps in data file correct
- Sensor values in expected range
- No missing or corrupted entries

**Example Expected Data Format:**
```
2025/12/27 14:30:00, 1250, 1248, 1252, 1249, 1251, 1250, 1248, 1251, 1249, 1250, 4150
```
*Shows date/time, 10 distance readings in mm and battery voltage in mV*

### Telemetry Testing (If Enabled)

- Initial connection to network successful
- Data transmitted to server
- Data visible on ThingsBoard dashboard
- Transmission timing as configured
- Connection LED patterns normal
- No excessive connection attempts


## Troubleshooting Quick Reference

If problems arise, see:

- [Common Issues](../troubleshooting/common-issues.md): Quick fixes
- [Diagnostic Flowcharts](../troubleshooting/diagnostic-flowcharts.md): Systematic diagnosis
- [FAQ](../troubleshooting/faq.md): Frequently asked questions

**Common Pre-Deployment Issues:**

| Symptom          | Likely Cause          | Quick Fix                                |
|------------------|-----------------------|------------------------------------------|
| No LED activity  | Battery dead/reversed | Check voltage & polarity                 |
| Erratic readings | Sensor obstructed     | Clean sensor, check beam path            |
| No SD writes     | SD card failure       | Check card, consider Flash memory backup |
| Clock reset      | No backup battery     | Install CR1220                           |
| No telemetry     | XBee not configured   | Verify API mode, settings                |

!!! success "Ready for Deployment!"
    If you've completed all items on this checklist, your logger is ready for reliable field operation. Keep this checklist for future reference and use it for subsequent deployments.

## Next Steps

- Proceed to [Mounting Guide](../../installation/mounting-guide.md)
- Set up monitoring on [ThingsBoard](../../telemetry/thingsboard-configuration.md)
- Review [Maintenance Schedule](../hardware/maintenance.md)
