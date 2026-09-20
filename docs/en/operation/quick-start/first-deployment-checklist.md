# First Deployment Checklist

## Purpose

This checklist helps you checking that your logger is ready for deployment.

### Where to start

To check proper functioning of the logger, press the reset button. There is no risk for data loss or change of any settings; resetting the logger simply restarts its internal software.

When the logger (re)starts, it will perform some internal checks such as checking access to the SD card and/or telemetry modem. A successful check will result in a single flash of the red LED. This is typically a short flash (< 1s) but can be longer if the logger finds data in the internal memory and flushes them to the SD card (see the [data handling](../../hardware/data-handling.md) section). When this is finished, pressing reset again will result in a short flash (as all data have already been flushed).

A series of 3x3 flashes indicates an issue.

No red flash means that the logger does not start up, most likely because of a power issue.
x
### Battery and power

- Logger powers on (LED activity observed)
- Battery fully charged and polarity verified. (See [Battery & Power Guide](../hardware/battery-power-guide.md) for voltage interpretation).
- CR1220 backup battery installed
- No loose connections or rattling
- If using a solar panel, check that it is properly and correctly wired, and that the charging LED lights up under sunlight.

### SD Card (if applicable)

- MicroSD card properly seated in slot
- No error LED patterns (3 x 3 flashes)

### Logger enclosure

- Enclosure intact with no cracks or damage
- Seals and gaskets in good condition
- Cable glands are tight
- Sensor clean and undamaged
- Sensor lens/transducer free of debris
- All screws and fasteners are tight
- Silica gel pack present and fresh

### Telemetry (If Applicable)

- XBee module installed and secure
- XBee configured for API mode with escapes
- Network APN settings correct for SIM provider
- ThingsBoard device credentials entered
- Telemetry interval appropriate for battery life
- SIM card installed with active data plan
- Test transmission successful

See the [Telemetry Setup Guide](../../telemetry/setup-guide.md) for detailed configuration.

### Logger output

Logger output can be tested by reading out the SD card, checking the telemetry returns, or attaching a serial cable and observing the serial debugging output. It is advised to run the logger for 30 minutes and collect several readings before taking it out for installation.

- Clock shows correct time
- Sensor measurements returning valid readings
- Readings consistent and reasonable
- Multiple readings show low variance
- LED flashes at expected intervals
- SD card contains data file
- Data file readable and properly formatted
- Sensor values in expected range
- No missing or corrupted entries

**For Wari Ultrasonic:**

- Test targets at various distances (0.5 m, 1 m, 2 m, etc.)
- Verify no interference from nearby objects
- Check beam has clear path

**For Wari Lidar:**

- Test on different surface types
- Verify angle measurements (if using)
- Confirm readings from 10 cm to max expected range


## Troubleshooting Quick Reference

If problems arise, see:

- [Common Issues](../troubleshooting/common-issues.md): Quick fixes
- [Diagnostic Flowcharts](../troubleshooting/diagnostic-flowcharts.md): Systematic diagnosis
- [FAQ](../troubleshooting/faq.md): Frequently asked questions

**Common Pre-Deployment Issues:**

| Symptom          | Likely Cause          | Quick Fix                                |
|------------------|-----------------------|------------------------------------------|
| No LED activity  | Battery dead/reversed | Check voltage and polarity               |
| Erratic readings | Sensor obstructed     | Clean sensor, check beam path            |
| No SD writes     | SD card failure       | Check card, consider flash memory backup |
| Clock reset      | No backup battery     | Install CR1220                           |
| No telemetry     | XBee not configured   | Verify API mode and settings             |

## Next Steps

- Proceed to [Mounting Guide](../../installation/mounting-guide.md)
- Set up monitoring on [ThingsBoard](../../telemetry/thingsboard-configuration.md)
- Review [Maintenance Schedule](../hardware/maintenance.md)
