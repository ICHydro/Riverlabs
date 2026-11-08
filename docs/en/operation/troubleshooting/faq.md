# Frequently Asked Questions

!!! abstract "Overview"
    This page answers common questions about Riverlabs loggers.

## General Questions

### What is a Riverlabs logger?

A **Riverlabs logger** is an open-source, low-cost water level monitoring device designed for hydrological research. It uses ultrasonic or lidar sensors to measure distance to water surface, recording data to SD card and optionally transmitting via cellular telemetry.

**Key features:**
- open hardware design
- Battery-powered (rechargeable Li-ion)
- SD card data storage
- Optional telemetry via cellular connection or LoRaWAN
- Weatherproof enclosure
- Low cost (~$100-300 depending on configuration)

---

### Ultrasound vs. Lidar - which should I choose?

**Quick answer:**

| Feature | Ultrasonic | Lidar |
|---------|-------------------|-------|
| **Range** | 0.3m - 5m | 0.05m - 40m |
| **Accuracy** | ±10mm | ±25mm |
| **Battery life** | 30-60 days | 15-30 days |
| **Cost** | Lower (~$30 sensor) | Higher (~$150 sensor) |
| **Temperature sensitivity** | Yes (built-in compensation) | No |
| **Works in snow/foam** | No (absorbed) | Yes |
| **Best for** | Rivers, ponds (stable targets) | Wide range, snow, long distances |

**Choose Ultrasonic if:**
- Water level range <5m
- Budget-constrained
- Longer battery life needed
- Stable liquid water target

**Choose Lidar if:**
- Need >5m range
- Measuring through snow or foam
- Temperature extremes (Lidar unaffected)
- Accuracy less critical (±25mm acceptable)

See [Logger Identification Guide](../quick-start/logger-identification.md) for detailed comparison.

---

### How long does the battery last?

**It depends on configuration:**

**ultrasonic, SD-only:**
- 15-minute logging: 30-60 days
- 5-minute logging: 15-30 days
- 1-minute logging: 7-10 days

**Lidar, SD-only:**
- 15-minute logging: 15-30 days
- 5-minute logging: 7-15 days
- 1-minute logging: 3-5 days

**With telemetry (add transmission overhead):**
- Hourly transmission: Reduce by 30-40%
- Daily transmission: Reduce by 10-20%

**Factors:**
- Battery capacity (2600mAh typical)
- Temperature (cold reduces capacity)
- Battery age (degrades over time)
- Telemetry signal strength (weak signal = more power)

See [Battery & Power Guide](../hardware/battery-power-guide.md) for detailed calculations and optimization tips.

---

### Can I add solar power?

The wari model contains a solar charging circuit, which is activated or deactivated at assembly. We do not recommend users to change this setting. If the solar panel of a logger with active charging circuit is too small, then it can be replaced by a larger solar panel, as long as the output voltage of the panel is **6V**.

---

### Is it open source? Can I modify the code?

**Yes! Fully open source:**

**License:** MIT License (permissive)

**Source code:** https://github.com/ICHydro/Riverlabs

**You can:**
- ✅ Use for any purpose (commercial or non-commercial)
- ✅ Modify code to fit your needs
- ✅ Share modifications
- ✅ Contribute improvements back to project

**You should:**
- Include original license in derivatives
- Credit original authors
- Share improvements (helps community)

**Common modifications:**
- Change logging interval
- Add additional sensors (temperature, humidity)
- Modify data format
- Implement custom sleep schedules
- Add new telemetry protocols

---

## Hardware Questions

### Can I use a different sensor?

**Yes, with code modifications:**

**Ultrasonic sensors:**
- MaxBotix MB7389 (default)
- Other MaxBotix models (MB7092, MB7360, etc.)
- HC-SR04 (cheap, but less reliable)

**Code changes needed:**
- Pin assignments
- Voltage levels (3.3V vs 5V)
- Communication protocol (analog vs serial vs I2C)

**Lidar sensors:**
- Garmin LIDAR-Lite v3 (default)
- TF-Luna (cheaper alternative)
- VL53L1X (short range only)

**Other distance sensors:**
- Pressure transducers (for submerged use)
- Float switches (on/off levels only)

**Recommendation:** Stick with default sensors unless you have specific requirements. They're well-tested and code is ready to use.

---

### What SD card should I use?

**Recommendations:**

