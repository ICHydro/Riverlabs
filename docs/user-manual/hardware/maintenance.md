# Maintenance Guide

!!! abstract "Overview"
    Regular maintenance ensures reliable, long-term operation of your Riverlabs logger. This guide covers routine checks, cleaning procedures, and preventive maintenance schedules.

## Maintenance Philosophy

**Preventive maintenance is cheaper than reactive repair.**

Regular, systematic checks catch problems before they cause data loss or equipment failure. A 10-minute maintenance visit every 3-6 months prevents days of troubleshooting and potential data gaps.

---

## Maintenance Schedule Overview

### Quick Reference

| Frequency | Duration | Tasks | Priority |
|-----------|----------|-------|----------|
| **Monthly** | 1 min | Data Review - check data and battery levels, data download as appropriate | Medium |
| **Quarterly** | 5 min | Basic visual inspection & external cleaning  | High |
| **Annually** | 15 hours | External cleaning, Silica Replacement and battery change recommended | High |

### Flexible Scheduling

Adjust frequency based on:
- **Deployment criticality** (research vs. operational)
- **Environmental harshness** (urban vs. extreme conditions)
- **Accessibility** (easy vs. difficult site visits)
- **Telemetry availability** (remote vs. manual data retrieval)
- **Logger age** (new vs. established deployment)

---

## Remote Monitoring (Quarterly)

### If Telemetry is Available

**Time Required:** 1 minutes

**Tasks:**

1. **Check Latest Data Transmission**
   - [ ] Data received in last 24-48 hours
   - [ ] No gaps in data stream
   - [ ] Timestamps continuous and correct

2. **Review Battery Voltage**
   - [ ] Voltage trend is stable/expected
   - [ ] No sudden drops
   - [ ] Above 3.5V (plan visit if approaching this level of depletion)

3. **Inspect Sensor Readings**
   - [ ] Values within expected range
   - [ ] No stuck readings (same value repeatedly)
   - [ ] Variance appropriate for conditions

4. **Check Telemetry Health (if applicable) **
   - [ ] Signal strength adequate
   - [ ] Connection successful
   - [ ] No excessive retries

**Red Flags Requiring Site Visit:**
- ⚠️ No data for >48 hours - likely power loss or logger displacement.
- ⚠️ Battery voltage <3.4V - Power drain
- ⚠️ Sensor readings stuck or erratic
- ⚠️ Telemetry signal lost

### Without Telemetry

If no telemetry, increase frequency of physical site visits (monthly instead of quarterly).

---

## Monthly Inspection

### Time Required: 15-30 minutes

### Visual External Inspection

**Enclosure:**
- [ ] No visible damage or cracks
- [ ] Seals intact
- [ ] No water inside enclosure (look for condensation and check colour or silica inside).
- [ ] Mounting secure, no loosening
- [ ] No evidence of tampering or vandalism

- Check no pooling of water on seals.
- Ensure no hairs or strand materials acorss the seals.

**Environmental:**
- Vegetation clearance maintained
- No new obstructions in sensor path
- Drainage working (no water pooling)
- No animal/insect nests visible 

**Sensor:**
- Sensor face clean
- No cobwebs or debris
- No physical damage
- Cable secure and undamaged


### Quick Functional Check

**If accessible without opening:**
- LED activity visible (when RST button is pressed)
- No error LED patterns (See LED Patterns)

### Data Download 

If accessible and time permits:
- [ ] Download SD card data
- [ ] Verify recent measurements
- [ ] Check file integrity
- [ ] Compare with remote data (if telemetry)

---

## Yearly Maintenance

### Time Required: 30-60 minutes

### Pre-Maintenance Checklist

**Bring:**
- [ ] Cleaning supplies (see Cleaning section)
- [ ] Spare batteries if required (charged)
- [ ] Cross head screwdriver basic tools
- [ ] Multimeter
- [ ] FTDI cable and laptop (encase clock/code updates needed - ensure arduino and required libraries are installed)

### Detailed Inspection

**1. External Condition (10 min)**

- [ ] **Enclosure integrity**
  - No cracks, scratches, or damage
  - All screws/fasteners present and tight
  - Gaskets/seals in good condition
  - Cable glands tight and sealed
  - No water marks inside

- [ ] **Mounting security**
  - Mounting hardware tight
  - No corrosion on brackets/ties
  - Zip ties not brittle (replace if degraded)
  - No movement or wobble
  - Alignment still correct

- [ ] **Environmental factors**
  - Vegetation trimmed (clear zones maintained)
  - No new nearby obstructions
  - Sun shield intact (if present)
  - Bird deterrents working
  - No animal activity

**2. Internal Inspection (10 min)**

Open enclosure and inspect:

- [ ] **No moisture inside**
  - Desiccant dry (replace if saturated)
  - No condensation on PCB
  - No corrosion on components

- [ ] **Battery condition**
  - Battery voltage: _______ V
  - No swelling or damage
  - Contacts clean and shiny
  - Good mechanical connection

