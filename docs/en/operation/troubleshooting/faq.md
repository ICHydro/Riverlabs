# Frequently Asked Questions

!!! abstract "Overview"
    Common questions about Riverlabs loggers answered clearly and concisely. From technical specs to field deployment tips.

## General Questions

### What is a Riverlabs logger?

A **Riverlabs logger** is an open-source, low-cost water level monitoring device designed for hydrological research. It uses ultrasonic or lidar sensors to measure distance to water surface, recording data to SD card and optionally transmitting via cellular telemetry.

**Key features:**
- Arduino-based (ATmega328P)
- Battery-powered (rechargeable LiPo)
- SD card data storage
- Optional cellular telemetry (XBee 3G/4G)
- Weatherproof enclosure
- Low cost (~$100-300 depending on configuration)

---

### Wari vs. Lidar - which should I choose?

**Quick answer:**

| Feature | Wari (Ultrasonic) | Lidar |
|---------|-------------------|-------|
| **Range** | 0.3m - 5m | 0.05m - 40m |
| **Accuracy** | ±10mm | ±25mm |
| **Battery life** | 30-60 days | 15-30 days |
| **Cost** | Lower (~$30 sensor) | Higher (~$150 sensor) |
| **Temperature sensitivity** | Yes (built-in compensation) | No |
| **Works in snow/foam** | No (absorbed) | Yes |
| **Best for** | Rivers, ponds (stable targets) | Wide range, snow, long distances |

**Choose Wari if:**
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

### Can I use Riverlabs loggers in saltwater?

**Yes, with precautions:**

**Sensor:** Both ultrasonic and lidar work fine measuring distance *to* saltwater (non-contact measurement).

**Enclosure:**
- Standard plastic enclosures: OK for temporary (days-weeks)
- For long-term (months): Use marine-grade enclosure
- Risk: Salt spray corrosion of electronics and connectors

**Mounting:**
- Stainless steel hardware (316 marine grade)
- Avoid aluminum (corrodes in saltwater)

**Maintenance:**
- Rinse logger with freshwater after retrieval
- Check for corrosion monthly
- Apply dielectric grease to connectors

**Recommendation:** Riverlabs loggers are primarily designed for freshwater. For permanent saltwater installations, consider commercial marine-grade equipment.

---

### How long does the battery last?

**It depends on configuration:**

**Wari (ultrasonic), SD-only:**
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

**Technically yes, but complicated:**

**Challenges:**
1. **Charge controller needed** - LiPo requires specific charging profile
2. **Weatherproof integration** - Solar panel mounting, wiring penetrations
3. **Northern latitudes** - Winter solar insufficient
4. **Cost** - Adds $50-100, approaching cost of commercial solution

**Recommendation:**
- For most users: Just swap batteries periodically (simpler, cheaper)
- For remote/inaccessible sites: Consider commercial solar-powered loggers
- If DIY: Use dedicated solar charge controller (e.g., Adafruit Universal USB), 5-10W panel

**Calculation:**
- Daily consumption: ~50mAh (Wari, 15-min logging)
- Solar panel needed: ~2-5W (accounting for clouds, winter)
- Battery size: 10,000+ mAh (for winter reserve)

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

### Can I buy pre-assembled loggers?

**Current status:** Riverlabs is primarily a DIY project.

**Options:**

1. **Assemble yourself** (cheapest, ~$100-150)
   - Purchase components
   - Follow assembly guide
   - Solder required

2. **University/research partnerships** (for research projects)
   - Contact: ICHydro/original developers
   - May provide assembled units for collaborative research

3. **Commercial alternatives** (if DIY not feasible)
   - HOBO Water Level Logger (~$500-1000)
   - Solinst Levelogger (~$1000-2000)
   - Campbell Scientific sensors (~$2000+)

**Note:** As the project grows, assembled kits may become available. Check GitHub for updates.

---

### What components do I need?

**Minimum Wari configuration:**

| Component | Qty | Est. Cost |
|-----------|-----|-----------|
| ATmega328P PCB (custom) | 1 | $30 |
| Ultrasonic sensor (MaxBotix MB7389) | 1 | $30 |
| RTC (DS3231) | 1 | $5 |
| SD card module | 1 | $3 |
| LiPo battery (3.7V 2600mAh) | 1 | $10 |
| SD card (8-32GB) | 1 | $10 |
| Enclosure (waterproof) | 1 | $15 |
| FTDI programmer | 1 | $10 |
| Misc (wires, connectors) | - | $10 |
| **Total** | | **~$123** |

**Add for telemetry:**
- XBee3 Cellular module: $70
- SIM card + data plan: $2-10/month
- USB adapter (for configuration): $25

See [parts list] for detailed component specifications and suppliers.

---

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

### Can I use alkaline batteries instead of LiPo?

**Not recommended, but possible with modifications:**

