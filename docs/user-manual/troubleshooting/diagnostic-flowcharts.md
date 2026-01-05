# Diagnostic Flowcharts

!!! abstract "Overview"
    Visual decision trees for diagnosing common Riverlabs logger problems. Follow the flowchart from symptom to solution.

## How to Use These Flowcharts

1. **Identify your symptom** from the list below
2. **Start at the top** of the relevant flowchart
3. **Answer yes/no questions** and follow arrows
4. **Arrive at a solution** or next diagnostic step
5. **Cross-reference** with detailed guides as needed

---

## Flowchart 1: Logger Won't Record Data

### Symptom
Logger appears to power on but no new data appears on SD card.

```
┌─────────────────────────────────┐
│  Logger Won't Record Data       │
└────────────┬────────────────────┘
             │
             ▼
      Does LED blink?
             │
      ┌──────┴──────┐
     No             Yes
      │              │
      ▼              ▼
Is battery      SD card LED
  >3.3V?        or Power LED?
      │              │
  ┌───┴───┐      ┌───┴───┐
 No      Yes    SD    Power
  │       │      │      │
  ▼       ▼      ▼      ▼
Replace  Check  Go to  Logger is
battery  power  SD     recording
         wiring Flow   (check card)
```

### Full Decision Tree

**START: Logger won't record data**

**Q1: Does any LED blink when logger should be measuring?**

- **No LED activity** → Go to Q2
- **LED blinks** → Go to Q5

**Q2: Is battery voltage >3.3V?**

    - Measure with multimeter at battery terminals
    - **No** (<3.3V) → **SOLUTION: Replace/charge battery**
    - **Yes** (≥3.3V) → Go to Q3

**Q3: Is power switch ON?**

    - Check physical switch position
    - **No** → **SOLUTION: Turn on power switch**
    - **Yes** → Go to Q4

**Q4: Are battery connections secure?**

    - Inspect JST connector
    - Check polarity (red=+, black=-)
    - **Loose/disconnected** → **SOLUTION: Reconnect battery securely**
    - **Secure but no power** → **SOLUTION: Check PCB power circuitry or replace logger**

**Q5: Which LED is blinking?**
- **SD card LED** → Logger is working, go to Q6
- **Power LED only** → Go to Q7

**Q6: Is SD card inserted?**

    - Check physical presence
    - **No** → **SOLUTION: Insert formatted SD card**
    - **Yes** → Go to Q8

**Q7: Does Serial Monitor show error messages?**

    - Connect FTDI cable, open Serial Monitor at 115200 baud
    - **"SD card init failed"** → Go to Q6
    - **"RTC not responding"** → **SOLUTION: RTC failure, check connections or replace**
    - **"Sensor timeout"** → Go to Q9
    - **No serial output** → **SOLUTION: Upload correct firmware**

**Q8: Is SD card formatted correctly?**

    - Remove card, check on computer
    - **Not formatted / wrong format** → **SOLUTION: Format as FAT32, <32GB**
    - **Formatted correctly** → Go to Q10

**Q9: Sensor issue - which sensor type?**

    - **Ultrasonic (Wari)** → **SOLUTION: Check sensor cable connections, verify sensor powered (should click)**
    - **Lidar** → **SOLUTION: Check I2C connections (SDA/SCL), verify sensor address 0x62**

**Q10: Does SD card have recent .CSV file?**

    - Check file dates on card
    - **Recent file present** → Logger is recording! Check filename/location
    - **Old files only / no files** → Go to Q11

**Q11: Is RTC time set correctly?**

    - Check .CSV filename (includes date/time)
    - **Files dated 2000/01/01** → **SOLUTION: Set RTC with set_clock utility**
    - **Files dated correctly** → **SOLUTION: Check measurement interval in code (might be very long)**

---

## Flowchart 2: Sensor Readings Look Wrong

### Symptom
Logger records data but values are incorrect, constant, or erratic.

```
┌─────────────────────────────────┐
│  Sensor Readings Wrong          │
└────────────┬────────────────────┘
             │
             ▼
    What's the symptom?
             │
      ┌──────┼──────┬──────┐
      │      │      │      │
  Always  Changes  Random  Negative
   same    slowly   jumps   values
      │      │      │      │
      ▼      ▼      ▼      ▼
  Sensor  Temp or  Noise/  Install
  frozen  fouling  EMI     height
                           error
```

