# Solución de problemas de conexión de telemetría

!!! abstract "Descripción general"
    Esta guía de diagnóstico ayuda a resolver problemas de telemetría celular, desde la ausencia de conexión hasta fallos intermitentes, mediante comprobaciones sistemáticas.

## Descripción general del diagnóstico

### Método de diagnóstico

Cuando falla la telemetría, el problema puede estar en distintas capas:

```
┌──────────────────┐
│  ThingsBoard     │ ← Problemas del panel o servidor
├──────────────────┤
│  Internet        │ ← Enrutamiento, DNS y cortafuegos
├──────────────────┤
│  Red celular     │ ← Señal, APN y registro
├──────────────────┤
│  Módulo XBee     │ ← Configuración y firmware
├──────────────────┤
│ Registrador/Arduino│ ← Código y comunicación serie
├──────────────────┤
│ Alimentación/hardware│ ← Batería y conexiones
└──────────────────┘
```

**Procedimiento sistemático:**

1. Identifique el síntoma.
2. Aísle la capa afectada.
3. Pruebe el componente concreto.
4. Aplique la corrección.
5. Compruebe el resultado.

---

## Lista rápida de diagnóstico

**Empiece aquí:**

- **Alimentación:** ¿batería >3,3 V?
- **Hardware:** ¿XBee bien encajado?
- **SIM:** ¿activa e insertada correctamente?
- **Señal:** ¿RSSI superior a -105 dBm?
- **Configuración:** ¿modo API = 2?
- **Red:** ¿APN correcto?
- **Servidor:** ¿dispositivo activo en ThingsBoard?
- **Código:** ¿token de acceso correcto?

**Si todo está correcto y el fallo continúa:** siga con el diagnóstico detallado.

---

## Síntomas y soluciones habituales

### Síntoma 1: El registrador nunca transmite

**Observaciones:**

- No hay conexión de red.
- El LED del XBee nunca parpadea (si se ve).
- Monitor serie: "Network connection failed".

#### Capa 1: Comprobar el hardware

**1. Tensión de la batería**

La tensión baja provoca fallos en el XBee:

| Tensión | Estado | Acción |
|---------|--------|--------|
| >3.8 V | Buena | Continúe el diagnóstico |
| 3.5–3.8 V | Marginal | Cargue o sustituya y repita la prueba |
| <3.5 V | Demasiado baja | Sustituya la batería |

**Cómo comprobarla:**

- Mídala con un multímetro.
- O consúltela en el monitor serie.
- O en ThingsBoard (si se cargaron manualmente los datos de la SD).

**2. Instalación física del XBee**

- Retírelo e inspecciónelo:
    - ¿Están rectos los pines?
    - ¿Entran todos en el zócalo de la PCB?
    - ¿Hay pines doblados o ausentes?
- Vuelva a encajarlo con firmeza.
- Inspeccione las soldaduras si lo montó usted mismo.

**3. Tarjeta SIM**

- Retírela.
- Compruebe si está dañada.
- Verifique la orientación.
- Vuelva a insertarla hasta que encaje.
- Confirme su activación con el operador.

#### Capa 2: Red celular

**1. Intensidad de señal**

**Pruebe otra ubicación:**

- Acerque el registrador a una ventana.
- Elévelo si es posible.
- Aléjelo de estructuras metálicas.
- Pruebe con un teléfono del mismo operador en el mismo sitio.

**Señal esperada en el teléfono:**

- 2 barras o más: debería funcionar.
- 1 barra: marginal.
- 0 barras: no funcionará.

**2. Tecnología de red incompatible**

Algunos operadores solo admiten LTE-M O NB-IoT:

**Pruebe ambas:**

1. Conecte el XBee a XBee Studio.
2. Lea la configuración.
3. Consulte el valor actual de NT.
4. Cambie al otro:
    - Si NT=0 (LTE-M), cambie a NT=1 (NB-IoT).
    - Si NT=1 (NB-IoT), cambie a NT=0 (LTE-M).
5. Guarde la configuración.
6. Repita la prueba de conexión.

**3. Configuración de APN**

**Compruebe el APN:**

1. Consulte al operador de la SIM.
2. Confirme el nombre exacto (¡distingue mayúsculas y minúsculas!).
3. Confirme usuario y contraseña (normalmente vacíos).
4. Reconfigure el XBee si es necesario.

**Errores habituales:**

- Erratas en el APN.
- Espacios adicionales.
- Mayúsculas o minúsculas incorrectas.

**APN habituales:**

