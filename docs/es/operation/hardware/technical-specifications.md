### Descripción general

La plataforma de registradores Riverlabs es compatible con muchos sensores. No obstante, las configuraciones más habituales utilizan sensores de distancia para medir sin contacto el nivel del agua en ríos y lagos. Utilizamos sensores ultrasónicos (principalmente el Maxbotix MB7389) para mediciones de corto alcance (hasta 5 m) y lidar (Garmin Lidarlite) para mediciones de mayor alcance (hasta 40 m). El sensor lidar tiene la ventaja adicional de poder instalarse inclinado respecto de la vertical, aunque es menos preciso.

Hemos utilizado satisfactoriamente registradores Riverlabs con otros sensores, como humedad del suelo, calidad del agua y pluviómetros de balancín. Algunos requieren ajustes o módulos electrónicos adicionales. No se tratan expresamente en esta guía, pero si le interesan, póngase en contacto con nosotros. A medida que maduren nuestros diseños, esperamos incluirlos aquí.

A continuación se resumen las principales características de los modelos ultrasónico y lidar:

## Especificaciones detalladas

| Especificación               | Ultrasonidos         | Lidar                       |
|------------------------------|----------------------|-----------------------------|
| **Modelo de sensor**         | Maxbotix MB7389 HRXL | Garmin Lidarlite v3HP       |
| **Rango de medición**        | 0.3 m–5 m            | 0.05 m–35 m                 |
| **Resolución**               | 1 mm                 | 1 cm                        |
| **Resolución (nivel del agua)** | 1 mm              | 1 cm                        |
| **Resolución (temperatura)** | 0.25°C               | 0.25°C                      |
| **Resolución (tensión)**     | 0.01 V               | 0.01 V                      |
| **Precisión (nivel del agua)** | ±5mm               | ± 1%                        |
| **Anchura del haz**          | Cono de ~15°         | 8 milirradianes             |
| **Ángulo de montaje**        | Preferiblemente vertical | Hasta 40° respecto de la vertical |
| **Superficie objetivo**      | Agua, objetos sólidos | La mayoría de superficies   |
| **Intervalo de temperatura** | -40°C a +65°C        | -20°C a +60°C               |
| **CPU**                      | Atmel Atmega328      | Atmel Atmega328             |


## Identificación física

### Cómo encontrar el número de modelo

La designación del modelo aparece en una etiqueta en el lateral de la carcasa, aunque también puede estar en el interior.

**Busque:**

- **Número de serie:** formato "RL0XXX" (p. ej., RL000123)
- **Tipo de sensor:** ultrasonidos (Maxbotix) o lidar (Garmin)
- **Telemetría:** compruebe si hay un módem celular XBee o una radio LoRa

### Selección del firmware según el número de serie

| Intervalo de números de serie | Sensor       | Telemetría          | Archivo de firmware        |
|-------------------------------|--------------|---------------------|----------------------------|
| RL000001–RL000277             | Ultrasonidos | Ninguna             | `wari_v1.ino`              |
| RL000001–RL000277             | Ultrasonidos | Celular 3G/LTE      | `wari_3G.ino`              |
| RL000278–RL000330             | Ultrasonidos | Ninguna             | `wari_v2.0.ino`            |
| RL000278+                     | Ultrasonidos | Celular 3G/LTE      | `wari_3G_v2.ino`           |
| RL000331+                     | Ultrasonidos | Ninguna             | `wari_v2.1.ino`            |
| Cualquiera                    | Ultrasonidos | LTE-M/NB-IoT        | `wari_4G.ino`              |
| Cualquiera                    | Lidar        | Ninguna             | `wari_lidar.ino`           |
| Cualquiera                    | Lidar        | Celular             | `wari_lidar_cellular.ino`* |
| Cualquiera                    | Lidar        | Radio LoRa          | `wari_lidar_lora.ino`      |

**\*** Establezca el tipo de módem correcto (3G, LTE-M/NB-IoT, o LTE) en la definición del compilador situada al principio del código

!!! info "Nombres anteriores"
    Si lleva tiempo utilizando nuestro repositorio de GitHub, quizá haya observado algunos cambios de nombre. Para evitar confusiones:
    - `wari_lidar.ino` se llamaba anteriormente `WMO_SD.ino`
    - `wari_lidar_cellular.ino` se llamaba anteriormente `WMOnode.ino`
    - `wari_lidar_lora.ino` se llamaba anteriormente `WMO_SD_lora.ino`