### Full Decision Tree

**START: Sensor readings look wrong**

**Q1: What is the symptom?**

- **Value never changes** → Go to Q2 (frozen sensor)
- **Value changes but seems wrong** → Go to Q6 (calibration/install)
- **Erratic/random jumps** → Go to Q9 (noise/interference)
- **Negative or impossible values** → Go to Q12 (configuration error)

**Q2: FROZEN SENSOR - Does value change at all over 1 hour?**

    - Take multiple readings
    - **Never changes (e.g., stuck at 5000mm)** → Go to Q3
    - **Changes slightly** → Go to Q6

**Q3: What is the constant value?**

    - **5000mm or max range** → **SOLUTION: Sensor timeout, no target detected**
        - Check: Target in range? Beam aimed correctly? Sensor working?
    - **0 or near-zero** → **SOLUTION: Sensor hardware failure or wiring issue**
    - **Other constant** → Go to Q4

**Q4: Sensor type?**
- **Ultrasonic (Wari)** → Go to Q5
- **Lidar** → **SOLUTION: Lidar I2C communication failure, check connections**

**Q5: ULTRASONIC TROUBLESHOOTING**

    - **Can you hear clicking?** (sensor should click each measurement)
        - **No clicking** → **SOLUTION: Sensor not powered, check cable**
        - **Clicks** → **SOLUTION: Sensor transmitting but no echo (no target, aimed wrong, or target too absorbent)**

**Q6: WRONG BUT CHANGING - How wrong are values?**

    - **Offset by constant amount** (e.g., always 500mm too high) → Go to Q7
    - **Changes over days/weeks** → Go to Q8
    - **Values too high or too low** → Go to Q7

**Q7: Is installation height configured correctly?**

    - Check: Do values make physical sense?
    - **Example:** Sensor 3000mm above water, reads 1500mm → water depth = 3000-1500 = 1500mm
        - Does that match reality?
    - **Height wrong in data interpretation** → **SOLUTION: Correct installation height in analysis**
    - **Height correct but values still wrong** → Go to Q8

**Q8: Physical obstruction or fouling?**

    - Inspect sensor face
    - **Cobwebs, insects, dirt** → **SOLUTION: Clean sensor**
    - **Ice/frost (winter)** → **SOLUTION: Ice on sensor, wait for thaw or relocate**
    - **Vegetation grown into beam path** → **SOLUTION: Trim vegetation**
    - **Clean and clear** → **SOLUTION: Sensor may be failing, replace or check temperature compensation (Wari)**

**Q9: ERRATIC READINGS - Pattern to the noise?**

    - Look at data over time
    - **Random spikes** → Go to Q10
    - **Regular pattern (e.g., every night)** → Go to Q11

**Q10: Random spikes troubleshooting**

    - **Spikes very short (1-2 samples)** → **SOLUTION: EMI/electrical noise**
        - Check: Power supply clean? Nearby motors/pumps?
        - Fix: Add filtering capacitors, isolate power, move logger
    - **Spikes longer (minutes-hours)** → **SOLUTION: Physical interference (bird, debris)**
        - Fix: Review installation, add physical barriers

**Q11: Regular pattern troubleshooting**

    - **Pattern matches temperature cycle** → **SOLUTION: Temperature affecting sensor (normal for ultrasonic)**
        - Note: Wari measures temp for compensation, but not perfect
    - **Pattern matches sunlight** → **SOLUTION: Thermal expansion of mounting or structure**
    - **Pattern matches tides/upstream rain** → This is real! Not an error.

**Q12: NEGATIVE OR IMPOSSIBLE VALUES**

    - **Distance negative** → **SOLUTION: Math error in code or data processing**
        - Check: Installation height configured? Units consistent (mm vs. cm)?
    - **Distance > installation height** → **SOLUTION: Sensor aimed at ground/obstruction instead of water**
        - Fix: Re-aim sensor
    - **Distance > sensor max range** → **SOLUTION: Target out of range, increase height or use Lidar**

---

## Flowchart 3: No Telemetry Data

### Symptom
Logger records to SD card but ThingsBoard receives no data.

