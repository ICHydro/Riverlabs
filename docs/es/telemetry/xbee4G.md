---
title: Configuración y resolución de problemas del módem XBee 4G LTE
tags:
keywords: 
last_updated: 1 febrero 2026
summary: "Guía de configuración y resolución de problemas para módems DIGI XBee 4G LTE con integración de ThingsBoard"
sidebar: home_sidebar
permalink: xbee4G.html
---

## Resumen

Los módems DIGI XBee 4G LTE proporcionan conectividad celular utilizando redes LTE Cat 1. Esta guía cubre la configuración para su uso con registradores Riverlabs e integración con ThingsBoard Cloud.

## Configuraciones Clave

### Interfaz Serie

```
BD: 9600
NB: sin paridad
SB: un bit de parada
AP: modo API con escapes
```

### Configuración de Red

```
AN: APN (específico de la red, p.ej., "eapn1.net")
CP: Perfil del Operador (0 = Auto)
N#: Tecnología de Red (2 = LTE Cat 1)
```

### Comandos de Sueño

```
SM: pin sleep [1]
```

## Integración con ThingsBoard Cloud

### Selección del Protocolo

ThingsBoard Cloud admite múltiples protocolos para la ingesta de datos:

#### HTTP (Recomendado)
- **Puerto**: 80
- **Protocolo**: TCP (1)
- **Host**: `thingsboard.cloud`
- **Endpoint**: `/api/v1/{ACCESS_TOKEN}/telemetry`
- **Content-Type**: `application/json`

**Configuración en el código:**
```cpp
//#define COAP                    // Comentar para usar HTTP
#define HOST "thingsboard.cloud"
const uint16_t Port = 0x50;     // Puerto 80
uint8_t protocol = 1;           // TCP
```

#### CoAPS (CoAP Seguro)
- **Puerto**: 5684
- **Protocolo**: SSL sobre TCP (4)
- **Host**: `coap.thingsboard.cloud`
- **Nota**: Requiere soporte DTLS - **no recomendado** para XBee 4G ya que el SSL estándar sobre TCP no es compatible con el requisito DTLS de CoAP

**Configuración en el código:**
```cpp
#define COAP                           // Habilitar CoAP
#define HOST "coap.thingsboard.cloud"
const uint16_t Port = 0x1634;          // Puerto 5684
uint8_t protocol = 4;                  // SSL sobre TCP
```

#### CoAP Plano (No Compatible con ThingsBoard Cloud)
- **Puerto**: 5683 (UDP)
- **Nota**: ThingsBoard Cloud **no** admite CoAP plano sin cifrado

### Probar la Conexión

Pruebe su configuración de ThingsBoard con curl:

```bash
curl -v -X POST http://thingsboard.cloud/api/v1/{ACCESS_TOKEN}/telemetry \
  --header Content-Type:application/json \
  --data "{temperature:25}"
```

Reemplace `{ACCESS_TOKEN}` con el token de acceso de su dispositivo en ThingsBoard.

## Resolución de Problemas

### Códigos de Estado AI

El comando AI (Association Indication) del XBee devuelve el estado de la conexión celular:

| Código | Estado | Significado |
|--------|--------|-------------|
| 0x00 | Conectado | ✅ Conectado a internet - listo para enviar datos |
| 0x22 | Registrando | Registrándose en la red celular |
| 0x23 | Conectando | Conectándose a internet |
| 0x25 | Denegado | Registro denegado por la red |
| 0xFF | Inicializando | Módem inicializándose o escaneando redes |

**Monitorear el estado AI:**
```cpp
Serial.print(F("AI status = "));
Serial.println(AIstatus, HEX);
```

### Problemas Comunes

#### El dispositivo se conecta pero no aparecen datos en ThingsBoard

**Síntomas:**
- Estado AI = 0 (conectado)
- Dirección IP recibida correctamente
- Sin errores en la salida serie
- Sin datos en el panel de ThingsBoard

