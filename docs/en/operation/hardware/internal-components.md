# Internal Components Reference

!!! abstract "Overview"
    This guide provides detailed information about the internal components of Riverlabs loggers, their locations, functions and maintenance requirements.

!!! info "PCB Photos Needed"
    This section will be enhanced with annotated PCB photographs and 3D renderings. Component callouts will be generated using Fusion 360 as suggested.

## Component Layout

### Main Components Overview

The Riverlabs logger PCB contains the following key components:

1. **Microcontroller (CPU)**: Atmel Atmega328P
2. **Real-Time Clock (RTC)**: DS3231 precision RTC
3. **SD Card Slot**: MicroSD card interface (optional)
4. **FTDI Programming Header**: 6-pin serial programming interface
5. **Main Battery Contacts**: e.g., 18650 battery holder
6. **Backup Battery Holder**: CR1220 coin cell clip
7. **Power Regulation**: Voltage regulators and protection
8. **XBee Socket** (telemetry models): Cellular modem interface 
9. **Sensor Connector**: Connection to ultrasound/Lidar sensor
10. **LED Indicators**: Status indication
11. **EEPROM**: External data buffering

![PCB component layout - coming soon](../../assets/images/pcb-layout-annotated.png)<br>
*Annotated PCB layout showing component locations*

---

## Component Details

### 1. Microcontroller (Atmega328P)

**Location:**

- Center of PCB
- Square IC with many pins

**Function:**

- Main processor running Arduino bootloader
- Executes logging code
- Controls all peripherals
- Manages power states (sleep/wake)

**Specifications:**

- 8-bit AVR architecture
- 32KB flash memory (program storage)
- 2KB SRAM (working memory)
- 1KB EEPROM (optional data buffer)
- 16MHz clock speed

**User Serviceable:**

- No — do not attempt to remove or replace

### 2. Real-Time Clock (DS3231)

**Location:**

- Near coin battery holder

**Function:**

- Keeps accurate time
- Generates wake-up alarms
- Temperature-compensated crystal oscillator
- Maintains time during main battery removal (if CR1220 installed)

**Specifications:**

- Accuracy: ±2 ppm (±1 minute/year)
- Temperature range: -40°C to +85°C
- Battery backup input for CR1220
- I2C communication interface

**Maintenance:**

- No maintenance required
- Powered by CR1220 backup battery
- Automatically maintains time

**Troubleshooting:**

- If clock resets: replace CR1220 battery
- If clock drifts: check code for proper RTC initialization

### 3. SD Card Slot

**Location:**

- Edge of PCB, metallic spring-loaded socket

**Function:**

- Local data storage
- Records all measurements with timestamps
- Removable for data retrieval

**Specifications:**

- MicroSD card interface
- SPI communication
- Supports SD and SDHC cards (up to 32GB)
- FAT32 filesystem

**Maintenance:**

- Keep contacts clean
- Ensure card clicks into place
- Don't force card insertion
- Regularly check card seating

**Common Issues:**

- Card not detected: reseat card and check formatting
- Write errors: replace card (wear-out)
- Slow writes: use faster card (Class 10)

### 4. FTDI Programming Header

**Location:**

- Row of 6 pins
- Usually at edge of PCB

**Pin Configuration:**
```
GRN side          BLK side
  ↓                  ↓
[GND][CTS][VCC][TXD][RXD][DTR]
```

**Marking on Board:**

- **"GRN"** marking indicates green wire side
- **"BLK"** marking indicates black wire side
- On cellular/lidar models: black pin (GND) is on SD card side

**Function:**

- Serial programming interface for code upload
- Connect FTDI cable for Arduino IDE programming
- Serial monitor communication
- Clock setting via RTC library

**Usage:**

- **FTDI Cable Orientation:** CRITICAL
    - Green wire aligns with "GRN" marking
    - Black wire aligns with "BLK" marking
- **Voltage:** 5 V or 3.3 V FTDI cables supported (3.3 V recommended)
- **Auto-reset:** DTR pin triggers automatic reset for programming
- **Power:** can power logger when connected (see Power Switch section)

**Protection:**

- Header not protected against reverse polarity
- ⚠️ **Always double-check FTDI cable orientation before connecting**
- Incorrect connection can damage logger or prevent programming

**Cable Types:**

