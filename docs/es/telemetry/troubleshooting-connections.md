# Resolución de Problemas de Conexiones de Telemetría

!!! abstract "Resumen"
    Esta es una guía de diagnóstico para resolver problemas de telemetría celular, desde la falta de conexión hasta los fallos intermitentes, con enfoques sistemáticos de resolución de problemas.

## Descripción General del Diagnóstico

### Filosofía de Resolución de Problemas

Cuando la telemetría falla, los problemas pueden ocurrir en múltiples capas:

```
┌──────────────────┐
│  ThingsBoard     │ ← Problemas del panel/servidor
├──────────────────┤
│  Internet        │ ← Enrutamiento, DNS, cortafuegos
├──────────────────┤
│  Red Celular     │ ← Señal, APN, registro
├──────────────────┤
│  Módulo XBee     │ ← Configuración, firmware
├──────────────────┤
│  Registrador/Arduino │ ← Código, comunicación serie
├──────────────────┤
│  Alimentación/Hardware │ ← Batería, conexiones
└──────────────────┘
```

**Enfoque sistemático:**

1. Identificar el síntoma
2. Aislar la capa
3. Probar el componente específico
4. Aplicar la corrección
5. Verificar la resolución

---

## Lista de Verificación Rápida de Diagnóstico

**Comenzar aquí para un triaje rápido:**

- **Alimentación:** ¿Batería >3.3 V?
- **Hardware:** ¿XBee correctamente asentado?
- **SIM:** ¿Activa e insertada correctamente?
- **Señal:** ¿RSSI mejor que -105 dBm?
- **Configuración:** ¿Modo API = 2?
- **Red:** ¿APN correcto?
- **Servidor:** ¿Dispositivo ThingsBoard activo?
- **Código:** ¿Token de acceso correcto?

**Si todo está verificado y sigue fallando:** Continuar con el diagnóstico detallado a continuación.

---

## Síntomas Comunes y Soluciones

### Síntoma 1: El Registrador Nunca Transmite

**Observaciones:**

- Sin conexión de red
- El LED del XBee nunca parpadea (si es visible)
- Monitor Serie: "Network connection failed"

#### Capa 1: Verificación de Hardware

**1. Voltaje de la Batería**

El bajo voltaje causa fallos en el XBee:

| Voltaje     | Estado    | Acción                      |
|-------------|-----------|------------------------------|
| >3.8 V      | Bueno     | Continuar diagnóstico        |
| 3.5–3.8 V  | Marginal  | Cargar/reemplazar, volver a probar |
| <3.5 V      | Demasiado bajo | Reemplazar la batería   |

**Cómo verificar:**

- Medir con multímetro
- O: El Monitor Serie muestra el voltaje
- O: ThingsBoard (si los datos de SD se cargaron manualmente)

**2. Instalación Física del XBee**

- Retirar e inspeccionar:
    - ¿Pines rectos?
    - ¿Todos los pines en contacto con el zócalo del PCB?
    - ¿Sin pines doblados/faltantes?
- Volver a asentar firmemente
- Inspección visual de las uniones de soldadura (si es ensamblaje DIY)

**3. Tarjeta SIM**

- Retirar la SIM
- Inspeccionar si hay daños
- Asegurar la orientación correcta
- Volver a insertar hasta que haga clic
- Verificar que la SIM esté activada (llamar/enviar mensaje al operador)

#### Capa 2: Red Celular

**1. Intensidad de la Señal**

**Probar la ubicación:**

- Mover el registrador cerca de una ventana
- Elevar si es posible
- Alejarse de estructuras metálicas
- Probar con el teléfono (mismo operador) en la misma ubicación

**Señal de teléfono esperada:**

- 2+ barras: Debería funcionar
- 1 barra: Marginal
- 0 barras: No funcionará

**2. Incompatibilidad de Tecnología de Red**

Algunos operadores solo admiten LTE-M O NB-IoT:

**Probar ambos:**

1. Conectar el XBee a XBee Studio
2. Leer la configuración
3. ¿Valor de NT actual?
4. Probar lo contrario:
    - Si NT=0 (LTE-M): Cambiar a NT=1 (NB-IoT)
    - Si NT=1 (NB-IoT): Cambiar a NT=0 (LTE-M)