| Operador | APN correcto | Error habitual |
|----------|--------------|----------------|
| AT&T | `m2m.com.attwireless.com` | `attwireless.com` (incorrecto) |
| T-Mobile | `iot.t-mobile.com` | `epc.tmobile.com` (consumo, no IoT) |
| Verizon | `vzwinternet` | `vzwm2m` (antiguo) |
| Hologram | `hologram` | `hologram.io` (incorrecto) |

#### Capa 3: Configuración del XBee

**Ajustes críticos:**

1. Conéctelo a XBee Studio.
2. Lea la configuración.
3. Compruebe:

| Parámetro | Valor necesario | Si no coincide |
|-----------|-----------------|----------------|
| AP | 2 | **CRÍTICO**: debe ser 2 |
| BD | 9600 | Debe coincidir con Arduino |
| AN | [Su APN] | Consulte al operador |
| SM | 1 | Para ahorrar energía |

**Si alguno es incorrecto:**

- Corríjalo en XBee Studio.
- **Haga clic en Write.**
- Pulse Read para verificar.
- Reinicie eléctricamente el XBee.
- Vuelva a probar.

#### Capa 4: Código Arduino

**1. Inicialización del XBee**

**Salida del monitor serie** (si DEBUG está activado):

```cpp
Initializing XBee...
XBee ready           // ← Debería aparecer
```

**Si aparece "XBee timeout" o similar:**

- Compruebe la conexión serie (pines TX/RX).
- Compruebe que la velocidad coincide (9600).
- Compruebe la presencia del XBee.

**2. Lógica de conexión de red**

**Mensajes de depuración esperados:**
```cpp
Connecting to network...
Network status: 0x00
Waiting for network...
Network registered!
IP address obtained
```

**Si se queda en "Waiting for network":**

- Problema de red (véase capa 2).
- Configuración del XBee (véase capa 3).

---

### Síntoma 2: Transmisiones intermitentes

**Observaciones:**

- A veces funciona y otras falla.
- "Success rate: 60–80%".
- Lagunas aleatorias en ThingsBoard.

#### Causa 1: Variaciones de la señal

**Diagnóstico:**

- Compare el RSSI de los fallos con el de las transmisiones correctas.
- Busque patrones horarios (p. ej., congestión por la tarde).

**Soluciones:**

1. **Mejore la posición de la antena:**
    - Sitúela más alta.
    - Aléjela de obstáculos.
    - Use una antena externa si tiene una interna.

2. **Añada reintentos** (modificación del código):
   ```cpp
   int retries = 3;
   bool success = false;
   
   while(retries > 0 && !success) {
     success = sendData();
     if(!success) {
       delay(30000); // Esperar 30 s
       retries--;
     }
   }
   ```

3. **Aumente los tiempos de espera** de la comunicación XBee.

#### Causa 2: Congestión de la red

**LTE-M/NB-IoT son recursos compartidos:**

- Horas punta: más congestión.
- Eventos: posible saturación temporal de antenas.

**Soluciones:**

1. **Escalone las transmisiones** de los distintos registradores.
2. **Evite horas punta** si puede (programe entre las 2 y las 5 de la mañana).
3. **Aumente el intervalo** entre transmisiones.

#### Causa 3: Problemas de alimentación

**Caída de tensión durante la transmisión:**

El XBee consume ~200 mA al transmitir. Si la batería está débil:

**Síntomas:**

- Fallan más transmisiones al descargarse la batería.
- El porcentaje de éxito sigue la tensión.

**Solución:**

1. Sustituya antes la batería (no espere a 3,3 V).
2. Use una batería de más capacidad.
3. Reduzca las transmisiones.

**Código de diagnóstico** (comprobar la tensión antes de transmitir):
```cpp
float voltage = readBatteryVoltage();
if(voltage < 3.5) {
  // Omitir la telemetría: tensión insuficiente
  Serial.println("Battery too low for telemetry");
  return;
}
// Continuar con la transmisión
```

#### Causa 4: Tiempo de espera del servidor agotado

**Problemas de ThingsBoard o del servidor:**

**Compruebe la disponibilidad:**

```bash
# Desde el ordenador
curl -X POST \
  http://demo.thingsboard.io/api/v1/YOUR_TOKEN/telemetry \
  -H 'Content-Type: application/json' \
  -d '{"test":123}'
```

**Esperado:** `{}` (JSON vacío = éxito).

**Si hay tiempo agotado o error:**

- ThingsBoard podría estar caído (consulte su página de estado).
- Problema de enrutamiento.
- Cortafuegos que bloquea la conexión.

