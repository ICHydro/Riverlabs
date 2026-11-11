# Guía de configuración de LoRa / LoRaWAN

!!! abstract "Descripción general"
    Esta página explica cómo configurar su dispositivo LoRaWAN con la biblioteca MCCI LMIC y conectarlo a The Things Network (TTN). Incluye el formato de los EUI, la optimización de las cargas útiles, las políticas de uso de TTN y las buenas prácticas.

## ¿Qué es LoRaWAN?

**LoRaWAN** (red de área amplia de largo alcance) es un protocolo inalámbrico de bajo consumo y largo alcance diseñado para dispositivos IoT. Es ideal para sensores alimentados por batería que envían pequeñas cantidades de datos a grandes distancias.

**Características principales:**

- **Alcance:** hasta 10–15 km con línea de visión; 2–5 km en zonas urbanas
- **Consumo:** extremadamente bajo (años de autonomía con una sola batería)
- **Velocidad de datos:** baja (0.3–50 kbps según el factor de dispersión)
- **Frecuencia:** bandas ISM sin licencia (868 MHz en la UE, 915 MHz en EE. UU.)
- **Infraestructura:** requiere una pasarela conectada al servidor de red

**LoRa frente a la red celular:**

| Característica | LoRaWAN | Red celular |
|---------------|---------|---------------------|
| **Alcance** | 2–15 km | Depende del operador |
| **Consumo** | Muy bajo | Mayor |
| **Coste** | Requiere pasarela; sin cuotas de SIM | Cuotas mensuales de SIM |
| **Velocidad de datos** | Muy baja (bytes/minuto) | Alta (MB/s) |
| **Latencia** | Mayor | Menor |
| **Adecuado para** | Actualizaciones poco frecuentes; autonomía prioritaria | Datos en tiempo real; actualizaciones frecuentes |

---

## Arquitectura del sistema

### Componentes necesarios

1. **Dispositivo final (registrador)**: registrador Riverlabs con radio LoRa
2. **Pasarela**: recibe las señales LoRa y las reenvía al servidor de red
3. **Servidor de red**: The Things Network (TTN) u otra red LoRaWAN
4. **Servidor de aplicaciones**: panel de control o base de datos (p. ej., ThingsBoard)

### Flujo de datos

```
Registrador (LoRa) → Pasarela → Servidor de red TTN → Servidor de aplicaciones
```

**Funcionamiento:**

1. El registrador envía un paquete de radio LoRa cifrado.
2. Una o varias pasarelas cercanas reciben el paquete.
3. La pasarela lo reenvía a TTN por internet.
4. TTN lo descifra, valida y dirige a su aplicación.
5. La aplicación procesa y muestra los datos.

---

## Configuración de The Things Network (TTN)

### Paso 1: Crear una cuenta de TTN