5. Escribir la configuración
6. Volver a probar la conexión

**3. Configuración APN**

**Verificar APN:**

1. Contactar al operador de SIM
2. Confirmar el nombre del APN (¡distingue mayúsculas de minúsculas!)
3. Confirmar el nombre de usuario/contraseña (normalmente en blanco)
4. Reconfigurar el XBee si es necesario

**Errores comunes:**

- Error tipográfico en el APN
- Espacios adicionales
- Mayúsculas/minúsculas incorrectas (algunos APN distinguen mayúsculas)

**APNs Comunes:**

| Operadora | APN Correcto              | Errores Comunes                       |
|-----------|---------------------------|---------------------------------------|
| AT&T      | `m2m.com.attwireless.com` | `attwireless.com` (incorrecto)        |
| T-Mobile  | `iot.t-mobile.com`        | `epc.tmobile.com` (consumidor, no IoT)|
| Verizon   | `vzwinternet`             | `vzwm2m` (antiguo)                    |
| Hologram  | `hologram`                | `hologram.io` (incorrecto)            |

#### Capa 3: Configuración del XBee

**Configuraciones críticas:**

1. Conectar a XBee Studio
2. Leer la configuración
3. Verificar:

| Parámetro | Valor Requerido | Si es Incorrecto              |
|-----------|-----------------|-------------------------------|
| AP        | 2               | **CRÍTICO** — ¡Debe ser 2!    |
| BD        | 9600            | Debe coincidir con Arduino    |
| AN        | [Su APN]        | Verificar operador            |
| SM        | 1               | Para ahorro de energía        |

**Si algo está incorrecto:**

- Corregir en XBee Studio
- **Hacer clic en Write**
- Hacer clic en Read para verificar
- Ciclo de alimentación del XBee
- Volver a probar

#### Capa 4: Código de Arduino

**1. Inicialización del XBee**

**Salida del Monitor Serie** (si DEBUG está habilitado):

```cpp
Initializing XBee...
XBee ready           // ← Debería ver esto
```

**Si "XBee timeout" o similar:**

- Verificar la conexión serie (pines TX/RX)
- Verificar la coincidencia de velocidad de baudios (9600)
- Verificar que el XBee esté físicamente presente

**2. Lógica de Conexión de Red**

**Mensajes de depuración esperados:**
```cpp
Connecting to network...
Network status: 0x00
Waiting for network...
Network registered!
IP address obtained
```

**Si está atascado en "Waiting for network":**

- Problema de red (ver Capa 2)
- Configuración del XBee (ver Capa 3)

---

### Síntoma 2: Transmisiones Intermitentes

**Observaciones:**

- A veces funciona, a veces falla
- "Tasa de éxito: 60–80%"
- Brechas aleatorias en los datos de ThingsBoard

#### Causa 1: Fluctuaciones en la Intensidad de la Señal

**Diagnóstico:**

- Verificar el RSSI durante los fallos vs. los éxitos
- Patrón: ¿Fallos en momentos específicos? (p.ej., tardes = congestión)

**Soluciones:**

1. **Mejorar la posición de la antena:**
    - Mayor elevación
    - Alejarse de obstáculos
    - Antena externa si se usa la interna

2. **Agregar lógica de reintento** (modificación del código):
   ```cpp
   int retries = 3;
   bool success = false;
   
   while(retries > 0 && !success) {
     success = sendData();
     if(!success) {
       delay(30000); // Esperar 30 seg
       retries--;
     }
   }
   ```

3. **Aumentar los valores de tiempo de espera** en la comunicación del XBee

#### Causa 2: Congestión de la Red

**LTE-M/NB-IoT son recursos compartidos:**

- Horas pico: Más congestión
- Eventos: Sobrecarga temporal de torres

**Soluciones:**

1. **Escalonar los tiempos de transmisión** entre múltiples registradores
2. **Evitar las horas pico** si es posible (programar para las 2–5 AM)
3. **Aumentar el intervalo de transmisión** (reducir la frecuencia)

#### Causa 3: Problemas de Alimentación

**Brown-out durante la transmisión:**