- [ ] **SD card**
  - Seated properly
  - No physical damage
  - Recent data present

- [ ] **Connections**
  - Sensor cable secure
  - No frayed wires
  - Solder joints intact
  - XBee seated (if present)

- [ ] **Visual PCB check**
  - No burn marks
  - No corrosion
  - All components present
  - No loose parts rattling

**3. Functional Testing (10 min)**

- [ ] **Power test**
  - Logger powers on
  - LED sequence normal
  - No error indicators

- [ ] **Clock check**
  - Time accurate (within 1 minute)
  - Date correct
  - If off: Re-set using set_clock.ino

- [ ] **Sensor test**
  - Take several test readings
  - Values reasonable and consistent
  - No error messages
  - Standard deviation acceptable

- [ ] **SD card test**
  - Card readable
  - Recent data present
  - Files not corrupted
  - Free space adequate

- [ ] **Telemetry test** (if applicable)
  - XBee LED activity
  - Recent successful transmission
  - Signal strength adequate

**4. Cleaning (10 min)**

See [Cleaning Procedures](#cleaning-procedures) section below.

**5. Data Management (5 min)**

- [ ] Download all data from SD card
- [ ] Verify data integrity
- [ ] Backup data (multiple locations)
- [ ] Optional: Replace SD card with fresh formatted card
- [ ] Document data retrieval in log

**6. Documentation (5 min)**

Record in maintenance log:
- Date and time of visit
- Person performing maintenance
- Battery voltage
- Any issues found and resolved
- Parts replaced
- Next scheduled maintenance date
- Photos taken

---

## Annual Service

### Time Required: 1-2 hours

Annual service is a comprehensive overhaul and includes all quarterly tasks plus:

### Additional Annual Tasks

**1. Battery Management**

- [ ] **Main battery (18650)**
  - Replace regardless of voltage (12-month lifecycle)
  - Or test capacity with battery analyzer
  - Clean battery compartment thoroughly
  - Record old battery voltage and new battery info

- [ ] **Backup battery (CR1220)**
  - Replace (inexpensive, ensure timekeeping)
  - Check holder for corrosion
  - Test clock retention after replacement

**2. Component Replacement**

- [ ] **Desiccant packs**
  - Replace all desiccant
  - Use indicator type to monitor

- [ ] **Seals and gaskets**
  - Inspect for wear, cracks, compression set
  - Replace if questionable
  - Apply thin layer of silicone grease

- [ ] **Fasteners**
  - Check all screws, bolts, zip ties
  - Replace any corroded or damaged
  - Apply thread locker if needed

**3. Deep Cleaning**

- [ ] Clean all external surfaces
- [ ] Clean sensor thoroughly (see Sensor Cleaning)
- [ ] Clean PCB if any dust/debris (isopropyl alcohol)
- [ ] Clean battery contacts with DeoxIT
- [ ] Clean enclosure gasket surfaces

**4. Firmware and Configuration**

- [ ] **Update firmware** (if new version available)
  - Backup current configuration
  - Test new firmware
  - Verify all functions

- [ ] **Verify configuration**
  - Review measurement intervals
  - Check telemetry settings
  - Confirm clock accuracy
  - Update if requirements changed

**5. Calibration Check**

- [ ] **Distance measurement verification**
  - Measure to known target
  - Compare logger reading with manual measurement
  - Difference should be <2% for Wari, <1% for Lidar
  - Document any offset

- [ ] **Voltage measurement**
  - Compare logged voltage with multimeter reading
  - Should agree within 0.05V
  - If off, may need calibration constant update

**6. Structural Assessment**

- [ ] **Mounting evaluation**
  - Is mounting still optimal?
  - Any structural changes to site?
  - Clearances still adequate?
  - Consider improvements/repositioning

- [ ] **Environmental changes**
  - New vegetation growth patterns?
  - Bank erosion affecting mounting?
  - Access still reasonable?
  - Site security adequate?

**7. Comprehensive Testing**

- [ ] **Extended functional test**
  - Run for 1+ hour while on-site
  - Verify multiple measurement cycles
  - Check all LED behaviors
  - Test telemetry transmission (if applicable)
  - Download and inspect data

- [ ] **Stress test** (optional)
  - Test with high frequency logging
  - Verify no errors under load
  - Check battery drain rate

**8. Documentation and Planning**

- [ ] Complete annual maintenance report
- [ ] Update site documentation
- [ ] Take comprehensive photos
- [ ] Plan next year's maintenance
- [ ] Identify any needed replacements/upgrades
- [ ] Budget for parts and labor

---

## Cleaning Procedures

### Sensor Cleaning

#### Ultrasound (Wari)

**Frequency:** Quarterly minimum, monthly in dusty/insect-heavy areas

**Materials:**
- Soft brush (e.g., lens brush, soft toothbrush)
- Compressed air (clean, dry)
- Lint-free cloth
- Isopropyl alcohol (optional, for stubborn deposits)

**Procedure:**

1. **Power off logger** (or time cleaning between measurements)

2. **Brush away loose debris**
   - Use soft brush to remove dust, cobwebs, insects
   - Brush in one direction (top to bottom)
   - Don't apply pressure to sensor face

3. **Compressed air**
   - Short bursts from 6-12 inches away
   - Don't invert can (liquid may spray)
   - Keep nozzle straight, not angled

4. **Stubborn deposits**
   - Dampen (not wet) lint-free cloth with isopropyl alcohol
   - Gently wipe sensor mesh
   - Allow to air dry completely

5. **Verify clearance**
   - Check nothing obstructing sensor
   - Take test measurement
   - Compare with pre-cleaning reading

!!! warning "Do Not:"
    - Use water (may damage sensor)
    - Apply pressure to transducer
    - Use harsh chemicals
    - Touch with fingers (oils transfer)
    - Clean while sensor is active

#### Lidar

**Frequency:** Weekly to monthly depending on conditions

**Materials:**
- Lens cleaning tissue or microfiber cloth
- Compressed air
- Isopropyl alcohol (90%+)
- NO abrasive materials

**Procedure:**

1. **Inspect lens**
   - Look for dust, condensation, fingerprints, cobwebs
   - Identify type of contamination

2. **Dust removal**
   - Use compressed air first
   - Lens brush for stubborn particles
   - Never wipe dry dust (scratches lens)

3. **Lens cleaning**
   - Dampen lens tissue with alcohol
   - Wipe in circular motion, center outward
   - Use clean section of tissue for each pass
   - Buff with dry lens tissue

4. **Test measurement**
   - Take reading at known distance
   - Compare with expected value
   - Repeat cleaning if readings off

!!! danger "Lidar Lens is Critical"
    The lidar lens is precision optics. **Never:**
    - Touch with fingers
    - Use paper towel (scratches)
    - Use household cleaners
    - Apply pressure
    - Clean with contaminated cloth

### Enclosure Cleaning

**Frequency:** Annually or as needed

**Exterior:**
1. Wipe with damp cloth
2. Remove dirt, algae, bird droppings
3. Check for damage to coating/paint
4. Dry thoroughly

**Interior:**
1. Remove batteries
2. Inspect for dust, insect debris, corrosion
3. Vacuum loose debris (be gentle near components)
4. Wipe with lint-free cloth and isopropyl alcohol if needed
5. Allow to dry completely
6. Replace desiccant before closing

### Battery Contact Cleaning

**Frequency:** Annually or if corrosion appears

**Procedure:**
1. Remove battery
2. Inspect contacts for oxidation/corrosion
3. Clean with pencil eraser (light abrasion)
4. Wipe with isopropyl alcohol
5. Optional: Apply thin layer of DeoxIT contact enhancer
6. Allow to dry
7. Reinstall battery

### SD Card Maintenance

**Frequency:** Annually

**Procedure:**
1. Download all data
2. Remove card
3. Clean contacts with isopropyl alcohol and lint-free cloth

---


## Troubleshooting During Maintenance

### Common Issues Found During Inspection

**1. Battery Low/Dead**
- **Solution:** Replace battery
- **Prevention:** Monitor voltage trend, replace proactively

**2. SD Card Full**
- **Solution:** Download data, delete old files or replace card
- **Prevention:** Track data accumulation rate

**3. Clock Reset**
- **Solution:** Replace CR1220, reset clock with set_clock.ino
- **Prevention:** Replace CR1220 annually

**4. Sensor Dirty/Obstructed**
- **Solution:** Clean per procedures above
- **Prevention:** More frequent cleaning schedule

**5. Mounting Loose**
- **Solution:** Tighten/replace fasteners
- **Prevention:** Use thread locker, check fastener condition

**6. Corrosion on Contacts**
- **Solution:** Clean with isopropyl alcohol, apply DeoxIT
- **Prevention:** Keep enclosure sealed, use desiccant

**7. Vegetation Obstruction**
- **Solution:** Trim vegetation, expand clear zone
- **Prevention:** More frequent trimming, herbicide (carefully)

**8. Water Ingress**
- **Solution:** Dry completely, replace gaskets, reseal
- **Prevention:** Check seals regularly, improve drainage

**9. No Recent Data**
- **Solution:** Check code running, verify SD card, test sensor
- **Prevention:** Remote monitoring alerts

**10. Telemetry Failure**
- **Solution:** Check XBee, SIM card, signal, settings
- **Prevention:** Monitor telemetry health remotely


## Next Steps

- 📖 [First Deployment Checklist](../quick-start/first-deployment-checklist.md) - Initial setup verification
- 🔋 [Battery & Power Guide](battery-power-guide.md) - Battery health monitoring
- 🔧 [Internal Components](internal-components.md) - Component-specific maintenance
- 🚨 [Troubleshooting](../troubleshooting/common-issues.md) - Fixing issues found during maintenance

---

!!! success "Well-Maintained Logger = Reliable Data"
    Consistent, documented maintenance is the foundation of long-term successful monitoring. A small investment of time prevents major problems and ensures continuous, high-quality data collection.