```
┌─────────────────────────────────┐
│  No Telemetry Data              │
└────────────┬────────────────────┘
             │
             ▼
    Does SD card work?
             │
      ┌──────┴──────┐
     No             Yes
      │              │
      ▼              ▼
   Fix SD      Is XBee
  first!      installed?
      │              │
      └──────┬───┌───┴───┐
             │  No      Yes
             │   │       │
             ▼   ▼       ▼
         (Done) Add   Is SIM
                XBee  active?
                       │
                  ┌────┴────┐
                 No        Yes
                  │          │
                  ▼          ▼
              Activate    Go to
               SIM      Network
                        Flowchart
```

### Full Decision Tree

**START: No telemetry data received**

**Q1: Does logger record data to SD card successfully?**

    - Check: Recent .CSV file with data?
    - **No** → **SOLUTION: Fix SD card recording first** (use Flowchart 1)
    - **Yes** → Go to Q2

**Q2: Is XBee module physically installed?**

    - Visual inspection of logger PCB
    - **No XBee present** → **SOLUTION: This logger doesn't have telemetry capability**
        - Options: Add XBee module, or manually retrieve SD card data
    - **XBee installed** → Go to Q3

**Q3: Is SIM card inserted in XBee?**

    - Check: SIM slot on underside of XBee
    - **No SIM** → **SOLUTION: Insert activated SIM card** (power off first!)
    - **SIM present** → Go to Q4

**Q4: Is SIM activated with carrier?**

    - Test: Insert SIM in phone, check for service
    - **No service in phone** → **SOLUTION: Contact carrier to activate**
    - **SIM works in phone** → Go to Q5

**Q5: Is battery voltage sufficient?**

    - Check: Voltage >3.5V (telemetry requires more power than SD-only)
    - **<3.5V** → **SOLUTION: Charge/replace battery, retest**
    - **≥3.5V** → Go to Q6

**Q6: Does Serial Monitor show network connection attempts?**

    - Connect FTDI, check debug output
    - **No mention of XBee/network** → Go to Q7
    - **Shows "Connecting to network..."** → Go to Q8

**Q7: XBee communication issue**
- **Serial shows "XBee timeout"** → **SOLUTION: Check XBee physical connection (pins seated)**
- **No XBee messages at all** → **SOLUTION: Check Arduino code has telemetry enabled**

**Q8: What network status appears?**

    - **"Network registered"** → Go to Q10 (connected but transmission failing)
    - **"Searching..."** forever → Go to Q9
    - **"Connection failed"** → Go to Q9

**Q9: Network connection failure**

    - Check signal strength at location (test with phone)
    - **No cell signal** → **SOLUTION: Move logger to location with coverage, or use external antenna**
    - **Phone has signal** → Go to Q11 (XBee configuration issue)

**Q10: Network connected but no data**

    - Serial shows: "HTTP POST: 200 OK" ?
    - **Yes, shows 200 OK** → Go to Q13 (server receiving but not displaying)
    - **No, shows error code** → Go to Q12 (transmission failing)
    - **No transmission attempt** → **SOLUTION: Check TELEMETRY_INTERVAL setting (might be too long)**

**Q11: XBee configuration check**

    - Connect XBee to XBee Studio
    - **APN correct?** → If no: **SOLUTION: Configure correct APN**
    - **API Mode = 2?** → If no: **SOLUTION: Set AP=2** (critical!)
    - **Network Technology (NT) match carrier?** → Try NT=0 vs NT=1
    - **All settings correct** → **SOLUTION: Check carrier account (data plan active? ICCID registered?)**

**Q12: Transmission error codes**

    - **400 Bad Request** → **SOLUTION: JSON format error, check Arduino code**
    - **401 Unauthorized** → **SOLUTION: Wrong access token, verify in ThingsBoard**
    - **404 Not Found** → **SOLUTION: Wrong server URL in Arduino code**
    - **Timeout** → **SOLUTION: Network issue, check signal strength, try reducing payload size**

**Q13: Server receives data but dashboard doesn't show it**

    - **Check ThingsBoard "Latest Telemetry" tab** on device
        - **Data appears there** → **SOLUTION: Dashboard widget misconfigured** (check data keys match)
        - **No data there either** → **SOLUTION: Wrong device token or device inactive**