El XBee consume ~200 mA durante la transmisión. Si la batería está débil:

**Síntomas:**

- Las transmisiones fallan a medida que la batería se agota
- La tasa de éxito se correlaciona con el voltaje de la batería

**Solución:**

1. Reemplazar la batería antes (no esperar hasta 3.3 V)
2. Usar batería de mayor capacidad
3. Reducir la frecuencia de transmisión

**Código de diagnóstico** (verificar el voltaje antes de transmitir):
```cpp
float voltage = readBatteryVoltage();
if(voltage < 3.5) {
  // Omitir telemetría, demasiado bajo
  Serial.println("Battery too low for telemetry");
  return;
}
// Proceder con la transmisión
```

#### Causa 4: Tiempos de Espera del Servidor

**Problemas de ThingsBoard o del servidor:**

**Probar la disponibilidad del servidor:**

```bash
# Desde la computadora
curl -X POST \
  http://demo.thingsboard.io/api/v1/SU_TOKEN/telemetry \
  -H 'Content-Type: application/json' \
  -d '{"test":123}'
```

**Esperado:** `{}` (JSON vacío = éxito)

**Si hay tiempo de espera o error:**

- El servidor ThingsBoard está caído (verificar la página de estado)
- Problema de enrutamiento de red
- Cortafuegos bloqueando

**Solución:**

- Esperar y volver a intentar
- Usar servidor alternativo (Cloud vs. Demo)

---

### Síntoma 3: Los Datos Aparecen pero los Valores son Incorrectos

**Observaciones:**

- ThingsBoard muestra datos
- Pero los valores son incorrectos, nulos o ilegibles

#### Causa 1: Error de Formato JSON

**El registrador envía:**
```json
{"distance_mm":1234,"battery_V":3.85}
```

**ThingsBoard espera que las claves coincidan:**

- Distingue mayúsculas: `distance_mm` ≠ `Distance_MM`
- La ortografía importa

**Diagnóstico:**

1. Verificar el código de Arduino:
   ```cpp
   String json = "{\"distance_mm\":";
   json += distance;
   json += ",\"battery_V\":";
   json += voltage;
   json += "}";
   ```

2. Verificar los nombres de las claves en la configuración del widget de ThingsBoard

**Solución:**

- Corregir los nombres de las claves en el código o el panel
- Nomenclatura consistente

#### Causa 2: Análisis de Datos en el Servidor

**ThingsBoard interpreta los tipos de datos:**

**Ejemplo de problema:**

- Enviado como cadena: `"distance_mm":"1234"`
- Esperado como entero: `"distance_mm":1234`

**Solución:**

- Eliminar las comillas alrededor de los valores numéricos en el código
- Asegurar que `String(value)` no agregue comillas

#### Causa 3: Problemas de Marca de Tiempo

**Si las marcas de tiempo son incorrectas:**

**El registrador no establece la marca de tiempo:**

- ThingsBoard usa la hora del servidor (normalmente correcta)

**El registrador envía la marca de tiempo:**
```json
{"distance_mm":1234,"ts":1703688000000}
```

**Problema:** Formato epoch incorrecto

- ThingsBoard espera milisegundos
- El registrador envía segundos

**Solución:**
```cpp
long timestamp = rtc.now().unixtime() * 1000L; // Convertir a milisegundos
```

---

### Síntoma 4: Costos de Datos Elevados

**Observaciones:**

- El uso de datos de la SIM es mayor de lo esperado
- El plan excede la cuota

#### Causa 1: Frecuencia de Transmisión Demasiado Alta

**Calcular el uso esperado:**

- Tamaño de transmisión: ~500 bytes (0.5 KB)
- Transmisiones por día: 24 (horario)
- Uso diario: 24 × 0.5 = 12 KB
- Mensual: 12 KB × 30 = 360 KB

**Si el uso real >> esperado:**

- Verificar el intervalo de transmisión en el código
- Verificar que no se esté transmitiendo en cada medición

**Solución:**

- Aumentar `TELEMETRY_INTERVAL` en el código
- Equilibrio: Actualidad de los datos vs. costos

#### Causa 2: Transmisiones Fallidas con Reintentos

