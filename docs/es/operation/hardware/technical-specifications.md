## Registrador Wari

### Resumen

El registrador Wari está basado en el sensor de distancia por ultrasonido Maxbotix MB7389. Debido al corto alcance de los sensores de ultrasonido (5&nbsp;m en el caso del MB7389), los registradores de ultrasonido son principalmente adecuados para la detección del nivel del agua en ríos pequeños y embalses. El Wari no tiene funcionalidad de telemetría y, por lo tanto, solo es adecuado para la recopilación de datos sin conexión con almacenamiento local en una tarjeta SD. Está diseñado para un consumo de energía ultraBajo, con una vida útil de la batería de más de 5 años en uso típico.

## Especificaciones Detalladas

### Wari (Ultrasonido)

| Especificación          | Valor                | Notas                                 |
|-------------------------|----------------------|---------------------------------------|
| **Tipo de Sensor**      | Ultrasónico          | Maxbotix MB7389 HRXL                  |
| **Rango de Medición**   | 0.3 m–5 m            | Rango práctico ~4.5 m                 |
| **Resolución**          | 1 mm                 | En condiciones ideales                |
| **Precisión**           | ±1%                  | Dependiente de la temperatura         |
| **Ancho del Haz**       | ~cono de 15°         | Requiere espacio libre de obstáculos  |
| **Ángulo de Montaje**   | Preferiblemente vertical | El montaje inclinado reduce la precisión |
| **Superficie Objetivo** | Agua, objetos sólidos | Buena reflectividad de la superficie del agua |
| **Rango de Temperatura**| -40°C a +65°C        | Rango operativo del sensor            |
| **Consumo de Energía**  | ~50 mA activo        | Bajo consumo entre lecturas           |
| **CPU**                 | Atmel Atmega328      | Compatible con Arduino Pro Mini       |


### Lidar

| Especificación          | Valor              | Notas                                |
|-------------------------|--------------------|--------------------------------------|
| **Tipo de Sensor**      | Telémetro láser    | Garmin Lidarlite v3HP                |
| **Rango de Medición**   | 0.05 m–35 m        | Depende de la reflectividad del objetivo |
| **Resolución**          | 1 cm               | Consistente en todo el rango         |
| **Precisión**           | ±2.5 cm            | Hasta 40 m                           |
| **Divergencia del Haz** | 8 milirradianes    | Muy estrecho, <1° efectivo           |
| **Ángulo de Montaje**   | Hasta 40°          | Pérdida de precisión mínima          |
| **Superficie Objetivo** | La mayoría de superficies | Mejor con agua rugosa/turbia  |
| **Rango de Temperatura**| -20°C a +60°C      | Rango operativo del sensor           |
| **Consumo de Energía**  | ~100 mA activo     | Mayor pero poco frecuente            |
| **CPU**                 | Atmel Atmega328    | Compatible con Arduino Pro Mini      |

## Identificación Física

### Encontrar el Número de Modelo

!!! tip "Ubicación del Número de Modelo"
    La designación del modelo se encuentra normalmente en una etiqueta en la **parte trasera de la carcasa** o **dentro del compartimento de la batería**.

**Buscar:**

- **Número de Serie:** formato "RL0XXX" (p.ej., RL000123)
- **Tipo de Sensor:** Ultrasonido (Maxbotix) o lidar (Garmin)
- **Telemetría:** Verificar la presencia de módem celular XBee o radio LoRa

### Selección de Firmware por Número de Serie

| Rango de Número de Serie | Sensor      | Telemetría          | Archivo de Firmware         |
|--------------------------|-------------|---------------------|-----------------------------|
| RL000001–RL000277        | Ultrasonido | Ninguna             | `wari_v1.ino`               |
| RL000001–RL000277        | Ultrasonido | 3G Celular          | `wari_3G.ino`               |
| RL000278–RL000330        | Ultrasonido | Ninguna             | `wari_v2.0.ino`             |
| RL000278+                | Ultrasonido | 3G Celular          | `wari_3G_v2.ino`            |
| RL000331+                | Ultrasonido | Ninguna             | `wari_v2.1.ino`             |
| Cualquiera               | Ultrasonido | 4G LTE-M/NB-IoT     | `wari_4G.ino`               |
| Cualquiera               | Lidar       | Ninguna             | `wari_lidar.ino`            |
| Cualquiera               | Lidar       | 3G o 4G Celular     | `wari_lidar_cellular.ino`*  |
| Cualquiera               | Lidar       | Radio LoRa          | `wari_lidar_lora.ino`       |

**\*** Establezca el tipo de módem correcto (3G o LTE-M) en la definición del compilador al inicio del código

!!! info "Nombres Legados"
    - `wari_lidar.ino` fue anteriormente `WMO_SD.ino`
    - `wari_lidar_cellular.ino` fue anteriormente `WMOnode.ino`
    - `wari_lidar_lora.ino` fue anteriormente `WMO_SD_lora.ino`

### Especificaciones Técnicas

| Especificación                   | Valor                              |
|----------------------------------|------------------------------------|
| **Rango**                        | 300 mm–5000 mm                     |
| **Resolución (nivel del agua)**  | 1 mm                               |
| **Resolución (temperatura)**     | 0.25°C                             |
| **Resolución (voltaje)**         | 0.01 V                             |
| **Precisión (nivel del agua)**   | ~5 mm                              |
| **Fuente de alimentación**       | 1 x batería Li-ion de 3.6 V (18650)|
| **Resistencia al agua**          | IP67                               |

### Distribución del PCB

![Distribución del PCB del Wari](../../assets/images/wari_layout.jpg "Distribución del PCB Wari")

### Consideraciones de Energía

## WMOnode

### Resumen

El WMOnode fue desarrollado como parte del proyecto Innovation Hub financiado por la OMM. Está basado en un sensor de distancia lidar Lidarlite v3HP con un alcance de hasta 35 m, y tiene un zócalo para un módulo de telemetría (formato XBee). Normalmente lo combinamos con un módem celular DIGI 3G para telemetría.

### Especificaciones Técnicas

| Especificación                   | Valor                              |
|----------------------------------|------------------------------------|
| **Rango**                        | 0.05 m–35 m                        |
| **Resolución (nivel del agua)**  | 1 cm                               |
| **Resolución (temperatura)**     | 0.25°C                             |
| **Resolución (voltaje)**         | 0.01 V                             |
| **Precisión (nivel del agua)**   | ~5–10 cm                           |
| **Fuente de alimentación**       | 1 x batería Li-ion de 3.6 V (18650)|
| **Resistencia al agua**          | IP67                               |

### Distribución del PCB

### Consideraciones de Energía
