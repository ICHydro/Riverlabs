---
title: Herramientas de hardware y software
layout: page_es
topnav: topnav_es
summary: "Esta pagina revisa los herramientas de hardware y software que se necesita para programar y operar los registradores Riverlabs"
sidebar: home_sidebar_es
permalink: start_es.html
---

## Introducción

Los registradores Riveralbs estan basados en el *bootloader* Arduino. El bootloader ya esta programado en el procesador del registrador, y permite la programación del registrador a traves de la puerta USB de una computadora, sin la necesidad de usar un programador profesional. Tambien significa que un gran rango de herramientas y software esta disponible como parte del *ecosistema* de hardware abierto Arduino. 

The Riverlabs loggers are based on the Arduino bootloader. This means that they can be programmed easily using the USB port of a PC. It also means that a very large range of tools and documentation is available as part of the Arduino open hardware ecosystem. 

## Cable FTDI

Los registradores Wari y WMOnode estan basados en un procesador Atmel328. Es un procesador de bajo costo y muy robusto, pero tiene como desventaja que no tiene interfaz USB, entonces se require un convertor del interfaz Serial del procesarod a USB. Los convertidores mas comunes están basados en el chip FTDI, y vienen como adaptador o cable. Aqui hay algunos ejemploe:

- [FTDI cable (3.3V)](https://ftdichip.com/products/ttl-232r-3v3/)
- [Sparkfun FTDI basic breakout board](https://www.sparkfun.com/products/9873)
- [Seeed UARTSBee](https://wiki.seeedstudio.com/UartSBee_V4/)

Si usa el sistema operativa Windows, habrá que instalar el driver del chip, disponible en (sitio web de FTDI)[https://ftdichip.com/drivers/vcp-drivers/].

## Software Arduino

El sistema Arduino simplifica considerablemente la programación del registrador, usando solo un cable FTDI. El ambiente de programación (IDE) Arduino es software de codigo abierto y se puede bajar sin costo del [sitio web Arduino](https://www.arduino.cc/en/software). El sitio web Arduino tambien tiene un rango muy aplio de [documentacion] (https://www.arduino.cc/en/Guide) por si quiere aprender mas sobre Arduino y el ecosistema de manuales, bibliotecas de software, y otros recursos comunicatarios.

## Herramientas opcionales

### XBee development board

Si usa un modulo XBee para la funcionalidad de telemetría, se puede considerar la compra de un [XBee Development board] (https://www.digi.com/products/models/xbib-c-smt) de DIGI, lo cual es necesario para cambiar las opciones en el XBee. Si ha comprado su registrador de Riverlabs, las opciones ya estarán programadas de manera correcta; sin embargo se se compra los modulos directamente de DIGI, el development board es necesario para grabar los valores correctos en el XBee. Sino, el registrador no reconocera el modulo.

### XCTU

Para usar el XBee development board, es necesario bajar y usar el softare XCTU, disponible sin costo del [sitio web de DIGI](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xctu).