**Cada reintento = más datos:**

- Transmisión fallida: Todavía usa datos (sobrecarga)
- Reintento: Doble de datos
- Múltiples reintentos: Multiplica los costos

**Solución:**

1. Corregir la causa raíz de los fallos (señal, configuración)
2. Limitar los reintentos en el código (máximo 3)
3. Agregar retroceso exponencial

#### Causa 3: Datos Innecesarios en el Payload

**Minimizar el tamaño del JSON:**

**Detallado (malo):**
```json
{
  "sensor_distance_millimeters": 1234,
  "battery_voltage_volts": 3.85,
  "temperature_celsius": 22.5,
  "signal_strength_dbm": -85,
  "logger_id": "Wari-001",
  "firmware_version": "1.2.3"
}
```
Tamaño: ~150 bytes

**Compacto (bueno):**
```json
{"d":1234,"b":3.85,"t":22.5}
```
Tamaño: ~30 bytes

**¡5× de ahorro de datos!**

**Equilibrio:** Claves compactas vs. legibilidad

---

## Diagnóstico Avanzado

### Uso del Monitor Serie para la Depuración

**Habilitar la salida de depuración en el código de Arduino:**

```cpp
#define DEBUG 1

#if DEBUG
  #define DEBUG_PRINT(x) Serial.print(x)
  #define DEBUG_PRINTLN(x) Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif
```

**Salida esperada:**
```
[INIT] Starting Riverlabs Logger v2.1
[INIT] RTC initialized
[INIT] SD card OK
[INIT] XBee initializing...
[XBEE] Baud: 9600, API Mode
[XBEE] Waiting for network...
[XBEE] Network registered (30s)
[XBEE] RSSI: -85 dBm
[XBEE] IP: 10.123.45.67
[SENSOR] Distance: 1234 mm, Temp: 22.5 C
[DATA] JSON: {"distance_mm":1234,"battery_V":3.85}
[XBEE] Sending to ThingsBoard...
[XBEE] HTTP POST: 200 OK
[XBEE] Transmission successful (8.5s)
[POWER] Entering sleep mode
```

**Indicadores clave:**

| Mensaje                 | Significado                         | Si falta                         |
|-------------------------|-------------------------------------|----------------------------------|
| `XBee initializing...`  | Comunicación serie funcionando      | Comprobar conexiones             |
| `Network registered`    | Celular conectado                   | Comprobar SIM, APN, señal        |
| `RSSI: -XX dBm`         | Intensidad de señal conocida        | Problema de comunicación XBee    |
| `HTTP POST: 200 OK`     | Servidor recibió datos              | Comprobar URL, token             |

### Diagnósticos de Red de XBee Studio

**1. Conectar XBee a XBee Studio**

**2. Navegar a la pestaña "Cellular Network"**

**3. Ver el estado actual:**

- **Estado de registro:**
    - Not registered: Sin red
    - Registered (home): Correcto
    - Registered (roaming): Puede funcionar (comprobar roaming en la SIM)
    - Searching: Todavía buscando

- **Intensidad de señal:**
    - Valor RSSI numérico
    - Barras de calidad (0–5)

- **Dirección IP:**
    - Si está asignada: La red está conectada
    - Si está en blanco: DHCP falló o no registrado

- **Uso de datos:**
    - Algunos módulos registran el uso
    - Comparar con el plan de la SIM

**4. Prueba de conexión:**

- Hacer clic en "Ping" o "Test Connection"
- Envía un ping al servidor de Digi
- Confirma la conectividad de extremo a extremo

**5. Comandos de diagnóstico (Avanzado):**

Terminal de XBee Studio:
```
AI      // Indicación de asociación (0 = conectado)
DB      // Intensidad de señal
CI      // Estado del protocolo
MY      // Dirección IP
```

### Captura de Paquetes

**Para depuración avanzada:**

**1. Instalar Wireshark** en el ordenador

**2. Capturar tráfico HTTP:**

- Usar XBee conectado al ordenador
- Ejecutar el código del registrador
- Wireshark captura el HTTP saliente

**3. Inspeccionar paquetes:**

- Detalles de la solicitud POST
- Cabeceras
- Carga útil
- Respuesta del servidor

