---
title: Xbee 4G LTE modem configuration and troubleshooting
tags:
keywords: 
last_updated: 1 February 2026
summary: "Configuration and troubleshooting guide for DIGI XBee 4G LTE modems with ThingsBoard integration"
sidebar: home_sidebar
permalink: xbee4G.html
---

## Overview

The DIGI XBee 4G LTE modems provide cellular connectivity using LTE Cat 1 networks. This guide covers configuration for use with Riverlabs loggers and ThingsBoard Cloud integration.

## Key Settings

### Serial Interfacing

```
BD: 9600
NB: no parity
SB: one stop bit
AP: API mode with escapes
```

### Network Configuration

```
AN: APN (network specific, e.g., "eapn1.net")
CP: Carrier Profile (0 = Auto)
N#: Network Technology (2 = LTE Cat 1)
```

### Sleep Commands

```
SM: pin sleep [1]
```

## ThingsBoard Cloud Integration

### Protocol Selection

ThingsBoard Cloud supports multiple protocols for data ingestion:

#### HTTP (Recommended)
- **Port**: 80
- **Protocol**: TCP (1)
- **Host**: `thingsboard.cloud`
- **Endpoint**: `/api/v1/{ACCESS_TOKEN}/telemetry`
- **Content-Type**: `application/json`

**Configuration in code:**
```cpp
//#define COAP                    // Comment out to use HTTP
#define HOST "thingsboard.cloud"
const uint16_t Port = 0x50;     // Port 80
uint8_t protocol = 1;           // TCP
```

#### CoAPS (Secure CoAP)
- **Port**: 5684
- **Protocol**: SSL over TCP (4)
- **Host**: `coap.thingsboard.cloud`
- **Note**: Requires DTLS support - **not recommended** for XBee 4G as standard SSL over TCP is not compatible with CoAP's DTLS requirement

**Configuration in code:**
```cpp
#define COAP                           // Enable CoAP
#define HOST "coap.thingsboard.cloud"
const uint16_t Port = 0x1634;          // Port 5684
uint8_t protocol = 4;                  // SSL over TCP
```

#### Plain CoAP (Not Supported by ThingsBoard Cloud)
- **Port**: 5683 (UDP)
- **Note**: ThingsBoard Cloud does **not** support plain CoAP without encryption

### Testing Connection

Test your ThingsBoard configuration with curl:

```bash
curl -v -X POST http://thingsboard.cloud/api/v1/{ACCESS_TOKEN}/telemetry \
  --header Content-Type:application/json \
  --data "{temperature:25}"
```

Replace `{ACCESS_TOKEN}` with your device's access token from ThingsBoard.

## Troubleshooting

### AI Status Codes

The XBee's AI (Association Indication) command returns the cellular connection status:

| Code | Status | Meaning |
|------|--------|---------|
| 0x00 | Connected | ✅ Connected to internet - ready to send data |
| 0x22 | Registering | Registering with cellular network |
| 0x23 | Connecting | Connecting to internet |
| 0x25 | Denied | Registration denied by network |
| 0xFF | Initializing | Modem initializing or scanning for networks |

**Monitor AI status:**
```cpp
Serial.print(F("AI status = "));
Serial.println(AIstatus, HEX);
```

### Common Issues

#### Device connects but no data appears in ThingsBoard

**Symptoms:**
- AI status = 0 (connected)
- IP address received successfully
- No errors in serial output
- No data in ThingsBoard dashboard

**Possible causes:**

1. **Wrong protocol/port combination**
   - CoAP defined but using HTTP port (80)
   - HTTP mode but using CoAP port (5683 or 5684)
   - SSL protocol (4) used with plain CoAP

2. **Wrong hostname**
   - Using `thingsboard.cloud` for CoAPS (should be `coap.thingsboard.cloud`)
   - Using `coap.thingsboard.cloud` for HTTP (should be `thingsboard.cloud`)

3. **Incorrect access token**
   - Verify the token matches your device in ThingsBoard
   - Check for extra spaces or characters

**Solution:**
- Use HTTP mode (recommended) with proper configuration
- Verify all three parameters match: HOST, Port, and protocol

#### Connection timeouts

**Symptoms:**
- "Timeout or error. Trying again next wakeup."
- Message sent but no response

**Possible causes:**
- Network congestion
- Wrong protocol for the port
- Firewall blocking connections

**Solution:**
- Increase `TIMEOUT` value in configuration
- Verify protocol matches the endpoint requirements
- Check APN settings for your carrier

#### DNS lookup fails

**Symptoms:**
- "Sending DNS Lookup" appears but no IP address received
- No "Callback - IP Address" message

**Possible causes:**
- APN not configured correctly
- Network not allowing DNS queries
- XBee firmware issue

**Solution:**
- Verify APN setting matches your carrier
- Try hardcoding IP address instead of hostname (not recommended for production)
- Update XBee firmware

### Monitoring Connection Status

Add debug output to track connection progress:

```cpp
#ifdef DEBUG > 0
    Serial.print(F("AI status = "));
    Serial.println(AIstatus, HEX);
    Serial.print(F("IP Address: "));
    Serial.println(IP, HEX);
#endif
```

## Data Format

### HTTP JSON Payload

```json
{
  "ts": 1769952439000,
  "values": {
    "h": -1,
    "v": 3416,
    "t": 2075
  }
}
```

Where:
- `ts`: Timestamp in milliseconds since epoch
- `h`: Height/distance measurement
- `v`: Battery voltage (in centivolts)
- `t`: Temperature (in centidegrees Celsius)

### CoAP Payload

CoAP uses the same JSON format but encapsulated in CoAP message structure with options for the API path.

## Best Practices

1. **Start with HTTP** - It's simpler and more reliable for initial testing
2. **Monitor AI status** - Always check connection status before sending data
3. **Use debug output** - Enable full debug (`DEBUG 2`) during development
4. **Test with curl first** - Verify your ThingsBoard endpoint works before testing with hardware
5. **Check timeout settings** - Allow sufficient time for cellular connection (default 600s)
6. **Verify APN** - Incorrect APN is the most common configuration error

## References

- [DIGI XBee 4G LTE Product Page](https://www.digi.com/products/embedded-systems/digi-xbee/cellular-modems/xbee-cellular-lte-cat-1)
- [ThingsBoard HTTP Device API](https://thingsboard.io/docs/reference/http-api/)
- [ThingsBoard CoAP Device API](https://thingsboard.io/docs/reference/coap-api/)
- [XBee API Command Reference](https://www.digi.com/resources/documentation/digidocs/90001477/references)