---

## Flowchart 4: Battery Drains Too Fast

### Symptom
Battery voltage drops faster than expected.

```
┌─────────────────────────────────┐
│  Battery Drains Too Fast        │
└────────────┬────────────────────┘
             │
             ▼
    With or without telemetry?
             │
      ┌──────┴──────┐
   Without        With
  Telemetry    Telemetry
      │            │
      ▼            ▼
  Logging      Transmit
  interval     frequency
    too         too high
   short          │
      │           ▼
      └──────> Optimize
               settings
```

### Full Decision Tree

**START: Battery drains too fast**

**Q1: Does logger have telemetry (XBee installed)?**
- **No telemetry** → Go to Q2 (SD-only configuration)
- **Has telemetry** → Go to Q6 (telemetry configuration)

**Q2: SD-ONLY LOGGER - What is logging interval?**

    - Check code: measurement interval
    - **<5 minutes** → Go to Q3
    - **≥5 minutes** → Go to Q4

**Q3: High-frequency logging**

    - **Do you need measurements this often?**
        - **Yes (e.g., event logging)** → **SOLUTION: Normal high drain. Use larger battery or solar.**
        - **No** → **SOLUTION: Increase logging interval to 15 minutes**
    - **Expected battery life:**
        - 1-min interval: ~7 days
        - 5-min interval: ~15 days
        - 15-min interval: ~30-60 days

**Q4: Logging interval reasonable but battery still drains fast**

    - **Sensor type?**
        - **Lidar** → Go to Q5 (Lidar draws more power)
        - **Ultrasonic** → **SOLUTION: Possible defective component, check for heat (regulator failing?)**

**Q5: Lidar power consumption**

    - **Expected Lidar battery life:** ~15-30 days (15-min logging, 3.7V 2600mAh)
    - **Actual life much shorter (< 1 week)?**
        - **SOLUTION: Check for firmware issues (sensor not sleeping), or hardware short**
    - **Life 10-15 days** → Normal for Lidar

**Q6: TELEMETRY LOGGER - Transmission frequency?**

    - Check code: TELEMETRY_INTERVAL
    - **Every measurement** → **SOLUTION: Way too frequent! Set to every 4-12 measurements**
    - **Every 4-12 measurements** → Go to Q7 (reasonable, but check other factors)
    - **Daily or less** → Go to Q8 (very infrequent, issue elsewhere)

**Q7: Reasonable telemetry interval but fast drain**

    - **Check signal strength:**
        - **RSSI < -100 dBm (weak signal)** → **SOLUTION: Weak signal causes XBee to transmit at higher power**
            - Fix: External antenna, relocate logger, or increase interval
        - **RSSI > -100 dBm (good signal)** → Go to Q8

**Q8: Other power drains**

    - **Is XBee sleeping between transmissions?**
        - Check XBee config: SM=1 (pin sleep)
        - **SM=0 (no sleep)** → **SOLUTION: Set SM=1**
    - **Are there transmission failures with retries?**
        - Check serial log: many failed attempts?
        - **Many failures** → **SOLUTION: Fix network issues, reduce retry attempts**
    - **All settings optimal** → Go to Q9

**Q9: Battery health check**

    - **Is battery old or damaged?**
        - Age > 2 years?
        - Battery hot during charging?
        - Capacity degraded?
        - **SOLUTION: Replace battery** (LiPo degrades over time)
    - **Battery new and healthy** → Go to Q10

**Q10: Hardware fault diagnosis**

    - **Measure current draw with ammeter:**
        - Sleep mode: Should be <1 mA
        - Active (measuring): <50 mA (ultrasonic) or <100 mA (Lidar)
        - Transmitting: ~200 mA
    - **Current much higher than expected?**
        - **SOLUTION: Hardware fault (short, failing component)**
        - Requires: PCB inspection, component testing, or logger replacement

---

## Flowchart 5: SD Card Issues

### Symptom
SD card won't initialize, data corrupted, or filesystem errors.

