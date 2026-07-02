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
| **Annually** | 20 mins | External cleaning, Silica Replacement and battery change recommended | High |

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

   - Data received in last 24-48 hours
   - No gaps in data stream
   - Timestamps continuous and correct

2. **Review Battery Voltage**

   - Voltage trend is stable/expected
   - No sudden drops
   - Above 3.5V (plan visit if approaching this level of depletion)

3. **Inspect Sensor Readings**

   - Values within expected range
   - No stuck readings (same value repeatedly)
   - Variance appropriate for conditions

4. **Check Telemetry Health (if applicable)**

   - Signal strength adequate
   - Connection successful
   - No excessive retries


!!! danger Red Flags Requiring Site Visit:**
- ⚠️ No data for >48 hours - likely power loss or logger displacement.
- ⚠️ Battery voltage <3.4V - Power drain
- ⚠️ Sensor readings stuck or erratic
- ⚠️ Telemetry signal lost

### Without Telemetry

If no telemetry, increase frequency of physical site visits (monthly instead of quarterly).

---

## Site Inspection (Quarterly or Anually)

### Time Required: 15-30 minutes

### Visual External Inspection

1. **Enclosure:**

   - No visible damage or cracks
   - Seals intact
   - No water inside enclosure (look for condensation and check colour of silica inside)
   - Mounting secure, no loosening
   - No evidence of tampering or vandalism
   - Check no pooling of water on seals
   - Ensure no hairs or strand materials across the seals

2. **Environmental:**

   - Vegetation clearance maintained
   - No new obstructions in sensor path
   - Drainage working (no water pooling)
   - No animal/insect nests visible

3. **Sensor:**

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
- Download SD card data
- Verify recent measurements
- Check file integrity
- Compare with remote data (if telemetry)

---

## Yearly Maintenance

### Time Required: 30-60 minutes

### Pre-Maintenance Checklist

**Bring:**

- Cleaning supplies (see Cleaning section)
- Spare batteries if required (charged)
- Cross head screwdriver and basic tools
- Multimeter
- FTDI cable and laptop (in case clock/code updates needed - ensure Arduino and required libraries are installed)

### Detailed Inspection

**1. External Condition (10 min)**

1. **Enclosure integrity**

    - No cracks, scratches, or damage
    - All screws/fasteners present and tight
    - Gaskets/seals in good condition
    - Cable glands tight and sealed
    - No water marks inside

2. **Mounting security**

    - Mounting hardware tight
    - No corrosion on brackets/ties
    - Zip ties not brittle (replace if degraded)
    - No movement or wobble
    - Alignment still correct

3. **Environmental factors**

    - Vegetation trimmed (clear zones maintained)
    - No new nearby obstructions
    - Sun shield intact (if present)
    - Bird deterrents working
    - No animal activity

**2. Internal Inspection (10 min)**

Open enclosure and inspect:

1. **No moisture inside**

    - Desiccant dry (replace if saturated)
    - No condensation on PCB
    - No corrosion on components

2. **Battery condition**

    - No swelling or damage
    - Contacts clean and shiny
    - Good mechanical connection

3. **SD card**

    - Seated properly
    - No physical damage
    - Recent data present

4. **Connections**

    - Sensor cable secure
    - No frayed wires
    - Solder joints intact
    - XBee seated (if present)

5. **Visual PCB check**

    - No burn marks
    - No corrosion
    - All components present
    - No loose parts rattling

**3. Functional Testing (10 min)**

1. **Power test**

    - Logger powers on
    - LED sequence normal
    - No error indicators

2. **Clock check**

    - Time accurate (within 1 minute)
    - Date correct
    - If off: Re-set using set_clock.ino

3. **Sensor test**

    - Take several test readings
    - Values reasonable and consistent
    - No error messages
    - Standard deviation acceptable

4. **SD card test**

    - Card readable
    - Recent data present
    - Files not corrupted
    - Free space adequate

5. **Telemetry test** (if applicable)

    - XBee LED activity
    - Recent successful transmission
    - Signal strength adequate


**5. Data Management (5 min)**

    - Download all data from SD card
    - Verify data integrity
    - Backup data (multiple locations)
    - Optional: Replace SD card with fresh formatted card
    - Document data retrieval in log






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
