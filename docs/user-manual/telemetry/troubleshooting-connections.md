# Troubleshooting Telemetry Connections

!!! abstract "Overview"
    Diagnostic guide for resolving cellular telemetry issues. From no connection to intermittent failures, with systematic troubleshooting approaches.

## Diagnostic Overview

### Troubleshooting Philosophy

When telemetry fails, problems can occur at multiple layers:

```
┌──────────────────┐
│  ThingsBoard     │ ← Dashboard/Server issues
├──────────────────┤
│  Internet        │ ← Routing, DNS, firewalls
├──────────────────┤
│  Cellular Network│ ← Signal, APN, registration
├──────────────────┤
│  XBee Module     │ ← Configuration, firmware
├──────────────────┤
│  Logger/Arduino  │ ← Code, serial communication
├──────────────────┤
│  Power/Hardware  │ ← Battery, connections
└──────────────────┘
```

**Systematic approach:**
1. Identify symptom
2. Isolate layer
3. Test specific component
4. Apply fix
5. Verify resolution

---

## Quick Diagnostic Checklist

**Start here for fast triage:**

- **Power:** Battery >3.3V?
- **Hardware:** XBee seated properly?
- **SIM:** Active and inserted correctly?
- **Signal:** RSSI better than -105 dBm?
- **Configuration:** API Mode = 2?
- **Network:** APN correct?
- **Server:** ThingsBoard device active?
- **Code:** Correct access token?

**If all checked and still failing:** Continue to detailed diagnostics below.

---

## Common Symptoms and Solutions

### Symptom 1: Logger Never Transmits

**Observations:**
- No network connection
- XBee LED never blinks (if visible)
- Serial Monitor: "Network connection failed"

#### Layer 1: Hardware Check

**1. Battery Voltage**

Low voltage causes XBee failures:

| Voltage | Status | Action |
|---------|--------|--------|
| >3.8V | Good | Continue diagnostics |
| 3.5-3.8V | Marginal | Charge/replace, retest |
| <3.5V | Too low | Replace battery |

**How to check:**
- Measure with multimeter
- Or: Serial Monitor shows voltage
- Or: ThingsBoard (if SD data uploaded manually)

**2. XBee Physical Installation**

- Remove and inspect:
  - Pins straight?
  - All pins engaging PCB socket?
  - No bent/missing pins?
- Re-seat firmly
- Visual inspection of solder joints (if DIY assembly)

**3. SIM Card**

- Remove SIM
- Inspect for damage
- Ensure correct orientation
- Re-insert until clicks
- Verify SIM activated (call/text carrier)

#### Layer 2: Cellular Network

**1. Signal Strength**

**Test location:**
- Move logger to window
- Elevate if possible
- Away from metal structures
- Test with phone (same carrier) at same location

**Expected phone signal:**
- 2+ bars: Should work
- 1 bar: Marginal
- 0 bars: Won't work

**2. Network Technology Mismatch**

Some carriers only support LTE-M OR NB-IoT:

**Test both:**

1. Connect XBee to XBee Studio
2. Read configuration
3. Current NT value?
4. Try opposite:
 - If NT=0 (LTE-M): Change to NT=1 (NB-IoT)
 - If NT=1 (NB-IoT): Change to NT=0 (LTE-M)
5. Write configuration
6. Retest connection

**3. APN Configuration**

**Verify APN:**

1. Contact SIM carrier
2. Confirm APN name (case-sensitive!)
3. Confirm username/password (usually blank)
4. Reconfigure XBee if needed

**Common mistakes:**
- Typo in APN
- Extra spaces
- Wrong case (some APNs are case-sensitive)

**Common APNs:**

| Carrier | Correct APN | Common Mistakes |
|---------|-------------|-----------------|
| AT&T | `m2m.com.attwireless.com` | `attwireless.com` (wrong) |
| T-Mobile | `iot.t-mobile.com` | `epc.tmobile.com` (consumer, not IoT) |
| Verizon | `vzwinternet` | `vzwm2m` (old) |
| Hologram | `hologram` | `hologram.io` (wrong) |

