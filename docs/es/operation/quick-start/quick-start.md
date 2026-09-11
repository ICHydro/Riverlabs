# Guía de Inicio Rápido

## Lo Que Necesitará

Antes de comenzar, asegúrese de tener:

- Su registrador Riverlabs (ultrasónico o lidar)
- Batería principal (una 14500 o 18650 según el tipo)
- Batería de moneda CR1220 (para respaldo del RTC)
- Tarjeta MicroSD 
- Cable o adaptador FTDI (para programación)
- Arduino IDE instalado en su computadora

## 1. Identifique Su Registrador

Primero, identifique qué modelo de registrador tiene. Consulte la [Guía de Identificación del Registrador](logger-identification.md) para comparaciones detalladas.

<div class="grid cards logger-cards" markdown>

-    **Registrador Wari Ultrasónico**

    ---

    **Sensor de Distancia por Ultrasonido**

    ![Registrador Wari](../../assets/images/Wari_v1.jpg){ width="250" }

    **Sensor:** Maxbotix MB7389  
    **Rango:** 0.3 m–5 m  
    **Resolución:** 1 mm  
    **Ángulo del haz:** Amplio (~15°)  
    **Ideal para:**

    - Monitoreo del nivel del agua
    - Proyectos con presupuesto limitado
    - Aplicaciones de corto alcance
    - Posiciones de montaje vertical

-   :material-laser-pointer:{ .lg .middle } **Registrador Lidar**

    ---

    **Registrador Wari Lidar**

    ![Registrador Lidar](../../assets/images/WMOnode.jpg){ width="250" }

    **Sensor:** Garmin Lidarlite v3HP  
    **Rango:** 0.05 m–35 m  
    **Resolución:** 1 cm  
    **Ángulo del haz:** Muy estrecho (~0.5°)  
    **Ideal para:**
    
    - Mediciones de largo alcance
    - Instalaciones inclinadas (hasta 40°)
    - Aplicaciones de alta precisión
    - Situaciones de montaje difíciles

</div>

## 2. Insertar las Baterías

!!! danger "¡Verifique la Polaridad Primero!"
    Compruebe dos veces la orientación de la batería antes de insertarla. Haga coincidir las marcas **+** y **-** de la batería con las marcas en el compartimento de la batería.

**Batería RTC (CR1220):**

!!! tip "Preservación de la Fecha" 
    La celda de moneda puede estar ya instalada. Solo reemplácela si es necesario para preservar la fecha programada. Instale primero la batería principal y enciéndala.

1. Apague la placa
2. Localice el pequeño compartimento de la batería de moneda en el PCB (a veces en la parte trasera)
3. Inserte la batería CR1220 (normalmente con el lado **+** hacia arriba)
4. Presione firmemente hasta que encaje en su lugar
5. Si corresponde, atornille la placa de circuito de nuevo en la carcasa

**Batería Principal (normalmente una 18650 o 14500):**

