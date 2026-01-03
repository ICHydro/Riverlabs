# Common Issues and Solutions

!!! abstract "Overview"
    Comprehensive reference of frequently encountered problems with Riverlabs loggers. Symptom → Cause → Solution format for quick troubleshooting.

## How to Use This Guide

1. **Scan symptoms** to find your issue
2. **Read likely causes** to understand why
3. **Follow solution steps** to fix
4. **Check "Verified by"** for real-world confirmation

Each issue includes:
- 🔴 **Symptom** - What you observe
- 🔍 **Likely Cause** - Why it happens
- ✅ **Solution** - How to fix
- ⚙️ **Prevention** - How to avoid in future

---

## Category: Power and Battery

### Issue 1: Logger Won't Power On

**🔴 Symptom:**
- No LED activity when power switch ON
- Logger appears completely dead
- No serial output

**🔍 Likely Causes:**
1. **Battery completely discharged** (<2.5V)
2. **Battery disconnected** or poor contact
3. **Reverse polarity** (battery connected backward)
4. **Power switch failure** (mechanical)
5. **PCB damage** (component failure, trace damage)

**✅ Solution:**

**Step 1: Check battery voltage**
```
- Disconnect battery from logger
- Measure with multimeter: Red probe to red wire, black to black
- Reading:
  - 3.7-4.2V: Battery good → issue is in logger (go to Step 3)
  - 2.5-3.5V: Battery discharged → charge/replace, retest
  - <2.5V: Battery dead → replace (likely won't accept charge)
  - 0V: No battery or complete failure → replace
```

**Step 2: Inspect battery connector**
```
- JST connector properly inserted?
- Pins making contact? (wiggle gently, does it power on intermittently?)
- Correct polarity? (Red wire to + pad on PCB, black to -)
- If backward: IMMEDIATELY disconnect! Check for damage (burned components, smell)
```

**Step 3: Test power switch**
```
- Toggle switch multiple times (may be dirty contacts)
- If available: Bypass switch (temporary wire across switch terminals) for testing
- Switch broken? Desolder and replace, or permanently bypass
```

**Step 4: Check for PCB damage**
```
- Visual inspection: burned components? broken traces?
- Smell: burnt electronics smell?
- Voltage regulator hot when battery connected? (regulator failure)
- If PCB damaged: Professional repair or logger replacement
```

**⚙️ Prevention:**
- Monitor battery voltage, replace before <3.3V
- Use JST connectors (keyed, prevents reverse polarity)
- Protect logger from physical damage (good enclosure)
- Don't short circuit battery (causes permanent damage)

---

### Issue 2: Battery Drains Overnight

**🔴 Symptom:**
- Battery fully charged, installed in evening
- Next morning: Battery dead or very low
- Logger may have logged a few samples or none

**🔍 Likely Causes:**
1. **Sleep mode not functioning** (logger running continuously)
2. **Telemetry stuck trying to connect** (XBee never sleeps)
3. **Firmware bug** (sleep never triggered)
4. **Short circuit** (hardware fault)

**✅ Solution:**

**Step 1: Verify sleep mode in code**
```cpp
// In main loop, should have:
LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
// Or similar sleep command

// For XBee:
pinMode(XBEE_SLEEP_PIN, OUTPUT);
digitalWrite(XBEE_SLEEP_PIN, HIGH); // Sleep XBee when not transmitting
```

**Step 2: Measure current draw**
```
- Equipment needed: Multimeter with current measurement (mA mode)
- Disconnect battery, connect ammeter in series
- Power on logger
- Observe current:
  - Sleep: <1 mA (good)
  - Active/measuring: 50-100 mA (normal, brief)
  - Continuous >50 mA: Not sleeping! (problem)
```

**Step 3: Check serial debug output**
```
- Connect FTDI cable
- Open Serial Monitor
- Watch for:
  - "Entering sleep" messages? (if not, sleep not triggered)
  - "XBee timeout" or similar? (stuck waiting for XBee)
  - Continuous output? (should be periodic, not constant)
```

**Step 4: Isolate telemetry**
```
If logger has XBee:
- Remove XBee module
- Test overnight with SD-only logging
- If battery life good now: XBee configuration issue (SM not set to pin sleep)
- If battery still drains: Issue is in logger code/hardware
```