**Solución:**

- Espere y vuelva a intentarlo.
- Use otro servidor (Cloud en lugar de Demo).

---

### Síntoma 3: Aparecen datos, pero los valores son incorrectos

**Observaciones:**

- ThingsBoard muestra datos.
- Los valores son incorrectos, nulos o ilegibles.

#### Causa 1: Error de formato JSON

**El registrador envía:**
```json
{"distance_mm":1234,"battery_V":3.85}
```

**Las claves en ThingsBoard deben coincidir:**

- Distinguen mayúsculas: `distance_mm` ≠ `Distance_MM`.
- La ortografía importa.

**Diagnóstico:**

1. Revise el código Arduino:
   ```cpp
   String json = "{\"distance_mm\":";
   json += distance;
   json += ",\"battery_V\":";
   json += voltage;
   json += "}";
   ```

2. Compruebe los nombres de las claves en los widgets de ThingsBoard.

**Solución:**

- Corrija las claves en el código o en el panel.
- Mantenga una nomenclatura coherente.

#### Causa 2: Procesamiento en el servidor

**ThingsBoard interpreta los tipos de datos:**

**Ejemplo:**

- Enviado como texto: `"distance_mm":"1234"`.
- Se esperaba un entero: `"distance_mm":1234`.

**Solución:**

- Elimine las comillas de los valores numéricos en el código.
- Compruebe que `String(value)` no añada comillas.

#### Causa 3: Marcas de tiempo

**Si las fechas son incorrectas:**

**El registrador no envía una marca:**

- ThingsBoard utiliza la hora del servidor (normalmente correcta).

**El registrador envía una marca:**
```json
{"distance_mm":1234,"ts":1703688000000}
```

**Problema:** formato de época incorrecto.

- ThingsBoard espera milisegundos.
- El registrador envía segundos.

**Solución:**
```cpp
long timestamp = rtc.now().unixtime() * 1000L; // Convertir a milisegundos
```

---

### Síntoma 4: Costes de datos elevados

**Observaciones:**

- El consumo de datos de la SIM supera lo esperado.
- Se agota el plan.

#### Causa 1: Transmisiones demasiado frecuentes

**Calcule el consumo esperado:**

- Tamaño de una transmisión: ~500 bytes (0,5 KB).
- Transmisiones al día: 24 (cada hora).
- Consumo diario: 24 × 0,5 = 12 KB.
- Mensual: 12 KB × 30 = 360 KB.

**Si el consumo real es mucho mayor:**

- Compruebe el intervalo de transmisión en el código.
- Verifique que no transmita cada medición.

**Solución:**

- Aumente `TELEMETRY_INTERVAL` en el código.
- Equilibre la actualidad de los datos y el coste.

#### Causa 2: Reintentos tras fallos

**Cada intento consume más datos:**

- Una transmisión fallida aún genera consumo.
- Reintentar duplica los datos.
- Muchos reintentos multiplican el coste.

**Solución:**

1. Corrija la causa principal (señal o configuración).
2. Limite los reintentos a 3.
3. Añada esperas crecientes entre intentos.

#### Causa 3: Datos innecesarios en la carga

**Reduzca el tamaño del JSON:**

**Detallado (ineficiente):**
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
Tamaño: ~150 bytes.

**Compacto (eficiente):**
```json
{"d":1234,"b":3.85,"t":22.5}
```
Tamaño: ~30 bytes.

**¡Ahorro de datos de 5 veces!**

**Equilibrio:** claves compactas frente a legibilidad.

---

## Diagnóstico avanzado

### Depuración con el monitor serie

**Active la salida de depuración en Arduino:**

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

| Mensaje | Significado | Si falta |
|---------|-------------|----------|
| `XBee initializing...` | Funciona la comunicación serie | Revise las conexiones |
| `Network registered` | Conectado a la red celular | Revise SIM, APN y señal |
| `RSSI: -XX dBm` | Se conoce la intensidad de señal | Problema de comunicación XBee |
| `HTTP POST: 200 OK` | El servidor recibió datos | Revise URL y token |

### Diagnóstico de red en XBee Studio

**1. Conecte el XBee a XBee Studio.**

**2. Abra la pestaña "Cellular Network".**

**3. Consulte el estado:**

- **Registro:**
    - No registrado: sin red.
    - Registrado (red propia): correcto.
    - Registrado (itinerancia): podría funcionar (revise el plan de SIM).
    - Buscando: aún no la encuentra.

