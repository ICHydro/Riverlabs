# Guía de Configuración de Telemetría

!!! abstract "Resumen"
    Esta guía cubre la configuración completa de la telemetría celular usando módulos XBee para la transmisión remota de datos, desde la instalación del hardware hasta la primera transmisión exitosa.

## Resumen

Los registradores Riverlabs pueden equiparse con telemetría celular para transmitir datos de forma remota, eliminando la necesidad de visitas frecuentes al sitio y habilitando el monitoreo en tiempo real.

**Lo que Necesitará:**

- Registrador Riverlabs con zócalo XBee (versión de telemetría)
- Módulo Digi XBee3 Cellular LTE-M/NB-IoT
- Tarjeta SIM activa con plan de datos
- Adaptador USB para la configuración del XBee
- Software XBee Studio (reemplaza a XCTU)
- Cuenta en ThingsBoard o plataforma IoT compatible
- Computadora para la configuración

**Tiempo Requerido:** 1–2 horas para la primera configuración

---

## Arquitectura del Sistema

### Cómo Funciona la Telemetría

```
┌─────────────┐
│  Registrador │ → Recopila datos del sensor
│  (Arduino)  │    Escribe en la tarjeta SD
└──────┬──────┘
       │
       │ UART (Serie)
       ↓
┌─────────────┐
│ Módulo XBee │ → Módem celular
│  (LTE-M)   │    Se conecta a la red
└──────┬──────┘
       │
       │ Red Celular
       ↓
┌─────────────┐
│  Internet   │
└──────┬──────┘
       │
       │ HTTP/CoAP
       ↓
┌─────────────┐
│ ThingsBoard │ → Plataforma IoT
│  o Servidor │    Almacena y muestra datos
└─────────────┘
```

### Flujo de Datos

1. **El registrador mide** los datos del sensor al intervalo configurado
2. **Arduino procesa** los datos y los almacena en la tarjeta SD
3. **Arduino envía** los datos al XBee a través de serie
4. **XBee se conecta** a la red celular (LTE-M o NB-IoT)
5. **XBee transmite** los datos al servidor configurado
6. **El servidor recibe** y almacena los datos
7. **El panel muestra** los datos al usuario

---

## Descripción General del Módulo XBee

### XBee3 Cellular LTE-M/NB-IoT

**Especificaciones:**

| Característica   | Especificación                |
|------------------|-------------------------------|
| **Tecnología**   | LTE-M (Cat-M1) y NB-IoT       |
| **Frecuencias**  | Específicas de la región (verificar modelo) |
| **Alimentación** | Operación 3.3 V–4.3 V         |
| **Consumo de Corriente** | ~200 mA durante la transmisión |
| **Factor de Forma** | Through-hole o montaje en superficie |
| **Protocolos**   | HTTP, HTTPS, CoAP, MQTT       |
| **SIM**          | Ranura Micro-SIM               |

**Números de Modelo:**

- **XB3-C-A2-UT-001**: LTE-M (EE.UU./Canadá)
- **XB3-C-A2-EU-001**: LTE-M/NB-IoT (Europa)
- **Consultar el sitio web de Digi** para su región

### XBee vs. XCTU Heredado

!!! info "XBee Studio Reemplaza a XCTU"
    Digi ha reemplazado XCTU con **XBee Studio** para módulos XBee3. Si tiene documentación más antigua que hace referencia a XCTU, los conceptos son los mismos pero la interfaz está actualizada.
    
    Descarga: [https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xbee-studio](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xbee-studio)

---

## Paso 1: Configuración de la Tarjeta SIM

### Elección de un Proveedor de Tarjeta SIM

**Requisitos:**

- Soporte LTE-M o NB-IoT (no todos los operadores lo admiten)
- Plan de datos IoT/M2M
- Buena cobertura en la ubicación de despliegue
- Tarifas de datos razonables

**Proveedores Recomendados (por región):**

**Estados Unidos:**

- AT&T IoT DataConnect
- Verizon ThingSpace
- T-Mobile IoT
- Hologram (multi-operadora)
- Soracom (multi-operadora)

**Europa:**

- Vodafone M2M
- Telefonica M2M
- 1NCE (multi-operadora, específico para IoT)
- Eseye (multi-operadora)

**Reino Unido:**

- EE M2M
- Vodafone IoT
- O2 M2M

