---
title: Thingsboard
tags:
keywords: 
layout: default_es
summary: "A short overview of the Thingsboard IoT server and its functionality"
sidebar: home_sidebar_es
permalink: es/thingsboard.html
---

The WMOnode logger uses COAP as transfer protocol (MQTT is under development). This is a widely used protocol that is implemented by several IoT software stacks. One option is [Thingsboard](https://thingsboard.io/), which is an open source project with a free community version and a more powerful commercial version. It also includes an online [demo version](https://thingsboard.cloud) which you can use to test your telemetry without the need to install any software.

The arduino code is set by default with the settings on of the Thingsboard demo. However, you can easily change the settings according to your own setup and requirements:

```cpp
#define HOST "demo.thingsboard.io"                // internet address of the IoT server to report to
#define ACCESSTOKEN "A1_TEST_TOKEN"               // COAP access token
#define LOGGERID "MyLogger1"                      // Logger ID. Set to whatever you like
```