1. Vaya a la [consola de The Things Network](https://console.thethingsnetwork.org/).
2. Cree una cuenta gratuita.
3. Seleccione su región (p. ej., EU868 o US915).

### Paso 2: Registrar una pasarela

Si dispone de una pasarela propia:

1. **Haga clic en "Gateways" → "Add Gateway".**
2. **Gateway ID:** identificador único de la pasarela.
3. **Gateway EUI:** EUI del hardware (normalmente figura en la etiqueta).
4. **Frequency Plan:** debe corresponder a su región (p. ej., Europa 863-870 MHz SF9).
5. **Router:** automático.

**Opciones de pasarela:**

- **Comercial:** Dragino LPS8, Mikrotik wAP LoRa, pasarelas RAK Wireless
- **Montaje propio:** Raspberry Pi + tarjeta concentradora LoRa
- **Pública:** pasarelas comunitarias (consulte la cobertura en [TTN Mapper](https://ttnmapper.org/))

### Paso 3: Registrar una aplicación

1. **Haga clic en "Applications" → "Add Application".**
2. **Application ID:** nombre descriptivo (p. ej., `river-monitoring`).
3. **Description:** opcional.
4. **Handler:** automático.

### Paso 4: Registrar un dispositivo final

1. **En su aplicación, haga clic en "Register Device".**
2. **Device ID:** nombre único (p. ej., `wari-logger-001`).
3. **Device EUI (DevEUI):** identificador único del dispositivo.
    - Suele venir preprogramado en el módulo LoRa.
    - También puede generarse en TTN.
4. **Application Key (AppKey):** generada por TTN para la activación OTAA.
5. **Activation Method:** se recomienda OTAA (activación inalámbrica).

---

## Requisitos de formato de EUI y claves

!!! danger "Importante: el orden de los bytes importa"
    La consola de TTN emplea el formato **MSB primero** (byte más significativo primero), mientras que la biblioteca MCCI LMIC espera **LSB primero** (byte menos significativo primero) para la mayoría de los identificadores. Un orden incorrecto impedirá que el dispositivo se una a la red.

### Comparación de formatos

| Campo | Formato en la consola TTN | Formato en el código LMIC | Notas |
|-------|---------------------------|--------------------------|-------|
| **DevEUI** | MSB | LSB (invertido) | Identificador único del dispositivo |
| **AppEUI/JoinEUI** | MSB | LSB (invertido) | Identificador de aplicación o servidor de unión |
| **AppKey** | MSB | MSB (mismo orden) | Clave de cifrado de la aplicación |

### Ejemplo de conversión

**Consola TTN (MSB):**
```
DevEUI:  70 B3 D5 7E D0 00 00 11
AppEUI:  88 77 66 55 44 33 22 11
AppKey:  00 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF
```

**Código LMIC (observe que DevEUI y AppEUI están invertidos):**
```cpp
// DevEUI - INVERTIDO (LSB)
static const u1_t PROGMEM DEVEUI[8] = { 
    0x11, 0x00, 0x00, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 
};

// AppEUI (JoinEUI) - INVERTIDO (LSB)
static const u1_t PROGMEM APPEUI[8] = { 
    0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88 
};

// AppKey - MISMO ORDEN (MSB)
static const u1_t PROGMEM APPKEY[16] = { 
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF 
};
```

!!! tip "Cómo invertir los bytes fácilmente"
    Puede invertir los bytes manualmente o copiarlos de TTN en orden inverso. A algunos desarrolladores les resulta más sencillo introducir los valores de la consola TTN al revés para que coincidan con el código.

---

## Configuración de RX2

Los ajustes de RX2 (segunda ventana de recepción) deben coincidir entre el dispositivo y TTN.

**Valores predeterminados de TTN EU868:**

- **Frecuencia:** 869.525 MHz
- **Factor de dispersión:** SF9
- **Ancho de banda:** 125 kHz

**En el código LMIC:**
```cpp
LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);
// ... configurar los demás canales ...
LMIC.dn2Dr = DR_SF9;  // Velocidad de datos de RX2
```

**Verificación en TTN:**

1. Vaya a la configuración del dispositivo.
2. Consulte la sección "Network Layer".
3. Confirme la frecuencia y la velocidad de datos de RX2.

---

## Política de uso justo de TTN

!!! warning "Límites de tiempo de transmisión"
    TTN es una red comunitaria gratuita con límites de **uso justo**:
    
    - **Unos 30 segundos de tiempo de transmisión al día** por dispositivo
    - **10 mensajes descendentes al día** (del servidor al dispositivo)
    
    Superar estos límites puede provocar restricciones o el bloqueo del dispositivo.

### Cálculo del tiempo de transmisión

Depende de:

1. **Tamaño de la carga útil** (bytes)
2. **Factor de dispersión** (SF7–SF12)
3. **Ancho de banda** (normalmente 125 kHz)

**Ejemplos de tiempo de transmisión:**

| Carga útil | SF7 | SF9 | SF12 |
|------------|-----|-----|------|
| 10 bytes | 41 ms | 165 ms | 1318 ms |
| 20 bytes | 72 ms | 288 ms | 2302 ms |
| 51 bytes | 154 ms | 616 ms | 4916 ms |

**Número máximo de mensajes al día:**

| SF | Carga de 10 bytes | Carga de 20 bytes |
|----|-------------------|-------------------|
| SF7 | ~730 mensajes | ~416 mensajes |
| SF9 | ~182 mensajes | ~104 mensajes |
| SF12 | ~23 mensajes | ~13 mensajes |

!!! tip "Respete los límites"
    - Utilice SF7–SF9 cuando sea posible (un SF menor reduce el tiempo de transmisión).
    - Mantenga pequeñas las cargas útiles (normalmente 10–20 bytes).
    - Evite los mensajes ascendentes confirmados (duplican el tiempo de transmisión).
    - Envíe datos cada hora o con menor frecuencia.

---

## Selección del factor de dispersión

El **factor de dispersión (SF)** determina el equilibrio entre alcance, velocidad de datos y tiempo de transmisión.

| Nivel SF | Alcance | Velocidad de datos | Tiempo de transmisión | Adecuado para |
|----------|---------|--------------------|-----------------------|---------------|
| **SF7** | Corto | Rápida (5.5 kbps) | Muy bajo | Cerca de la pasarela; actualizaciones frecuentes |
| **SF8** | Medio | Media | Bajo | Uso equilibrado |
| **SF9** | Bueno | Moderada | Moderado | Buen equilibrio entre alcance y tiempo de transmisión |
| **SF10** | Largo | Lenta | Alto | Ubicaciones difíciles |
| **SF12** | Máximo | Muy lenta | Muy alto | Último recurso; actualizaciones poco frecuentes |

**LMIC ajusta SF automáticamente:**
La biblioteca MCCI LMIC utiliza la velocidad de datos adaptable (ADR), que ajusta automáticamente el SF según la calidad de la señal. Empiece con SF9–SF10 y deje que TTN optimice los ajustes.

**Control manual de SF:**
```cpp
// Desactivar ADR y fijar el SF
LMIC_setAdrMode(0);
LMIC_setDrTxpow(DR_SF9, 14);  // SF9, potencia de transmisión de 14 dBm
```

---

## Optimización de la carga útil

!!! success "Mantenga pequeñas las cargas útiles"
    Cargas más pequeñas = menos tiempo de transmisión = más mensajes al día = mayor duración de la batería.

### Tamaños de carga recomendados

| Factor de dispersión | Carga máxima | Carga recomendada |
|----------------------|--------------|-------------------|
| SF7 | 51 bytes | 20–30 bytes |
| SF9 | 51 bytes | 15–20 bytes |
| SF12 | 51 bytes | 10–15 bytes |

### Técnicas de optimización

**1. Usar enteros en lugar de números de coma flotante**

```cpp
// Ineficiente: 4 bytes por float
float distance = 1234.56;
payload[0-3] = distance;  // 4 bytes

// Eficiente: 2 bytes por entero
uint16_t distance_mm = 1234;  // milímetros
payload[0] = distance_mm >> 8;
payload[1] = distance_mm & 0xFF;  // 2 bytes
```

**2. Escalar valores**

```cpp
// Distancia en mm, tensión en mV
uint16_t distance_mm = 1234;  // intervalo de 0-65535 mm
uint16_t voltage_mv = 3700;   // intervalo de 0-65535 mV

// Empaquetar en un total de 4 bytes
payload[0] = distance_mm >> 8;
payload[1] = distance_mm & 0xFF;
payload[2] = voltage_mv >> 8;
payload[3] = voltage_mv & 0xFF;
```

**3. Codificación diferencial (avanzado)**

Enviar únicamente el *cambio* desde la última transmisión:

```cpp
static uint16_t last_distance = 0;
int16_t delta = current_distance - last_distance;
// Enviar delta (2 bytes) en vez del valor completo
last_distance = current_distance;
```

## Decodificador de cargas útiles de TTN

Cree una función decodificadora en TTN para convertir la carga binaria a JSON:

**En la consola de TTN:**

1. Vaya a su aplicación.
2. Haga clic en "Payload Formatters".
3. Seleccione "Custom JavaScript formatter".
4. Pegue la función:

```javascript
function Decoder(bytes, port) {
  var decoded = {};
  
  // Bytes 0-1: distancia en mm
  decoded.distance_mm = (bytes[0] << 8) | bytes[1];
  
  // Bytes 2-3: batería en mV
  decoded.battery_mv = (bytes[2] << 8) | bytes[3];
  decoded.battery_v = decoded.battery_mv / 1000.0;
  
  // Bytes 4-7: marca de tiempo
  decoded.timestamp = (bytes[4] << 24) | (bytes[5] << 16) | 
                      (bytes[6] << 8) | bytes[7];
  
  // Byte 8: temperatura (con desplazamiento de 40)
  decoded.temperature_c = bytes[8] - 40;
  
  
  return decoded;
}
```

---

## Solución de problemas de LoRaWAN

### El dispositivo no se une a la red

**Compruebe:**

1. **Orden de bytes del EUI**: ¡es el problema más habitual! Compruebe que DevEUI y AppEUI estén invertidos.
2. **Coincidencia de AppKey**: debe ser idéntica en el dispositivo y TTN.
3. **Pasarela conectada**: compruebe su estado en TTN.
4. **Plan de frecuencias**: dispositivo y pasarela deben utilizar la misma región.
5. **Antena conectada**: el dispositivo necesita la antena instalada.

### Alcance o conectividad deficientes

**Soluciones:**

- Utilice un SF más alto (SF10–SF12).
- Mejore la ubicación de la antena (vertical, elevada y con línea de visión despejada).
- Compruebe la cobertura de la pasarela en [TTN Mapper](https://ttnmapper.org/).
- Reduzca los obstáculos (edificios, árboles) entre el dispositivo y la pasarela.

### Se ha superado el límite de uso justo

**Síntomas:** los mensajes dejan de llegar tras haber funcionado correctamente al principio.

**Soluciones:**

- Reduzca la frecuencia de transmisión.
- Disminuya el tamaño de la carga útil.
- Reduzca el SF (SF7–SF9).
- Desactive los mensajes ascendentes confirmados.

---

## Siguientes pasos

- [Configuración de ThingsBoard](thingsboard-configuration.md): configurar el panel para los datos LoRaWAN
- [Guía de baterías y alimentación](../operation/hardware/battery-power-guide.md): optimizar el consumo de LoRa
- [Configuración de Arduino](../operation/quick-start/arduino-setup.md): programar el registrador LoRa

---

## Recursos

- [Consola de The Things Network](https://console.thethingsnetwork.org/)
- [Mapa de cobertura de TTN](https://ttnmapper.org/)
- [Biblioteca MCCI LMIC](https://github.com/mcci-catena/arduino-lmic)
- [Calculadora de tiempo de transmisión LoRaWAN](https://www.thethingsnetwork.org/airtime-calculator)
- [Política de uso justo de TTN](https://www.thethingsnetwork.org/docs/lorawan/duty-cycle/)