**Challenges:**
1. **Voltage:** Alkaline = 1.5V per cell, need 2-3 cells = 3-4.5V (works, but voltage drops faster)
2. **No recharging:** Disposable only
3. **Connector:** Need battery holder (instead of JST for LiPo)
4. **Code changes:** Battery voltage reading calibration

**Advantages:**
- Widely available
- No fire risk (safer than LiPo)
- Longer shelf life

**Recommendation:** Use 3× AA lithium (not alkaline) if you must avoid LiPo:
- Energizer Ultimate Lithium
- 1.5V × 3 = 4.5V nominal
- Better cold performance than alkaline
- Longer life than alkaline

**Code change needed:**
```cpp
// Voltage divider calibration for 3× AA (4.5V max instead of 4.2V)
float batteryVoltage = analogRead(BATTERY_PIN) * (4.5 / 1023.0) * 2.0;
```

---

## Telemetry Questions

### Do I need telemetry?

**Depends on your use case:**

**You NEED telemetry if:**
- Site is difficult/dangerous to access
- Real-time monitoring required (flood warning)
- Multiple loggers, checking each is impractical
- Want to detect logger failure quickly

**You DON'T need telemetry if:**
- Accessible site (easy to visit monthly)
- Post-event analysis (not real-time)
- Budget-constrained ($100 vs $200)
- No cellular coverage at site

**Compromise option:** SD card + occasional telemetry
- Logger records to SD (high frequency, reliable)
- Transmit subset of data (hourly summary)
- Full resolution data on SD card

---

### What cellular carriers work?

**Requirements:** LTE-M or NB-IoT support

**United States:**
- ✅ AT&T (best LTE-M coverage)
- ✅ T-Mobile (LTE-M, expanding)
- ✅ Verizon (Cat-M1)
- ❌ Sprint (merged with T-Mobile)

**Europe:**
- ✅ Vodafone M2M
- ✅ Telefonica
- ✅ Deutsche Telekom
- ✅ Orange

**United Kingdom:**
- ✅ EE
- ✅ Vodafone
- ✅ O2

**Global/Multi-carrier:**
- ✅ Hologram (roaming across multiple carriers)
- ✅ Twilio Super SIM
- ✅ 1NCE

**Check coverage:** Use carrier's coverage map for LTE-M/NB-IoT (not regular LTE) at your deployment site.

---

### How much data does telemetry use?

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

### Can I use WiFi instead of cellular?

**Possible, but impractical for most deployments:**

**Challenges:**
1. **Range:** WiFi limited to ~50m (line of sight)
   - Deployment sites rarely have WiFi
2. **Power:** WiFi modules (ESP8266, ESP32) draw ~70mA continuously
   - Reduces battery life significantly
3. **Configuration:** Need SSID/password setup
   - Changes if network changes

**Better alternatives:**
- **LoRa/LoRaWAN:** Long range (km), low power, but requires gateway
- **Cellular:** Works anywhere with coverage
- **Satellite:** Very expensive, but global

**Recommendation:** If WiFi available at site, consider:
- Mains power (plug in), then WiFi is feasible
- Or: SD card only, retrieve manually (simpler)

---

### Can the logger send me email/SMS alerts?

**Not directly from logger, but via server:**

**Architecture:**
```
Logger → XBee → Cellular → ThingsBoard → Email/SMS
```

**Setup:**
1. Logger sends data to ThingsBoard (as normal)
2. Configure ThingsBoard "Rule Chain"
3. Add rule: If (condition), then (send email)
4. Example: If (battery < 3.4V), send email alert

**Email:** ThingsBoard can send directly (configure SMTP)

**SMS:** Requires third-party service:
- Twilio (pay per SMS)
- AWS SNS
- Other SMS gateway

See [ThingsBoard Configuration](../../telemetry/thingsboard-configuration.md) for alert setup instructions.

**Limitation:** Logger can't proactively alert if it loses power or connection (dead logger can't send "I'm dead" message). ThingsBoard can alert on "no data received in X hours."

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

### What if water level exceeds sensor range?

**Two scenarios:**

**1. Water too close (closer than minimum range):**
- Ultrasonic minimum: ~0.3m
- Lidar minimum: ~0.05m
- **Solution:** Mount sensor higher

**2. Water too far (beyond maximum range):**
- Ultrasonic maximum: ~5m
- Lidar maximum: ~40m
- **Symptoms:** Logger reads max range (5000mm or 40000mm)
- **Solution:** Mount sensor lower, or use longer-range sensor

**Designing for range:**
- Determine water level range: Low flow and high flow
- Choose sensor range: Must cover low to high + margin
- Example:
  - Low flow: 2m below sensor
  - High flow: 0.5m below sensor
  - Range needed: 0.5-2m → Use ultrasonic (0.3-5m range)

**Safety margin:** ±0.5m buffer on each end (don't use absolute limits)

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

**2. LiPo freezing risk:**
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
