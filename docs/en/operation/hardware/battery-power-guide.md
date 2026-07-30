# Battery & Power Guide

!!! abstract "Overview"
    Understanding battery behavior and power management is essential for reliable field deployments. This guide covers battery selection and voltage interpretation.

## Battery System Overview

Riverlabs loggers use a dual-battery system for power and timekeeping.

![Battery types comparison](../../assets/images/Battery_Types.png)
*18650 and 14500 battery sizes with LiPo and LiFePO4 chemistry options*


### Main Power: 18650 or 14500 Battery

Riverlabs loggers support two battery sizes with two chemistry options:

| Battery Type | Chemistry                        | Nominal Voltage | Capacity        | Form Factor   | Notes                                   |
|--------------|----------------------------------|-----------------|-----------------|---------------|-----------------------------------------|
| **18650**    | LiPo (Lithium Polymer)           | 3.7 V           | 2000–3000 mAh   | 18 mm × 65 mm | Most common, high capacity              |
| **18650**    | LiFePO4 (Lithium Iron Phosphate) | 3.2 V           | 1500–2000 mAh   | 18 mm × 65 mm | Safer, longer cycle life, lower voltage |
| **14500**    | LiPo (Lithium Polymer)           | 3.7 V           | 600–800 mAh     | 14 mm × 50 mm | Smaller, lower capacity                 |
| **14500**    | LiFePO4 (Lithium Iron Phosphate) | 3.2 V           | 400–600 mAh     | 14 mm × 50 mm | Safer, smaller form factor              |

**Chemistry Comparison:**

| Feature                   | LiPo (3.7 V)                 | LiFePO4 (3.2 V)               |
|---------------------------|------------------------------|-------------------------------|
| **Voltage Range**         | 4.2 V (full)–3.0 V (empty)   | 3.65 V (full)–2.5 V (empty).  |
| **Energy Density**        | Higher                       | Lower                         |
| **Safety**                | Good (with protection)       | Excellent (very stable)       |
| **Cycle Life**            | 300–500 cycles               | 2000+ cycles                  |
| **Temperature Tolerance** | -20°C to 60°C                | -20°C to 60°C                 |
| **Cost**                  | Lower                        | Slightly higher               |
| **Best For**              | Maximum runtime              | Long-term reliability, safety |

**Critical Safety Information:**

!!! danger "Battery Safety"
    - ⚠️ Never use damaged or dented batteries
    - ⚠️ Check polarity before insertion
    - ⚠️ Do not charge below 0°C
    - ⚠️ Do not expose to heat >60°C
    - ⚠️ Replace if swollen or leaking
    - ⚠️ Dispose of properly (battery recycling)

### Backup Battery: CR1220 Coin Cell

**Specifications:**

- **Type:** CR1220 Lithium coin cell (non-rechargeable)
- **Nominal Voltage:** 3.0 V
- **Capacity:** ~40 mAh
- **Purpose:** Real-time clock backup only
- **Lifespan:** 5–10 years typical

**Function:**

- Maintains clock time when main battery removed
- Does NOT power logger or sensor
- Logger operates without it (but loses time)
- Essential for telemetry applications (UTC time)

---

## Battery Voltage Interpretation

Understanding battery voltage helps predict remaining life and identify issues.

### Voltage vs. State of Charge

Battery voltage indicates remaining capacity, but the voltage ranges differ between LiPo and LiFePO4:

#### LiPo (3.7 V Nominal) Batteries

