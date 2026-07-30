# Guía de Configuración LoRa / LoRaWAN

!!! abstract "Resumen"
    Esta guía explica cómo configurar su dispositivo LoRaWAN usando la biblioteca MCCI LMIC y conectarlo a The Things Network (TTN). Cubre el formato de EUI, la optimización del payload, las políticas de uso de TTN y las mejores prácticas.

## ¿Qué es LoRaWAN?

**LoRaWAN** (Long Range Wide Area Network) es un protocolo inalámbrico de bajo consumo y largo alcance diseñado para dispositivos IoT. Es ideal para sensores alimentados por batería que necesitan enviar pequeñas cantidades de datos a largas distancias.

**Características Clave:**

- **Alcance:** Hasta 10–15 km en línea visual, 2–5 km en áreas urbanas
- **Energía:** Consumo de energía extremadamente bajo (años con batería)
- **Tasa de Datos:** Baja (0.3–50 kbps según el factor de dispersión)
- **Frecuencia:** Bandas ISM sin licencia (868 MHz en UE, 915 MHz en EE.UU.)
- **Infraestructura:** Requiere gateway conectado al servidor de red

**LoRa vs. Celular:**

| Característica | LoRaWAN                                   | Celular (3G/4G)                  |
|----------------|-------------------------------------------|----------------------------------|
| **Alcance**    | 2–15 km                                   | Dependiente de la operadora      |
| **Energía**    | Ultra-bajo                                | Mayor                            |
| **Costo**      | Gateway necesario, sin cuotas SIM         | Cuotas mensuales de SIM          |
| **Tasa de Datos** | Muy baja (bytes/minuto)               | Alta (MB/s)                      |
| **Latencia**   | Mayor                                     | Menor                            |
| **Mejor Para** | Actualizaciones poco frecuentes, vida crítica de la batería | Datos en tiempo real, actualizaciones frecuentes |

---

## Arquitectura del Sistema

### Componentes Necesarios

1. **Dispositivo Final (Registrador)**: Su registrador Riverlabs con radio LoRa
2. **Gateway**: Recibe señales LoRa y las reenvía al servidor de red
3. **Servidor de Red**: The Things Network (TTN) u otra red LoRaWAN
4. **Servidor de Aplicaciones**: Su panel de control/base de datos (p.ej., ThingsBoard)

### Flujo de Datos

```
Registrador (LoRa) → Gateway → Servidor de Red TTN → Servidor de Aplicaciones → Panel
```

**Cómo Funciona:**

1. El registrador envía un paquete de radio LoRa (cifrado)
2. El/los gateway(s) cercano(s) reciben el paquete
3. El gateway reenvía a TTN a través de internet
4. TTN descifra, valida y enruta a su aplicación
5. Su aplicación procesa y muestra los datos

---

## Configuración de The Things Network (TTN)

### Paso 1: Crear Cuenta en TTN