- **Señal:**
    - Valor numérico RSSI.
    - Barras de calidad (0–5).

- **Dirección IP:**
    - Asignada: conectado a la red.
    - Vacía: DHCP fallido o sin registro.

- **Consumo de datos:**
    - Algunos módulos lo registran.
    - Compárelo con el plan de SIM.

**4. Prueba de conexión:**

- Haga clic en "Ping" o "Test Connection".
- Enviará una prueba al servidor de Digi.
- Verificará la conexión completa.

**5. Comandos de diagnóstico (avanzado):**

Terminal de XBee Studio:
```
AI      // Indicación de asociación (0 = conectado)
DB      // Intensidad de señal
CI      // Estado del protocolo
MY      // Dirección IP
```

### Captura de paquetes

**Para depuración avanzada:**

**1. Instale Wireshark** en el ordenador.

**2. Capture el tráfico HTTP:**

- Conecte el XBee al ordenador.
- Ejecute el código del registrador.
- Wireshark captura el tráfico HTTP saliente.

**3. Inspeccione los paquetes:**

- Detalles de la petición POST.
- Cabeceras.
- Carga útil.
- Respuesta del servidor.

**Útil para:**
- Comprobar el formato JSON.
- Revisar las cabeceras de autenticación.
- Ver los errores reales del servidor.

---

## Referencia de códigos de error

### Códigos de error del XBee

**Parámetro AI (indicación de asociación):**

| Código | Significado | Acción |
|--------|-------------|--------|
| 0x00 | Conectado | Correcto |
| 0x22 | Registrándose en la red | Espere |
| 0x23 | Conectándose a internet | Espere |
| 0x24 | Modo de derivación activo | Revise la configuración |
| 0x25 | Inicializándose | Espere o reinicie eléctricamente |
| 0x2A | Modo avión | Desactive el modo avión |
| 0x2F | Apagado | Revise SM |

### Códigos de respuesta HTTP

**De ThingsBoard o del servidor:**

| Código | Significado | Solución |
|--------|-------------|----------|
| 200 | Correcto | Ninguna |
| 400 | Petición incorrecta | Compruebe el JSON |
| 401 | No autorizado | Compruebe el token |
| 404 | No encontrado | Revise la URL |
| 429 | Demasiadas peticiones | Reduzca la frecuencia |
| 500 | Error del servidor | Espere a ThingsBoard |
| 503 | Servicio no disponible | Espere a que vuelva el servidor |

### Errores de la biblioteca XBee Arduino

**Si usa la biblioteca XBee Arduino:**

| Error | Significado | Solución |
|-------|-------------|----------|
| XBEE_TIMEOUT | No hay respuesta | Revise conexiones y velocidad en baudios |
| XBEE_INVALID_RESPONSE | Datos dañados | Compruebe el modo API (debe ser 2) |
| XBEE_TX_FAILURE | Fallo de transmisión | Problema de red |

---

## Mantenimiento preventivo

### Comprobaciones periódicas

**Cada semana:**

- Revise la tendencia de tensión de la batería.
- Compruebe que más del 90 % de transmisiones tengan éxito.
- Revise que la intensidad de señal sea estable.

**Cada mes:**

- Compare el consumo de la SIM con el plan.
- Inspeccione el XBee (corrosión, etc.).
- Compruebe si el firmware está actualizado.

**Cada trimestre:**

- Pruebe la recogida y reconfiguración del registrador.
- Limpie las conexiones de la antena.
- Revise y actualice las rutas del servidor si hace falta.

### Optimización

**Para maximizar la fiabilidad:**

1. **Señal:**
    - Use una antena externa en zonas de cobertura marginal.
    - Eleve el registrador si es posible.

2. **Alimentación:**
    - Dimensione la batería para el doble de las transmisiones previstas.
    - Vigile la tensión y sustitúyala antes de que se agote.

3. **Intervalo de transmisión:**
    - Equilibre actualidad y autonomía.
    - Transmita más durante eventos y menos en condiciones estables.

4. **Reintentos:**
    - Máximo de 3.
    - Esperas crecientes (5 s, 30 s, 60 s).
    - Registre los fallos en la tarjeta SD.

5. **Supervisión:**
    - Configure alertas en ThingsBoard.
    - Correo si no llegan datos durante el doble del intervalo esperado.
    - Correo si la batería baja de 3,5 V.

---

## Escalado del problema

### Cuándo contactar con soporte

**Si, después de todas las comprobaciones:**
- Sigue sin haber conexión.
- Los fallos son constantes.
- El comportamiento no tiene explicación.

