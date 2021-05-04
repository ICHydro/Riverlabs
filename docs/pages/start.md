---
title: Setting up
tags:
keywords: 
last_updated: 11 April 2021
summary: "What do you need to get going with the Riverlabs loggers? This page gives an overview of the hardware and software required to program the loggers."
sidebar: home_sidebar
permalink: start.html
---

## Introduction

The Riverlabs loggers are based on the Arduino bootloader. This means that they can be programmed easily using the USB port of a PC. It also means that a very large range of tools and documentation is available as part of the Arduino open hardware ecosystem. 

## Hardware

### FTDI cable

The loggers that are based on the Atmel328 processor (Wari and WMOnode) do not have a USB port. Instead they need to be programmed via the serial port using a serial-to-USB convertor. The most common converters are based on the FTDI chip, and come either as a cable, or an adapter with a USB port to connect with a PC. Here are some examples:

- FTDI cable (3.3V) https://ftdichip.com/products/ttl-232r-3v3/
- Sparkfun FTDI basic breakout board https://www.sparkfun.com/products/9873
- Seeed UARTSBee https://wiki.seeedstudio.com/UartSBee_V4/

The Seeed UARTSBee has the advantage that it can also be used to program the DIGI XBee modem (see below)

### XBee programmer

If you intend to use the XBee module for telemetry, then you may consider buying an XBee programmer. If you have purchased the logger with XBee modem from Riverlabs, then this is not strictly necessary because in that case the XBee will have been programmed already. However, if you purchase the XBee separately, or want to change any of the default configuration, then a programmer will be necessary.

The official programmer of the XBee is the DIGI programmer. This is the recommended option, as it is fully supported by DIGI and comes with a lot of functionality. They also have an external power supply (except for the USB-C version, which provides sufficient power in itself) for the cellular modem.

If you are working with a tight budget, then cheaper options exist, such as the Seeed UARTSBee and the Sparkfun Explorer. However, these options have a specific disadvantage for the XBee Cellular 3G: they are powered by the USB port, which can only provide  up to 500mA of current. This is not sufficient for the 3G modem, and therefore the modem may not work properly. So these options are still fine to program the XBee, but you may not be able to test the cellular modem during programming (the modem will still work fine when inserted in the logger).

## Software

### Arduino IDE



### XCTU





