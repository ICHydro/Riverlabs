# Telemetry Setup Guide

!!! abstract "Overview"
    This guide covers end-to-end setup of cellular telemetry using XBee modules for remote data transmission, from hardware installation to first successful transmission.

## Overview

Riverlabs loggers can be equipped with cellular telemetry to transmit data remotely, eliminating the need for frequent site visits and enabling real-time monitoring.

**What You'll Need:**

- Riverlabs logger with XBee socket (telemetry version)
- Digi XBee3 Cellular LTE-M/NB-IoT module
- Active SIM card with data plan
- USB adapter for XBee configuration
- XBee Studio software (replaces XCTU)
- ThingsBoard account or compatible IoT platform
- Computer for configuration

**Time Required:** 1–2 hours for first-time setup

---

## System Architecture

### How Telemetry Works

```
┌─────────────┐
│   Logger    │ → Collects sensor data
│   (Arduino) │    Writes to SD card
└──────┬──────┘
       │
       │ UART (Serial)
       ↓
┌─────────────┐
│ XBee Module │ → Cellular modem
│  (LTE-M)    │    Connects to network
└──────┬──────┘
       │
       │ Cellular Network
       ↓
┌─────────────┐
│  Internet   │
└──────┬──────┘
       │
       │ HTTP/CoAP
       ↓
┌─────────────┐
│ ThingsBoard │ → IoT Platform
│  or Server  │    Stores & displays data
└─────────────┘
```

### Data Flow

1. **Logger measures** sensor data at configured interval
2. **Arduino processes** data and stores to SD card
3. **Arduino sends** data to XBee via serial
4. **XBee connects** to cellular network (LTE-M or NB-IoT)
5. **XBee transmits** data to configured server
6. **Server receives** and stores data
7. **Dashboard displays** data to user

---

## XBee Module Overview

### XBee3 Cellular LTE-M/NB-IoT

**Specifications:**

| Feature          | Specification                 |
|------------------|-------------------------------|
| **Technology**   | LTE-M (Cat-M1) and NB-IoT     |
| **Frequencies**  | Region-specific (check model) |
| **Power**        | 3.3 V–4.3 V operation         |
| **Current Draw** | ~200 mA during transmission   |
| **Form Factor**  | Through-hole or surface mount |
| **Protocols**    | HTTP, HTTPS, CoAP, MQTT       |
| **SIM**          | Micro-SIM slot                |

**Model Numbers:**

- **XB3-C-A2-UT-001**: LTE-M (US/Canada)
- **XB3-C-A2-EU-001**: LTE-M/NB-IoT (Europe)
- **Check Digi website** for your region

### XBee vs. Legacy XCTU

!!! info "XBee Studio Replaces XCTU"
    Digi has replaced XCTU with **XBee Studio** for XBee3 modules. If you have older documentation referencing XCTU, the concepts are the same but the interface is updated.
    
    Download: [https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xbee-studio](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xbee-studio)

---

## Step 1: SIM Card Setup

### Choosing a SIM Card Provider

**Requirements:**

- LTE-M or NB-IoT support (not all carriers support these)
- IoT/M2M data plan
- Good coverage at deployment location
- Reasonable data rates

**Recommended Providers (by region):**

**United States:**

- AT&T IoT DataConnect
- Verizon ThingSpace
- T-Mobile IoT
- Hologram (multi-carrier)
- Soracom (multi-carrier)

**Europe:**

- Vodafone M2M
- Telefonica M2M
- 1NCE (multi-carrier, IoT-specific)
- Eseye (multi-carrier)

**United Kingdom:**

- EE M2M
- Vodafone IoT
- O2 M2M

**Global/Multi-Region:**

- Hologram
- Twilio Super SIM
- 1NCE

### Data Plan Sizing

**Estimate Data Usage:**

**Typical Data per Transmission:**

- Measurement data: ~50–100 bytes
- HTTP overhead: ~200–500 bytes
- Total per transmission: ~300–600 bytes
- Safety factor: Use 1 KB per transmission

**Monthly Data Calculation:**

```
Example: 15-minute logging, hourly transmission
- Measurements per day: 96 (every 15 min)
- Transmissions per day: 24 (hourly)
- Data per day: 24 × 1 KB = 24 KB
- Data per month: 24 KB × 30 = 720 KB ≈ 1 MB

Recommended plan: 5-10 MB/month
```

