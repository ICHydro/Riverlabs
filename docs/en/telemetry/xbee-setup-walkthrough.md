# XBee Setup Walkthrough

!!! abstract "Overview"
    Step-by-step visual guide to configuring Digi XBee Cellular modules for Riverlabs loggers. This guide covers both XBee Studio (recommended) and XCTU (legacy) configuration tools.
    
    **Compatibility:** XBee 3G, XBee Cellular LTE-M/NB-IoT and XBee3 Cellular

!!! info "Configuration Tools"
    Most XBee configuration is done directly in the Arduino code. However, certain parameters must be set manually to ensure proper communication between the logger and XBee:
    
    - **Serial interface settings** (baud rate, parity and timeouts)
    - **Sleep mode configuration** (pin sleep)
    - **API mode** (with escaped characters)
    
    You can use either:
    
    - **XBee Studio** (recommended): Modern interface, better for XBee3
    - **XCTU**: Legacy tool, works with all XBee models including older 3G modules
    
    Both tools are free from [Digi's website](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools).

## Prerequisites

Before starting, ensure you have:

-  **XBee Cellular module** (3G, LTE-M or NB-IoT)
-  **Active SIM card** (inserted in XBee)
-  **USB adapter** for XBee (XBee Grove Board or similar)
-  **Computer** (Windows, macOS or Linux)
-  **XBee Studio** or **XCTU** installed
-  **Carrier APN details** (from SIM provider)

---

## Part 1: Installing XBee Studio

### Download and Install

**1. Navigate to Digi Website:**

- URL: https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xbee-studio
- Or search "XBee Studio download"

**2. Select Your Platform:**

- Windows (64-bit)
- macOS
- Linux

**3. Download Installer**

**4. Install:**

**Windows:**

- Run .exe installer
- Accept license
- Choose installation directory
- Install drivers when prompted
- Finish

**macOS:**

- Open .dmg file
- Drag XBee Studio to Applications
- First launch: Right-click > Open (bypass Gatekeeper)
- Drivers installed automatically (may require restart)


**Linux:**
```bash
# Extract archive
tar -xzf XBeeStudio-linux-x64.tar.gz

# Run installer or AppImage
./XBeeStudio.AppImage

# Drivers (if needed)
sudo apt-get install libusb-1.0-0
```

### Verify Installation

1. Launch XBee Studio
2. Welcome screen appears
3. Check version (Help > About): should be 1.4.0+ for XBee3 cellular support

---

## Part 2: Hardware Connection

### Physical Setup

**1. Insert SIM Card (if not already done):**

!!! warning "Power Off First"
    Ensure XBee is NOT connected to power or USB when inserting/removing SIM!

- Locate SIM slot (underside of XBee)
- Note slot orientation (diagram on module)
- Insert micro-SIM (3FF size)
- Push until clicks (flush with module)
- Verify seated properly

**2. Mount XBee on USB Adapter:**

- Align pins carefully
- Press firmly until seated
- All pins should engage
- Module sits flat against adapter

**3. Connect USB Cable:**

- Adapter to computer USB port
- Wait for driver recognition

### Driver Verification

**Windows:**

1. Open Device Manager (Win+X > Device Manager)
2. Expand "Ports (COM & LPT)"
3. Look for "USB Serial Port (COMx)" — note COM number
4. If "Unknown Device": reinstall XBee Studio (includes drivers)

**macOS:**

1. Open Terminal
2. List devices:
   ```bash
   ls /dev/tty.*
   ```
3. Look for `/dev/tty.usbserial-XXXXXXXX`

**Linux:**

1. Check device:
   ```bash
   ls /dev/ttyUSB*
   ```
2. Add user to dialout group (if permission denied):
   ```bash
   sudo usermod -a -G dialout $USER
   # Log out and back in
   ```

---

## Part 3: Adding XBee to XBee Studio

### Step 1: Add New Device

**1. Launch XBee Studio**

**2. Main Window:**

- Left panel: "Devices" (currently empty)
- Center: "Get Started" prompts
- Right: Info panels

**3. Click "+ Add Device"** (top left or center button)

**4. Connection Dialog Appears**

### Step 2: Select Connection Method

**In "Add Radio" dialog:**

**Connection Type:**

- Select: **"USB"** (default)

**Port:**

- Windows: Select **"COM3"** (or your COM port from Device Manager)
- macOS/Linux: Select **/dev/ttyUSB0** (or your device)

**Baud Rate:**

- Leave at **"9600"** (or "Auto" if available)

!!! tip "Can't See Your Port?"
    - Try clicking "Refresh" button
    - Check USB cable (data-capable, not charge-only)
    - Verify drivers installed
    - Try different USB port

**5. Click "Add Selected"**

### Step 3: Initial Read

**XBee Studio:**

- Connects to XBee
- Reads firmware version
- Reads current configuration
- Displays module info

**You should see:**

- Device appears in left panel
- Module details (center):
    - Model: XB3-C-A2-UT-001 (or similar)
    - Firmware: Cellular (e.g., 11415)
    - Hardware version: 1E (or current)
- Configuration tabs appear

**If connection fails:**

- Check COM port selection
- Verify baud rate (try Auto or 9600)
- Ensure XBee properly seated
- See troubleshooting section

---

## Part 4: Configuration Tabs Overview

### Main Configuration Interface

**Left Panel:**

- List of added devices
- Click device to select

**Center Panel Tabs:**

**1. Radio Configuration**

- Most important tab
- All settings here

**2. Cellular Network**

- Network status
- Signal strength
- Connection diagnostics

**3. MicroPython**

- For advanced programming (not needed for logger)

**4. Firmware**

- Update firmware

**5. Terminal**

- Serial terminal for debugging

**6. File System**

- Access XBee file system (advanced)

### Navigating Radio Configuration

**Settings are organized in collapsible sections:**

- **Addressing** (PAN ID, addresses)
- **Serial Interfacing** (baud rate, parity)
- **I/O Settings** (pin configuration)
- **Networking** (APN, connection settings)
- **Security** (TLS, certificates)
- **RF Interfacing** (cellular modem settings)
- **Sleep Modes** (power management)
- **AT Command Options** (API mode)

**How to edit:**

1. Click section to expand
2. Click setting name to edit
3. Type new value or select from dropdown
4. Setting turns **bold** when modified
5. Must click **"Write"** to save to XBee

---

## Part 5: Essential Configuration

### Overview of Required Settings

**Goal:** Configure XBee for:

1. Cellular network connection
2. Serial communication with Arduino
3. API mode for data transmission
4. Power-saving sleep mode

### Section 1: Serial Interfacing

**Expand "Serial Interfacing" section**

!!! info "Critical Serial Settings"
    These settings MUST match exactly for the XBee to communicate with the Riverlabs logger. These values are required by the custom xbee-arduino library used in the logger firmware.

| Setting                    | Parameter | Value             | Description              |
|----------------------------|-----------|-------------------|--------------------------|
| **Baud Rate**              | BD        | **9600**          | Must match Arduino code  |
| **Parity**                 | NB        | **0** (No Parity) | Standard, no parity bit  |
| **Stop Bits**              | SB        | **1**             | One stop bit             |
| **Packetization Timeout**  | RO        | **3**             | Character timeout (3 ms) |
| **Transmit Delivery**      | TD        | **0**             | Disable delivery status  |
| **Flow Control Threshold** | FT        | **681**           | UART buffer threshold    |

**How to set:**

1. Click "BD (Baud Rate)"
2. Dropdown: Select **"9600"**
3. Set RO = **3**
4. Set TD = **0**
5. Set FT = **681** (hexadecimal: 0x2A9)
6. Verify NB = 0, SB = 1 (usually defaults)
7. Don't click Write yet (batch all changes)

!!! warning "Extended xbee-arduino Library Required"
    The Riverlabs code uses an extended version of the [xbee-arduino library](https://github.com/ICHydro/xbee-arduino) with cellular modem functionality. This custom library is included in the Riverlabs code packages, so you don't need to install it separately.

### Section 2: Networking

**Expand "Network" or "Mobile Network" section**

!!! danger "Critical Settings"
    Wrong APN prevents cellular connection!

| Setting                | Parameter | Value                   | Description       |
|------------------------|-----------|-------------------------|-------------------|
| **Access Point Name**  | AN        | **[Your carrier APN]**  | From SIM provider |
| **APN Username**       | AU        | **[If required]**       | Usually blank     |
| **APN Password**       | AV        | **[If required]**       | Usually blank     |
| **Network Technology** | NT        | **0** (LTE-M preferred) | Or 1 (NB-IoT)     |

**Common APNs:**

| Carrier        | APN                       |
|----------------|---------------------------|
| AT&T (USA)     | `m2m.com.attwireless.com` |
| T-Mobile (USA) | `iot.t-mobile.com`        |
| Verizon (USA)  | `vzwinternet`             |
| Vodafone (EU)  | `iot.vodafone.com`        |
| Hologram       | `hologram`                |

**Example Configuration:**
```
AN = m2m.com.attwireless.com
AU = (blank)
AV = (blank)
NT = 0
```

### Section 3: API Mode

**Expand "AT Command Options" or "API" section**

| Setting             | Parameter | Value | Description           |
|---------------------|-----------|-------|-----------------------|
| **API Enable**      | AP        | **2** | API mode with escapes |
| **API Output Mode** | AO        | **0** | Standard              |

!!! danger "API Mode Must Be 2 — Critical!"
    The Riverlabs firmware **requires API Mode 2** (API mode with escaped characters). 
    
    - **Mode 0** (Transparent) = Will NOT work
    - **Mode 1** (API without escapes) = Will NOT work  
    - **Mode 2** (API with escapes) = ✅ **REQUIRED**
    
    Using the wrong mode will prevent the logger from communicating with the XBee, resulting in no telemetry data.

**How to set:**

1. Click "AP (API Enable)"
2. Dropdown: Select **"2 - API mode with escaped characters"**
3. Verify AO = 0

### Section 4: Sleep Mode

**Expand "Sleep Modes" or "Power Management" section**

| Setting           | Parameter | Value | Description   |
|-------------------|-----------|-------|---------------|
| **Sleep Mode**    | SM        | **1** | Pin Sleep [1] |
| **Sleep Options** | SO        | **0** | Standard      |

!!! info "Pin Sleep Mode Required"
    **SM must be set to 1 (Pin Sleep)** for proper power management with the Riverlabs logger.

**Pin Sleep Operation:**

- Arduino controls when XBee sleeps via hardware pin
- Pin 9 (Sleep_RQ/DTR): 
    - **High** = XBee sleeps (low power)
    - **Low** = XBee wakes (active)
- Saves significant power between transmissions
- Essential for long battery life

**How to set:**

1. Click "SM (Sleep Mode)"
2. Dropdown: Select **"1 - Pin Sleep [1]"**
3. Verify SO = 0

!!! warning "Do Not Use Other Sleep Modes"
    - **SM = 0** (Disabled) will drain battery quickly
    - **SM = 4** (Cyclic Sleep) won't work with logger's control logic
    - **SM = 5** (Cyclic Sleep with Pin Wake) not compatible

### Section 5: Additional Settings (Optional but Recommended)

| Setting                    | Parameter | Value             | Description        |
|----------------------------|-----------|-------------------|--------------------|
| **Guard Time**             | GT        | **3E8** (1000 ms) | Command mode delay |
| **Flow Control Threshold** | FT        | **681**           | UART buffering     |
| **Transmit Options**       | TO        | **0**             | Standard           |

---

## Part 6: Writing Configuration

### Batch Write

**After configuring ALL settings above:**

1. **Review changes:**
    - Modified settings appear **bold**
    - Verify all critical settings changed

2. **Click "Write" button** (top of Radio Configuration tab)
    - Button usually in toolbar
    - Or: Right-click device > "Write Settings"

3. **Wait for write to complete:**
    - Progress indicator
    - "Write successful" message
    - Settings no longer bold

4. **Verify:**
    - Click "Read" button to refresh
    - Confirm all settings match expected values

!!! warning "Write Before Disconnecting"
    Changes are NOT saved until you click Write! If you disconnect without writing, changes are lost.

### Setting Profile (Optional)

**Save configuration for future use:**

1. **File > Export Profile**
2. **Save as:** `Riverlabs_XBee3_Config.xpro`
3. **Use later:**
    - File > Import Profile
    - Apply to new XBee modules
    - Ensures consistency across fleet

---

## Part 7: Network Connection Test

### Using Cellular Network Panel

**1. Navigate to "Cellular Network" tab**

**2. Current Status Display:**

- Network registration: Not registered / Registered
- Signal strength (RSSI): dBm value
- IP address: (none) or assigned IP
- Connection state: Disconnected / Connected

**3. Click "Connect to Network" button**

**4. Wait for connection:**

- May take 30–90 seconds
- Progress messages appear
- Watch for:
    - "Searching for network..."
    - "Network found"
    - "Registering..."
    - "Connected"

**5. Success Indicators:**

- ✅ Network registered: **Yes**
- ✅ Signal strength: **-90 dBm or better** (less negative = stronger)
- ✅ IP address: **Assigned** (e.g., 10.x.x.x)
- ✅ Connection state: **Connected**

**6. Signal Strength Guide:**

| RSSI (dBm)   | Quality   | Usability                      |
|--------------|-----------|--------------------------------|
| -50 to -70   | Excellent | Perfect for continuous use     |
| -70 to -90   | Good      | Reliable for most applications |
| -90 to -105  | Fair      | Usable but may have issues     |
| -105 to -115 | Poor      | Marginal, high failure rate    |
| < -115       | Very Poor | Unreliable                     |

### Troubleshooting Connection Failures

**"No network found":**

**Causes:**

- SIM not activated
- Wrong network technology (LTE-M vs NB-IoT)
- Poor signal location

**Solutions:**

1. Verify SIM active (contact carrier)
2. Try changing NT parameter:
    - NT=0 (LTE-M)
    - NT=1 (NB-IoT)
    - Write and retry connection
3. Move to window or outdoors
4. Check antenna connected (if external)

**"Network found but registration failed":**

**Causes:**

- Wrong APN
- SIM authentication issue

**Solutions:**

1. Verify APN spelling (case-sensitive!)
2. Check AU/AV credentials
3. Contact carrier for APN details
4. Try default APN (often works)

**"Registered but no IP address":**

**Causes:**

- DHCP failure
- Network congestion

**Solutions:**

1. Disconnect and reconnect
2. Wait 2–3 minutes
3. Power cycle XBee (remove/reinsert USB)
4. Contact carrier (possible account issue)

---

## Part 8: Data Transmission Test

### Using Terminal Tab

**Test HTTP POST** (to verify complete setup):

1. **Navigate to "Terminal" tab**

2. **Ensure connected to network** (from previous step)

3. **Enter API commands** (advanced users):

**Alternatively, test from Arduino:**

1. **Remove XBee from USB adapter**
2. **Install in logger**
3. **Upload test sketch** (or production code)
4. **Connect serial monitor**
5. **Observe transmission attempts**

**Expected flow:**
```
Initializing XBee...
XBee ready
Connecting to network...
Network connected
Reading sensor...
Distance: 1234 mm
Preparing HTTP request...
Sending to ThingsBoard...
Response: 200 OK
Transmission successful!
```

**If transmission fails:**

- See [Troubleshooting Connections](troubleshooting-connections.md)

---

## Part 9: Finalizing Setup

### Power Cycle Test

**Verify configuration persists:**

1. **Disconnect USB** (remove XBee from adapter)
2. **Wait 10 seconds**
3. **Reconnect USB** (re-insert in adapter)
4. **Open XBee Studio, add device**
5. **Read configuration**
6. **Verify all settings** still correct

If settings reset:

 - Configuration wasn't written properly. Repeat Part 6.

### Label XBee Module

**Document configuration:**

- Use label maker or permanent marker
- Write on XBee (away from antenna):
    - "Configured DD/MM/YY"
    - "Logger: Wari-001"
    - APN (if helpful)
- Prevents confusion with unconfigured modules

### Installation in Logger

**1. Power off logger** (disconnect battery)

**2. Install XBee:**

- Align pin 1 (look for mark on PCB)
- Press gently but firmly
- Ensure all pins engage
- Module sits flush

**3. Check antenna:**

- External antenna: Connect U.FL or similar
- Internal PCB antenna: Ensure clearance (no contact with enclosure)

**4. Reconnect battery**

**5. Test boot:**

- Logger boots
- XBee initializes
- Network connection within 60 seconds

---

## Part 10: Troubleshooting

### XBee Studio Won't Detect Module

**Symptom:** No device found when adding

**Causes & Solutions:**

| Cause               | Check                      | Solution                      |
|---------------------|----------------------------|-------------------------------|
| Wrong port selected | Device Manager (Windows)   | Select correct COM port       |
| Baud rate mismatch  | Previous config changed BD | Try 9600, 115200, or Auto     |
| Driver issue        | Device Manager shows error | Reinstall XBee Studio, reboot |
| Bad USB cable       | Cable charge-only          | Use data-capable USB cable    |
| XBee not seated     | Physical inspection        | Re-seat XBee on adapter       |
| Adapter power issue | LED on adapter             | Try different USB port/hub    |

### Settings Not Saving

**Symptom:** Configuration resets after power cycle

**Causes:**

- Didn't click "Write" button
- Write failed (but no error shown)
- Firmware corruption (rare)

**Solutions:**

1. Configure settings
2. Click "Write"
3. **Wait for confirmation**
4. Click "Read" to verify
5. Power cycle and read again
6. If still resetting: Update firmware (see below)

### Poor Signal Strength

**Symptom:** RSSI < -105 dBm

**Solutions:**

1. **Antenna:**
    - Ensure connected properly
    - Try external antenna if using internal
    - Check for damage
2. **Location:**
    - Move to window
    - Elevate device
    - Away from metal structures
3. **Network:**
    - Try different technology (NT=0 vs NT=1)
    - Check carrier coverage map
    - Test SIM in phone at same location

### Firmware Update

**When needed:**

- Bug fixes from Digi
- New features
- Compatibility improvements

**How to update:**

1. **Download firmware:**
    - https://www.digi.com/support/productdetail?pid=5817
    - Select XBee3 Cellular LTE-M/NB-IoT
    - Choose latest version (e.g., 11415 or newer)

2. **In XBee Studio:**
    - Navigate to "Firmware" tab
    - Click "Update Firmware"
    - Browse to downloaded file (.ebin)
    - Click "Update"
    - Wait (5–10 minutes, **do not disconnect**)
    - Success message

3. **Verify:**
    - Check firmware version (Radio Configuration tab)
    - Reconfigure settings (firmware update may reset)

## Next Steps

- [ThingsBoard Configuration](thingsboard-configuration.md): Set up your dashboard
- [Troubleshooting Connections](troubleshooting-connections.md): Fix telemetry issues
- [Telemetry Setup Guide](setup-guide.md): Complete setup overview
- [Battery & Power Guide](../operation/hardware/battery-power-guide.md): Optimize for telemetry

---

!!! success "XBee Configured!"
    Your XBee is now properly configured and ready for field deployment. Keep the configuration profile saved for future modules!
