# Sensor Positioning Guide

!!! abstract "Overview"
    Proper sensor positioning is critical for accurate measurements. This guide provides detailed information on optimal sensor placement, angle considerations, and target surface requirements.

## Understanding Sensor Types

### Wari (Ultrasound) Beam Characteristics

The ultrasound sensor emits a conical beam that spreads as distance increases.

**Beam Properties:**

- **Frequency:** 42 kHz ultrasonic pulses
- **Beam Angle:** Approximately 15° cone
- **Footprint Growth:** ~26cm diameter per meter of distance
- **Reflection:** Requires reflective surface within beam cone

**Distance vs. Footprint:**

| Distance | Beam Diameter | Area Coverage |
|----------|---------------|---------------|
| 0.5m | ~13cm | 0.013 m² |
| 1.0m | ~26cm | 0.053 m² |
| 2.0m | ~52cm | 0.212 m² |
| 3.0m | ~80cm | 0.502 m² |
| 5.0m | ~130cm | 1.327 m² |

![Ultrasound beam pattern](../../images/ultrasound-beam-pattern.png)
*Ultrasound beam spreads in a cone pattern*

### Lidar Beam Characteristics

The lidar sensor emits a highly focused laser beam with minimal divergence.

**Beam Properties:**

- **Wavelength:** 905nm near-infrared
- **Beam Divergence:** 8 milliradians (~0.5°)
- **Spot Size:** Minimal growth with distance
- **Reflection:** Requires diffuse reflection from target

**Distance vs. Spot Size:**

| Distance | Spot Diameter | Area Coverage |
|----------|---------------|---------------|
| 1m | ~0.8cm | 0.0005 m² |
| 5m | ~4cm | 0.0013 m² |
| 10m | ~8cm | 0.005 m² |
| 20m | ~16cm | 0.020 m² |
| 35m | ~28cm | 0.062 m² |

![Lidar beam pattern](../../images/lidar-beam-pattern.png)
*Lidar beam maintains tight focus over distance*

---

## Vertical Mounting (Standard Configuration)


**Using a Level:**
1. Attach level to logger housing
2. Adjust until bubble centered
3. Verify from multiple angles
4. Secure mounting hardware
5. Re-check after tightening

**Using Plumb Bob:**
1. Hang plumb line from mounting point
2. Align sensor axis with string
3. Verify alignment from 90° perspective
4. Tighten while maintaining position

!!! tip "Verification Method"
    After mounting, take measurements at known distance and compare with manual measurement. Difference >2% suggests misalignment.

---

## Angled Mounting (Lidar Only)

### When to Use Angled Mounting

Consider angled mounting when:

- Vertical mounting is physically impossible
- Bridge/structure geometry requires it
- Access/safety concerns prevent vertical installation

### Lidar: Angled Mounting Capability

!!! success "Lidar Supports Angled Mounting"
    Lidar can measure accurately up to 40° from vertical with minimal correction needed.

**Angle Impact on Accuracy:**

| Angle from Vertical | Accuracy Impact | Notes |
|---------------------|-----------------|-------|
| 0° - 10° | <0.5% error | Negligible correction |
| 10° - 20° | <1% error | Recommended maximum |
| 20° - 30° | <1.5% error | Acceptable with correction |
| 30° - 40° | <2.5% error | Maximum recommended |
| >40° | >3% error | Not recommended |

**Installation Steps:**

1. **Measure and record angle** using protractor or inclinometer
2. **Mark angle on logger** for future reference
3. **Apply angle correction** in data processing:
   ```
   Vertical Distance = Measured Distance × cos(angle)
   ```
4. **Document thoroughly** in installation records

**Example Calculation:**

```
Sensor at 25° angle measures 10.5m
Vertical distance = 10.5m × cos(25°)
Vertical distance = 10.5m × 0.906
Vertical distance = 9.51m
```

**Angle Correction Table:**

| Angle | Cosine | Multiply Measured Distance By |
|-------|--------|------------------------------|
| 5° | 0.996 | 0.996 |
| 10° | 0.985 | 0.985 |
| 15° | 0.966 | 0.966 |
| 20° | 0.940 | 0.940 |
| 25° | 0.906 | 0.906 |
| 30° | 0.866 | 0.866 |
| 35° | 0.819 | 0.819 |
| 40° | 0.766 | 0.766 |



## Target Surface Considerations

### Water Surfaces

The condition of the water surface significantly affects measurement reliability.

#### Ultrasound on Water

**Excellent Conditions:**
- Calm water with light ripples
- Slightly turbulent water
- Sediment-laden water
- Water with foam/debris

**Poor Conditions:**
- Perfectly smooth glass-like surface
- Heavy turbulence/white water
- Water surface at angle to beam
- Surface covered by floating debris

**Optimization Tips:**
- Take multiple readings and average
- Increase number of readings in turbulent conditions
- Avoid mounting where wind creates consistent wave patterns
- Consider measurement timing (calm periods)

#### Lidar on Water

**Excellent Conditions:**
- Slightly rough water surface
- Turbid/sediment-laden water
- Water with suspended particles
- Foam or debris on surface

**Poor Conditions:**
- Perfectly clear, smooth water (mirror-like)
- Very clean water in calm conditions
- Highly transparent water
- Dark water with no suspended material

**Why Smooth/Clear Water is Difficult for Lidar:**
- Specular reflection (mirror-like) reflects beam away
- Transparent water allows beam to penetrate surface
- Requires diffuse reflection for reliable measurement
- Natural water usually has sufficient turbidity

**Optimization:**
- Most natural rivers/streams work well
- Laboratory/clean water may be problematic
- Test at site before permanent installation
- Consider ultrasound for very clean water applications

### Non-Water Surfaces

Both sensors can measure non-water targets:

**Good Targets:**
- Concrete (rough finish)
- Wood
- Soil/ground
- Granular materials
- Textured surfaces

**Challenging Targets:**
- Smooth metal (specular reflection)
- Glass or mirrors
- Highly polished surfaces
- Transparent materials
- Very dark, non-reflective materials

---

## Next Steps

- 📐 [Mounting Guide](mounting-guide.md) - Install your optimized sensor position
- 🌡️ [Environmental Considerations](environmental-considerations.md) - Site-specific factors
- 🔋 [Battery & Power Guide](../hardware/battery-power-guide.md) - Power planning
- 🔧 [Maintenance](../hardware/maintenance.md) - Keep sensor positioned correctly

---

!!! success "Optimal Positioning Achieved"
    With careful attention to positioning, clearances, and target considerations, your logger will provide accurate, reliable measurements for years of operation.