**Útil para:**
- Verificar el formato JSON
- Comprobar cabeceras de autenticación
- Ver errores reales del servidor

---

## Referencia de Códigos de Error

### Códigos de Error del XBee

**Parámetro AI (Association Indication):**

| Código | Significado                  | Acción                    |
|--------|------------------------------|---------------------------|
| 0x00   | Conectado                    | ¡Correcto!                |
| 0x22   | Registrándose en la red      | Esperar                   |
| 0x23   | Conectando a internet        | Esperar                   |
| 0x24   | Modo bypass activo           | Comprobar configuración   |
| 0x25   | Inicializando                | Esperar o reiniciar       |
| 0x2A   | Modo avión                   | Desactivar modo avión     |
| 0x2F   | Apagado                      | Comprobar ajuste SM       |

### Códigos de Respuesta HTTP

**De ThingsBoard o el servidor:**

| Código | Significado          | Solución                        |
|--------|----------------------|---------------------------------|
| 200    | Éxito                | No se requiere acción           |
| 400    | Solicitud incorrecta | Comprobar formato JSON          |
| 401    | No autorizado        | Verificar token de acceso       |
| 404    | No encontrado        | Comprobar URL del servidor      |
| 429    | Demasiadas solicitudes | Reducir la frecuencia         |
| 500    | Error del servidor   | Problema de ThingsBoard, esperar |
| 503    | Servicio no disponible | Servidor caído, esperar       |

### Errores de la Librería XBee de Arduino

**Si se usa la librería XBee de Arduino:**

| Error                  | Significado          | Solución                         |
|------------------------|----------------------|----------------------------------|
| XBEE_TIMEOUT           | Sin respuesta        | Comprobar conexiones, baudios    |
| XBEE_INVALID_RESPONSE  | Datos corruptos      | Verificar modo API (debe ser 2)  |
| XBEE_TX_FAILURE        | Transmisión fallida  | Problema de red                  |

---

## Mantenimiento Preventivo

### Comprobaciones Regulares

**Semanal:**

- Comprobar la tendencia del voltaje de la batería
- Verificar que la tasa de éxito de transmisión sea >90%
- Revisar la intensidad de señal (debe ser constante)

**Mensual:**

- Comprobar el uso de datos de la SIM frente al plan
- Inspeccionar el estado físico del XBee (corrosión, etc.)
- Verificar que el firmware esté actualizado

**Trimestral:**

- Probar la recuperación y reconfiguración del registrador
- Limpiar las conexiones de la antena
- Revisar y actualizar los endpoints del servidor si es necesario

### Optimización

**Maximizar la fiabilidad:**

1. **Intensidad de señal:**
    - Usar antena externa en ubicaciones con señal marginal
    - Elevar el registrador si es posible

2. **Alimentación:**
    - Dimensionar la batería para 2× las transmisiones esperadas
    - Monitorear el voltaje, reemplazar de forma preventiva

3. **Intervalo de transmisión:**
    - Equilibrar frecuencia frente a vida de la batería
    - Más frecuente durante eventos, menos durante condiciones estables

4. **Lógica de reintentos:**
    - Máximo 3 reintentos
    - Retroceso exponencial (5 s, 30 s, 60 s)
    - Registrar fallos en la tarjeta SD

5. **Monitoreo:**
    - Configurar alertas en ThingsBoard
    - Correo en: Sin datos durante 2× el intervalo esperado
    - Correo en: Batería < 3.5 V

---

## Ruta de Escalada

### Cuándo Contactar con Soporte

**Si tras todos los diagnósticos:**
- Sigue sin haber conexión
- Fallos constantes
- Comportamiento inexplicable

**Recopilar información antes de contactar:**

1. **Detalles del registrador:**
    - Modelo (Wari, Lidar)
    - Versión de firmware
    - ID del registrador

2. **Detalles del XBee:**
    - Modelo (XB3-C-A2-XX-001)
    - Versión de firmware
    - Configuración (exportar perfil)

3. **SIM/Operador:**
    - Nombre del operador
    - Detalles del plan
    - APN

4. **Síntomas:**
    - Mensajes de error específicos
    - Registros del Monitor Serie
    - Capturas de pantalla de ThingsBoard

