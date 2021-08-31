---
title: Especificaciones técnicas
tags:
keywords: 
layout: default_es
summary: "Especificaciones técnicas de los registradores Riverlabs"
sidebar: home_sidebar_es
permalink: es/techspec.html
---

## Registrador Wari

### Resumen

The Wari logger is based around the Maxbotix MB7389 Ultrasound distance sensor. Due to the short range of ultrasound sensors (5&nbsp;m in the case of the MB7389) ultrasound loggers are mostly suitable for water level sensing in small rivers and reservoirs. The Wari does not have telemetry functionality, and is therefore only suited for offline data collection with local storage onto an SD card. It is engineered for ultra low power consumption, with a battery life of over 5 years under typical use.


The Wari has the following characteristics:

### Especificaciones técnicas

| Rango                      | 300 mm - 5000 mm |
| Resolución (nivel de agua) | 1 mm             |
| Resolución (temperatura)   | 0.25&deg;C           |
| Resolución (voltage)       | 0.01 V           |
| Precisión (nivel de agua)  | ~ 5 mm           |
| Fuente de poder            | 1 x 3.6 V Batería Li-ion (18650) |
| Resistencia al agua        | IP67             |

### Layout del PCB

![PCB layout of the Wari](images/wari_layout.jpg "Layout del PCB Wari")

Sensor y conexión al sensor: el sensor puede ser desconectado del registrador si es necesario. Cuando los reconecte, compruebe que el sensor es conectado correctamente. En el tablero verde principal, el cable proveniente del pin vecino a la X en MAXBOTIX (pin más alejado de la batería) debe estar hacia el extremo externo del terminal conector. El sensor genera “clicks” apenas perceptibles cuando está midiendo (10 clicks un período de medición en torno a 1.5s).

Reset: el botón de reseteo puede ser oprimido en cualquier momento. El registrador simplemente recomenzará y continuará a funcionar como antes. Oprimir el botón de reseteo también traspasa los contenidos de la memoria interna a la tarjeta SD.

Tarjeta SD: para remover la tarjeta SD, simplemente tire de ella hacia afuera. Para insertarla, empuje la tarjeta cuidadosamente en la ranura hasta que esté completamente insertada. Verifique que esté correctamente insertada, con los contactos orientados hacia afuera y el tablero pequeño de color azul. Notar que el sensor almacena las mediciones en una memoria interna, y sólo traspasa a la tarjeta SD cada 24h (seteo estándar). La memoria interna puede ser traspasado a la tarjeta SD en cualquier momento oprimiendo el botón "Reset". No extraiga la tarjeta SD mientras se esté traspasando los datos de  la memoria (lo cual se indica con el LED rojo iluminándose) pues esto puede corromper la tarjeta.

LED rojo: este LED se ilumina brevemente cuando el sensor toma una medición. El LED también se ilumina cuando la memoria interna es traspasada a la tarjeta SD. Esto puede tomar varios segundos. Notar que la luz LED puede ser difícil de ver a luz de día. 

Numero de serie: reporte el número de serie cuando nos contacte por cualquier problema.


## WMOnode

### Overview

The WMOnode was developed as part of the WMO funded Innovation Hub project. It is based around a Lidarlite v3HP lidar distance sensor with a range of up to 35 m, and has a slot for a telemetry module (XBee form factor). We typically combine it with a DIGI 3G Cellular modem for telemetry.

### Technical specifications

| Range                      | 0.05 m - 35 m     |
| Resolution (water level)   | 1 cm             |
| Resolution (temperature)   | 0.25°C           |
| Resolution (voltage)       | 0.01 V           |
| Accuracy (water level)     | ~ 5 - 10 cm      |
| Power supply               | 1 x 3.6 V Li-ion battery (18650) |
| Water resistance           | IP67             |