**Typical Plans:**

- 1 MB/month: ~$2–5/month
- 10 MB/month: ~$5–10/month
- 100 MB/month: ~$10–15/month

!!! tip "Start Small"
    Begin with the smallest plan. You can always upgrade. Most IoT plans don't have overage charges — they just stop working.

### SIM Card Installation

**SIM Type:**

- Micro-SIM (3FF)

**Installation Steps:**

1. **Obtain activated SIM** from provider

2. **Note APN settings** (provided by carrier)
    - APN name (e.g., `m2m.com.attwireless.com`)
    - Username (if required)
    - Password (if required)

3. **Locate SIM slot** on XBee module
    - Usually on underside
    - Spring-loaded tray

4. **Insert SIM**
    - Correct orientation (notched corner)
    - Push until clicks
    - Ensure fully seated

5. **Record ICCID** (SIM card number for tracking)

---

## Step 2: XBee Configuration

### Hardware Setup for Configuration

**Required:**

- XBee module with SIM installed
- XBee USB adapter (e.g., Digi XBee Grove Development Board or similar)
- USB cable
- Computer with XBee Studio installed

**Connection:**

1. Insert XBee into USB adapter
2. Connect adapter to computer via USB
3. Wait for drivers to install (Windows)
4. Note COM port or device (check Device Manager/System Preferences)

### XBee Studio Configuration

#### Initial Connection

1. **Launch XBee Studio**
2. **Click "Add Device"**
3. **Select USB connection**
4. **Choose correct COM port**
5. **Click "Add Device"**
6. **XBee Studio reads module configuration**

#### Essential Settings

Navigate through tabs and configure the following:

**1. Network Settings**

| Setting                | Parameter              | Value              | Notes                           |
|------------------------|------------------------|--------------------|---------------------------------|
| **Mobile Network**     | AN (Access Point Name) | [Your carrier APN] | e.g., `m2m.com.attwireless.com` |
| **APN Username**       | AU                     | [If required]      | Leave blank if not needed       |
| **APN Password**       | AV                     | [If required]      | Leave blank if not needed       |
| **Network Technology** | NT                     | LTE-M (or NB-IoT)  | Match your SIM                  |

**2. Serial Settings**

| Setting       | Parameter | Value     | Notes                   |
|---------------|-----------|-----------|-------------------------|
| **Baud Rate** | BD        | 9600      | Must match Arduino code |
| **Parity**    | NB        | No parity | Standard                |
| **Stop Bits** | SB        | 1         | Standard                |
| **Data Bits** | --        | 8         | Default                 |

**3. API Mode**

| Setting             | Parameter | Value | Notes                 |
|---------------------|-----------|-------|-----------------------|
| **API Enable**      | AP        | 2     | API Mode with escapes |
| **API Output Mode** | AO        | 0     | Standard              |

!!! danger "API Mode is Critical"
    The logger firmware expects **API Mode 2 (with escapes)**. Setting AP=1 (without escapes) or AP=0 (transparent) will prevent communication.

**4. Power Management**

| Setting           | Parameter | Value | Notes     |
|-------------------|-----------|-------|-----------|
| **Sleep Mode**    | SM        | 1     | Pin Sleep |
| **Sleep Options** | SO        | 0     | Standard  |

**5. Additional Settings**

| Setting     | Parameter | Value | Notes                  |
|-------------|-----------|-------|------------------------|
| **Timeout** | RO        | 3     | Character timeout      |
| **TD**      | TD        | 0     | TX Delay               |
| **FT**      | FT        | 681   | Flow control threshold |

#### Verify Configuration

1. **Click "Read" to refresh current settings**
2. **Verify all parameters match above**
3. **Click "Write" to save configuration to module**
4. **Wait for confirmation**
5. **Click "Read" again to verify settings saved**

### Network Connection Test

**Test Cellular Connection:**

1. In XBee Studio, navigate to **"Cellular Network"** section
2. Click **"Connect to Network"**
3. Wait for connection (may take 30–60 seconds)
4. Check for:
    - Network registered: Yes
    - Signal strength: >-100 dBm (stronger is better)
    - IP address assigned

**Troubleshooting Connection Issues:**

