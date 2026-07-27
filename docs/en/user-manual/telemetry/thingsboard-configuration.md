# ThingsBoard Configuration

!!! abstract "Overview"
    Complete guide to setting up ThingsBoard IoT platform for Riverlabs data visualization and monitoring. From account creation to advanced dashboards.

## What is ThingsBoard?

[ThingsBoard](https://thingsboard.io/) is an open-source IoT platform for device management, data collection and visualization. It's ideal for Riverlabs loggers because:

- **Free tier available** (demo server)
- **Easy HTTP API** integration
- **Powerful dashboards** with widgets
- **Alerts and notifications**
- **Multi-user support**
- **Mobile-friendly**
- **Self-hosting option** for advanced users

---

## Deployment Options

### Option 1: ThingsBoard Demo Server (Recommended for Testing)

**Pros:**

- Free to use
- No setup required
- Good for evaluation and learning
- Includes all features

**Cons:**

- Data deleted after inactivity
- Shared infrastructure
- No guaranteed uptime
- Not for production deployments

**URL:** https://demo.thingsboard.io

**Use Case:** Testing, learning and short-term deployments

### Option 2: ThingsBoard Cloud

**Pros:**

- Managed hosting
- Guaranteed uptime
- Technical support
- Automatic updates
- Production-ready

**Cons:**

- Monthly fees (~$10–50+)
- Subscription required

**URL:** https://thingsboard.cloud

**Use Case:** Long-term monitoring, critical deployments and multiple loggers

### Option 3: Self-Hosted

**Pros:**

- Full control
- No monthly fees (after setup)
- Data privacy
- Custom configuration

**Cons:**

- Requires server management skills
- Infrastructure costs (AWS/DigitalOcean)
- Maintenance responsibility
- Security management

**Use Case:** Organizations with IT staff, many loggers and custom requirements

!!! tip "Start with Demo Server"
    Use the demo server to learn ThingsBoard and test your logger. Migrate to Cloud or self-hosted once you're confident in the setup.

---

## Creating a ThingsBoard Account

### Demo Server Registration

1. **Navigate to** https://demo.thingsboard.io
2. **Click "Sign Up"** (top right)
3. **Fill registration form:**
    - Email address
    - First and last name
    - Password (use a strong password and save in password manager)
4. **Check email** for activation link
5. **Click activation link**
6. **Log in** to ThingsBoard

!!! warning "Demo Server Data Retention"
    Demo server deletes inactive accounts periodically. Don't use for long-term deployments without regular access.

### ThingsBoard Cloud Registration

1. **Navigate to** https://thingsboard.cloud
2. **Click "Try It Free"** or similar
3. **Complete registration**
4. **Select plan** (usually free tier to start)
5. **Confirm email**
6. **Log in**

---

## Adding Your Logger as a Device

### Step 1: Create Device

1. **Log in to ThingsBoard**
2. **Navigate to "Devices"** (left sidebar)
3. **Click "+ Add Device"** (top right)
4. **Fill device form:**
    - **Name:** `Wari-001` (or your logger ID)
    - **Label:** `Creek Monitoring - Site A` (descriptive location)
    - **Device Profile:** `Default` (or custom profile)
    - **Gateway:** Leave unchecked
5. **Click "Add"**
6. **Device created!**

### Step 2: Get Access Token

**After creating device:**

1. **Device appears in list**
2. **Click device name** to open details
3. **Navigate to "Credentials" tab**
4. **Copy "Access Token"** (long string like `A1B2C3D4E5F6G7H8I9J0`)
5. **Save token** securely (you'll need it for Arduino code)

**Example Token:**
```
A1B2C3D4E5F6G7H8I9J0K1L2M3N4O5P6
```

!!! danger "Keep Token Secure"
    This token authenticates your logger. Anyone with it can send data as your device. Don't share publicly!

### Step 3: Configure Arduino Code

**Update your Arduino sketch** (e.g., `wari_3G.ino`):

```cpp
// Old:
#define SERVER_URL "http://demo.thingsboard.io/api/v1/PASTE_TOKEN_HERE/telemetry"

// New (with your token):
#define SERVER_URL "http://demo.thingsboard.io/api/v1/A1B2C3D4E5F6G7H8I9J0K1L2M3N4O5P6/telemetry"
```

**For ThingsBoard Cloud:**
```cpp
#define SERVER_URL "https://thingsboard.cloud/api/v1/YOUR_TOKEN/telemetry"
```

**For Self-Hosted:**
```cpp
#define SERVER_URL "http://your-server.com/api/v1/YOUR_TOKEN/telemetry"
```

### Step 4: Test Connection

**Manual Test with curl** (optional):

```bash
curl -X POST \
  http://demo.thingsboard.io/api/v1/YOUR_TOKEN/telemetry \
  -H 'Content-Type: application/json' \
  -d '{"temperature":22.5, "distance_mm":1234}'
```

**Expected response:**
```json
{}
```

Empty JSON response = success!

**In ThingsBoard:**

1. Navigate to device
2. Click "Latest Telemetry" tab
3. Values should appear

**If no data appears:** Check token, URL and network connectivity.

---

## Understanding Data Structure

### JSON Format

Riverlabs loggers send data in JSON format:

**Wari (Ultrasonic) Example:**
```json
{
  "distance_mm": 1234,
  "temperature_C": 22.5,
  "battery_V": 3.85,
  "timestamp": 1703688000
}
```

**Lidar Example:**
```json
{
  "distance_mm": 2456,
  "battery_V": 3.78,
  "signal_strength": 85,
  "timestamp": 1703688000
}
```

### Telemetry Keys

| Key               | Type    | Description                 | Units               |
|-------------------|---------|-----------------------------|---------------------|
| `distance_mm`     | Integer | Sensor-to-water distance    | Millimeters         |
| `temperature_C`   | Float   | Air temperature (Wari only) | Celsius             |
| `battery_V`       | Float   | Battery voltage             | Volts               |
| `signal_strength` | Integer | Lidar signal quality        | Arbitrary (0–100)   |
| `timestamp`       | Integer | Unix timestamp              | Seconds since epoch |

**ThingsBoard stores:**

- **Telemetry:** Time-series data (e.g., distance, temperature, etc.)
- **Attributes:** Static data (device model, location and install date)

---

## Creating Your First Dashboard

### Step 1: Create Dashboard

1. **Navigate to "Dashboards"** (left sidebar)
2. **Click "+ Add Dashboard"** (top right)
3. **Enter dashboard title:** `Logger Monitoring - Site A`
4. **Click "Add"**
5. **Dashboard created** (currently empty)

### Step 2: Add Widgets

**Open dashboard in edit mode:**

1. Click dashboard name to open
2. Click **pencil icon** (edit mode, top right)

**Add your first widget:**

1. **Click "+ Add Widget"** (center or bottom right)
2. **Select widget bundle:** `Charts` (or `Gauges`, `Cards`, etc.)
3. **Select widget type:** `Timeseries Line Chart`
4. **Configure datasource:**
    - **Type:** `Entity`
    - **Entity:** Select your device
    - **Data keys:** Check `distance_mm`; `battery_V`
5. **Configure appearance:**
    - **Title:** `Water Level & Battery`
    - **Time window:** Last 7 days
    - **Line colors:** Blue (distance); Red (battery)
6. **Click "Add"**
7. **Widget appears on dashboard**

**Resize and position:**

- Drag widget to move
- Drag edges/corners to resize
- Add more widgets

### Step 3: Additional Useful Widgets

**1. Current Distance Card**

- **Bundle:** `Cards`
- **Type:** `Simple Card`
- **Data key:** `distance_mm`
- **Title:** `Current Distance`
- **Shows:** Latest value large and clear

**2. Battery Gauge**

- **Bundle:** `Gauges`
- **Type:** `Radial Gauge`
- **Data key:** `battery_V`
- **Min value:** 3.0
- **Max value:** 4.2
- **Color ranges:**
    - 3.0–3.3: Red (critical)
    - 3.3–3.6: Orange (low)
    - 3.6–4.2: Green (good)

**3. Temperature Chart** (Wari only)

- **Bundle:** `Charts`
- **Type:** `Timeseries Line Chart`
- **Data key:** `temperature_C`
- **Time window:** Last 24 hours

**4. Device Status Card**

- **Bundle:** `Cards`
- **Type:** `Entity Table`
- **Shows:** Last activity, attributes and status

### Step 4: Save Dashboard

1. **Click checkmark** (top right) to exit edit mode
2. **Dashboard saved** automatically
3. **Share or bookmark** URL for quick access

---

## Advanced Widget Configuration

### Customizing Time Windows

**Fixed Window:**

- Last hour
- Last 24 hours
- Last 7 days
- Custom range

**Example:** Show only business hours

- Configure: 8:00 AM–6:00 PM
- Useful for solar-powered loggers

### Adding Thresholds

**Alarm Levels on Charts:**

1. Edit widget (pencil icon)
2. Navigate to "Advanced" tab
3. Add **threshold lines:**
    - **High water warning:** 500 mm (red line)
    - **Low water warning:** 2000 mm (orange line)
    - **Battery critical:** 3.3 V (red line)

### Data Aggregation

**For long time periods** (months), aggregate data:

- **None:** Show all points (slow for large datasets)
- **Average:** Daily/hourly averages
- **Min/Max:** Show extremes
- **Sum:** Total over period

**Example:** Daily maximum water level

- Aggregation: `Max`
- Interval: `1 day`

### Calculated Fields

**Derive new values from telemetry:**

**Water Depth** (install height - distance):

1. Add device attribute: `install_height_mm = 3000`
2. Use widget with calculation:
   ```
   ${install_height_mm} - ${distance_mm}
   ```

**Battery Percentage:**

Estimate % from voltage:
```
(${battery_V} - 3.0) / (4.2 - 3.0) * 100
```

---

## Alarms and Notifications

### Creating Alarm Rules

1. **Navigate to device**
2. **Click "Alarms" tab**
3. **Click "+ Add Alarm Rule"**
4. **Configure rule:**

**Example: Low Battery Alert**

- **Name:** `Low Battery Warning`
- **Type:** `battery_low`
- **Severity:** `Warning`
- **Condition:**
  ```
  $[battery_V] < 3.4
  ```
- **Schedule:** Always active
- **Action:** Create alarm

**Example: High Water Alert**

- **Name:** `Flood Warning`
- **Type:** `high_water`
- **Severity:** `Critical`
- **Condition:**
  ```
  $[distance_mm] < 500
  ```
  (Assuming 500mm = high water threshold)

### Configuring Notifications

**Email Notifications:**

1. **Navigate to "Rule Chains"** (left sidebar)
2. **Open "Root Rule Chain"**
3. **Add "Send Email" node**
4. **Configure SMTP settings:**
    - Server (Gmail, SendGrid, etc.)
    - Credentials
    - Recipient addresses
5. **Connect alarm nodes** to email node

**SMS Notifications** (via third-party service):

- Integrate with Twilio, AWS SNS
- Requires ThingsBoard Cloud or self-hosted
- Configure in Rule Chain

**Dashboard Alarms:**

- Add "Alarms" widget to dashboard
- Shows active alarms with severity colors
- Click to acknowledge/clear

---

## Data Export

### Exporting Historical Data

1. **Navigate to device**
2. **Click "Latest Telemetry" tab**
3. **Select date range**
4. **Click export icon** (top right)
5. **Choose format:**
    - CSV (Excel-compatible)
    - JSON (for processing)
6. **Download file**

**CSV Format:**
```csv
Timestamp,distance_mm,battery_V,temperature_C
2025-12-27 10:00:00,1234,3.85,22.5
2025-12-27 10:15:00,1256,3.84,23.1
...
```

### API Data Access

**For programmatic access:**

```bash
curl -X GET \
  'http://demo.thingsboard.io/api/plugins/telemetry/DEVICE/YOUR_DEVICE_ID/values/timeseries?keys=distance_mm,battery_V&startTs=1703520000000&endTs=1703606400000' \
  -H 'X-Authorization: Bearer YOUR_JWT_TOKEN'
```

**JWT Token:** Obtain from user profile > Security settings

---

## Multi-Device Management

### Adding Multiple Loggers

**Repeat for each logger:**

1. Add device (unique name: `Wari-001`, `Wari-002`, etc.)
2. Get access token
3. Configure Arduino with respective token
4. Deploy

### Device Groups

**Organize by location or type:**

1. **Navigate to "Entity Groups"**
2. **Create group:** `Creek Monitoring Sites`
3. **Add devices** to group
4. **Filter dashboards** by group

### Fleet Dashboard

**Create overview dashboard:**

1. **Add "Devices" widget**
2. **Entity type:** Device group
3. **Select group:** `Creek Monitoring Sites`
4. **Columns:** Name, Last Activity, Battery, Latest Distance
5. **Color rules:** Highlight low battery/offline devices

**Example Layout:**

| Device    | Location | Last Seen   | Battery | Distance | Status         |
|-----------|----------|-------------|---------|----------|----------------|
| Wari-001  | Site A   | 2 min ago   | 3.85 V  | 1234 mm  | 🟢 OK          |
| Wari-002  | Site B   | 17 min ago  | 3.45 V  | 2456 mm  | 🟡 Low Battery |
| Lidar-001 | Site C   | 3 hours ago | 3.12 V  | --       | 🔴 Offline     |

---

## User Management

### Adding Users (Cloud/Self-Hosted Only)

Demo server doesn't support multiple users.

**On Cloud/Self-Hosted:**

1. **Navigate to "Users"** (left sidebar)
2. **Click "+ Add User"**
3. **Fill form:**
    - Email, name
    - Role (Customer, Tenant, etc.)
4. **Assign permissions:**
    - View-only (for stakeholders)
    - Full access (for technicians)
5. **Send activation** email

### Sharing Dashboards

**Public Link** (read-only):

1. Open dashboard
2. Click **"Share"** icon (top right)
3. Toggle **"Public"**
4. Copy link
5. Share URL with stakeholders

**Pros:** Easy sharing, no login required  
**Cons:** Anyone with link can view

---

## ThingsBoard Mobile App

### Installing the App

**iOS:**

- App Store: Search "ThingsBoard Live"
- Or: https://apps.apple.com/app/thingsboard-live

**Android:**

- Google Play: Search "ThingsBoard Live"
- Or: https://play.google.com/store/apps/details?id=org.thingsboard.demo.app

### Configuring the App

1. **Open app**
2. **Select server:**
    - Demo: `demo.thingsboard.io`
    - Cloud: `thingsboard.cloud`
    - Custom: Your server URL
3. **Log in** with credentials
4. **Dashboards** appear on home screen
5. **Tap to view** real-time data

**Mobile Features:**

- View dashboards
- Check alarms
- Export data
- Push notifications (if configured)

---

## Troubleshooting

### No Data Appearing

**Checklist:**

1. **Verify logger sending data**
    - Check Serial Monitor for "HTTP 200 OK"
    - Verify SD card has recent data
2. **Check access token** in Arduino code
3. **Verify server URL** matches ThingsBoard instance
4. **Test with curl** (manual POST request)
5. **Check device "Latest Telemetry"** tab (not just dashboard)
6. **Inspect network connectivity** (XBee registered?)

### Data Appears but Dashboard Blank

1. **Check widget configuration:**
    - Correct device selected?
    - Data keys match telemetry keys?
    - Time window includes data period?
2. **Refresh dashboard** (F5)
3. **Check browser console** for errors

### Alarms Not Triggering

1. **Verify alarm rule** condition syntax
2. **Check telemetry values** meet condition
3. **Ensure alarm cleared** (won't re-trigger if already active)
4. **Test with manual data** (curl POST with alarm-triggering values)

### Data Delayed

- ThingsBoard processes data quickly (< 1 second typically)
- Delay is usually in transmission (XBee connection time)
- Check widget refresh rate (auto-refresh interval)

---

## Best Practices

### Dashboard Design

**Do:**

- Use clear widget titles
- Show key metrics prominently
- Color-code by severity (red = bad, green = good)
- Include time context (last updated)
- Group related data

**Don't:**

- Overcrowd with too many widgets
- Use misleading scales
- Forget mobile users (test on phone)

### Data Management

- **Export data regularly** (backup in case of account issues)
- **Document device attributes** (install location, height, etc.)
- **Use consistent naming** (Wari-001, not "logger at creek")
- **Review alarms weekly** (adjust thresholds based on experience)

### Security

- **Use HTTPS** for ThingsBoard Cloud/self-hosted
- **Rotate access tokens** if compromised
- **Limit public dashboard** sharing to non-sensitive data
- **Use strong passwords**
- **Enable 2FA** if available (Cloud/self-hosted)

---

## Cost Considerations

### Demo Server

- **Cost:** Free
- **Limits:** Data retention, no SLA, periodic deletion
- **Best for:** Testing, education

### ThingsBoard Cloud

- **Free Tier:** 
    - 1 device
    - Limited dashboards
    - Community support
- **Paid Plans:** ~$10–50+/month
    - More devices
    - Advanced features
    - Technical support
    - SLA

### Self-Hosted
- **Infrastructure:** $5–20+/month (DigitalOcean, AWS)
- **Setup time:** 4–8 hours (first time)
- **Maintenance:** Ongoing
- **Best for:** Organizations with IT resources

---

## Alternatives to ThingsBoard

If ThingsBoard doesn't fit your needs:

**Open-Source:**

- **Grafana + InfluxDB:** Popular, powerful, steep learning curve
- **Node-RED:** Visual flow programming, flexible
- **Emoncms:** Energy monitoring focus

**Commercial:**

- **Losant:** Enterprise IoT platform
- **Ubidots:** Easy to use, moderate pricing
- **TagoIO:** Good for prototyping

**DIY:**

- Custom server with PHP/Python + MySQL
- Google Sheets (via IFTTT or custom script)

---

## Next Steps

- [XBee Setup Walkthrough](xbee-setup-walkthrough.md): Detailed XBee configuration
- [Troubleshooting Connections](troubleshooting-connections.md): Fix telemetry issues
- [ThingsBoard Documentation](https://thingsboard.io/docs/): Official docs for advanced features
- [Battery & Power Guide](../hardware/battery-power-guide.md): Monitor battery health

---

!!! success "Dashboard Ready!"
    Your ThingsBoard dashboard is now configured and displaying real-time data. Check it daily for the first week to ensure stable operation!