**Global/Multi-Región:**

- Hologram
- Twilio Super SIM
- 1NCE

### Dimensionamiento del Plan de Datos

**Estimar el Uso de Datos:**

**Datos Típicos por Transmisión:**

- Datos de medición: ~50–100 bytes
- Sobrecarga HTTP: ~200–500 bytes
- Total por transmisión: ~300–600 bytes
- Factor de seguridad: Usar 1 KB por transmisión

**Cálculo de Datos Mensuales:**

```
Ejemplo: Registro cada 15 min, transmisión horaria
- Mediciones por día: 96 (cada 15 min)
- Transmisiones por día: 24 (horario)
- Datos por día: 24 × 1 KB = 24 KB
- Datos por mes: 24 KB × 30 = 720 KB ≈ 1 MB

Plan recomendado: 5-10 MB/mes
```

**Planes Típicos:**

- 1 MB/mes: ~$2–5/mes
- 10 MB/mes: ~$5–10/mes
- 100 MB/mes: ~$10–15/mes

!!! tip "Comenzar con Poco"
    Comience con el plan más pequeño. Siempre puede actualizar. La mayoría de los planes IoT no tienen cargos por exceso — simplemente dejan de funcionar.

### Instalación de la Tarjeta SIM

**Tipo de SIM:**

- Micro-SIM (3FF)

**Pasos de Instalación:**

1. **Obtener la SIM activada** del proveedor

2. **Tomar nota de la configuración del APN** (proporcionada por el operador)
    - Nombre del APN (p.ej., `m2m.com.attwireless.com`)
    - Nombre de usuario (si es necesario)
    - Contraseña (si es necesaria)

3. **Localizar la ranura de la SIM** en el módulo XBee
    - Normalmente en la parte inferior
    - Bandeja con resorte

4. **Insertar la SIM**
    - Orientación correcta (esquina con muesca)
    - Empujar hasta que haga clic
    - Asegurar que esté completamente asentada

5. **Registrar el ICCID** (número de la tarjeta SIM para seguimiento)

---

## Paso 2: Configuración del XBee

### Configuración de Hardware para la Configuración

**Necesario:**

- Módulo XBee con SIM instalada
- Adaptador USB XBee (p.ej., Digi XBee Grove Development Board o similar)
- Cable USB
- Computadora con XBee Studio instalado

**Conexión:**

1. Insertar el XBee en el adaptador USB
2. Conectar el adaptador a la computadora a través de USB
3. Esperar a que se instalen los controladores (Windows)
4. Anotar el puerto COM o dispositivo (verificar en el Administrador de Dispositivos/Preferencias del Sistema)

### Configuración de XBee Studio

#### Conexión Inicial

1. **Iniciar XBee Studio**
2. **Hacer clic en "Add Device"**
3. **Seleccionar conexión USB**
4. **Elegir el puerto COM correcto**
5. **Hacer clic en "Add Device"**
6. **XBee Studio lee la configuración del módulo**

#### Configuraciones Esenciales

Navegar por las pestañas y configurar lo siguiente:

**1. Configuración de Red**

| Configuración           | Parámetro              | Valor              | Notas                              |
|-------------------------|------------------------|--------------------|-------------------------------------|
| **Red Móvil**           | AN (Access Point Name) | [Su APN del operador] | p.ej., `m2m.com.attwireless.com` |
| **Nombre de Usuario APN**| AU                    | [Si es necesario]  | Dejar en blanco si no es necesario |
| **Contraseña APN**      | AV                     | [Si es necesario]  | Dejar en blanco si no es necesario |
| **Tecnología de Red**   | NT                     | LTE-M (o NB-IoT)   | Coincidir con su SIM               |

**2. Configuración Serie**

| Configuración   | Parámetro | Valor     | Notas                   |
|-----------------|-----------|-----------|-------------------------|
| **Velocidad de Baudios** | BD | 9600   | Debe coincidir con el código Arduino |
| **Paridad**     | NB        | Sin paridad | Estándar               |
| **Bits de Parada** | SB     | 1         | Estándar                |
| **Bits de Datos** | --      | 8         | Predeterminado          |

**3. Modo API**

| Configuración         | Parámetro | Valor | Notas                  |
|-----------------------|-----------|-------|------------------------|
| **Habilitar API**     | AP        | 2     | Modo API con escapes   |
| **Modo de Salida API**| AO        | 0     | Estándar               |

