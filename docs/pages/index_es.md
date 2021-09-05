---
title: "Documentación para los registradores Riverlabs"
keywords: Riverlabs sensors water telemetry xbee
layout: page_es
topnav: topnav_es
summary: "Esta pagina presenta un breve resumen de la operación de los registradores Riverlabs. El resto de esta documentación provee mas detalles"
sidebar: home_sidebar_es
permalink: index_es.html
---

## Introducción

Los registradores ("loggers") Riverlabs son una serie de registradores de bajo costo para el monitoreo automatico de variables ambientales. Son compatibles con la plataforma electronica de codigo abierto Arduino, la cual provee un sistema conveniente y flexible para la programación del registrador sin la necesidad de un programador comercial. Tambien permite la adaptación del software y el hardware para objectivos específicos.

## Resumen y especificaciones

<div class="row">
    <div class="col-md-6">
        <center>
        <h3>Wari</h3>
        <img src="images/Wari_v1.jpg" alt="Registrador Wari">
        </center>
        <ul>
        <li>Sensor ultrasonido Maxbotix MB7389</li>
        <li>Rango 0.3m - 5m, resolución 1mm</li>
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
        <li>Rango entre 0.05 y 35m, resolución de 1cm</li>
        <li>CPU Atmel Atmega328</li>
        <li>Arduino Pro Mini bootloader</li>
        </ul>
    </div>
</div>

## Consideraciones energéticas

El registrador utiliza la energía provista por una batería de litio de 3.7V con factor de forma 18650. Ponga atención a la polaridad. Aunque el circuito del registrador está protegido del caso de inserción inversa de la batería, el registrador no funcionará si la batería se inserta en el sentido contrario. Este tipo de batería requiere un tipo de cargador específico como el XTAR VC2 o similar. El período de funcionamiento del registrador depende de la capacidad de la batería y la frecuencia de la medición, pero bajo un uso típico, el consumo del registrador es menor a 500 mAh por año.

El registrador también tiene una ranura para una batería de litio del tipo botón CR1220 en el lado inferior (reverso) del tablero verde. El tablero requiere ser removido de la caja para acceder a esta batería. Esta batería provee de energía de respaldo al reloj y evita que éste pierda la hora cuando se remueve la batería principal. Cuando la batería principal es insertada, el reloj usa de fuente de energía la batería principal, preservando así la energía de la batería de respaldo. Si tanto la batería principal como la de respaldo son removidas, el  reloj perderá la hora y fecha (vuelve a 01/01/2000) por lo que deberán ser reprogramados.


## Programación del registrador

<!-- The loggers can be programmed with any Arduino compatible IDE. The instructions here use the official Arduino IDE but you can also use other systems such as Platform.io. The loggers based on an Atmega CPU do not come with an USB port (to save cost) so you will need an FTDI cable or adapter.

On first use of the logger, you will need to set the clock with the set_clock.ino script. This only has to be done once, except if both batteries are removed or have run out. When using telemetry, the clock needs to be set to the UTC time zone.

The main script of the logger is wari.ino. The main settings, such as logging and telemetry frequency, can be found at the start of the script. Set them as you like, but be aware that a higher frequency of logging and telemetry will lead to a shorter battery life. -->

El registrador está basado en el sistema del microprocesador Arduino de arquitectura abierta. El código de operación y las instrucciones para programar el registrador se encuentran en la página Github.


## Telemetría

La versión del registrador que cuenta con funcionalidad de telemetría, esta equipada con un modem del tipo DIGI XBee 3G y transmita datos usando el protocolo COAP. Es necesario instalar un servidor COAP para recibir los datos a traves del internet.

El XBee debe estar configurado en modo "API mode (with escapes". Si el registrador esta comprado a traves de Riverlabs, ya será el caso. La configuración manual del XBee require el software XCTU, lo cual se puede bajar sin costo del sitio web de DIGI. Tambien se require un adaptador, o cable FTDI. Los siguientes parámatros del XBee deben estar programados:

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

El registrador no podrá comunicar con el XBee si estos parametros no estan configurados correctamente.


{% include links.html %}