1. Ir a [The Things Network Console](https://console.thethingsnetwork.org/)
2. Crear cuenta gratuita
3. Seleccionar su región (p.ej., EU868 o US915)

### Paso 2: Registrar Gateway

Si tiene su propio gateway:

1. **Hacer clic en "Gateways" → "Add Gateway"**
2. **ID del Gateway:** Identificador único para su gateway
3. **EUI del Gateway:** EUI de hardware (normalmente en la etiqueta del gateway)
4. **Plan de Frecuencia:** Coincidir con su región (p.ej., Europe 863-870 MHz SF9)
5. **Router:** Automático

**Opciones de Gateway:**

- **Comercial:** Dragino LPS8, Mikrotik wAP LoRa, gateways RAK Wireless
- **DIY:** Raspberry Pi + tarjeta concentradora LoRa
- **Público:** Usar gateways de la comunidad (verificar [TTN Mapper](https://ttnmapper.org/) para cobertura)

### Paso 3: Registrar Aplicación

1. **Hacer clic en "Applications" → "Add Application"**
2. **ID de Aplicación:** Nombre descriptivo (p.ej., `river-monitoring`)
3. **Descripción:** Opcional
4. **Handler:** Automático

### Paso 4: Registrar Dispositivo Final

1. **En su aplicación, hacer clic en "Register Device"**
2. **ID del Dispositivo:** Nombre único (p.ej., `wari-logger-001`)
3. **EUI del Dispositivo (DevEUI):** Identificador único para su dispositivo
    - Normalmente preprogramado en el módulo LoRa
    - O generar uno en TTN
4. **Clave de Aplicación (AppKey):** Generada por TTN (para activación OTAA)
5. **Método de Activación:** OTAA (Over-The-Air Activation) recomendado

---

## Requisitos de Formato EUI y Clave

!!! danger "Crítico: El Orden de Bytes Importa"
    La consola TTN usa el formato **MSB primero** (Byte Más Significativo primero), mientras que la biblioteca MCCI LMIC espera **LSB primero** (Byte Menos Significativo primero) para la mayoría de los identificadores. Obtener esto incorrectamente impide que su dispositivo se una a la red.

### Tabla de Comparación de Formatos

| Campo              | Formato en Consola TTN | Formato en Código LMIC | Notas                      |
|--------------------|-----------------------|------------------------|----------------------------|
| **DevEUI**         | MSB                   | LSB (invertido)        | ID único del dispositivo   |
| **AppEUI/JoinEUI** | MSB                   | LSB (invertido)        | ID de aplicación/servidor  |
| **AppKey**         | MSB                   | MSB (mismo orden)      | Clave de cifrado de app    |

### Ejemplo de Conversión

**Consola TTN (MSB):**
```
DevEUI:  70 B3 D5 7E D0 00 00 11
AppEUI:  88 77 66 55 44 33 22 11
AppKey:  00 11 22 33 44 55 66 77 88 99 AA BB CC DD EE FF
```

**Código LMIC (nota: DevEUI y AppEUI invertidos):**
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

!!! tip "Inversión Fácil de Bytes"
    Puede invertir los bytes manualmente, o copiar los valores de TTN en orden inverso. Algunos desarrolladores encuentran más fácil ingresar los valores de la consola TTN en orden inverso para que coincidan con su código.

---

## Configuración de RX2

Las configuraciones de RX2 (Ventana de Recepción 2) deben coincidir entre su dispositivo y TTN.

**TTN EU868 Predeterminado:**

- **Frecuencia:** 869.525 MHz
- **Factor de Dispersión:** SF9
- **Ancho de Banda:** 125 kHz

**En Código LMIC:**
```cpp
LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7), BAND_CENTI);
// ... configurar otros canales ...
LMIC.dn2Dr = DR_SF9;  // Tasa de datos RX2
```

**Verificar en TTN:**

1. Ir a la configuración del dispositivo
2. Verificar la sección "Network Layer"
3. Confirmar la frecuencia y la tasa de datos de RX2

---

## Política de Uso Justo de TTN

!!! warning "Límites de Tiempo en el Aire"
    TTN es una red gratuita de la comunidad con límites de **uso justo**:
    
    - **~30 segundos de tiempo en el aire por día** por dispositivo
    - **10 mensajes descendentes por día** (mensajes del servidor al dispositivo)
    
    Exceder los límites puede resultar en que su dispositivo sea limitado o bloqueado.

### Cálculo del Tiempo en el Aire

El tiempo en el aire depende de:

1. **Tamaño del payload** (bytes)
2. **Factor de Dispersión** (SF7–SF12)
3. **Ancho de Banda** (generalmente 125 kHz)

**Tiempos en el Aire de Ejemplo:**

| Payload  | SF7    | SF9    | SF12    |
|----------|--------|--------|---------|
| 10 bytes | 41 ms  | 165 ms | 1318 ms |
| 20 bytes | 72 ms  | 288 ms | 2302 ms |
| 51 bytes | 154 ms | 616 ms | 4916 ms |

**Máximo de Mensajes por Día:**

| SF   | Payload de 10 bytes | Payload de 20 bytes |
|------|---------------------|---------------------|
| SF7  | ~730 mensajes       | ~416 mensajes       |
| SF9  | ~182 mensajes       | ~104 mensajes       |
| SF12 | ~23 mensajes        | ~13 mensajes        |

!!! tip "Mantenerse Dentro de los Límites"
    - Usar SF7–SF9 cuando sea posible (menor SF = menor tiempo en el aire)
    - Mantener los payloads pequeños (10–20 bytes típico)
    - Evitar los uplinks confirmados (duplican el tiempo en el aire)
    - Transmitir con una frecuencia horaria o menor

---

## Selección del Factor de Dispersión

El **Factor de Dispersión (SF)** controla el equilibrio entre alcance, tasa de datos y tiempo en el aire.

| Nivel SF | Alcance | Tasa de Datos   | Tiempo en el Aire | Mejor Para                               |
|----------|---------|-----------------|-------------------|------------------------------------------|
| **SF7**  | Corto   | Rápido (5.5 kbps) | Muy bajo        | Cerca del gateway, actualizaciones frecuentes |
| **SF8**  | Medio   | Medio           | Bajo              | Equilibrado                              |
| **SF9**  | Bueno   | Moderado        | Moderado          | Buen equilibrio alcance/tiempo en el aire |
| **SF10** | Largo   | Lento           | Alto              | Ubicaciones difíciles                    |
| **SF12** | Máximo  | Muy lento       | Muy alto          | Último recurso, actualizaciones raras    |

**LMIC Ajusta Automáticamente el SF:**
La biblioteca MCCI LMIC usa Adaptive Data Rate (ADR), que ajusta automáticamente el SF según la calidad de la señal. Comience con SF9–SF10, y deje que TTN optimice.

**Control Manual del SF:**
```cpp
// Deshabilitar ADR y establecer SF fijo
LMIC_setAdrMode(0);
LMIC_setDrTxpow(DR_SF9, 14);  // SF9, potencia de transmisión de 14dBm
```

---

## Optimización del Payload

!!! success "Mantener los Payloads Pequeños"
    Payloads más pequeños = menor tiempo en el aire = más mensajes por día = mejor vida de la batería.

### Tamaños de Payload Recomendados

| Factor de Dispersión | Payload Máximo | Payload Recomendado |
|----------------------|----------------|---------------------|
| SF7                  | 51 bytes       | 20–30 bytes         |
| SF9                  | 51 bytes       | 15–20 bytes         |
| SF12                 | 51 bytes       | 10–15 bytes         |

### Técnicas de Optimización

**1. Usar Enteros en Lugar de Flotantes**

```cpp
// Malo: 4 bytes por flotante
float distance = 1234.56;
payload[0-3] = distance;  // 4 bytes

// Bueno: 2 bytes por entero
uint16_t distance_mm = 1234;  // milímetros
payload[0] = distance_mm >> 8;
payload[1] = distance_mm & 0xFF;  // 2 bytes
```

**2. Escalar Valores**

```cpp
// Distancia en mm, voltaje en mV
uint16_t distance_mm = 1234;  // Rango 0-65535 mm
uint16_t voltage_mv = 3700;   // Rango 0-65535 mV

// Empaquetar en 4 bytes en total
payload[0] = distance_mm >> 8;
payload[1] = distance_mm & 0xFF;
payload[2] = voltage_mv >> 8;
payload[3] = voltage_mv & 0xFF;
```

**3. Codificación Delta (Avanzado)**

Solo enviar el *cambio* desde la última transmisión:

```cpp
static uint16_t last_distance = 0;
int16_t delta = current_distance - last_distance;
// Enviar delta (2 bytes) en lugar del valor completo
last_distance = current_distance;
```

## Decodificador de Payload de TTN

Crear una función de decodificador en TTN para convertir el payload binario a JSON:

**En la Consola TTN:**

1. Ir a su aplicación
2. Hacer clic en "Payload Formatters"
3. Seleccionar "Custom JavaScript formatter"
4. Pegar la función de decodificador:

```javascript
function Decoder(bytes, port) {
  var decoded = {};
  
  // Bytes 0-1: Distancia en mm
  decoded.distance_mm = (bytes[0] << 8) | bytes[1];
  
  // Bytes 2-3: Batería en mV
  decoded.battery_mv = (bytes[2] << 8) | bytes[3];
  decoded.battery_v = decoded.battery_mv / 1000.0;
  
  // Bytes 4-7: Marca de tiempo
  decoded.timestamp = (bytes[4] << 24) | (bytes[5] << 16) | 
                      (bytes[6] << 8) | bytes[7];
  
  // Byte 8: Temperatura (desplazada por 40)
  decoded.temperature_c = bytes[8] - 40;
  
  
  return decoded;
}
```

---

## Resolución de Problemas de LoRaWAN

### El Dispositivo No Se Une a la Red

**Verificar:**

1. **Orden de bytes del EUI**: ¡El problema más común! Verificar que DevEUI y AppEUI estén invertidos
2. **AppKey coincide**: Debe ser idéntico en el dispositivo y TTN
3. **Gateway en línea**: Verificar el estado del gateway en TTN
4. **Plan de frecuencia**: El dispositivo y el gateway deben usar la misma región
5. **Antena conectada**: El dispositivo necesita antena conectada

### Alcance Deficiente / Problemas de Conectividad

**Soluciones:**

- Usar SF más alto (SF10–SF12)
- Mejorar la colocación de la antena (vertical, elevada, línea visual clara)
- Verificar la cobertura del gateway en [TTN Mapper](https://ttnmapper.org/)
- Reducir los obstáculos (edificios, árboles) entre el dispositivo y el gateway

### Uso Justo Excedido

**Síntomas:** Los mensajes dejan de pasar después de haber funcionado inicialmente

**Soluciones:**

- Reducir la frecuencia de transmisión
- Disminuir el tamaño del payload
- Bajar el SF (SF7–SF9)
- Deshabilitar los uplinks confirmados

---

## Próximos Pasos

- [Configuración de ThingsBoard](thingsboard-configuration.md): Configurar el panel de control para datos LoRaWAN
- [Guía de Batería y Alimentación](../operation/hardware/battery-power-guide.md): Optimizar el consumo de energía LoRa
- [Configuración de Arduino](../operation/quick-start/arduino-setup.md): Programar su registrador LoRa

---

## Recursos

- [The Things Network Console](https://console.thethingsnetwork.org/)
- [Mapa de Cobertura TTN](https://ttnmapper.org/)
- [Biblioteca MCCI LMIC](https://github.com/mcci-catena/arduino-lmic)
- [Calculadora de Tiempo en el Aire LoRaWAN](https://www.thethingsnetwork.org/airtime-calculator)
- [Política de Uso Justo de TTN](https://www.thethingsnetwork.org/docs/lorawan/duty-cycle/)
