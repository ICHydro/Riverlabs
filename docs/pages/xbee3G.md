---
title: Xbee 3G Global modem configuration
tags:
keywords: 
last_updated: 11 April 2021
summary: "Configuration of the DIGI Xbee 3G Global modem for use with the Riverlabs loggers"
sidebar: home_sidebar
permalink: xbee3G.html
---

## Configuring the Cellular XBee 3G

The Riverlabs loggers use a [DIGI XBee Cellular 3G](https://www.digi.com/products/embedded-systems/digi-xbee/cellular-modems/digi-xbee-cellular-3g) for telemetry. XBees are extremely solid and reliable embedded modems that integrate nicely with Arduino thanks to the xbee-arduino library. We have extended the [original library](https://github.com/andrewrapp/xbee-arduino) with the functionality required to interact with the Cellular modems. Our version is available on our [github page](https://github.com/ICHydro/xbee-arduino) but is already included in the WMOnode package here, so you do not need to install it separately.

Most of the configuration of the Cellular XBee is done directly in the Arduino code. However you need to set some parameters manually to make sure that the logger is able to communicate with the XBee. This can be done most easily with DIGI's free [XCTU software](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xctu). We plan to elaborate a short manual here, but for now please follow the [manual here](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xctu).

The following settings should be set correctly for a proper functioning of the modem:

### Serial interfacing:

```
BD: 9600
NB: no parity
SB: one stop bit
RO: 3
TD: 0
FT: 681
AP: API mode with escapes
```

### Sleep commands:

```
SM: pin sleep [1]
```