**⚙️ Prevention:**
- Test battery life in bench setup (overnight) before field deployment
- Enable debug output to verify sleep
- Use XBee pin sleep mode (SM=1)
- Measure current draw with ammeter during development

---

### Issue 3: Battery Percentage Seems Wrong

**🔴 Symptom:**
- Logger reports 50% battery
- Next reading: 10% or dead
- Or: Reports 80% for days, then suddenly dies

**🔍 Likely Causes:**
1. **LiPo voltage curve** (non-linear discharge)
2. **Cold temperatures** (voltage sag)
3. **Old battery** (capacity faded)
4. **Voltage measurement error** (ADC inaccuracy)

**✅ Solution:**

**Understanding LiPo discharge:**
```
LiPo batteries hold voltage until near end of life:

Voltage   Approximate Remaining %
4.2V      100%
4.0V      80%
3.9V      60%
3.8V      40%
3.7V      30%
3.6V      20%
3.5V      10%
3.4V      5%
3.3V      1% (emergency, retrieve logger!)
<3.3V     Damage risk
```

**Note:** Percentage is non-linear and temperature/load dependent.

**Step 1: Use voltage thresholds, not percentage**
```
Instead of:  "Battery at 50%"
Use:         "Battery voltage: 3.7V" (more accurate)

Alert thresholds:
- 3.5V: Plan retrieval in next week
- 3.4V: Retrieve within 2 days
- 3.3V: Retrieve immediately (may not transmit)
```

**Step 2: Temperature compensation**
```
Cold weather causes temporary voltage drop:
- Measured voltage may read 3.5V in 0°C
- Actual capacity may be higher (warms up → voltage recovers)
- Don't panic on single low reading in cold
- Retrieve if multiple consecutive readings <3.4V
```

**Step 3: Battery aging**
```
LiPo batteries degrade over time:
- New: Full 2600 mAh capacity
- 1 year: ~90% capacity (2340 mAh)
- 2 years: ~80% capacity (2080 mAh)
- 3+ years: <70% capacity, replace

Solution: Replace batteries annually for critical deployments
```

**⚙️ Prevention:**
- Set alerts based on voltage, not percentage
- Replace batteries yearly
- Retrieve loggers before voltage critical
- Store spare batteries properly (40-60% charge, cool location)

---

## Category: Sensor and Data Quality

### Issue 4: Ultrasonic Sensor Reads Max Range Constantly

**🔴 Symptom:**
- Wari logger always reads 5000mm (or max range)
- Value never changes, even when moving sensor

**🔍 Likely Causes:**
1. **No echo returned** (no target, or target too absorbent)
2. **Sensor not powered** (cable disconnected)
3. **Sensor aimed wrong** (pointing at sky or ground)
4. **Sensor failed** (rare)

**✅ Solution:**

**Step 1: Check for sensor clicking**
```
- Power on logger
- Hold sensor near ear (carefully, don't touch face)
- Should hear: "tick" or "click" each measurement (every 15 sec or your interval)
- Clicking: Sensor powered and transmitting → go to Step 2
- Not clicking: Sensor not powered → go to Step 3
```

**Step 2: Sensor powered but no echo**
```
Causes of no echo:
1. No target in beam path
   - Too far: Move closer or use Lidar
   - Aimed at sky: Re-aim toward target

2. Target too absorbent
   - Snow (fresh powder): Very absorbent
   - Foam: Absorbs ultrasound
   - Solution: Wait for snow to melt, or use Lidar

3. Beam width too narrow, missing target
   - Ultrasound beam: ~15-20° cone
   - Small target (e.g., thin stream): May miss
   - Solution: Aim more carefully, or use wider beam sensor
```

**Step 3: Sensor not powered**
```
- Disconnect sensor cable from logger
- Check continuity:
  - Cable pins to connector: Should have <1Ω resistance
  - Cable damaged? (crushed, cut, corroded): Replace cable
- Check PCB connector:
  - Solder joints intact? (visual inspection)
  - Pins making contact? (try re-seating cable)
- Test with multimeter:
  - Measure voltage at sensor connector when logger active
  - Should see 5V or 12V (depending on sensor model)
  - No voltage: Power circuit failure on PCB
```