!!! danger "El Modo API es Crítico"
    El firmware del registrador espera **Modo API 2 (con escapes)**. Configurar AP=1 (sin escapes) o AP=0 (transparente) impedirá la comunicación.

**4. Gestión de Energía**

| Configuración       | Parámetro | Valor | Notas         |
|---------------------|-----------|-------|---------------|
| **Modo de Sueño**   | SM        | 1     | Pin Sleep     |
| **Opciones de Sueño** | SO      | 0     | Estándar      |

**5. Configuraciones Adicionales**

| Configuración | Parámetro | Valor | Notas                  |
|---------------|-----------|-------|------------------------|
| **Timeout**   | RO        | 3     | Timeout de caracteres  |
| **TD**        | TD        | 0     | Retraso TX             |
| **FT**        | FT        | 681   | Umbral de control de flujo |

#### Verificar la Configuración

1. **Hacer clic en "Read" para actualizar la configuración actual**
2. **Verificar que todos los parámetros coincidan con lo anterior**
3. **Hacer clic en "Write" para guardar la configuración en el módulo**
4. **Esperar confirmación**
5. **Hacer clic en "Read" nuevamente para verificar que la configuración se guardó**

### Prueba de Conexión de Red

**Probar la Conexión Celular:**

1. En XBee Studio, navegar a la sección **"Cellular Network"**
2. Hacer clic en **"Connect to Network"**
3. Esperar la conexión (puede tardar 30–60 segundos)
4. Verificar:
    - Red registrada: Sí
    - Intensidad de la señal: >-100 dBm (más fuerte es mejor)
    - Dirección IP asignada

**Resolución de Problemas de Conexión:**

| Problema               | Causa Probable              | Solución                              |
|------------------------|-----------------------------|---------------------------------------|
| No se encontró red     | SIM inactiva o APN incorrecto | Verificar con el operador, revisar el APN |
| Señal débil            | Ubicación                   | Moverse cerca de una ventana/lugar más alto |
| Tiempo de espera de conexión | Incompatibilidad de tecnología de red | Cambiar LTE-M ↔ NB-IoT |
| Autenticación fallida  | Credenciales de APN incorrectas | Verificar nombre de usuario/contraseña |

---

## Paso 3: Configuración del Código de Arduino

### Parámetros del Código

La configuración de telemetría se establece en el sketch de Arduino (p.ej., `wari_3G.ino`).

**Definiciones Clave:**

```cpp
// Configuración de Telemetría
#define TELEMETRY_INTERVAL 12 // Transmitir cada N mediciones
#define SERVER_URL "http://demo.thingsboard.io/api/v1/[SU_TOKEN]/telemetry"
// Configuración XBee (debe coincidir con la configuración del XBee)
#define XBEE_BAUD 9600
```

**Ajustar para su Configuración:**

1. **TELEMETRY_INTERVAL**
    - Con qué frecuencia transmitir
    - Ejemplo: Si se registra cada 15 min, intervalo 4 = transmitir cada hora
    - Mayor intervalo = mejor vida de la batería

2. **SERVER_URL**
    - Su endpoint de ThingsBoard o servidor
    - Incluir el token de autenticación en la URL
    - Consulte [Configuración de ThingsBoard](thingsboard-configuration.md) para detalles

3. **XBEE_BAUD**
    - Debe coincidir con la configuración BD del XBee
    - Predeterminado: 9600

### Cargar el Código

1. **Conectar el registrador** a través del cable FTDI
2. **Abrir el Arduino IDE**
3. **Cargar el sketch apropiado**
    - `wari_3G.ino` para Wari con telemetría
    - `wari_lidar_cellular.ino` para lidar con telemetría
4. **Verificar que la configuración** anterior sea correcta
5. **Compilar** (verificar errores)
6. **Cargar** en el registrador
7. **Abrir el Monitor Serie** (115200 baudios si DEBUG está habilitado)
8. **Verificar los mensajes de inicialización**

---

## Paso 4: Prueba de Integración

### Prueba de Banco Antes del Despliegue

**Configuración:**

1. Registrador con XBee instalado (tarjeta SIM en su lugar)
2. Encender con batería cargada
3. Monitor serie conectado (opcional pero recomendado)
4. Cerca de una ventana para buena señal celular