**Reúna esta información antes de contactar:**

1. **Datos del registrador:**
    - Modelo (Wari, Lidar).
    - Versión del firmware.
    - ID del registrador.

2. **Datos del XBee:**
    - Modelo (XB3-C-A2-XX-001).
    - Versión del firmware.
    - Configuración (exporte el perfil).

3. **SIM/operador:**
    - Nombre del operador.
    - Detalles del plan.
    - APN.

4. **Síntomas:**
    - Mensajes de error concretos.
    - Registros del monitor serie.
    - Capturas de ThingsBoard.

5. **Diagnóstico realizado:**
    - Pruebas efectuadas.
    - Resultados.

### Recursos de soporte

**Riverlabs/ICHydro:**

- Incidencias GitHub: https://github.com/ICHydro/Riverlabs/issues
- Correo: si el proyecto lo facilita.

**Soporte Digi (XBee):**

- Foros: https://www.digi.com/support/forum
- Soporte técnico: para problemas específicos de XBee.

**ThingsBoard:**

- Comunidad: https://groups.google.com/forum/#!forum/thingsboard
- Soporte profesional: planes de pago.

**Operador de la SIM:**

- Consulte a su proveedor por problemas de red.

---

## Casos prácticos

### Caso 1: Fallos intermitentes durante la noche

**Síntomas:**

- Las transmisiones funcionan de día (8:00–18:00).
- Fallan de noche (18:00–8:00).

**Investigación:**

- Señal igual de día y de noche.
- Batería suficiente.
- 100 % de éxito durante el día, 0 % de noche.

**Causa:**

- Cambio de antena del operador.
- La antena nocturna tenía otra configuración.

**Solución:**

- Se contactó con el operador.
- Se actualizó la configuración de la antena.
- Éxito superior al 95 % a cualquier hora.

### Caso 2: Llegan datos, pero con fechas incorrectas

**Síntomas:**

- ThingsBoard recibe los datos.
- Las marcas de tiempo indican 1970 (época cero).

**Causa:**

- No se ajustó el RTC del registrador.
- Envía timestamp:0.

**Solución:**

- Ajustar el RTC con `set_clock`.
- O eliminar la marca de tiempo de la carga (ThingsBoard usará la hora del servidor).

### Caso 3: Descarga rápida de la batería

**Síntomas:**

- La batería dura 5 días en lugar de los 30 previstos.
- La telemetría funciona.

**Investigación:**

- Transmite cada medición (demasiado frecuente).
- El XBee no entra correctamente en suspensión.

**Causa:**

- Modo de suspensión mal configurado.
- SM no estaba en suspensión por pin.

**Solución:**

- Configurar XBee: SM=1 (suspensión por pin).
- Aumentar TELEMETRY_INTERVAL a 12 (una transmisión cada 12 mediciones).
- Ahora la batería dura 35 días.

---

## Referencia rápida: diagrama de diagnóstico

```
¿El registrador no transmite?
├─ ¿Alimentación >3,3 V? ──No──> Sustituir batería
│   └─Sí
├─ ¿XBee bien encajado? ──No──> Volver a insertarlo
│   └─Sí
├─ ¿SIM activa? ──No──> Consultar al operador
│   └─Sí
├─ ¿Señal >-105 dBm? ──No──> Cambiar de lugar o usar antena externa
│   └─Sí
├─ ¿Modo API = 2? ──No──> Reconfigurar XBee (¡crítico!)
│   └─Sí
├─ ¿APN correcto? ──No──> Consultar al operador y reconfigurar
│   └─Sí
├─ ¿Registrado en la red? ──No──> Probar NT=0/1 y comprobar SIM
│   └─Sí
├─ ¿URL del servidor correcta? ──No──> Corregir código Arduino
│   └─Sí
├─ ¿Token válido? ──No──> Obtener otro y actualizar código
│   └─Sí
└─ Contactar con soporte y enviar el registro de diagnóstico
```

---

## Siguientes pasos

- [Guía de configuración de telemetría](setup-guide.md): visión general
- [Guía paso a paso de XBee](xbee-setup-walkthrough.md): configuración detallada
- [Configuración de ThingsBoard](thingsboard-configuration.md): preparar el panel
- [Guía de baterías y alimentación](../operation/hardware/battery-power-guide.md): optimizar el consumo

---

!!! tip "El método sistemático funciona"
    La mayoría de los problemas de telemetría se deben a un APN incorrecto, señal insuficiente o errores de configuración. Siga los pasos en orden para encontrar la causa.