#### Layer 3: XBee Configuration

**Critical settings:**

1. Connect to XBee Studio
2. Read configuration
3. Verify:

| Parameter | Required Value | If Wrong |
|-----------|---------------|----------|
| AP | 2 | **CRITICAL** - Must be 2! |
| BD | 9600 | Must match Arduino |
| AN | [Your APN] | Check carrier |
| SM | 1 | For power saving |

**If any wrong:**
- Correct in XBee Studio
- **Click Write**
- Click Read to verify
- Power cycle XBee
- Retest

#### Layer 4: Arduino Code

**1. XBee Initialization**

**Serial Monitor output** (if DEBUG enabled):

```cpp
Initializing XBee...
XBee ready           // ← Should see this
```

**If "XBee timeout" or similar:**
- Check serial connection (TX/RX pins)
- Verify baud rate match (9600)
- Check XBee physically present

**2. Network Connection Logic**

**Expected debug messages:**
```cpp
Connecting to network...
Network status: 0x00
Waiting for network...
Network registered!
IP address obtained
```

**If stuck at "Waiting for network":**
- Network issue (see Layer 2)
- XBee configuration (see Layer 3)

---

### Symptom 2: Intermittent Transmissions

**Observations:**
- Sometimes works, sometimes fails
- "Success rate: 60-80%"
- Random gaps in ThingsBoard data

#### Cause 1: Signal Strength Fluctuations

**Diagnosis:**

- Check RSSI during failures vs. successes
- Pattern: Failures during specific times? (e.g., evenings = congestion)

**Solutions:**

1. **Improve antenna position:**
    - Higher elevation
    - Away from obstacles
    - External antenna if using internal

2. **Add retry logic** (code modification):
   ```cpp
   int retries = 3;
   bool success = false;
   
   while(retries > 0 && !success) {
     success = sendData();
     if(!success) {
       delay(30000); // Wait 30 sec
       retries--;
     }
   }
   ```

3. **Increase timeout values** in XBee communication

#### Cause 2: Network Congestion

**LTE-M/NB-IoT are shared resources:**

- Peak hours: More congestion
- Events: Temporary towers overload

**Solutions:**

1. **Stagger transmission times** across multiple loggers
2. **Avoid peak hours** if possible (schedule for 2-5 AM)
3. **Increase transmission interval** (reduce frequency)

#### Cause 3: Power Issues

**Brown-out during transmission:**

XBee draws ~200 mA during transmit. If battery weak:

**Symptoms:**
- Transmissions fail as battery drains
- Success rate correlates with battery voltage

**Solution:**