```
┌─────────────────────────────────┐
│  SD Card Issues                 │
└────────────┬────────────────────┘
             │
             ▼
    Can computer read the card?
             │
      ┌──────┴──────┐
     No             Yes
      │              │
      ▼              ▼
   Card     Is card >32GB
  failed    or wrong format?
             │
      ┌──────┴──────┐
     Yes            No
      │              │
      ▼              ▼
   Format     Is card
  as FAT32    write-
   <32GB     protected?
```

### Full Decision Tree

**START: SD card issues**

**Q1: Is SD card physically inserted?**

    - Visual check
    - **No** → **SOLUTION: Insert SD card into socket until clicks**
    - **Yes** → Go to Q2

**Q2: Can computer read the card?**

    - Remove card, insert into computer (adapter if needed)
    - **Computer can't read card** → Go to Q3 (card failed)
    - **Computer reads card** → Go to Q5 (logger-specific issue)

**Q3: CARD FAILED - Does computer ask to format?**

    - **"Disk not initialized" or similar**
        - **SOLUTION: Format card** (FAT32, allocation unit 32KB)
        - If format fails → Card physically damaged, replace
    - **Computer doesn't detect card at all**
        - **SOLUTION: Card dead, replace**

**Q4: After formatting, does logger work?**

    - **Yes** → Solved! (Card was corrupt)
    - **No** → Go to Q5

**Q5: COMPUTER READS CARD - Card capacity?**

    - Check properties
    - **>32GB** → **SOLUTION: Card too large, use ≤32GB**
    - **≤32GB** → Go to Q6

**Q6: Card filesystem format?**

    - Check: FAT32, exFAT, NTFS?
    - **Not FAT32** → **SOLUTION: Format as FAT32**
    - **Is FAT32** → Go to Q7

**Q7: Does card have existing .CSV files?**

    - **No files** → Go to Q8 (never worked)
    - **Has files** → Go to Q9 (was working, now stopped)

**Q8: NEVER WORKED - Is card write-protected?**

    - Check physical switch on side of SD card
    - **Switch in "Lock" position** → **SOLUTION: Slide switch to unlock**
    - **Not locked** → Go to Q10 (logger SD slot issue)

**Q9: WAS WORKING - When did it stop?**

    - Check date of last file
    - **Stopped after card full** → **SOLUTION: Card full, delete old files or use larger card**
    - **Stopped randomly** → Go to Q11 (corruption)

**Q10: Logger SD slot issue**

    - **Is SD card making contact?**
        - Try: Re-insert card, ensure fully seated
        - Try: Different (known-good) card
        - **Other cards also don't work** → **SOLUTION: SD slot hardware failure, repair or replace logger**
        - **Other cards work** → Original card incompatible, use different brand

**Q11: Data corruption troubleshooting**

    - **Pattern to corruption?**
        - After specific event (power loss, battery died, removed card while logger running)
    - **SOLUTION: Corruption from improper shutdown**
        - Fix: Always power off logger before removing card
        - Prevention: Implement safe shutdown in code (write buffers, close files)
    - **Random corruption:**
        - **SOLUTION: Card quality issue, use SanDisk/Samsung/Kingston (avoid off-brand)**

---

## Using These Flowcharts Effectively

### Tips

1. **Print and laminate** for field use
2. **Follow systematically** - don't skip steps
3. **Document your path** - note which branches you followed
4. **Cross-reference** detailed guides for solutions
5. **Multiple issues?** - Use multiple flowcharts

### When Flowcharts Don't Resolve Issue

If you've followed the appropriate flowchart and still have issues:

1. **Review [Common Issues](common-issues.md)** - similar symptoms with detailed explanations
2. **Check [FAQ](faq.md)** - edge cases and unusual situations
3. **Contact support** with:
    - Flowchart path followed
    - Diagnostic results
    - Serial Monitor logs
    - Photos of hardware

---

## Next Steps

- 📋 [Common Issues](common-issues.md) - Detailed problem descriptions and solutions
- ❓ [FAQ](faq.md) - Frequently asked questions
- 🔧 [Hardware Maintenance](../hardware/maintenance.md) - Preventive maintenance
- 🚨 [Troubleshooting Telemetry](../telemetry/troubleshooting-connections.md) - Detailed telemetry diagnostics

---

!!! tip "Visual Troubleshooting"
    These flowcharts provide a systematic path from symptom to solution. Keep them handy in the field!