<table>
<thead>
<tr>
<th>Voltage</th>
<th>State of Charge</th>
<th>Status</th>
<th>Action</th>
</tr>
</thead>
<tbody>
<tr style="background-color: #22c55e; color: white;">
<td><strong>4.20 V</strong></td>
<td>100%</td>
<td>Fresh/Fully Charged</td>
<td>Optimal</td>
</tr>
<tr style="background-color: #4ade80; color: white;">
<td><strong>4.10 V</strong></td>
<td>~95%</td>
<td>Excellent</td>
<td>Normal operation</td>
</tr>
<tr style="background-color: #86efac;">
<td><strong>4.00 V</strong></td>
<td>~85%</td>
<td>Very Good</td>
<td>Normal operation</td>
</tr>
<tr style="background-color: #bbf7d0;">
<td><strong>3.90 V</strong></td>
<td>~75%</td>
<td>Good</td>
<td>Normal operation</td>
</tr>
<tr style="background-color: #d9f99d;">
<td><strong>3.80 V</strong></td>
<td>~60%</td>
<td>Fair</td>
<td>Monitor</td>
</tr>
<tr style="background-color: #fde047;">
<td><strong>3.70 V</strong></td>
<td>~45%</td>
<td>Nominal</td>
<td>Plan replacement</td>
</tr>
<tr style="background-color: #fbbf24;">
<td><strong>3.60 V</strong></td>
<td>~30%</td>
<td>Low</td>
<td>Replace soon</td>
</tr>
<tr style="background-color: #fb923c;">
<td><strong>3.50 V</strong></td>
<td>~20%</td>
<td>Very Low</td>
<td>Replace immediately</td>
</tr>
<tr style="background-color: #f97316; color: white;">
<td><strong>3.40 V</strong></td>
<td>~10%</td>
<td>Critical</td>
<td><strong>Replace now</strong></td>
</tr>
<tr style="background-color: #dc2626; color: white;">
<td><strong>3.30 V</strong></td>
<td>~5%</td>
<td>Emergency</td>
<td><strong>Imminent failure</strong></td>
</tr>
<tr style="background-color: #991b1b; color: white;">
<td><strong>3.00 V</strong></td>
<td>0%</td>
<td>Depleted</td>
<td>Logger may stop</td>
</tr>
<tr style="background-color: #000000; color: white;">
<td><strong>&lt;3.00 V</strong></td>
<td>Over-discharged</td>
<td>Damaged</td>
<td>Battery may be ruined</td>
</tr>
</tbody>
</table>

#### LiFePO4 (3.2 V Nominal) Batteries

<table>
<thead>
<tr>
<th>Voltage</th>
<th>State of Charge</th>
<th>Status</th>
<th>Action</th>
</tr>
</thead>
<tbody>
<tr style="background-color: #22c55e; color: white;">
<td><strong>3.65 V</strong></td>
<td>100%</td>
<td>Fresh/Fully Charged</td>
<td>Optimal</td>
</tr>
<tr style="background-color: #4ade80; color: white;">
<td><strong>3.40 V</strong></td>
<td>~95%</td>
<td>Excellent</td>
<td>Normal operation</td>
</tr>
<tr style="background-color: #86efac;">
<td><strong>3.30 V</strong></td>
<td>~75%</td>
<td>Very Good</td>
<td>Normal operation</td>
</tr>
<tr style="background-color: #bbf7d0;">
<td><strong>3.25 V</strong></td>
<td>~50%</td>
<td>Good</td>
<td>Normal operation</td>
</tr>
<tr style="background-color: #d9f99d;">
<td><strong>3.20 V</strong></td>
<td>~40%</td>
<td>Fair</td>
<td>Monitor</td>
</tr>
<tr style="background-color: #fbbf24;">
<td><strong>3.10 V</strong></td>
<td>~20%</td>
<td>Low</td>
<td>Replace soon</td>
</tr>
<tr style="background-color: #fb923c;">
<td><strong>3.00 V</strong></td>
<td>~10%</td>
<td>Very Low</td>
<td>Replace immediately</td>
</tr>
<tr style="background-color: #f97316; color: white;">
<td><strong>2.90 V</strong></td>
<td>~5%</td>
<td>Critical</td>
<td><strong>Replace now</strong></td>
</tr>
<tr style="background-color: #dc2626; color: white;">
<td><strong>2.70 V</strong></td>
<td>~2%</td>
<td>Emergency</td>
<td><strong>Imminent failure</strong></td>
</tr>
<tr style="background-color: #991b1b; color: white;">
<td><strong>2.50 V</strong></td>
<td>0%</td>
<td>Depleted</td>
<td>Logger will stop</td>
</tr>
<tr style="background-color: #000000; color: white;">
<td><strong>&lt;2.50 V</strong></td>
<td>Over-discharged</td>
<td>Damaged</td>
<td>Battery may be ruined</td>
</tr>
</tbody>
</table>

!!! info "Flatter Discharge Curve"
    LiFePO4 batteries maintain voltage more consistently throughout discharge. They stay around 3.2–3.3 V for most of their capacity, then drop quickly when depleted.

### Reading Battery Voltage

**From Logger Data:**

- Voltage recorded with each measurement
- Typically last column in data file
- Value in millivolts (e.g., 3850 = 3.85 V)