**Step 4: Sensor replacement**
```
If all else fails:
- Sensor may be internally failed (rare but possible)
- Test: Swap sensor with known-good unit
- If new sensor works: Original sensor dead
- If new sensor also reads max range: Issue is aiming/target/environment, not sensor
```

**⚙️ Prevention:**
- Careful aiming during installation (test before leaving site)
- Protect cable from damage (secure routing, avoid pinch points)
- Choose appropriate sensor for target material (Lidar for snow/foam)
- Keep sensor face clean (dust/debris can block ultrasound)

---

### Issue 5: Distance Readings Drift Over Time

**🔴 Symptom:**
- Distance values change by 50-200mm over days
- But actual water level is stable (verified visually)
- Slow drift, not sudden jumps

**🔍 Likely Causes:**
1. **Temperature changes** (sensor mounting expands/contracts)
2. **Vegetation growth** (beam obstruction)
3. **Mounting loosening** (screws, zip ties)
4. **Sensor calibration drift** (rare)

**✅ Solution:**

**Step 1: Check temperature correlation**
```
- Plot distance vs. temperature over time
- If strong correlation (distance increases when temp increases):
  - Cause: Mounting structure expanding with heat
  - Example: Metal mounting heats in sun, expands, sensor moves 5mm down
  - Solution: 
    a) Accept as natural variation (±50mm usually acceptable)
    b) Use thermally stable mounting (minimize metal)
    c) Temperature-compensate in post-processing
```

**Step 2: Inspect mounting**
```
- Visual inspection of logger installation
- Check:
  - Zip ties tight? (may loosen over time)
  - Screws/bolts tight?
  - Mounting bracket bent or moved?
- Solution: Re-tighten, reinforce mounting
```

**Step 3: Check for vegetation**
```
- Inspect beam path
- Growing vegetation? (grass, weeds, branches)
  - New leaves in spring?
  - Algae growing on target surface?
- Solution: Trim vegetation, clean target
- Prevention: Install with clear beam path, anticipate seasonal growth
```

**Step 4: Sensor calibration check**
```
Rarely needed, but to verify:
- Measure known distance with tape measure (e.g., 2000mm)
- Place target at that distance
- Logger reading should match ±50mm
- If off by >100mm: Sensor may need replacement
```

**⚙️ Prevention:**
- Use thermally stable mounting materials
- Maintain clear beam path (quarterly trim vegetation)
- Secure mounting with redundant fasteners
- Account for seasonal expansion/contraction in data analysis

---

### Issue 6: Lidar Returns Negative Values

**🔴 Symptom:**
- Lidar logger occasionally records negative distances (e.g., -150mm)
- Most readings are positive and correct

**🔍 Likely Causes:**
1. **I2C communication error** (data corruption)
2. **Timeout interpreted as negative** (code bug)
3. **Integer overflow** (math error in code)

**✅ Solution:**

**Step 1: Check for pattern**
```
- When do negatives occur?
  - Randomly: I2C error (go to Step 2)
  - All readings after certain value: Overflow (go to Step 3)
  - During/after transmission: XBee interfering with I2C (go to Step 4)
```

**Step 2: I2C communication reliability**
```
I2C is sensitive to:
- Wire length (keep <30cm)
- Interference (EMI from nearby sources)
- Poor connections (loose wires)

Solutions:
a) Add pull-up resistors (2.2kΩ or 4.7kΩ on SDA and SCL)
b) Shorten I2C wires
c) Add ferrite bead on cable
d) Shield cable if near strong RF source
```

**Step 3: Code bug - overflow**
```cpp
// Bad code (potential overflow):
int distance = lidar.read(); // int = -32768 to 32767
// If actual distance > 32767mm, overflows to negative

// Good code:
unsigned int distance = lidar.read(); // unsigned = 0 to 65535
// Or:
long distance = lidar.read(); // long = much larger range
```

**Step 4: XBee interference**
```
If negatives occur during/after telemetry:
- XBee transmission may cause EMI
- Solution:
  a) Separate I2C wires from XBee antenna
  b) Measure before transmission (not during)
  c) Add I2C pull-ups (improves signal integrity)
```

**Step 5: Filter in post-processing**
```
As a last resort (if hardware fixes impractical):
- Remove negative values during data analysis
- Interpolate: Replace negative with average of neighboring values
- Example Python:
  df['distance'] = df['distance'].clip(lower=0)
```