1. Replace battery sooner (don't wait until 3.3V)
2. Use higher capacity battery
3. Reduce transmission frequency

**Diagnostic code** (check voltage before transmit):
```cpp
float voltage = readBatteryVoltage();
if(voltage < 3.5) {
  // Skip telemetry, too low
  Serial.println("Battery too low for telemetry");
  return;
}
// Proceed with transmission
```

#### Cause 4: Server Timeouts

**ThingsBoard or server issues:**

**Test server availability:**

```bash
# From computer
curl -X POST \
  http://demo.thingsboard.io/api/v1/YOUR_TOKEN/telemetry \
  -H 'Content-Type: application/json' \
  -d '{"test":123}'
```

**Expected:** `{}` (empty JSON = success)

**If timeout or error:**
- ThingsBoard server down (check status page)
- Network routing issue
- Firewall blocking

**Solution:**
- Wait and retry
- Use alternative server (Cloud vs. Demo)

---

### Symptom 3: Data Appears but Values Wrong

**Observations:**
- ThingsBoard shows data
- But values incorrect, null, or garbled

#### Cause 1: JSON Format Error

**Logger sends:**
```json
{"distance_mm":1234,"battery_V":3.85}
```

**ThingsBoard expects keys to match:**
- Case-sensitive: `distance_mm` ≠ `Distance_MM`
- Spelling matters

**Diagnosis:**

1. Check Arduino code:
   ```cpp
   String json = "{\"distance_mm\":";
   json += distance;
   json += ",\"battery_V\":";
   json += voltage;
   json += "}";
   ```

2. Verify key names in ThingsBoard widget configuration

**Solution:**
- Fix key names in code or dashboard
- Consistent naming

#### Cause 2: Data Parsing on Server

**ThingsBoard interprets data types:**

**Example issue:**
- Sent as string: `"distance_mm":"1234"`
- Expected integer: `"distance_mm":1234`

**Solution:**
- Remove quotes around numeric values in code
- Ensure `String(value)` doesn't add quotes

#### Cause 3: Timestamp Issues

**If timestamps wrong:**

**Logger not setting timestamp:**
- ThingsBoard uses server time (usually correct)

**Logger sending timestamp:**
```json
{"distance_mm":1234,"ts":1703688000000}
```

**Issue:** Wrong epoch format
- ThingsBoard expects milliseconds
- Logger sending seconds

**Solution:**
```cpp
long timestamp = rtc.now().unixtime() * 1000L; // Convert to milliseconds
```

---

### Symptom 4: High Data Costs

**Observations:**
- SIM data usage higher than expected
- Plan exceeds quota

#### Cause 1: Transmission Frequency Too High

**Calculate expected usage:**

- Transmission size: ~500 bytes (0.5 KB)
- Transmissions per day: 24 (hourly)
- Daily usage: 24 × 0.5 = 12 KB
- Monthly: 12 KB × 30 = 360 KB

**If actual usage >> expected:**
- Check transmission interval in code
- Verify not transmitting on every measurement

**Solution:**
- Increase `TELEMETRY_INTERVAL` in code
- Balance: Data freshness vs. costs

#### Cause 2: Failed Transmissions Retrying

**Each retry = more data:**

- Failed transmission: Still uses data (overhead)
- Retry: Double data
- Multiple retries: Multiplies costs

**Solution:**
1. Fix root cause of failures (signal, config)
2. Limit retries in code (max 3)
3. Add exponential backoff

#### Cause 3: Unnecessary Data in Payload

**Minimize JSON size:**

**Verbose (bad):**
```json
{
  "sensor_distance_millimeters": 1234,
  "battery_voltage_volts": 3.85,
  "temperature_celsius": 22.5,
  "signal_strength_dbm": -85,
  "logger_id": "Wari-001",
  "firmware_version": "1.2.3"
}
```
Size: ~150 bytes

**Compact (good):**
```json
{"d":1234,"b":3.85,"t":22.5}
```
Size: ~30 bytes

**5× data savings!**

**Balance:** Compact keys vs. readability

---

## Advanced Diagnostics

### Using Serial Monitor for Debugging

**Enable debug output in Arduino code:**

```cpp
#define DEBUG 1

#if DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif
```

**Expected output:**
```
[INIT] Starting Riverlabs Logger v2.1
[INIT] RTC initialized
[INIT] SD card OK
[INIT] XBee initializing...
[XBEE] Baud: 9600, API Mode
[XBEE] Waiting for network...
[XBEE] Network registered (30s)
[XBEE] RSSI: -85 dBm
[XBEE] IP: 10.123.45.67
[SENSOR] Distance: 1234 mm, Temp: 22.5 C
[DATA] JSON: {"distance_mm":1234,"battery_V":3.85}
[XBEE] Sending to ThingsBoard...
[XBEE] HTTP POST: 200 OK
[XBEE] Transmission successful (8.5s)
[POWER] Entering sleep mode
```

**Key indicators:**

| Message | Meaning | If Missing |
|---------|---------|------------|
| `XBee initializing...` | Serial communication working | Check connections |
| `Network registered` | Cellular connected | Check SIM, APN, signal |
| `RSSI: -XX dBm` | Signal strength known | XBee communication issue |
| `HTTP POST: 200 OK` | Server received data | Check URL, token |

### XBee Studio Network Diagnostics

**1. Connect XBee to XBee Studio**

**2. Navigate to "Cellular Network" tab**

**3. View current status:**

- **Registration Status:**
  - Not registered: No network
  - Registered (home): Good
  - Registered (roaming): May work (check roaming on SIM)
  - Searching: Still looking

- **Signal Strength:**
  - Numeric RSSI value
  - Quality bars (0-5)

- **IP Address:**
  - If assigned: Network is connected
  - If blank: DHCP failed or not registered

- **Data Usage:**
  - Some modules track usage
  - Compare to SIM plan

**4. Connection Test:**

- Click "Ping" or "Test Connection"
- Pings Digi server
- Confirms end-to-end connectivity

**5. Diagnostic Commands (Advanced):**

XBee Studio terminal:
```
AI      // Association indication (0 = connected)
DB      // Signal strength
CI      // Protocol status
MY      // IP address
```

### Packet Capture

**For advanced debugging:**

**1. Install Wireshark** on computer

**2. Capture HTTP traffic:**

- Use XBee connected to computer
- Run logger code
- Wireshark captures outbound HTTP

**3. Inspect packets:**

- POST request details
- Headers
- Payload
- Server response

**Useful for:**
- Verifying JSON format
- Checking authentication headers
- Seeing actual errors from server

---

## Error Code Reference

### XBee Error Codes

**AI (Association Indication) parameter:**

| Code | Meaning | Action |
|------|---------|--------|
| 0x00 | Connected | Good! |
| 0x22 | Registering to network | Wait |
| 0x23 | Connecting to internet | Wait |
| 0x24 | Bypass mode active | Check configuration |
| 0x25 | Initializing | Wait or power cycle |
| 0x2A | Airplane mode | Disable airplane mode |
| 0x2F | Switched off | Check SM setting |

### HTTP Response Codes

**From ThingsBoard or server:**

| Code | Meaning | Solution |
|------|---------|----------|
| 200 | Success | No action needed |
| 400 | Bad Request | Check JSON format |
| 401 | Unauthorized | Verify access token |
| 404 | Not Found | Check server URL |
| 429 | Too Many Requests | Reduce frequency |
| 500 | Server Error | ThingsBoard issue, wait |
| 503 | Service Unavailable | Server down, wait |

### Arduino XBee Library Errors

**If using XBee Arduino library:**

| Error | Meaning | Solution |
|-------|---------|----------|
| XBEE_TIMEOUT | No response | Check connections, baud rate |
| XBEE_INVALID_RESPONSE | Corrupt data | Check API mode (must be 2) |
| XBEE_TX_FAILURE | Transmission failed | Network issue |

---

## Preventive Maintenance

### Regular Checks

**Weekly:**
- Check battery voltage trend
- Verify transmission success rate >90%
- Review signal strength (should be consistent)

**Monthly:**
- Check SIM data usage vs. plan
- Inspect XBee physical condition (corrosion, etc.)
- Verify firmware up to date

**Quarterly:**
- Test logger retrieval and reconfiguration
- Clean antenna connections
- Review and update server endpoints if needed

### Optimization

**Maximize reliability:**

1. **Signal strength:**
 - Use external antenna in marginal locations
 - Elevate logger if possible

2. **Power:**
 - Size battery for 2× expected transmissions
 - Monitor voltage, replace proactively

3. **Transmission interval:**
 - Balance freshness vs. battery life
 - More frequent during events, less during stable conditions

4. **Retry logic:**
 - Max 3 retries
 - Exponential backoff (5s, 30s, 60s)
 - Log failures to SD card

5. **Monitoring:**
 - Set up ThingsBoard alerts
 - Email on: No data for 2× expected interval
 - Email on: Battery < 3.5V

---

## Escalation Path

### When to Contact Support

**If after all diagnostics:**
- Still no connection
- Consistent failures
- Unexplained behavior

**Gather information before contacting:**

1. **Logger details:**
 - Model (Wari, Lidar)
 - Firmware version
 - Logger ID

2. **XBee details:**
 - Model (XB3-C-A2-XX-001)
 - Firmware version
 - Configuration (export profile)

3. **SIM/Carrier:**
 - Carrier name
 - Plan details
 - APN

4. **Symptoms:**
 - Specific error messages
 - Serial Monitor logs
 - ThingsBoard screenshots

5. **Diagnostics performed:**
 - Tests run
 - Results

### Support Resources

**Riverlabs/ICHydro:**
- GitHub Issues: https://github.com/ICHydro/Riverlabs/issues
- Email: (if provided by project)

**Digi (XBee) Support:**
- Forums: https://www.digi.com/support/forum
- Technical support: (for XBee-specific issues)

**ThingsBoard:**
- Community: https://groups.google.com/forum/#!forum/thingsboard
- Professional support: (paid plans)

**Carrier (SIM) Support:**
- Contact your SIM provider for network issues

---

## Case Studies

### Case 1: Intermittent Failures at Night

**Symptoms:**
- Transmissions work during day (8 AM - 6 PM)
- Fail overnight (6 PM - 8 AM)

**Investigation:**
- Signal strength: Same day/night
- Battery: Adequate
- Pattern: 100% success during day, 0% at night

**Root Cause:**
- Carrier tower switch
- Night tower has different configuration

**Solution:**
- Contacted carrier
- Tower configuration updated
- Success rate now 95%+ all times

### Case 2: Data Appears but Timestamps Wrong

**Symptoms:**
- ThingsBoard receives data
- Timestamps show 1970 (epoch zero)

**Root Cause:**
- RTC not set on logger
- Logger sending timestamp:0

**Solution:**
- Set RTC with set_clock utility
- Or: Remove timestamp from payload (ThingsBoard uses server time)

### Case 3: High Battery Drain

**Symptoms:**
- Battery drains in 5 days (expected 30 days)
- Telemetry working

**Investigation:**
- Transmission frequency: Every measurement (too high)
- XBee not sleeping properly

**Root Cause:**
- Sleep mode configuration
- SM not set to pin sleep

**Solution:**
- Configure XBee: SM=1 (pin sleep)
- Increase TELEMETRY_INTERVAL to 12 (transmit every 12 measurements)
- Battery now lasts 35 days

---

## Quick Reference: Troubleshooting Flowchart

```
Logger not transmitting?
├─ Power >3.3V? ──No──> Replace battery
│   └─Yes
├─ XBee seated? ──No──> Re-seat XBee
│   └─Yes
├─ SIM active? ──No──> Contact carrier
│   └─Yes
├─ Signal >-105 dBm? ──No──> Move logger or external antenna
│   └─Yes
├─ API Mode = 2? ──No──> Reconfigure XBee (critical!)
│   └─Yes
├─ APN correct? ──No──> Verify with carrier, reconfigure
│   └─Yes
├─ Network registered? ──No──> Try NT=0/1, check SIM
│   └─Yes
├─ Server URL correct? ──No──> Fix Arduino code
│   └─Yes
├─ Token valid? ──No──> Get new token, update code
│   └─Yes
└─ Contact support with diagnostic log
```

---

## Next Steps

- 📡 [Telemetry Setup Guide](setup-guide.md) - Complete setup overview
- 🔧 [XBee Setup Walkthrough](xbee-setup-walkthrough.md) - Detailed XBee configuration
- 📊 [ThingsBoard Configuration](thingsboard-configuration.md) - Dashboard setup
- 🔋 [Battery & Power Guide](../hardware/battery-power-guide.md) - Power optimization

---

!!! tip "Systematic Approach Works"
    Most telemetry issues are simple: wrong APN, bad signal, or configuration mistake. Work through diagnostics methodically and you'll find the problem!