1. Apague la placa
2. Verifique que tenga la batería correcta (consulte la [Guía de Batería y Alimentación](../hardware/battery-power-guide.md))
3. Verifique la polaridad (**+**/**-**) y la carga
4. Inserte la batería, asegurando que los clips metálicos toquen la batería

### 3. Preparar la Tarjeta SD

Su registrador puede necesitar una tarjeta microSD.

- Inserte la tarjeta en el ranura microSD del registrador
- Asegúrese de que encaje en su lugar (debería poder sentirlo)

### 4. Configurar el Reloj

!!! warning "Paso Inicial Importante"
    El reloj debe configurarse antes del primer uso. Configure el reloj en **zona horaria UTC** si utiliza telemetría.

**Antes de programar:**

!!! danger "CRÍTICO: Desconectar el Sensor Primero"
    **Para modelos Wari Ultrasónico:** Desconecte el sensor Maxbotix (conector de 3 pines) del registrador antes de programar. El sensor y el cable FTDI usan el mismo puerto serie, causando interferencias que impiden la carga exitosa del código. Vuelva a conectar el sensor después de que la programación esté completa.

**Pasos para configurar el reloj:**

1. **Desconectar el sensor** (conector blanco) si usa Wari Ultrasónico
2. Conecte el cable FTDI a su registrador
   - Alinee el cable verde con la marca "GRN"
   - Alinee el cable negro con la marca "BLK"
3. Coloque el interruptor de alimentación en posición OFF (alimentado por FTDI)
4. Abra Arduino IDE
5. Configure la placa en **MiniCore → ATmega328**
6. Configure el reloj en **External 8 MHz**
7. Cargue el sketch `set_clock.ino` (en la raíz del repositorio)
8. Cárguelo en el registrador
9. Abra el Monitor Serie (velocidad: 57600) para verificar la hora

**El reloj solo necesita configurarse una vez** (a menos que se retiren ambas baterías).

### 5. Selección de Firmware por Número de Serie

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

*\*Establezca el tipo de módem correcto (3G o LTE-M) en la definición del compilador al inicio del código*

!!! info "Nombres Legados"
    - `wari_lidar.ino` fue anteriormente `WMO_SD.ino`
    - `wari_lidar_cellular.ino` fue anteriormente `WMOnode.ino`
    - `wari_lidar_lora.ino` fue anteriormente `WMO_SD_lora.ino`


### 6. Cargar el Script Principal

Ahora cargue el script de registro principal:

!!! warning "El Sensor Debe Estar Desconectado"
    Mantenga el sensor desconectado durante este paso también (solo Wari Ultrasónico).

1. Abra el script apropiado en Arduino IDE

2. Revise y ajuste la configuración al inicio del script:
   ```cpp
   #define INTERVAL 5             // Intervalo de medición en minutos
   #define NREADINGS 10           // Número de lecturas del sensor por medición
   #define FLUSHAFTER 288         // Lecturas antes de vaciar a la tarjeta SD
   ```
3. Cargue el script en su registrador
4. **Vuelva a conectar el sensor** (conector blanco) después de que la carga se complete
5. Verifique la operación mediante el Monitor Serie (si DEBUG está habilitado)
6. Desconecte el cable FTDI
7. Coloque la alimentación en posición ON

!!! success "Carga Completa"
    Cuando el LED parpadee después de la carga, su registrador está programado y listo. ¡No olvide volver a conectar el sensor!

!!! tip "Consideraciones sobre la Batería"
    Mayor frecuencia de registro = menor vida de la batería. Comience con configuraciones conservadoras (intervalos de 10–15 minutos) para las implementaciones.

### 7. Prueba Antes de la Implementación

Antes de llevar su registrador al campo, realice una prueba de banco:

- **Sensor reconectado** (conector de 3 pines Maxbotix conectado)
- Interruptor de alimentación en posición ON
- Verificar que el LED parpadea durante las mediciones
- Verificar que los datos se están escribiendo en la tarjeta SD
- Confirmar que las lecturas del sensor son razonables
- Probar durante al menos 30 minutos para asegurar operación estable

Consulte la [Lista de Verificación del Primer Despliegue](first-deployment-checklist.md) para pruebas completas previas al despliegue.

### 8. Instalar en el Campo

¡Su registrador ya está listo para la implementación en campo! Consulte la [Guía de Montaje](../../installation/mounting-guide.md) para instrucciones detalladas de instalación, incluyendo:

- Métodos de montaje con bridas
- Técnicas de montaje con adhesivo
- Posicionamiento y orientación del sensor
- Consideraciones ambientales

## Próximos Pasos

- [Identificación del Registrador](logger-identification.md): Comparar Wari vs Lidar en detalle
- [Lista de Verificación del Primer Despliegue](first-deployment-checklist.md): Verificación completa previa al despliegue
- [Configuración de Telemetría](../../telemetry/setup-guide.md): Configurar transmisión de datos celular
- [Guía de Batería y Alimentación](../hardware/battery-power-guide.md): Comprender la vida de la batería y el voltaje

## ¿Necesita Ayuda?

- Consulte [Problemas Comunes y Soluciones](../troubleshooting/common-issues.md) para soluciones rápidas
- Revise [Diagramas de Diagnóstico](../troubleshooting/diagnostic-flowcharts.md) para resolución sistemática de problemas
- Contacte soporte: info@riverlabs.uk

---