**⚙️ Prevention:**
- Use proper data types (unsigned int for distances)
- Add I2C pull-up resistors in hardware design
- Test I2C reliability before deployment (read sensor 1000× in loop, check for errors)

---

## Category: SD Card and Data Logging

### Issue 7: SD Card Shows "Corrupt" or "Needs Formatting"

**🔴 Symptom:**
- Remove SD card from logger
- Insert in computer
- Windows says: "Disk needs formatting" or "Corrupt"
- Files unreadable

**🔍 Likely Causes:**
1. **Improper shutdown** (removed card or power while writing)
2. **Battery died during write** (file not closed properly)
3. **SD card quality** (cheap cards fail easily)
4. **Filesystem corruption** (bug in logger code)

**✅ Solution:**

**Step 1: Attempt data recovery**
```
Before formatting, try recovery:

Windows:
- Right-click drive → Properties → Tools → Check
- Let Windows scan and repair

Mac:
- Disk Utility → First Aid → Run

Linux:
- fsck command

Recovery software (if above fails):
- Recuva (free, Windows)
- PhotoRec (free, multi-platform)
- DiskDigger (free/paid)
```

**Step 2: Format properly**
```
If recovery fails or you don't need data:

Windows:
- Right-click drive → Format
- File system: FAT32
- Allocation unit size: 32 kilobytes
- Quick Format: Unchecked (full format tests for bad sectors)

Mac:
- Disk Utility → Erase
- Format: MS-DOS (FAT)
- Scheme: Master Boot Record

Linux:
sudo mkfs.vfat -F 32 -S 512 -s 64 /dev/sdX1
```

**Step 3: Test card**
```
After formatting:
- Copy test file to card
- Verify file readable
- Remove and re-insert
- File still there? → Card OK
- File corrupted again? → Card failing, replace
```

**Step 4: Prevent future corruption**
```
Code improvements:
1. Flush data after each write:
   dataFile.flush(); // Forces write to disk

2. Close file before sleep:
   dataFile.close();
   // Sleep
   dataFile = SD.open("data.csv", FILE_WRITE);

3. Add battery-low check:
   if(battery < 3.3) {
     dataFile.close();
     enterEmergencyShutdown();
   }
```

**⚙️ Prevention:**
- Never remove SD card while logger powered on
- Ensure battery adequate before deployment
- Use quality SD cards (SanDisk, Samsung, Kingston)
- Implement flush() or close() in code regularly
- Retrieve loggers before battery critical

---

### Issue 8: CSV File Has Weird Characters or Formatting

**🔴 Symptom:**
- Open CSV in Excel/Numbers
- Strange symbols: `Ã`, `Â`, `â€™`
- Columns misaligned
- Data looks corrupted

**🔍 Likely Causes:**
1. **Character encoding mismatch** (UTF-8 vs. ASCII)
2. **Delimiter confusion** (comma vs. semicolon)
3. **Quote marks in data** (breaking CSV parsing)
4. **Incomplete lines** (power loss during write)

**✅ Solution:**

**Step 1: Try different encoding**
```
Excel (Windows):
- Open Excel (don't double-click CSV)
- Data → From Text/CSV
- File Origin: UTF-8
- Delimiter: Comma
- Import

Excel (Mac):
- Similar: Data → Get External Data → Import Text File
- Select encoding: UTF-8 or Western (ASCII)

LibreOffice/OpenOffice:
- Opens CSV with import dialog automatically
- Character set: Unicode (UTF-8)
- Separated by: Comma
```

**Step 2: Inspect raw file**
```
Use text editor (Notepad++, Sublime, VS Code):
- Open CSV as plain text
- Look for:
  - Proper line endings (should be LF or CRLF)
  - Consistent delimiters (all commas? all semicolons?)
  - Any binary characters (� symbols)

Example good CSV:
Date,Time,Distance_mm,Temp_C,Battery_V
2025-12-27,10:00:00,1234,22.5,3.85
2025-12-27,10:15:00,1256,23.1,3.84

Example bad CSV (missing delimiters):
Date,Time,Distance_mm,Temp_C,Battery_V
2025-12-2710:00:001234,22.5,3.85
```