**Procedimiento de Prueba:**

1. **Encender**
    - El registrador arranca
    - Inicializa el XBee
    - El XBee se conecta a la red (observar LED)

2. **Esperar la Medición**
    - El registrador toma medición(es)
    - Almacena en la tarjeta SD
    - Prepara el paquete de telemetría

3. **Observar la Transmisión**
    - Actividad LED del XBee (si es visible)
    - Mensajes de depuración serie (si están habilitados):
        ```
        Connecting to network...
        Network connected
        Sending data...
        HTTP POST: 200 OK
        Transmission successful
        ```

4. **Verificar la Recepción**
    - Verificar el panel de ThingsBoard
    - Los datos deben aparecer en 1–2 minutos
    - Verificar la marca de tiempo y los valores

**Tiempos Esperados:**

- Arranque: 5–10 segundos
- Conexión de red: 30–60 segundos (primera vez), 5–15 segundos (posteriores)
- Transmisión de datos: 5–10 segundos
- Total: ~1 minuto por transmisión

### Resolución de Problemas de Fallos en las Pruebas

**Sin conexión de red:**

- Verificar que la SIM esté activa
- Verificar la intensidad de la señal (probar cerca de una ventana/exterior)
- Verificar la configuración del APN
- Verificar los patrones LED del XBee

**La red se conecta pero no se reciben datos:**

- Verificar que la URL del servidor sea correcta
- Verificar el token de autenticación
- Inspeccionar el Monitor Serie en busca de códigos de error
- Probar el endpoint del servidor con curl/Postman

**Datos recibidos pero valores incorrectos:**

- Verificar el análisis de datos en el servidor
- Verificar el formato JSON
- Comparar con los datos de la tarjeta SD

---

## Paso 5: Despliegue en Campo

### Lista de Verificación Previa al Despliegue

- [ ] Prueba de banco exitosa (3+ transmisiones)
- [ ] El servidor recibe y muestra los datos correctamente
- [ ] Batería completamente cargada
- [ ] Intensidad de la señal aceptable en el sitio de despliegue (probar con el teléfono)
- [ ] Intervalo de telemetría apropiado para la vida de la batería
- [ ] Respaldo de tarjeta SD funcionando (en caso de que la telemetría falle)
- [ ] Antena segura y correctamente orientada

### Prueba de Señal en el Sitio

Antes de la instalación permanente:

1. **Traer el teléfono con el mismo operador** que la SIM
2. **Verificar la intensidad de la señal** en la ubicación de instalación
3. **Considerar la posición de la antena:**
    - La orientación vertical suele ser mejor
    - Mayor altura = mejor señal
    - Alejarse de obstrucciones metálicas
    - Se prefiere vista despejada del cielo

**Requisitos Mínimos de Señal:**

- LTE-M/NB-IoT: Mínimo -110 dBm, bueno -90 dBm
- Las señales más débiles funcionan pero drenan más la batería

### Verificación Post-Instalación

**En la Primera Hora:**

- [ ] Primera transmisión recibida
- [ ] Los datos aparecen en el panel
- [ ] Valores razonables

**En las Primeras 24 Horas:**

- [ ] Transmisiones regulares ocurriendo
- [ ] Sin brechas en los datos
- [ ] Voltaje de la batería estable
- [ ] Intensidad de la señal consistente

**Primera Semana:**

- [ ] Revisión diaria de datos
- [ ] Monitorear la tasa de drenaje de la batería
- [ ] Observar si hay fallos de transmisión
- [ ] Ajustar el intervalo si es necesario

---

## Consideraciones de Energía

### Presupuesto de Energía de Telemetría

**Consumo de Energía del XBee:**

- Sueño: <1 mA
- Inactivo conectado: ~10 mA
- Transmisión: ~200 mA (5–10 segundos)

**Impacto en la Vida de la Batería:**

Sin telemetría (solo SD):

- Wari: ~30–60 días (registro de 15 min)
- Lidar: ~15–30 días (registro de 15 min)

Con telemetría (transmisión horaria):

- Wari: ~20–40 días (registro de 15 min)
- Lidar: ~10–20 días (registro de 15 min)

**Estrategias de Optimización:**