5. **Diagnósticos realizados:**
    - Pruebas realizadas
    - Resultados

### Recursos de Soporte

**Riverlabs/ICHydro:**

- Issues de GitHub: https://github.com/ICHydro/Riverlabs/issues
- Correo: (si lo proporciona el proyecto)

**Soporte de Digi (XBee):**

- Foros: https://www.digi.com/support/forum
- Soporte técnico: (para problemas específicos del XBee)

**ThingsBoard:**

- Comunidad: https://groups.google.com/forum/#!forum/thingsboard
- Soporte profesional: (planes de pago)

**Soporte del Operador (SIM):**

- Contactar al proveedor de la SIM para problemas de red

---

## Casos Prácticos

### Caso 1: Fallos intermitentes por la noche

**Síntomas:**

- Las transmisiones funcionan de día (8:00–18:00)
- Fallan de noche (18:00–8:00)

**Investigación:**

- Intensidad de señal: Igual de día y de noche
- Batería: Adecuada
- Patrón: 100% de éxito durante el día, 0% por la noche

**Causa raíz:**

- Cambio de antena del operador
- La antena nocturna tiene una configuración diferente

**Solución:**

- Se contactó al operador
- Configuración de la antena actualizada
- Tasa de éxito ahora >95% en todo momento

### Caso 2: Los datos aparecen pero las marcas de tiempo son incorrectas

**Síntomas:**

- ThingsBoard recibe datos
- Las marcas de tiempo muestran 1970 (epoch cero)

**Causa raíz:**

- RTC no configurado en el registrador
- El registrador envía timestamp:0

**Solución:**

- Configurar el RTC con la utilidad set_clock
- O: Eliminar la marca de tiempo del payload (ThingsBoard usa la hora del servidor)

### Caso 3: Alto consumo de batería

**Síntomas:**

- La batería se agota en 5 días (se esperaban 30 días)
- La telemetría funciona

**Investigación:**

- Frecuencia de transmisión: En cada medición (demasiado alta)
- El XBee no entra en suspensión correctamente

**Causa raíz:**

- Configuración del modo de suspensión
- SM no configurado como pin sleep

**Solución:**

- Configurar XBee: SM=1 (pin sleep)
- Aumentar TELEMETRY_INTERVAL a 12 (transmitir cada 12 mediciones)
- La batería ahora dura 35 días

---

## Referencia Rápida: Diagrama de Flujo de Resolución de Problemas

```
¿El registrador no transmite?
├─ ¿Alimentación >3.3V? ──No──> Reemplazar la batería
│   └─Sí
├─ ¿XBee bien insertado? ──No──> Reinsertar el XBee
│   └─Sí
├─ ¿SIM activa? ──No──> Contactar al operador
│   └─Sí
├─ ¿Señal >-105 dBm? ──No──> Mover el registrador o usar antena externa
│   └─Sí
├─ ¿Modo API = 2? ──No──> Reconfigurar el XBee (¡crítico!)
│   └─Sí
├─ ¿APN correcto? ──No──> Verificar con el operador, reconfigurar
│   └─Sí
├─ ¿Red registrada? ──No──> Probar NT=0/1, comprobar SIM
│   └─Sí
├─ ¿URL del servidor correcta? ──No──> Corregir el código de Arduino
│   └─Sí
├─ ¿Token válido? ──No──> Obtener nuevo token, actualizar el código
│   └─Sí
└─ Contactar con soporte con el registro de diagnóstico
```

---

## Próximos Pasos

- [Guía de configuración de telemetría](setup-guide.md): Resumen completo de la configuración
- [Guía de configuración de XBee](xbee-setup-walkthrough.md): Configuración detallada del XBee
- [Configuración de ThingsBoard](thingsboard-configuration.md): Configuración del panel
- [Guía de batería y alimentación](../operation/hardware/battery-power-guide.md): Optimización de la alimentación

---

!!! tip "El enfoque sistemático funciona"
    La mayoría de los problemas de telemetría son simples: APN incorrecto, señal débil o error de configuración. ¡Trabaja de forma metódica en los diagnósticos y encontrarás el problema!