**Step 3: Fix code if needed**
```cpp
// Ensure consistent delimiters:
dataFile.print(year);
dataFile.print(",");  // Not "," sometimes and ";" other times
dataFile.print(month);
dataFile.print(",");
// etc.

// Handle special characters:
// If data might contain commas, use quotes:
dataFile.print("\"");
dataFile.print(dataString);
dataFile.print("\"");
```

**Step 4: Salvage partial data**
```
If file partially corrupted:
- Identify good rows vs. bad
- Manually fix or delete bad rows
- Or: Script to clean (Python pandas, etc.)
```

**⚙️ Prevention:**
- Write header row on first boot (clear template)
- Use consistent delimiters (comma standard)
- Test CSV parsing with sample data before deployment
- Flush file buffer regularly to ensure complete writes

---

## Category: Telemetry

### Issue 9: ThingsBoard Shows Old Data Only

**🔴 Symptom:**
- ThingsBoard dashboard has data
- But latest data is hours or days old
- Logger is still running (SD card has current data)

**🔍 Likely Causes:**
1. **Network lost** (logger moved out of coverage)
2. **SIM deactivated** (payment lapse, carrier issue)
3. **Battery too low** (telemetry disabled at low voltage)
4. **XBee failure** (hardware or configuration)

**✅ Solution:**

**Step 1: Check recent SD card data**
```
- Retrieve logger (or analyze last data on ThingsBoard)
- SD card shows recent measurements?
  - Yes: Logger working, telemetry failed → Go to Step 2
  - No: Logger stopped completely → See Flowchart 1 (Logger Won't Record)
```

**Step 2: Verify battery voltage**
```
- Check voltage in latest SD card data
- Voltage <3.5V?
  - Yes: Battery too low for reliable telemetry
  - Solution: Replace battery, retest
- Voltage >3.5V?
  - Telemetry failure for other reason → Go to Step 3
```

**Step 3: Test SIM card**
```
- Remove SIM from XBee
- Insert in phone (adapter if needed)
- Does phone get service?
  - No service: SIM deactivated
    - Solution: Contact carrier, reactivate, verify payment
  - Has service: SIM works → Go to Step 4
```

**Step 4: Check signal at site**
```
- Using phone (same carrier as SIM):
  - Test signal strength at logger location
  - Coverage adequate?
    - No coverage: Site moved out of coverage (tree fell, logger relocated?)
      - Solution: Move logger, or use external antenna
    - Coverage OK: XBee issue → Go to Step 5
```

**Step 5: XBee configuration check**
```
- Remove XBee, connect to XBee Studio
- Read configuration
- Critical settings:
  - AP = 2? (if not, set to 2)
  - AN = correct APN? (verify with carrier)
  - SM = 1? (pin sleep)
- Test connection in XBee Studio (Cellular Network tab)
  - Can connect? XBee works
    - Solution: Re-install in logger, check Arduino code
  - Can't connect? Network issue or XBee failure
    - Solution: Update firmware, or replace XBee
```

