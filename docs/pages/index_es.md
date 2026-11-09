---
title: "Documentación para los registradores Riverlabs"
keywords: Riverlabs sensors water telemetry xbee
layout: page_es
topnav: topnav_es
summary: "Esta página presenta un breve resumen de la operación de los registradores Riverlabs. Las demás secciones de esta documentación proporcionan más detalles."
sidebar: home_sidebar_es
permalink: index_es.html
---

## Introducción

Los registradores ("loggers") Riverlabs son una serie de dispositivos de bajo costo para el monitoreo automático de variables ambientales. Son compatibles con la plataforma electrónica de código abierto Arduino, que proporciona un sistema práctico y flexible para su programación, sin necesidad de utilizar un programador comercial. Esta plataforma también permite adaptar el software y el hardware a objetivos específicos.

## Resumen y especificaciones

<div class="row">
    <div class="col-md-6">
        <center>
        <h3>Wari</h3>
        <img src="images/Wari_v1.jpg" alt="Registrador Wari">
        </center>
        <ul>
        <li>Sensor ultrasonido Maxbotix MB7389</li>
        <li>Rango: de 0.3 a 5 m; resolución: 1 mm</li>
        <li>CPU Atmel Atmega328</li>
        <li>Arduino Pro Mini bootloader</li>
        </ul>
    </div>
    <div class="col-md-6">
        <center>
        <h3>Lidar</h3>
        <img src="images/WMOnode.jpg" alt="Registrador WMOode">
        </center>
        <ul>
        <li>Sensor Garmin Lidarlite v3HP</li>
        <li>Rango: de 0.05 a 35 m; resolución: 1 cm</li>
        <li>CPU Atmel Atmega328</li>
        <li>Arduino Pro Mini bootloader</li>
        </ul>
    </div>
</div>

## Consideraciones energéticas

El registrador se alimenta mediante una batería de litio de 3.7 V de formato 18650. Preste especial atención a la polaridad al insertar la batería. Aunque el circuito está protegido contra la inversión de polaridad, el registrador no funcionará si la batería se coloca con los polos invertidos. Este tipo de batería requiere un cargador específico, como el XTAR VC2 o uno similar. El período de funcionamiento del registrador depende de la capacidad de la batería y de la frecuencia de las mediciones, pero, en condiciones típicas de uso, su consumo es inferior a 500 mAh por año.

El registrador también dispone de una ranura para una batería de litio tipo botón CR1220, situada en el reverso de la placa verde. Para acceder a esta ranura, es necesario retirar la placa de la caja. La batería CR1220 proporciona energía de respaldo al reloj y evita que este pierda la hora cuando se retira la batería principal. Cuando esta se inserta, el reloj obtiene de ella la energía necesaria para funcionar, preservando así la carga de la batería de respaldo. Si se retiran ambas baterías, el reloj perderá la fecha y la hora (volverá al 01/01/2000), por lo que será necesario configurarlas de nuevo.


## Programación del registrador

<!-- The loggers can be programmed with any Arduino compatible IDE. The instructions here use the official Arduino IDE but you can also use other systems such as Platform.io. The loggers based on an Atmega CPU do not come with an USB port (to save cost) so you will need an FTDI cable or adapter.

On first use of the logger, you will need to set the clock with the set_clock.ino script. This only has to be done once, except if both batteries are removed or have run out. When using telemetry, the clock needs to be set to the UTC time zone.

The main script of the logger is wari.ino. The main settings, such as logging and telemetry frequency, can be found at the start of the script. Set them as you like, but be aware that a higher frequency of logging and telemetry will lead to a shorter battery life. -->

El registrador está basado en la plataforma Arduino, de arquitectura abierta. El código de operación y las instrucciones para su programación se encuentran en la página de GitHub.


## Telemetría

La versión del registrador que cuenta con funcionalidad de telemetría está equipada con un módem DIGI XBee 3G y transmite datos mediante el protocolo CoAP. Es necesario instalar un servidor CoAP para recibir los datos a través de Internet.

El XBee debe estar configurado en modo "API mode (with escapes)". Si el registrador se adquiere a través de Riverlabs, esta configuración ya viene establecida. La configuración manual del XBee requiere el software XCTU, que puede descargarse gratuitamente del sitio web de DIGI. También se requiere un adaptador o cable FTDI. Los siguientes parámetros del XBee deben estar programados:

```
BD: 9600
NB: no parity
SB: one stop bit
RO: 3
TD: 0
FT: 681
AP: API mode with escapes
SM: pin sleep [1]
```

El registrador no podrá comunicarse con el XBee si estos parametros no estan configurados correctamente.


{% include links.html %}