| Problem               | Likely Cause                | Solution                       |
|-----------------------|-----------------------------|--------------------------------|
| No network found      | SIM inactive or wrong APN   | Verify with carrier, check APN |
| Poor signal           | Location                    | Move to window/higher location |
| Connection timeout    | Network technology mismatch | Switch LTE-M ↔ NB-IoT          |
| Authentication failed | Wrong APN credentials       | Verify username/password       |

---

## Step 3: Arduino Code Configuration

### Code Parameters

The telemetry configuration is set in the Arduino sketch (e.g., `wari_3G.ino`).

**Key Definitions:**

```cpp
// Telemetry Configuration
#define TELEMETRY_INTERVAL 12 // Transmit every N measurements
#define SERVER_URL "http://demo.thingsboard.io/api/v1/[YOUR_TOKEN]/telemetry"
// XBee Settings (must match XBee configuration)
#define XBEE_BAUD 9600
```

**Adjust for Your Setup:**

1. **TELEMETRY_INTERVAL**
    - How often to transmit
    - Example: If logging every 15 min, interval 4 = transmit hourly
    - Higher interval = better battery life

2. **SERVER_URL**
    - Your ThingsBoard or server endpoint
    - Include authentication token in URL
    - See [ThingsBoard Configuration](thingsboard-configuration.md) for details

3. **XBEE_BAUD**
    - Must match XBee BD setting
    - Default: 9600

### Upload Code

1. **Connect logger** via FTDI cable
2. **Open Arduino IDE**
3. **Load appropriate sketch**
    - `wari_3G.ino` for Wari with telemetry
    - `wari_lidar_cellular.ino` for lidar with telemetry
4. **Verify settings** above are correct
5. **Compile** (check for errors)
6. **Upload** to logger
7. **Open Serial Monitor** (115200 baud if DEBUG enabled)
8. **Verify initialization messages**

---

## Step 4: Integration Testing

### Bench Test Before Deployment

**Setup:**

1. Logger with XBee installed (SIM card in place)
2. Power on with charged battery
3. Serial monitor connected (optional but recommended)
4. Near window for good cellular signal

**Test Procedure:**

1. **Power On**
    - Logger boots
    - Initializes XBee
    - XBee connects to network (watch LED)

2. **Wait for Measurement**
    - Logger takes measurement(s)
    - Stores to SD card
    - Prepares telemetry packet

3. **Observe Transmission**
    - XBee LED activity (if visible)
    - Serial debug messages (if enabled):
        ```
        Connecting to network...
        Network connected
        Sending data...
        HTTP POST: 200 OK
        Transmission successful
        ```

4. **Verify Reception**
    - Check ThingsBoard dashboard
    - Data should appear within 1–2 minutes
    - Verify timestamp and values correct

**Expected Timing:**

- Boot: 5–10 seconds
- Network connection: 30–60 seconds (first time), 5–15 seconds (subsequent)
- Data transmission: 5–10 seconds
- Total: ~1 minute per transmission

### Troubleshooting Test Failures

**No network connection:**

- Verify SIM active
- Check signal strength (try window/outside)
- Verify APN settings
- Check XBee LED patterns

**Network connects but no data received:**

- Verify server URL correct
- Check authentication token
- Inspect Serial Monitor for error codes
- Test server endpoint with curl/Postman

**Data received but values wrong:**

- Check data parsing on server
- Verify JSON format
- Compare with SD card data

---

## Step 5: Field Deployment

### Pre-Deployment Checklist

- [ ] Bench test successful (3+ transmissions)
- [ ] Server receiving and displaying data correctly
- [ ] Battery fully charged
- [ ] Signal strength acceptable at deployment site (test with phone)
- [ ] Telemetry interval appropriate for battery life
- [ ] SD card backup functioning (in case telemetry fails)
- [ ] Antenna secure and properly oriented

### Site Signal Testing

Before permanent installation:

1. **Bring phone on same carrier** as SIM
2. **Check signal strength** at installation location
3. **Consider antenna position:**
    - Vertical orientation often best
    - Higher = better signal
    - Away from metal obstructions
    - Clear view of sky preferred

**Minimum Signal Requirements:**

- LTE-M/NB-IoT: -110 dBm minimum, -90 dBm good
- Weaker signals work but drain battery faster

### Post-Installation Verification

**Within 1 Hour:**

- [ ] First transmission received
- [ ] Data appears on dashboard
- [ ] Values reasonable