**Posibles causas:**

1. **Combinación incorrecta de protocolo/puerto**
   - CoAP definido pero usando puerto HTTP (80)
   - Modo HTTP pero usando puerto CoAP (5683 o 5684)
   - Protocolo SSL (4) usado con CoAP plano

2. **Nombre de host incorrecto**
   - Usando `thingsboard.cloud` para CoAPS (debería ser `coap.thingsboard.cloud`)
   - Usando `coap.thingsboard.cloud` para HTTP (debería ser `thingsboard.cloud`)

3. **Token de acceso incorrecto**
   - Verificar que el token coincida con su dispositivo en ThingsBoard
   - Verificar si hay espacios adicionales o caracteres

**Solución:**
- Usar el modo HTTP (recomendado) con la configuración adecuada
- Verificar que los tres parámetros coincidan: HOST, Puerto y protocolo

#### Tiempos de espera de conexión

**Síntomas:**
- "Timeout or error. Trying again next wakeup."
- Mensaje enviado pero sin respuesta

**Posibles causas:**
- Congestión de la red
- Protocolo incorrecto para el puerto
- Cortafuegos bloqueando las conexiones

**Solución:**
- Aumentar el valor de `TIMEOUT` en la configuración
- Verificar que el protocolo coincida con los requisitos del endpoint
- Verificar la configuración del APN para su operador

#### Fallo en la búsqueda DNS

**Síntomas:**
- Aparece "Sending DNS Lookup" pero no se recibe ninguna dirección IP
- Sin mensaje "Callback - IP Address"

**Posibles causas:**
- APN no configurado correctamente
- La red no permite consultas DNS
- Problema de firmware del XBee

**Solución:**
- Verificar que la configuración del APN coincida con su operador
- Intentar codificar la dirección IP en lugar del nombre de host (no recomendado para producción)
- Actualizar el firmware del XBee

### Monitorear el Estado de la Conexión

Agregar salida de depuración para rastrear el progreso de la conexión:

```cpp
#ifdef DEBUG > 0
    Serial.print(F("AI status = "));
    Serial.println(AIstatus, HEX);
    Serial.print(F("IP Address: "));
    Serial.println(IP, HEX);
#endif
```

## Formato de Datos

### Payload JSON para HTTP

```json
{
  "ts": 1769952439000,
  "values": {
    "h": -1,
    "v": 3416,
    "t": 2075
  }
}
```

Donde:
- `ts`: Marca de tiempo en milisegundos desde el epoch
- `h`: Medición de altura/distancia
- `v`: Voltaje de la batería (en centivoltios)
- `t`: Temperatura (en centígrados Celsius)

### Payload CoAP

CoAP usa el mismo formato JSON pero encapsulado en la estructura de mensajes CoAP con opciones para la ruta de la API.

## Mejores Prácticas

1. **Comenzar con HTTP** - Es más simple y confiable para las pruebas iniciales
2. **Monitorear el estado AI** - Siempre verificar el estado de la conexión antes de enviar datos
3. **Usar la salida de depuración** - Habilitar la depuración completa (`DEBUG 2`) durante el desarrollo
4. **Probar primero con curl** - Verificar que su endpoint de ThingsBoard funcione antes de probar con hardware
5. **Verificar la configuración de tiempo de espera** - Permitir suficiente tiempo para la conexión celular (predeterminado 600s)
6. **Verificar el APN** - El APN incorrecto es el error de configuración más común

## Referencias

- [Página del Producto DIGI XBee 4G LTE](https://www.digi.com/products/embedded-systems/digi-xbee/cellular-modems/xbee-cellular-lte-cat-1)
- [API HTTP de Dispositivo ThingsBoard](https://thingsboard.io/docs/reference/http-api/)
- [API CoAP de Dispositivo ThingsBoard](https://thingsboard.io/docs/reference/coap-api/)
- [Referencia de Comandos API XBee](https://www.digi.com/resources/documentation/digidocs/90001477/references)
