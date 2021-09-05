---
title: Almacenamiento de datos en tarjeta SD
layout: page_es
topnav: topnav_es
summary: "Almacenamiento local de datos en la tarjeta SD"
sidebar: home_sidebar_es
permalink: SDcard_es.html
---

## Como funciona la tarjeta SD

Los registradores Riverlabs vienen con una entrada para una tarjeta SD, lo cual es conveniente para para el almacenamiento de datos, especialmente en el caso que el registrador esta usado sin telemetría. La tarjeta SD permite guardar los datos localmente para varios años.

El codigo estandar que esta disponible en el repositorio Github ya tiene activado esta funcionalidad en los siguientes scripts:
- wari.ino
- WMOnode_SD.ino

El script WMOnode_ino, que incluye funcionalidad de telemetría, aun no tiene incluido la funcionalidad SD. Estamos trabajando en eso.

Cabe destacar que el registrador no graba los datos a la tarjeta cada vez que toma una medición. Por razones de eficiencia energética, graba las mediciones en una memoria interna tipo EEPROM de 512 kbit, y copia periodicamente los datos del EEPROM a la tarjeta SD. La frecuencia de copiar esta determinada en la siguiente linea en el codigo:

`#define FLUSHAFTER 288`

El valor predeterminado en el codigo es 288 grabaciones, lo cual equivale a una frecuencia de una vez al dia, en el caso de una frecuencia de medicion de 5 minutos.

Para forzar el copio de los ultimos datos del EEPROM a la tarjeta SD, por ejemplo si uno quiere sacar las ultimas mediciones, es suficiente resetear el registrador con el boton *reset*. Cada vez que el registrador se resetea, chequeará si hay mediciones en la memoria interna que aun no han sido grabado a la tarjeta SD, y los copiará. Durante el périodo de copiar, la luz LED estará iluminada. **Es importante nunca sacar la tarjeta SD durante el proceso de copia, porque dañara a la tarjeta SD y puede corromper todos los datos grabados en la tarjeta**. Ese proceso puede durar varios segundos, dependiendo de la cantidad de datos a copiar.

La memoria interna es no-volatil, entonces guardará los datos tambien cuando no hay batería insertada.

## Tipo de tarjeta

Cualquier tarjeta micro SD o micro SDHC puede ser utilizada, pero se recomienda una tarjeta de buena calidad, especialmente si el registrador esta usado en condiciones de temperatura extremas. La tarjeta SD tiene que estar formateada en formato FAT estándar.

## Insertar y sacar la tarjeta SD

Para insertar la tarjeta SD, empújela cuidadosamente en la ranura hasta que se encuentre completamente insertada. Verifique que se encuentra correctamente insertada, con los contactos orientados hacia el externo (alejándose de la batería).

Para remover la tarjeta SD, simplemente tírela hacia afuera. No saque la tarjeta SD mientras la luz LED roja esta iluminada.

## Formato de grabación de los datos en la tarjeta SD

En el seteo estándar, durante el período de medición, el sensor tomará 10 mediciones. La tarjeta SD puede ser leída por cualquier computador sin requerir algún software particular. El registrador escribe un archivo al día en formato texto, con el formato de nombre tipo  YYYYMMDD.CSV. El contenido del archivo presenta formato del tipo:

`2019/01/01 12:00:00, 2215, 2214, 2214, 2215, 2214, 2214, 2214, 2214, 2215, 2215, 4100, 1950`

en el cual cada línea representa un período de medición, y en donde las columnas son respectivamente:

- Columna 1: fecha y hora en formato Año/Mes/Día hora:min:seg {AAAA/MM/DD HH:MM:SS}
- Columnas 2 – 11: mediciones de distancia en milímetros [mm].
- Columna 12: voltaje de la batería en milivolts [mV]. Una batería a plena carga debiera estar a aproximadamente 4200mV. El registrador se apaga cuando el voltaje baja a un valor menor a aproximadamente 3500mV.
- Columna 13: temperatura del sensor en 1/100 grados Celsius (entonces un valor de 1950 = 19.50 C).

El registrador puede operar sin una tarjeta SD insertada, pero la memoria interna será sobre-escrita si se llena.