1. **Reducir la frecuencia de transmisión**
    - Medir cada 15 min, transmitir cada hora
    - Mejora de 4× la vida de la batería frente a transmitir en cada medición

2. **Transmisiones programadas**
    - Transmitir una o dos veces al día
    - Ahorro masivo de energía
    - La tarjeta SD almacena datos a resolución completa

3. **Transmisión adaptativa**
    - Transmitir con frecuencia durante eventos
    - Reducir durante condiciones estables
    - Requiere código más complejo

Consultar la [Guía de batería y alimentación](../operation/hardware/battery-power-guide.md) para cálculos detallados.

---

## Consideraciones de Seguridad

### Seguridad de los Datos

**Seguridad en el transporte:**

- Usar HTTPS en lugar de HTTP cuando sea posible
- ThingsBoard admite TLS
- Cierta sobrecarga en datos y procesamiento

**Autenticación:**

- Tokens de dispositivo (ThingsBoard)
- Claves de API
- Nunca codificar credenciales en el código si este es público

**Seguridad de la SIM:**

- Usar APN de IP fija si está disponible
- Protección con PIN de la SIM
- Monitorear un uso de datos inusual

### Seguridad Física

- El XBee y la antena son objetivos valiosos para el robo
- Considerar ocultarlos o usar carcasas protectoras
- Documentar los números de serie para seguimiento

---

## Monitoreo y Mantenimiento

### Qué Monitorear

**Diario (alertas automatizadas):**

- [ ] Datos recibidos en las últimas 24 horas
- [ ] Tendencia del voltaje de la batería
- [ ] Intensidad de la señal

**Semanal:**

- [ ] Tasa de éxito de transmisión
- [ ] Calidad de los datos
- [ ] Uso de datos de la SIM

**Mensual:**

- [ ] Estado de la batería
- [ ] Datos restantes del plan
- [ ] Actualizaciones de firmware disponibles

### Problemas Comunes de Telemetría

Consultar [Resolución de problemas de conexión](troubleshooting-connections.md) para diagnósticos detallados.

**Referencia rápida:**

| Síntoma               | Verificación rápida             | Solución probable                    |
|-----------------------|---------------------------------|--------------------------------------|
| Sin transmisiones     | ¿SIM activa? ¿Señal?            | Comprobar estado del operador, mover el registrador |
| Intermitente          | ¿Varía la intensidad de señal?  | Mejorar la posición de la antena     |
| Alto consumo de batería | ¿Transmisión demasiado frecuente? | Reducir la frecuencia             |
| Datos corruptos       | ¿Problema de análisis en servidor? | Verificar formato JSON            |

---

## Plataformas Alternativas

Aunque esta guía se centra en ThingsBoard, los módulos XBee3 admiten varias plataformas:

- **Brokers MQTT** (Mosquitto, HiveMQ, AWS IoT)
- **APIs REST HTTP** (servidores personalizados)
- **Servidores CoAP**
- **Azure IoT Hub**
- **AWS IoT Core**
- **Google Cloud IoT**

Los principios de configuración son similares; consultar la documentación de la plataforma para los endpoints y la autenticación.

---

## Resumen de Costos

**Costos únicos:**

- Módulo XBee3 Cellular: $60–80
- Adaptador USB para configuración: $20–30
- Activación de la SIM: $0–20

**Costos recurrentes:**

- Plan de datos: $2–15/mes
- Alojamiento del servidor (si no se usa el nivel gratuito de ThingsBoard): $0–10/mes

**Total primer año:**

- Hardware: ~$100
- Servicio: ~$50–180
- **Total: ~$150–280**

Comparar con:

- Costos de visitas al sitio (combustible, tiempo)
- Brechas de datos por visitas poco frecuentes
- Valor del monitoreo en tiempo real

---

## Próximos Pasos

- [Configuración de ThingsBoard](thingsboard-configuration.md): Configurar el panel
- [Guía de configuración de XBee](xbee-setup-walkthrough.md): Guía detallada de XBee Studio con capturas de pantalla
- [Resolución de problemas de conexión](troubleshooting-connections.md): Solucionar problemas de telemetría

---

!!! success "¡Telemetría configurada!"
    Con la telemetría celular funcionando, ahora tienes monitoreo remoto en tiempo real de tu registrador. ¡No más visitas al sitio solo para comprobar si está funcionando!