**Capacity:** 8-32GB (larger cards waste money, logger won't fill them)

**Brand:** SanDisk, Samsung, Kingston (avoid off-brand, high failure rate)

**Type:** Standard SD or microSD with adapter (NOT SDXC)

**Format:** FAT32 (not exFAT or NTFS)

**Example calculation:**
- 15-minute logging
- 100 bytes per sample
- Samples per day: 96
- Data per day: 9.6 KB
- 16GB card: ~1000 years of data (!!)
- Realistic: 8GB is plenty

**Avoid:**
- Cards >32GB (may have exFAT, not FAT32)
- Ultra-high-speed (UHS-II) - waste of money for logging
- Micro SD without adapter (hard to handle)

---

### How do I waterproof the logger?

**Enclosure options:**

**1. IP67-rated enclosure** (recommended)
- Example: Hammond 1554, Bud Industries
- Gasket-sealed lid
- Cable glands for wires
- Cost: $15-30

**2. DIY waterproofing**
- Plastic food container (short-term only)
- Seal with silicone caulk
- Wrap in duct tape (adds some protection)
- Cost: $5

**Cable penetrations:**
- Use cable glands (best)
- Or: Drill hole, route cable, seal with silicone
- Strain relief: Tie knot inside enclosure (prevents pull-out)

**Testing:**
- Submerge enclosure in bucket (no electronics inside!)
- Wait 1 hour
- Open, check for water entry
- Fix leaks before deploying

**Desiccant:**
- Add silica gel packet inside enclosure
- Absorbs moisture (prevents condensation)
- Replace if saturated (turns pink)

See [Installation Guide](../../installation/mounting-guide.md) for detailed waterproofing instructions.

---

## Telemetry Questions

### What is the data usage of telemetry?

**Typical usage:**

**Per transmission:**
- JSON payload: 50-100 bytes (data only)
- HTTP overhead: 200-500 bytes (headers, handshake)
- Total: ~500-600 bytes per transmission

**Monthly usage examples:**

| Interval | Transmissions/day | Daily data | Monthly data |
|----------|-------------------|------------|--------------|
| Every measurement (15 min) | 96 | 58 KB | 1.7 MB |
| Hourly | 24 | 14 KB | 430 KB |
| Every 4 hours | 6 | 3.6 KB | 110 KB |
| Daily | 1 | 600 bytes | 18 KB |

**Recommended plan:** 5-10 MB/month (plenty of headroom)

**Cost:** ~$2-10/month depending on carrier and plan

---

## Field Deployment Questions

### How do I aim the sensor?

**General principles:**

**1. Target the water surface:**
- Beam should hit water (or expected water level range)
- Avoid banks, rocks, vegetation

**2. Vertical is ideal:**
- Straight down eliminates angle errors
- If angled: Use trigonometry to correct (see [Sensor Positioning](../../installation/sensor-positioning.md))

**3. Account for level changes:**
- Low flow: Where will water be? (don't aim too high)
- High flow: Ensure sensor not submerged (mount high enough)

**4. Beam width considerations:**
- Ultrasonic: ~15-20° cone
- Lidar: ~2° beam
- Ensure beam doesn't clip bridge/structure

**Testing:**
- Take measurements at deployment site (pre-install)
- Walk through expected water level range
- Verify readings make sense

---

### How do I know what installation height to use?

**Installation height** = Distance from sensor to datum (reference point)

**Common datums:**
1. **Bottom of channel** (streambed)
   - Measure: From sensor to streambed (lowest expected point)
   - Water depth = Installation height - Sensor reading

2. **Fixed benchmark** (survey monument)
   - Requires surveying equipment
   - Enables absolute elevation (e.g., meters above sea level)
   - Best for scientific studies

3. **Arbitrary reference**
   - "Top of bridge deck"
   - "Ground level at logger"
   - Consistent but not transferable to other locations

**Measurement:**
- Use tape measure, laser rangefinder, or survey equipment
- Measure when water is stable and measurable
- Record: Installation height and date
- Note: Streambed can change (erosion/deposition)

**Documentation:**
- Draw sketch showing logger, sensor, water, datum
- Photograph installation with tape measure visible
- GPS coordinates

---

### Can I deploy in winter/freezing conditions?

**Yes, with challenges:**

**Cold temperature effects:**

**1. Battery capacity reduction:**
- 0°C: ~80% capacity
- -20°C: ~50% capacity
- **Solution:** Larger battery, or retrieve more frequently

**2. Li-ion freezing risk:**
- LiPo freezes at ~-40°C
- At low temperatures, charging dangerous
- **Solution:** Don't charge below 0°C

**3. Ice on sensor:**
- Ice blocks ultrasound
- Lidar works through ice (to some extent)
- **Solution:** Heating element (complex), or use Lidar

**4. Condensation:**
- Temperature cycles cause moisture in enclosure
- **Solution:** Desiccant inside enclosure, IP67-rated enclosure

**5. Snow:**
- Ultrasonic absorbed by fresh snow (logger reads max range)
- Lidar works better but still affected
- **Solution:** Mount sensor to measure below ice (submerged pressure transducer)

**Recommendation for harsh winter:**
- Deploy late fall (before freeze)
- Retrieve early spring (after thaw)
- Or: Use heated enclosure + mains power (no longer low-cost)

---

### How often should I visit the site?

**Depends on:**
1. Battery life (see battery guide)
2. SD card capacity (non-issue for most)
3. Risk of logger loss/damage (vandalism, flooding)
4. Data criticality

**Typical schedules:**

**With telemetry:**
- Initial visit (1 week): Verify working
- Monthly visits: Check physical condition
- Battery swap: When voltage <3.5V (remotely monitored)

**Without telemetry:**
- Initial visit (1 week): Verify working
- Monthly visits: Check data, swap SD card if needed
- Battery swap: Monthly or based on expected life

**High-risk sites:**
- Weekly visits initially
- Consider more robust mounting or protective measures

**Remote sites:**
- Plan for seasonal access (winter may be inaccessible)
- Deploy in spring, retrieve in fall

---

### What if my logger is stolen or lost?

**Prevention:**

**1. Conceal:**
- Paint logger to match surroundings (brown/green)
- Hide behind vegetation
- Don't use bright colors or labels

**2. Secure mounting:**
- Security cable (bike lock style)
- Bolt to structure with tamper-resistant screws

**3. Signage:**
- Small label: "Research equipment, please do not disturb"
- Contact info (email, not phone)
- Some people will respect, but may attract attention

**4. Location choice:**
- Private property (with permission) safer than public
- Less visible locations

**Recovery:**

**1. Serial numbers:**
- Record SIM ICCID, XBee serial number, logger ID
- May help with insurance or carrier tracking

**2. Insurance:**
- Check if research equipment covered
- Document value with receipts

**3. Backup data:**
- If telemetry: Data safe on ThingsBoard
- If SD-only: Lost data (ouch)

**Acceptance:**
- Low-cost loggers = lower risk (easier to accept loss)
- Budget for 10% loss rate on multi-logger deployments

---

## Data and Analysis Questions

### What format is the data?

**CSV (Comma-Separated Values)**

**Example file: `2025-12-27.CSV`**

```csv
Date,Time,Distance_mm,Temperature_C,Battery_V
2025-12-27,10:00:00,1234,22.5,3.85
2025-12-27,10:15:00,1256,23.1,3.84
2025-12-27,10:30:00,1278,23.8,3.84
```

**Fields:**
- **Date:** YYYY-MM-DD
- **Time:** HH:MM:SS (24-hour)
- **Distance_mm:** Sensor reading (millimeters)
- **Temperature_C:** Air temperature (Wari only, Celsius)
- **Battery_V:** Battery voltage (volts)

**Opening:**
- Excel/Google Sheets: Directly open .CSV
- Python: `pandas.read_csv()`
- R: `read.csv()`
- MATLAB: `readtable()`

---

### How do I convert distance to water depth?

**Water depth = Installation height - Sensor distance**

**Example:**
- Logger mounted 3000mm above streambed
- Sensor reads 1500mm (distance to water surface)
- Water depth = 3000mm - 1500mm = 1500mm = 1.5m

**In Excel:**
```excel
=($A$1 - B2)
```
Where:
- A1: Installation height (constant)
- B2: Sensor reading (this row)

**In Python:**
```python
import pandas as pd
df = pd.read_csv('data.csv')
installation_height = 3000  # mm
df['water_depth_mm'] = installation_height - df['Distance_mm']
```

**Units:**
- Keep consistent (mm throughout)
- Convert to meters: Divide by 1000

---

### How accurate is the data?

**Sensor accuracy:**

**Wari (ultrasonic):**
- Manufacturer spec: ±10mm
- Real-world: ±20-50mm (accounting for temperature, mounting, etc.)

**Lidar:**
- Manufacturer spec: ±25mm
- Real-world: ±30-60mm

**Sources of error:**
1. **Sensor precision:** Inherent limitation
2. **Temperature:** Affects speed of sound (ultrasonic) or air refractive index (lidar)
3. **Mounting movement:** Wind, thermal expansion
4. **Beam angle:** If not perfectly vertical
5. **Target surface:** Turbulent water, foam, ripples
6. **Environmental:** Rain, snow, fog

**Improving accuracy:**
- Use Wari for absolute accuracy (<10mm)
- Vertical mounting (eliminates angle error)
- Stable mounting (minimize movement)
- Smooth target surface (use stilling well in turbulent conditions)
- Temperature compensation (Wari measures temp automatically)

**Good enough for:**
- Flood monitoring (0.5m changes)
- Stage-discharge relationships
- Presence/absence detection

**Not suitable for:**
- Precision surveying (<1mm)
- Legal water rights (use certified equipment)

---

### Can I synchronize multiple loggers?

**Time synchronization:**

**RTC accuracy:**
- DS3231 RTC: ±2 ppm (parts per million)
- Drift: ~1 minute per year
- Sufficient for most applications

**Synchronization methods:**

**1. Set all loggers same time:**
- Before deployment, run `set_clock` utility on all
- Use computer as time reference (sync computer with NTP)
- Drift: <1 minute per month (acceptable for most)

**2. GPS time synchronization (advanced):**
- Add GPS module to logger
- Sync RTC to GPS time daily
- Drift: <1 second
- Cost: +$20-40 per logger
- Power: Additional 20mA when GPS active

**3. Post-processing alignment:**
- Analyze data, find common event (e.g., rainfall spike)
- Shift time series to align events
- Software tools: R, Python, MATLAB

**For most users:** Method 1 is sufficient.

---

### Where can I find example code?

**GitHub repository:** https://github.com/ICHydro/Riverlabs

**Main sketches:**
- `wari_v2.1/wari_v2.1.ino` - Ultrasonic logger (latest)
- `wari_lidar/wari_lidar.ino` - Lidar logger
- `wari_3G/wari_3G.ino` - With 3G telemetry
- `wari_4G/wari_4G.ino` - With 4G LTE-M telemetry

**Utilities:**
- `set_clock/set_clock.ino` - RTC time setting
- `precip_SD/precip_SD.ino` - Precipitation logger (different sensor)

**Libraries used:**
- RTClib (Adafruit)
- SdFat (Bill Greiman)
- LowPower (Rocket Scream)
- XBee (Digi)
- Wire (I2C, built-in)

**Documentation:**
- Code comments explain functionality
- README.md in each directory
- This documentation site!

---

## Troubleshooting Questions (Quick Hits)

### Logger won't turn on

→ See [Issue 1: Logger Won't Power On](common-issues.md#issue-1-logger-wont-power-on)

**Quick check:** Battery >3.3V? Connected correctly? Power switch ON?

---

### SD card says "corrupt"

→ See [Issue 7: SD Card Corrupt](common-issues.md#issue-7-sd-card-shows-corrupt-or-needs-formatting)

**Quick fix:** Try recovery software, then format as FAT32.

---

### Sensor reads max range always

→ See [Issue 4: Sensor Reads Max Range](common-issues.md#issue-4-ultrasonic-sensor-reads-max-range-constantly)

**Quick check:** Sensor clicking? Aimed at target? Target in range?

---

### No telemetry data

→ See [Troubleshooting Telemetry Connections](../../telemetry/troubleshooting-connections.md)

**Quick check:** SIM active? Signal OK? API Mode = 2? Token correct?

---

### Battery drains too fast

→ See [Issue 2: Battery Drains Overnight](common-issues.md#issue-2-battery-drains-overnight)

**Quick check:** Logger sleeping? Telemetry interval reasonable? Battery old?

---

## Contributing and Community

### How can I contribute?

**Ways to contribute:**

**1. Code improvements:**
- Bug fixes
- New features
- Optimizations
- Submit pull requests on GitHub

**2. Documentation:**
- Correct errors
- Add examples
- Translate to other languages

**3. Hardware designs:**
- PCB improvements
- Enclosure designs (3D printable)
- Mounting solutions

**4. Testing and feedback:**
- Deploy loggers, report issues
- Share deployment photos and data
- Write deployment case studies

**5. Support community:**
- Answer questions on GitHub Issues
- Share your experience

**Getting started:**
- Fork repository
- Make changes
- Submit pull request
- Discuss in issue tracker

---

### Where can I get help?

**Resources:**

**1. This documentation:**
- Comprehensive guides for most scenarios
- Search function (top right)

**2. GitHub Issues:**
- https://github.com/ICHydro/Riverlabs/issues
- Search existing issues (maybe already answered)
- Create new issue with details:
  - Logger configuration
  - Problem description
  - What you've tried
  - Serial Monitor output
  - Photos

**3. Research group:**
- Contact: ICHydro group (if affiliated)
- Academic collaborations

**4. Community:**
- (Future: Forum or chat once community grows)

**Before asking:**
- Read documentation thoroughly
- Try diagnostic flowcharts
- Check common issues
- Gather diagnostic info (serial logs, photos)

---

## Next Steps

- 🚀 [Quick Start Guide](../quick-start/quick-start.md) - Get started now
- 📊 [Diagnostic Flowcharts](diagnostic-flowcharts.md) - Visual troubleshooting
- 📋 [Common Issues](common-issues.md) - Detailed problem solutions
- 🛠️ [Hardware Maintenance](../hardware/maintenance.md) - Keep logger healthy
- 📡 [Telemetry Setup](../../telemetry/setup-guide.md) - Add remote monitoring

---

!!! question "Still Have Questions?"
    If your question isn't answered here, check the [GitHub Issues](https://github.com/ICHydro/Riverlabs/issues) or create a new issue with your question. The community is here to help!
