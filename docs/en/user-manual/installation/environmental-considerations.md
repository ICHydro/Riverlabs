# Environmental Considerations

!!! abstract "Overview"
    Environmental factors significantly impact logger performance and longevity. This guide covers weather, climate, wildlife and site-specific considerations for successful long-term deployments.

## Climate and Weather

### Temperature Management

#### Operating Ranges

| Component               | Minimum | Maximum | Optimal Range  |
|-------------------------|---------|---------|----------------|
| **Electronics/CPU**     | -20°C   | +60°C   | 0°C to +50°C   |
| **Battery (Discharge)** | -20°C   | +60°C   | +10°C to +40°C |
| **Battery (Charging)**  | 0°C     | +45°C   | +10°C to +35°C |
| **SD Card**             | -25°C   | +85°C   | -10°C to +60°C |

#### Cold Weather Considerations

**Impact of Cold Temperatures:**

- **Battery capacity reduces** significantly
    - At 0°C: ~80% capacity
    - At -10°C: ~60% capacity
    - At -20°C: ~40% capacity
- **Condensation** risk when warming up

**Cold Weather Adaptations:**

❌ **Don't:**

- Attempt to charge batteries below 0°C
- Use solar charging in freezing conditions
- Expect normal battery life

**Impact of High Temperatures:**

- **Electronics heat stress** reduces lifespan
- **Battery degradation** accelerates
- **Solar gain** significantly raises internal temperature

**Hot Weather Adaptations:**

✅ **Do:**

- Install sun shields
- Reduce logging frequency to minimize active time
- Monitor battery health more frequently

❌ **Don't:**

- Mount in direct afternoon sun
- Use dark-colored enclosures
- Seal completely without ventilation plan
- Ignore battery swelling (danger sign)

**Sun Protection Methods:**

![Sun shield design](../../assets/images/sun-shield-diagram.png)<br>
*Simple sun shield reduces internal temperature by 15–20°C*

1. **Sun Shields:**
    - Aluminum or white plastic sheet
    - Mount 5–10 cm above logger
    - Allow air circulation underneath
    - Angle to block afternoon sun
2. **Reflective Coating:**
    - Paint enclosure white
    - Use reflective tape
    - Aluminum foil external wrap
3. **Shade Structures:**
    - Natural shade from trees/structures
    - Purpose-built canopy
    - Position on shaded side of structure

### Precipitation and Humidity

#### Rain and Water Ingress

1. **Cable Entries**
    - Check waterproof cable glands tight
    - Apply silicone sealant if mounting upwards to avoid pooling
    - Route cables downward (drip loop)
2. **Enclosure Seals**
    - Check gasket in good condition
    - Clean sealing surfaces before closing
    - Even pressure on all fasteners
3. **Sensor Housing**
    - No water pooling on sensor

**Water Pooling Prevention:**

![Drainage design](../../assets/images/drainage-design.png)

- Tilt enclosure 5–10° for runoff
- Cables exit from bottom
- No horizontal surfaces collecting water

#### Condensation

**Condensation Causes:**

- Temperature cycling (day/night)
- Humidity differential (inside/outside)
- Sealed enclosure trapping moisture
- "Breathing" of enclosure with pressure changes

**Anti-Condensation Measures:**

- **Desiccant Packs**
    - Resfresh silica gel packs inside enclosure
    - Replace every 3–6 months
    - Indicator type shows when saturated (typically yellow fresh green used)
    - Use food-grade desiccant (reusable)

## Biological Factors

### Insects and Spiders

**Common Problems:**

- Spider webs across sensor/lens
- Insects nesting in enclosure
- Webs triggering false readings
- Blockage of drainage holes

**Prevention:**

✅ **Physical Barriers:**

- Fine mesh over sensor (must not affect beam)
- Petroleum jelly around entry points
- Regular cleaning schedule

✅ **Chemical Deterrents:**

- Insect repellent spray around (not on) sensor
- Peppermint oil cotton balls in enclosure
- Commercial spider deterrent
- Re-apply monthly

**Lidar Specific:**

- Spiders particularly attracted to lens
- Weekly inspection in high-insect season
- Soft brush or compressed air for cleaning
- Never touch lens with fingers

### Flood Risk

**During Flooding:**

- Logger may be submerged
- Debris impact damage
- Siltation of sensor
- Power cable damage

**Flood Protection:**

1. **Mounting Height**
    - Above 100-year flood level if possible
    - Document flood history for area
    - Add 1 m safety margin
    - Mark maximum known flood on mounting
2. **Debris Protection**
    - Mount behind structure if possible
    - Debris deflector upstream
    - Protective cage around logger
    - Streamlined mounting (minimal snag points)
3. **Post-Flood Inspection**
    - Immediate visual check
    - Clean sensor thoroughly
    - Check for water ingress
    - Verify calibration

---

## Next Steps

- 🔋 [Battery & Power Guide](../hardware/battery-power-guide.md): Optimize for your climate
- 🔧 [Maintenance Guide](../hardware/maintenance.md): Regular checks for environmental factors
- 🚨 [Common Issues](../troubleshooting/common-issues.md): Environment-related problems
- 📞 Contact support: info@riverlabs.uk

---

!!! success "Environment-Ready Deployment"
    By considering and preparing for environmental factors, your logger deployment will be robust, reliable and long-lasting despite challenging conditions.