- [Sparkfun FTDI Cable 3.3V](https://www.sparkfun.com/products/9717): Direct USB connection
- [Sparkfun FTDI Basic Breakout 3.3V](https://www.sparkfun.com/products/9873): Requires micro-USB cable

### 4b. Debug Serial Pins (DBG)

**Location:**

- 2-pin header near edge of PCB, labelled "DBG"

**Pin Configuration:**
```
[GND] [TX]
```

**Function:**

- Hardware serial debugging output
- Monitor real-time logger activity without reprogramming
- View sensor readings, SD writes and telemetry status
- Does NOT require main FTDI header connection

**How to Use Debug Pins:**

**Hardware Setup:**

1. **Equipment needed:**
    - FTDI cable or USB-to-Serial adapter
    - 2× female-to-female breadboard jumper wires

2. **Connections:**
   ```
   Logger DBG GND → FTDI Black wire (GND)
   Logger DBG TX  → FTDI Yellow wire (RX/Serial In)
   ```

3. **Do NOT connect:**
    - FTDI to main programming header
    - Power from FTDI (logger should be battery-powered)

4. **Power:**
    - Logger must be powered by battery (switch ON)

**Software Setup:**

1. Connect FTDI cable to computer
2. Open Arduino IDE Serial Monitor (Tools → Serial Monitor)
3. Set baud rate to **115200**
4. Reset logger (press reset button)
5. Debug output will appear in Serial Monitor

**Debug Output Includes:**

- Boot messages and initialization
- RTC timestamp at each measurement
- Sensor readings (raw and processed)
- SD card write status
- Telemetry connection status
- XBee communication logs
- Error messages and warnings

!!! tip "Essential for Troubleshooting"
    The DBG pins are invaluable for diagnosing issues in the field. You can connect them while the logger is deployed to see what's happening without removing the SD card or reprogramming.

**Common Debug Messages:**

```
--- Wari Logger Boot ---
RTC Init: OK
SD Card Init: OK
Time: 2026-01-03 14:35:22
Sensor: 1.234m
SD Write: OK
XBee Wake
TX: Data sent
XBee Sleep
Entering sleep...
```

### 5. Main Battery Contacts

**Location:**

- Spring-loaded contacts or battery holder

**Function:**

- Primary power connection
- 18650 battery interface
- Positive and negative terminals

**Maintenance:**

- **Monthly:** Inspect for corrosion
- **As needed:** Clean with isopropyl alcohol
- **Check:** Spring tension adequate
- **Verify:** No oxidation or buildup

**Corrosion Prevention:**

- Thin layer of DeoxIT on contacts
- Ensure battery terminals are clean
- Keep compartment dry

### 6. Backup Battery (CR1220) Holder

**Location:**

- Small circular holder near RTC IC

**Function:**

- Powers RTC when main battery removed
- Maintains timekeeping
- Does not power logger

**Installation:**

- Positive (+) side typically faces up
- Press firmly until clicks
- Should sit flush in holder

**Lifespan:**

- 5–10 years typical
- Replace annually for critical applications
- Replace if clock resets when main battery removed

### 7. Power Switch

**Location:**

- Edge of PCB, slide or toggle switch

**Label:**

 - "ON (Batt) / OFF (ftdi)"

**Function:**

- Controls main battery power to logger
- Enables FTDI programming power when OFF
- Safety feature prevents simultaneous battery + USB power

**Switch Positions:**

| Position       | Battery      | FTDI Power | Use Case                           |
|----------------|--------------|------------|------------------------------------|
| **ON (Batt)**  | Connected    | Blocked    | Normal operation, field deployment |
| **OFF (ftdi)** | Disconnected | Enabled    | Programming via FTDI cable         |

**How It Works:**

!!! info "Intelligent Power Design"
    The power switch is designed to prevent accidentally connecting both battery power and USB power (via FTDI) simultaneously, which could damage the battery or components.

**ON Position (Battery):**

- Battery powers the entire logger
- Logger operates normally
- FTDI cable can still be connected for programming
- Logger gets power from battery, not FTDI
- LED will show normal operation

**OFF Position (FTDI):**

- Battery is disconnected from circuit
- If FTDI cable connected: logger powered via USB
- If FTDI not connected: logger has no power
- Convenient for programming without battery installed
- Prevents battery damage from USB power

**Programming Options:**

You can program the logger in three configurations:

1. **Switch OFF, no battery, FTDI connected**
    - Safest for first-time programming
    - Logger powered entirely from USB/FTDI
    - Most common approach

2. **Switch OFF, battery installed, FTDI connected**
    - Battery disconnected but present
    - Logger powered from USB/FTDI
    - Battery remains safe

3. **Switch ON, battery installed, FTDI connected**
    - Battery powers logger
    - FTDI provides data connection only
    - Useful if FTDI cable cannot supply enough current

**Best Practices:**

✅ **DO:**

- Turn switch OFF when programming (recommended)
- Verify switch position before connecting FTDI
- Use switch to fully power down logger

❌ **DON'T:**

- Worry about switch position confusion — the design prevents damage
- Leave switch ON during long-term storage (battery drain)

**Troubleshooting:**

| Issue                    | Check                                            |
|--------------------------|--------------------------------------------------|
| No power with switch ON  | Battery dead, check voltage                      |
| No power with switch OFF | FTDI cable not connected or no data pins         |
| Can't upload code        | Try switch OFF position, verify FTDI orientation |

### 8. Power Regulation Circuit

**Location:**

- Components near battery contacts

**Components:**

- Voltage regulators (3.3 V logic supply)
- Protection diodes
- Capacitors for smoothing
- Power management ICs

**Function:**

- Converts 3.7 V battery to 3.3 V logic level
- Provides stable power to electronics
- Protects against reverse polarity (some models)
- Over-current protection

**User Serviceable:**

- No — do not modify

### 8. XBee Socket (Telemetry Models Only)

**Location:**

 - Prominent socket, often at one end of PCB

**Function:**

- Hosts XBee cellular modem
- Through-hole socket for XBee module
- Power and communication pins

**XBee Installation:**

- Align notch on XBee with socket
- Press firmly until seated
- All pins must be in socket
- Should not wobble

**Removal:**

- Gently rock side-to-side while pulling
- Never pull on antenna
- Check for bent pins

**Maintenance:**

- Keep socket free of dust
- Check for bent pins
- Ensure SIM card installed in XBee

### 9. Sensor Connector

**Location:**

 - Connector for external sensor cable

**Types:**

- **Wari:** 3–4 pin screw terminal or JST
- **Lidar:** 6-pin JST connector

**Function:**

- Power to sensor
- Data communication
- Ground reference

**Maintenance:**

- Check connection tight
- Inspect for corrosion
- Verify no broken wires
- Strain relief adequate

**Pin Configuration (typical):**

**Wari (MB7389):**
```
1. +5V (power)
2. Serial Out
3. Serial In (sometimes unused)
4. GND
```

**Lidar (Garmin):**
```
1. Power Enable
2. Mode Control
3. +5V
4. I2C SDA
5. I2C SCL
6. GND
```

### 10. LED Indicators

**Location:**

 - Through-hole or surface-mount LEDs

**Common LEDs:**

| LED           | Color       | Function                |
|---------------|-------------|-------------------------|
| **Power**     | Red/green   | Logger powered          |
| **Activity**  | Yellow/blue | Measurement in progress |
| **Error**     | Red         | Error condition         |
| **Telemetry** | Blue/green  | XBee activity           |

**LED Behavior:**

**Normal Operation:**

- Brief flash during measurement
- Steady during sensor reading
- Quick flashes during SD write

**Error Patterns:**

- Rapid flashing: SD card error
- Continuous on: stuck in error state
- No activity: power/code issue

**Power Saving:**

- LEDs disabled in deployment mode
- Enable DEBUG mode to see activity

### 11. External EEPROM (Some Models)

**Location:**

- 8-pin IC near SD card slot

**Function:**

- Data buffering
- Stores measurements in memory before SD write
- Extends SD card life
- Prevents data loss during SD issues

**Specifications:**

- Typically 24LC512 or similar
- 64 KB capacity
- I2C interface
- Non-volatile memory

**Operation:**

- Measurements written to EEPROM first
- Batch written to SD card periodically
- User-configurable flush interval

**User Serviceable:**

- No

---

## Additional Components

### Capacitors

**Purpose:**

- Power supply smoothing
- Noise filtering
- Timing circuits

**Maintenance:**

- No user maintenance

### Resistors

**Purpose:**

- Pull-up/pull-down on signals
- Current limiting for LEDs
- Voltage dividers

**Maintenance:**

 - No user maintenance

### Crystals/Oscillators

**8MHz Crystal (External):**

- Main clock for microcontroller
- Determines timing accuracy

**Maintenance:**

 - No user maintenance

---

## PCB Versions

### Identifying Your PCB Version

**Look for:**

- Version number silk-screened on PCB (e.g., "v2.1")
- Date code
- Riverlabs logo/text

**Common Versions:**

| Version           | Features                  | Notes                 |
|-------------------|---------------------------|-----------------------|
| **Wari v1**       | Basic logging             | Original design       |
| **Wari v2.0**     | + EEPROM, improved power  | Enhanced reliability  |
| **Wari v2.1**     | + Telemetry optimizations | Current production    |
| **Lidar/WMOnode** | Lidar sensor support      | Higher power 5V boost |

---

## Safety and Handling

### ESD Protection

!!! warning "Static Electricity Can Damage Electronics"
    
**Precautions:**

- Touch grounded metal before handling PCB
- Avoid handling in dry conditions
- Use ESD wrist strap if available
- Store in anti-static bag when not installed

### Physical Handling

**Do:**

- ✅ Handle by edges only
- ✅ Use proper tools
- ✅ Work on non-conductive surface
- ✅ Keep away from liquids

**Don't:**

- ❌ Touch component pins
- ❌ Apply pressure to components
- ❌ Expose to moisture
- ❌ Drop or impact

### Cleaning

**If necessary:**

- Power off and remove batteries
- Use 90%+ isopropyl alcohol
- Soft brush (toothbrush or anti-static brush)
- Air dry completely before reassembly
- Do not use water or cleaners

---

## Component Testing

### Visual Inspection

**Check for:**

- [ ] No physical damage
- [ ] No bent pins
- [ ] No corrosion
- [ ] No burn marks
- [ ] Solder joints intact
- [ ] No cracks in PCB

### Functional Testing

**Power Test:**

1. Install fresh battery
2. Verify LED activity
3. Check voltage with multimeter

**Communication Test:**

1. Connect FTDI cable
2. Open Serial Monitor
3. Upload test sketch
4. Verify serial output

**Sensor Test:**

1. Connect sensor
2. Run sensor test code
3. Verify reasonable readings
4. Check for error messages

**SD Card Test:**

1. Insert formatted card
2. Run for several measurements
3. Remove and read data file
4. Verify format and content

**RTC Test:**

1. Set clock
2. Remove main battery (backup battery installed)
3. Wait 1 minute
4. Reinstall main battery
5. Check time — should be accurate

---

## Modifications and Upgrades

### Firmware Updates

**Safe and Recommended:**

- Upload new code via FTDI
- Modify user-configurable parameters
- Update to latest stable version

**Procedure:**

1. Download new firmware
2. Verify compatibility with hardware version
3. Upload via Arduino IDE
4. Test thoroughly before deployment

### Hardware Modifications

!!! danger "Hardware Modifications at User Risk"
    Modifications to PCB, components or circuits  may damage logger.

---

## Troubleshooting Component Issues

### By Symptom

**No power:**

- Check battery voltage
- Inspect battery contacts (bend inwards if requied)
- Verify power LED (if present)
- Check for shorts (loose wire or damage to the PCB Board)

**Clock resets:**

- Replace CR1220 backup battery (should read 3VDC)
- Check RTC battery holder
- Verify code sets clock properly

**SD card errors:**

- Clean card contacts
- Try different card
- Check socket for damage

**Sensor errors:**

- Check connector secure
- Verify cable continuity
- Test sensor separately (on working logger if possible)
- Check for 5 V on power pin (Lidar only)

**Serial communication failure:**

- Verify FTDI cable orientation
- Check for bent header pins
- Try different FTDI cable
- Test with simple blink sketch

---

## Replacement Parts

### User-Replaceable Components

| Component          | Part Number          | Source | Skill Level |
|--------------------|----------------------|--------|-------------|
| **18650 Battery**  | See Battery Guide    | Retail | Easy        |
| **CR1220 Battery** | CR1220               | Retail | Easy        |
| **MicroSD Card**   | Any FAT32 compatible | Retail | Easy        |
| **XBee Module**    | XBee3 Cellular LTE-M | Digi   | Moderate    |

### Professional Replacement Required

- Microcontroller
- RTC IC
- Voltage regulators
- Any surface-mount components
- Damaged PCB traces

**Contact Riverlabs for repair:** info@riverlabs.uk

---

## Documentation

### PCB Schematic

!!! info "Open Source Schematics"
    Full PCB schematics are available in the [GitHub repository](https://github.com/ICHydro/Riverlabs). Contact info@riverlabs.uk for more details.

### Bill of Materials (BOM)

Complete component lists available in the [repository documentation](https://github.com/ICHydro/Riverlabs).

---

## Next Steps

- 🔋 [Battery & Power Guide](battery-power-guide.md): Power system details
- 🛠️ [Maintenance Guide](maintenance.md): Regular maintenance procedures
- 🚨 [Troubleshooting](../troubleshooting/common-issues.md): Component-specific issues
- 💻 [Arduino Setup](../quick-start/arduino-setup.md): Programming and firmware

---

!!! note "PCB Evolution"
    Riverlabs continuously improves PCB designs. While component locations may vary slightly between versions, core functionality and programming remain consistent.
