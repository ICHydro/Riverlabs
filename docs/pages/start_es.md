---
title: Setting up
tags:
keywords: 
layout: default_es
summary: "What do you need to get going with the Riverlabs loggers? This page gives an overview of the hardware and software required to program the loggers."
sidebar: home_sidebar_es
permalink: es/start.html
---

## Introduction

The Riverlabs loggers are based on the Arduino bootloader. This means that they can be programmed easily using the USB port of a PC. It also means that a very large range of tools and documentation is available as part of the Arduino open hardware ecosystem. 

## Hardware

### FTDI cable

The loggers that are based on the Atmel328 processor (Wari and WMOnode) do not have a USB port. Instead they are programmed via the serial port using a serial-to-USB converter. The most common converters are based on the FTDI chip, and come either as a cable or an adapter. Here are some examples:

- [FTDI cable (3.3V)](https://ftdichip.com/products/ttl-232r-3v3/)
- [Sparkfun FTDI basic breakout board](https://www.sparkfun.com/products/9873)
- [Seeed UARTSBee](https://wiki.seeedstudio.com/UartSBee_V4/)

If you use a Windows operating system, then you may need to follow [these instructions](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers) to install the drivers of the FTDI chip on your computer.

### XBee development board

If you intend to use the XBee module for telemetry, then you may consider buying an [XBee Development board] (https://www.digi.com/products/models/xbib-c-smt) from DIGI, which is needed to adjust the settings of the XBee. If you have purchased the logger with XBee modem from Riverlabs, then this is not strictly necessary because in that case the XBee will have been configured already. However, if you purchase the XBee separately, or want to change any of the Riverlabs configurations, then a development board will be necessary.

## Software

### Arduino IDE

The Arduino system makes it very convenient to program the logger by simply connecting it to a PC via the FTDI cable, without the need for an expensive programmer. The arduino IDE can be downloaded for free from the [Arduino website](https://www.arduino.cc/en/software). The Arduino website has also fantastic [documentation](https://www.arduino.cc/en/Guide) in case that you want to learn more about Arduino and its vast ecosystem of manuals, libraries and other community resources.

### XCTU

To program the XBee modules, you will need DIGI's free [XCTU software](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xctu).