**Within 24 Hours:**

- [ ] Regular transmissions occurring
- [ ] No gaps in data
- [ ] Battery voltage stable
- [ ] Signal strength consistent

**First Week:**

- [ ] Daily data check
- [ ] Monitor battery drain rate
- [ ] Watch for transmission failures
- [ ] Adjust interval if needed

---

## Power Considerations

### Telemetry Power Budget

**XBee Power Consumption:**

- Sleep: <1 mA
- Connected idle: ~10 mA
- Transmission: ~200 mA (5–10 seconds)

**Impact on Battery Life:**

Without telemetry (SD only):

- Wari: ~30–60 days (15 min logging)
- Lidar: ~15–30 days (15 min logging)

With telemetry (hourly transmission):

- Wari: ~20–40 days (15 min logging)
- Lidar: ~10–20 days (15 min logging)

**Optimization Strategies:**

1. **Reduce transmission frequency**
    - Measure every 15 min, transmit hourly
    - 4× battery life improvement vs. transmit-every-measurement

2. **Scheduled transmissions**
    - Transmit once or twice daily
    - Massive power savings
    - SD card holds full resolution data

3. **Adaptive transmission**
    - Transmit frequently during events
    - Reduce during stable conditions
    - Requires more complex code

See [Battery & Power Guide](../operation/hardware/battery-power-guide.md) for detailed calculations.

---

## Security Considerations

### Data Security

**Transport Security:**

- Use HTTPS instead of HTTP when possible
- ThingsBoard supports TLS
- Some overhead in data and processing

**Authentication:**

- Device tokens (ThingsBoard)
- API keys
- Never hard-code credentials if code is public

**SIM Security:**

- Use fixed IP APN if available
- SIM PIN protection
- Monitor for unusual data usage

### Physical Security

- XBee and antenna are valuable targets for theft
- Consider concealment or protective enclosures
- Document serial numbers for tracking

---

## Monitoring and Maintenance

### What to Monitor

**Daily (automated alerts):**

- [ ] Data received in last 24 hours
- [ ] Battery voltage trend
- [ ] Signal strength

**Weekly:**

- [ ] Transmission success rate
- [ ] Data quality
- [ ] SIM data usage

**Monthly:**

- [ ] Battery health
- [ ] Plan data remaining
- [ ] Firmware updates available

### Common Telemetry Issues

See [Troubleshooting Connections](troubleshooting-connections.md) for detailed diagnostics.

**Quick Reference:**

| Symptom            | Quick Check                | Likely Fix                        |
|--------------------|----------------------------|-----------------------------------|
| No transmissions   | SIM active? Signal?        | Check carrier status, move logger |
| Intermittent       | Signal strength varying?   | Improve antenna position          |
| High battery drain | Transmission too frequent? | Reduce frequency                  |
| Data corrupted     | Server parsing issue?      | Check JSON format                 |

---

## Alternative Platforms

While this guide focuses on ThingsBoard, XBee3 modules support various platforms:

- **MQTT brokers** (Mosquitto, HiveMQ, AWS IoT)
- **HTTP REST APIs** (custom servers)
- **CoAP servers**
- **Azure IoT Hub**
- **AWS IoT Core**
- **Google Cloud IoT**

Configuration principles are similar; consult platform documentation for endpoints and authentication.

---

## Cost Summary

**One-Time Costs:**

- XBee3 Cellular module: $60–80
- USB adapter for configuration: $20–30
- SIM card activation: $0–20

**Recurring Costs:**

- Data plan: $2–15/month
- Server hosting (if not ThingsBoard free tier): $0–10/month

**Total First Year:**

- Hardware: ~$100
- Service: ~$50–180
- **Total: ~$150–280**

Compare to:

- Site visit costs (fuel, time)
- Data gaps from infrequent visits
- Real-time monitoring value

---

## Next Steps

- [ThingsBoard Configuration](thingsboard-configuration.md): Set up your dashboard
- [XBee Setup Walkthrough](xbee-setup-walkthrough.md): Detailed XBee Studio guide with screenshots
- [Troubleshooting Connections](troubleshooting-connections.md): Fix telemetry issues

---

!!! success "Telemetry Configured!"
    With cellular telemetry working, you now have real-time remote monitoring of your logger. No more site visits just to check if it's working!