**⚙️ Prevention:**
- Set up ThingsBoard alerts (no data for >2× expected interval)
- Monitor battery voltage remotely
- Keep SIM plan auto-renewing
- Test signal before deployment (don't assume coverage)

---

### Issue 10: Telemetry Works Intermittently

**🔴 Symptom:**
- Some transmissions succeed (200 OK)
- Some fail (timeout, error code)
- Success rate: 50-80%

**🔍 Likely Causes:**
1. **Weak signal** (marginal coverage)
2. **Network congestion** (peak times)
3. **Antenna issue** (loose connection, damaged)
4. **Power issue** (voltage sag during transmission)

**✅ Solution:**

**Step 1: Correlation analysis**
```
Look for patterns in failures:

By time of day?
- Failures at night: Network congestion (everyone home, using network)
- Failures during day: Possible but less common
- Random: Signal strength issue

By battery voltage?
- Failures increase as voltage drops: Power issue
- No correlation: Network issue

By temperature?
- Failures in cold: Battery voltage sag in cold
- No correlation: Other issue
```

**Step 2: Signal strength check**
```
If possible (XBee Studio or serial debug):
- Check RSSI during successful vs. failed transmissions
- RSSI < -100 dBm: Weak signal, high failure rate
  - Solution: External antenna, relocate logger higher/clearer location
- RSSI > -95 dBm but still failures: Not signal strength, go to Step 3
```

**Step 3: Antenna check**
```
- Inspect antenna connection (if external)
- U.FL connector seated properly? (tiny, fragile)
- Antenna wire damaged? (check for kinks, breaks)
- Internal PCB antenna: Clearance OK? (no contact with enclosure/metal)
- Solution: Re-seat connector, replace antenna if damaged
```

**Step 4: Add retry logic**
```cpp
// In Arduino code:
bool sendData() {
  int attempts = 0;
  while(attempts < 3) {
    if(transmitXBee()) {
      return true; // Success
    }
    delay(30000); // Wait 30 seconds
    attempts++;
  }
  return false; // Failed after 3 attempts
}
```

**Step 5: Adjust transmission timing**
```
If failures correlate with peak hours:
- Shift transmission time:
  - Instead of every hour on the hour (09:00, 10:00, 11:00)
  - Offset: Random minute (09:17, 10:42, 11:23)
  - Reduces network congestion (not everyone transmitting simultaneously)
```

**⚙️ Prevention:**
- Deploy in location with good signal (test with phone first)
- Use external antenna for marginal locations
- Implement retry logic with exponential backoff
- Monitor success rate via ThingsBoard (should be >90%)

---

## Category: Arduino and Firmware

### Issue 11: Upload Fails "programmer is not responding" or "avrdude: stk500_getsync()"

**🔴 Symptom:**
- Trying to upload sketch to logger
- Arduino IDE error: "programmer is not responding" or similar avrdude error
- Upload fails at 0% or during communication phase

**🔍 Likely Causes:**
1. **Sensor connected during upload** (CRITICAL - most common for Wari Ultrasonic)
2. **FTDI cable wrong orientation** (green/black reversed)
3. **Wrong board settings** (not Arduino Pro Mini)
4. **Wrong processor settings** (5V/16MHz instead of 3.3V/8MHz)
5. **FTDI cable not seated** properly
6. **Power issue** (insufficient power or no power)

**✅ Solution:**

**Step 1: Disconnect sensor (CRITICAL for Wari Ultrasonic)**
```
⚠️ THE MOST COMMON CAUSE ⚠️

For Wari Ultrasonic loggers with Maxbotix sensor:
- Unplug the WHITE CONNECTOR from the logger
- The sensor and FTDI use the same serial port
- Sensor causes interference preventing upload
- After upload completes, reconnect the sensor

This does NOT apply to Lidar models.
```

**Step 2: Verify FTDI orientation**
```
Check board markings:
- Green wire → "GRN" marking
- Black wire → "BLK" marking
- On cellular/lidar: Black wire on SD card side

If reversed:
- Disconnect immediately
- Rotate 180°
- Reconnect firmly
```

**Step 3: Verify Arduino IDE settings**
```
Tools → Board: "Arduino Pro or Pro Mini"
Tools → Processor: "ATmega328P (3.3V, 8MHz)"

NOT: "ATmega328P (5V, 16MHz)"  ← Wrong!
```

**Step 4: Check power**
```
- Power switch to OFF (powered by FTDI)
- Or battery installed and switch ON
- LED should briefly light when reset button pressed
```

**Step 5: Reseat FTDI cable**
```
- Disconnect FTDI from logger
- Check all pins aligned
- Press firmly onto header
- Should sit flush
```

**Step 6: Try different USB port**
```
- Some USB ports provide insufficient current
- Try USB 2.0 port (more reliable than USB 3.0)
- Avoid USB hubs
```

**⚙️ Prevention:**
- **Always disconnect sensor before programming** (Wari Ultrasonic)
- Label FTDI cable with orientation arrow
- Double-check board settings before every upload
- Keep FTDI connection list handy

---

### Issue 12: Upload Fails "Serial Port Already in Use"

**🔴 Symptom:**
- Trying to upload sketch to logger
- Arduino IDE error: "Serial port busy" or "Access denied"

**🔍 Likely Causes:**
1. **Serial Monitor open** (locks port)
2. **Another program using port** (PuTTY, Coolterm, etc.)
3. **Previous upload not finished** (port hung)
4. **Driver issue** (Windows)

**✅ Solution:**

**Step 1: Close Serial Monitor**
```
- In Arduino IDE: Close Serial Monitor window
- Wait 2 seconds
- Retry upload
```

**Step 2: Check for other programs**
```
Windows:
- Ctrl+Shift+Esc (Task Manager)
- Look for: PuTTY, Coolterm, Serial Monitor, Python scripts
- End task

Mac:
- Activity Monitor
- Search for serial terminal programs
- Quit

Linux:
- ps aux | grep tty
- kill [PID]
```

**Step 3: Disconnect and reconnect**
```
- Unplug FTDI cable
- Wait 5 seconds
- Reconnect
- Retry upload
```

**Step 4: Restart Arduino IDE**
```
- Close Arduino IDE completely
- Reopen
- Retry upload
```

**Step 5: Reboot computer** (if all else fails)

**⚙️ Prevention:**
- Always close Serial Monitor before uploading
- Don't run multiple serial programs simultaneously
- Unplug FTDI after use (prevents accidental port locking)

---

### Issue 13: Sketch Compiles but Logger Does Nothing

**🔴 Symptom:**
- Upload successful (100%, "Done uploading")
- Logger powers on (LED blinks once)
- Then: Nothing. No logging, no LED, no serial output

**🔍 Likely Causes:**
1. **Wrong board selected** (compiled for wrong MCU)
2. **Bootloader corruption** (rare)
3. **Clock configuration wrong** (code runs but timing off)
4. **Infinite loop in setup()** (code hangs)

**✅ Solution:**

**Step 1: Verify board selection**
```
Arduino IDE:
- Tools → Board → Should be: "Arduino Pro or Pro Mini"
- Tools → Processor → Should be: "ATmega328P (3.3V, 8MHz)"
- If wrong: Select correct, re-upload
```

**Step 2: Test basic blink**
```cpp
// Upload simplest possible code to verify functionality:
void setup() {
  pinMode(13, OUTPUT); // Or whatever pin has LED
}

void loop() {
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}

// Should blink every second
// If doesn't blink: Hardware or bootloader issue
// If blinks: Logger is OK, issue is in main code
```

**Step 3: Enable debug serial output**
```cpp
void setup() {
  Serial.begin(115200);
  Serial.println("Logger starting...");
  Serial.println("Initializing RTC...");
  // etc.
}

// Connect Serial Monitor, check for output
// No output: Serial initialization issue or bootloader problem
// Has output: Follow debug messages to find where code fails
```

**Step 4: Check for infinite loops**
```cpp
// Common mistake:
void setup() {
  while(!rtc.begin()) {
    // Stuck here forever if RTC not connected!
    delay(1000);
  }
}

// Better:
void setup() {
  int attempts = 0;
  while(!rtc.begin() && attempts < 10) {
    delay(1000);
    attempts++;
  }
  if(attempts == 10) {
    // RTC failed, but continue with default time
    Serial.println("RTC failed");
  }
}
```

**Step 5: Burn bootloader** (if hardware OK but nothing works)
```
Requires:
- Another Arduino as ISP programmer
- Or: AVR ISP programmer

Process:
- Arduino IDE → File → Examples → ArduinoISP
- Upload ArduinoISP to programmer Arduino
- Connect programmer to target logger (ISP header)
- Tools → Programmer → Arduino as ISP
- Tools → Burn Bootloader
- Wait for completion
- Re-upload your sketch
```

**⚙️ Prevention:**
- Always verify board selection before upload
- Test with simple blink sketch first (known-good code)
- Add debug serial output during development
- Avoid infinite loops (use timeouts)

---

### Issue 14: Compilation Errors - Missing Libraries

**🔴 Symptom:**
- Arduino IDE shows red error text
- Error message: `fatal error: [LibraryName].h: No such file or directory`
- Examples:
  - `SdFat.h: No such file or directory`
  - `LowPower.h: No such file or directory`
  - `RtcDS3231.h: No such file or directory`
  - `AltSoftSerial.h: No such file or directory`

**🔍 Likely Causes:**
1. **Required library not installed**
2. **Library installed but Arduino can't find it**
3. **Wrong library version**
4. **Library folder name incorrect**

**✅ Solution:**

**Step 1: Install via Library Manager**
```
Arduino IDE:
1. Sketch → Include Library → Manage Libraries
2. Search for library name (e.g., "SdFat")
3. Find correct library:
   - "RTC" by Makuna
   - "SdFat" by Bill Greiman  
   - "AltSoftSerial" by Paul Stoffregen
4. Click Install
5. Wait for installation
6. Restart Arduino IDE
7. Retry compilation
```

**Step 2: Manual Installation (for Rocketscream LowPower)**
```
The LowPower library requires manual installation:

1. Go to https://github.com/rocketscream/Low-Power
2. Click Code → Download ZIP
3. Extract ZIP file
4. Rename folder to "Low-Power" (if needed)
5. Move folder to Arduino libraries directory:
   - Windows: Documents\Arduino\libraries\
   - Mac: ~/Documents/Arduino/libraries/
   - Linux: ~/Arduino/libraries/
6. Restart Arduino IDE
7. Verify: Sketch → Include Library → Low-Power should appear
```

**Step 3: Check library folder structure**
```
Correct structure:
Arduino/libraries/Low-Power/
  ├── LowPower.h
  ├── LowPower.cpp
  └── examples/

Wrong structure (won't work):
Arduino/libraries/Low-Power-master/Low-Power/
  └── ...

Fix: Move inner folder up one level
```

**Step 4: Verify library compatibility**
```
Error: "This LowPower library only works on AVR processors"

Solution:
- Board must be: Arduino Pro or Pro Mini
- Processor must be: ATmega328P (3.3V, 8MHz)
- NOT: Arduino Uno, Mega, or other non-AVR boards
```

**Required Libraries for Riverlabs Loggers:**

| Library | Author | Installation Method | Used For |
|---------|--------|---------------------|----------|
| RTC | Makuna | Library Manager | Real-time clock |
| SdFat | Bill Greiman | Library Manager | SD card access |
| Rocketscream LowPower | Rocketscream | Manual (GitHub) | Sleep modes |
| SoftwareSerial | Arduino | Built-in | Cellular models |
| AltSoftSerial | Paul Stoffregen | Library Manager | Cellular models |

**⚙️ Prevention:**
- Install all required libraries before first compile
- Keep libraries updated (but test after updates)
- Document library versions that work
- Keep backup of working library versions

---

## Quick Reference Table

| Symptom | Most Likely Cause | Quick Fix | Page Ref |
|---------|-------------------|-----------|----------|
| Won't power on | Dead battery | Replace battery | Issue 1 |
| Battery drains overnight | Not sleeping | Check sleep code | Issue 2 |
| Reads max range | No target/aimed wrong | Re-aim sensor | Issue 4 |
| Distance drifts | Temperature expansion | Accept or compensate | Issue 5 |
| Negative lidar values | I2C error | Add pull-ups | Issue 6 |
| SD card corrupt | Improper shutdown | Recovery tools, then format | Issue 7 |
| Weird CSV characters | Encoding mismatch | Import as UTF-8 | Issue 8 |
| ThingsBoard shows old data | Network/SIM issue | Check signal/SIM | Issue 9 |
| Intermittent telemetry | Weak signal | External antenna | Issue 10 |
| Upload fails "not responding" | Sensor connected | Disconnect sensor | Issue 11 |
| Upload fails serial busy | Serial Monitor open | Close Serial Monitor | Issue 12 |
| Upload OK but no action | Wrong board selected | Check board settings | Issue 13 |
| Compile error missing library | Library not installed | Install library | Issue 14 |

---

## Still Stuck?

If your issue isn't listed here:

1. **Check [Diagnostic Flowcharts](diagnostic-flowcharts.md)** - Visual troubleshooting paths
2. **Review [FAQ](faq.md)** - Edge cases and unusual questions
3. **Search GitHub Issues** - Someone may have had same problem
4. **Contact Support** - With details: symptoms, diagnostics performed, serial logs

---

## Next Steps

- 📊 [Diagnostic Flowcharts](diagnostic-flowcharts.md) - Visual troubleshooting trees
- ❓ [FAQ](faq.md) - Frequently asked questions
- 🔧 [Maintenance Guide](../hardware/maintenance.md) - Preventive maintenance
- 📞 Support - [GitHub Issues](https://github.com/ICHydro/Riverlabs/issues)

---

!!! success "Most Issues Are Simple"
    80% of issues are: battery low, SD card not formatted, or XBee misconfigured. Check the basics first!