**Example Data Line:**
```
2025/12/27 14:30:00, 1250, 1248, 1252, 1249, 1251, 1250, 1248, 1251, 1249, 1250, 3850
                                                                                      ^^^^
                                                                                Battery voltage (mV)
```

**With Multimeter:**

1. Set multimeter to DC voltage
2. Access battery terminals (may require opening logger)
3. Red probe to + terminal
4. Black probe to - terminal
5. Read voltage (should show 3.0–4.2 V)

!!! tip "Voltage Sag During Measurement"
    Voltage drops briefly during active measurement due to high current draw. The logged voltage is typically measured during sleep (more accurate for capacity estimation).

!!! info "First Reading May Be Low"
    On some loggers, the first voltage reading after power-on may appear lower than subsequent readings. This occurs because the measurement circuit capacitors have not fully charged yet. Subsequent readings will show the correct voltage once the circuit has stabilized (typically after the second or third measurement).

---

## Solar Charging

### Solar Panel Specifications

**Typical Solar Setup:**

- Panel: 5–10 W, 6 V
- Charge Controller: 3.7 V Li-ion compatible
- Cable: Weatherproof, strain relief
- Mounting: Adjustable for sun angle

!!! warning "Solar Charging Limitations"
    - Only charges above 0°C
    - Requires good sun exposure, not suitable for heavily shaded locations
    - May not keep up with telemetry use

---

## Battery Storage

### Long-Term Storage

**Optimal Storage Conditions:**

- **Voltage:** 3.7–3.8 V for LiPo (50% charge) or 3.2–3.3 V for LiFePO4
- **Temperature:** 15–20°C
- **Humidity:** <60%
- **Location:** Cool, dry, away from metal objects

**Storage Duration:**

**LiPo Batteries:**

| Voltage at Storage | Time to Self-Discharge to 3.0 V |
|--------------------|---------------------------------|
| 4.2 V (full)       | ~12–18 months                   |
| 3.8 V (50%)        | 18–24 months                    |
| 3.4 V (low)        | 6–12 months                     |

**LiFePO4 Batteries:**

| Voltage at Storage | Time to Self-Discharge to 2.5 V |
|--------------------|---------------------------------|
| 3.65 V (full)      | ~18–24 months                   |
| 3.2 V (50%)        | 24–36 months                    |
| 2.9 V (low)        | 12–18 months                    |

!!! warning "Check Stored Batteries"
    Batteries in storage should be checked every 3–6 months. 
    
    - **LiPo:** Recharge to 3.7–3.8 V if below 3.4 V
    - **LiFePO4:** Recharge to 3.2–3.3 V if below 2.9 V

### Disposal

**Never throw batteries in regular trash!**

**Proper Disposal:**

1. Discharge to <3.0 V (use in logger until depleted)
2. Tape terminals with electrical tape
3. Take to battery recycling center
4. Many retailers accept 18650 batteries for recycling
5. Check local hazardous waste facilities

---

## Troubleshooting Power Issues

### Logger Won't Power On

**Check:**

1. Battery voltage >3.3 V
2. Battery polarity correct
3. Battery contacts clean and making contact
4. No physical damage to logger
5. Try known-good battery

### Rapid Battery Drain

**Possible Causes:**

| Symptom                    | Likely Cause                                       | Solution                                          |
|----------------------------|----------------------------------------------------|---------------------------------------------------|
| New battery drains in days | Excessive telemetry or failure to enter sleep mode | Reduce telemetry frequency and re-upload firmware |
| Gradual worsening          | Battery aging                                      | Replace battery                                   |

### Voltage Reading Errors

**Inconsistent Voltage Readings:**

- Poor contact: Clean terminals
- Code issue: Re-upload firmware
- Battery dying: Replace

**No Voltage Logged:**

- Check data file format
- Verify voltage measurement in code
- Test ADC with multimeter

---

## Next Steps

-  [Internal Components](internal-components.md): Understanding your logger's hardware
-  [Maintenance Guide](maintenance.md): Regular upkeep procedures
-  [ThingsBoard](../../telemetry/thingsboard-configuration.md): Monitor battery voltage remotely
-  [Troubleshooting](../troubleshooting/common-issues.md): Power-related issues

---

!!! success "Power Management Mastered"
    With proper battery selection, monitoring and optimization, your logger deployments can achieve reliable operation for weeks to months on a single battery charge.
