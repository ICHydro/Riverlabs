---
title: Subir codigo nuevo
layout: page_es
topnav: topnav_es
summary: "Esta página explica cómo subir codigo Arduino al logger"
sidebar: home_sidebar_es
permalink: upload_es.html
---

## Subir el código

### Adquisición de las bibliotecas

El código require las siguientes bibliotecas, que deben estar instalados separadamente del software Arduino:

* RTC by Makuna
* SoftwareSerial
* SDFat
* AltSoftSerial
* Rocketscream LowPower

Se puede instalar los primeros 4 a través del software Arduino, siguiendo [las instrucciones](https://www.arduino.cc/en/Guide/Libraries) en el sitio web de Arduino.

La biblioteca RocketScream LowPower no esta disponible a traves del app Arduino. Hay que bajar el codito manualmente de su [página Github](https://github.com/rocketscream/Low-Power) y ponerlo en la carpeta de las biblioteca Arduino en su maquina (vea Instalación Manual en la [pagina deinstrucciones Arduino](https://www.arduino.cc/en/Guide/Libraries)))

### Diseño energetico

El logger tiene un switch para inciar, indicado "ON (Batt) / OFF (ftdi)" en la placa. En la posición "ON", el logger recibe energía de la batería. En la posición "OFF", la entrada de poder esta conectado con el pin de poder de la conexión FTDI. Ese diseño permite programar el logger con el cable FTDI sin que hay batería insertada. Como consecuencia, el logger estara prendido cuando hay un cable FTDI esta conectado, aun cuando el switch esta en "OFF". Sin embargo no es recomendado operar el logger de esta manera porque el cable FTDI no provee suficiente poder para ciertas funciones como el sensor lidar y la telemetría. Entonces siempre opera el logger con batería.

### Instalar los drivers y conectar el logger a la computadora con un cable FTDI

Si usa el sistema operativa Windows, habrá que instalar el driver del chip FTDI, lo cual se puede bajar del (sitio web de FTDI)[https://ftdichip.com/drivers/vcp-drivers/]. Si usa Mac o Linux, típicamente no es necesario instalar ningun driver.

Conecta el cable FTDI a la entrada FTDI en el logger. Ojo: observar la dirección: el logger tiene indicaciones "GRN" para indicar el lado del hilo verde, y "BLK" para indicar el lado del hilo negro. Conecta el lado USB del cable a su computadora. Si el cable (y otro adaptador) esta conecto y el driver esta instalado correctamente, Arduino reconocerá el cable y lo presentará en el menu "ports". El nombre depende del sistama operative, pero en Windows típicamente sera "COM?" donde "?" es un number (ej. "COM3").

### Opciones del software Arduino

Escojer los sigientes opciones en Arduio (en el menu "Tools"):

* Board: Arduino Pro or Pro Mini
* Processor: Atmega328P (3.3V, 8MHz)

Escojer la puerta correcta del cable en el menu "ports". Si la opción "ports" esta marcado en gris y no hay ninguna opción disponible en este menu, revisar si el driver esta instalado correctamente, y el cable esta conectado a una puerta USB.

### Ajustar el tiempo del reloj

Este paso solo es necesario cuando la bateria de reloj CR1220 esta insertada o reemplazada. El reloj interno guardará el tiempo mientras que una bateria reloj esta insertada.

El repositorio cuenta con un script específico para poner el tiempo (set_clock.ino). Es una adaptación ligera del script "RS3231_Simple.ino" de la biblioteca Rtc by Makuna. Abrir el script, abrir tambien el Monitor de serie de Arduino, meter el "baud rate" en 115200, y aplastar el boton "upload". Si el proceso ha sido exitoso, el monitor mostrará el tiempo del registrador cada 10 segundos.

Un tutorial mas elaborado del uso del Monitor en serie en Arduino esta disponible en [Instructables](https://www.instructables.com/id/HOW-TO-use-the-ARDUINO-SERIAL-MONITOR/) (en Ingles).

NOTA 1: En su versión original el script pondrá el tiempo de su computadora, pero se puede ajustar por la zona de tiempo, por ejemplo para meter el tiempo UTC en el registrador. En ese caso hay que cambiar la siguiente linea el el script antes de subir el codido:

`#define TZ 0`

NOTA 2: El script toma en cuenta el retrazo entre complicación del codigo y su ejecución el el CPU del registrador. Ese retrazo es alrededor de 11s, pero depende de su sistema. Si quiere afinar ese parametro (para que el tiempo del registraror sea mas preciso), ajustar la siguiente linea:

`RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__) - TZ * 3600 + 11`

### Subir el codigo del registrador

Los codigos para operar el registrador estan en sus respectivas carpetas, wari y WMOnode.

Abrir en Arduino el script que corresponde a su modelo de registrador, y aplastar el botón "upload".

## Idenficar y resolver problemas (debugging)

El codigo incluye llamadas frecuentes a la función Serial.print(), la cual imprime mensajes a un interfaz serie. En el caso del modelo WMOnode, se puede usar el monitor en serie del software Arduino para leer estos mensajes, mientras que el registrador esta conectado a la computadora con el cable FTDI. Sin embargo, en el caso del modelo Wari, el sensor ultrasonido usa el mismo interfaz serie que el cable FTDI, entonces en ese caso el interfaz no esta disponible para imprimir mensajes. En este caso, el registrador usa un interfaz serie virtual (SoftSerial) contectado a los pins "debug" en la placa. Pare leer el output, se conecta el pin "GND" al hilo negro del cable FTDI, y el otro pin al hilo amarillo (serial in) del cable FTDI. Abrir el monitor de serie en el app Arduino, poner el baud rate en 115300, y aplastar el boton "reset" del registrador. Ahora apareceran los mensajes en el monitor.


## Problemas frecuentes

> Durante la compilacion, encuentro el error "This LowPower library only works on AVR processors"

Asegurar que en la opcion "board" en Arduino esta en "Arduino Pro or Pro Mini"

> Durante la subida del codigo, encuentro el error "programmer is not responding"

El software Arduino no encuentra el registrador. Asegurar que:
- El board adecuado esta seleccionado
- El cable FTDI esta conectado adecuadamente al registrador y a la computadora. Ojo la dirección de la conexión al registrador: el hilo verde debe estar al lado "GRN" y el hilo verde al lado "BLK".


