---
title: Configuración y solución de problemas del módem Xbee 4G LTE
tags:
keywords: 
last_updated: 1 de febrero de 2026
summary: "Guía de configuración y solución de problemas de los módems DIGI XBee 4G LTE con integración en ThingsBoard"
sidebar: home_sidebar
permalink: xbee4G.html
---

## Descripción general

Los módems DIGI XBee 4G LTE proporcionan conectividad celular mediante redes LTE Cat 1. Esta guía explica cómo configurarlos para utilizarlos con registradores Riverlabs e integrarlos con ThingsBoard Cloud.

## Ajustes principales

### Interfaz serie

```
BD: 9600
NB: sin paridad
SB: un bit de parada
AP: modo API con caracteres de escape
```

### Configuración de red

```
AN: APN (específico de la red; p. ej., "eapn1.net")
CP: perfil del operador (0 = automático)
N#: tecnología de red (2 = LTE Cat 1)
```

### Comandos de suspensión

```
SM: suspensión por pin [1]
```

## Integración con ThingsBoard Cloud

### Selección del protocolo

ThingsBoard Cloud admite varios protocolos para recibir datos:

#### HTTP (recomendado)
- **Puerto**: 80
- **Protocolo**: TCP (1)
- **Servidor**: `thingsboard.cloud`
- **Ruta**: `/api/v1/{ACCESS_TOKEN}/telemetry`
- **Content-Type**: `application/json`

**Configuración en el código:**
```cpp
//#define COAP                    // Mantener comentado para usar HTTP
#define HOST "thingsboard.cloud"
const uint16_t Port = 0x50;     // Puerto 80
uint8_t protocol = 1;           // TCP
```

#### CoAPS (CoAP seguro)
- **Puerto**: 5684
- **Protocolo**: SSL sobre TCP (4)
- **Servidor**: `coap.thingsboard.cloud`
- **Nota**: requiere compatibilidad con DTLS; **no se recomienda** para XBee 4G, ya que SSL estándar sobre TCP no es compatible con el requisito DTLS de CoAP.

**Configuración en el código:**
```cpp
#define COAP                           // Activar CoAP
#define HOST "coap.thingsboard.cloud"
const uint16_t Port = 0x1634;          // Puerto 5684
uint8_t protocol = 4;                  // SSL sobre TCP
```

#### CoAP sin cifrar (no admitido por ThingsBoard Cloud)
- **Puerto**: 5683 (UDP)
- **Nota**: ThingsBoard Cloud **no** admite CoAP sin cifrado.

### Prueba de la conexión

Pruebe la configuración de ThingsBoard con curl:

```bash
curl -v -X POST http://thingsboard.cloud/api/v1/{ACCESS_TOKEN}/telemetry \
  --header Content-Type:application/json \
  --data "{temperature:25}"
```

Sustituya `{ACCESS_TOKEN}` por el token de acceso de su dispositivo en ThingsBoard.

## Solución de problemas

### Códigos de estado AI

El comando AI (indicación de asociación) del XBee devuelve el estado de la conexión celular:

| Código | Estado | Significado |
|--------|--------|-------------|
| 0x00 | Conectado | ✅ Conectado a internet; listo para enviar datos |
| 0x22 | Registrándose | Registrándose en la red celular |
| 0x23 | Conectándose | Conectándose a internet |
| 0x25 | Rechazado | La red ha rechazado el registro |
| 0xFF | Inicializándose | El módem se está inicializando o buscando redes |

**Supervisar el estado AI:**
```cpp
Serial.print(F("AI status = "));
Serial.println(AIstatus, HEX);
```

### Problemas habituales

#### El dispositivo se conecta, pero no aparecen datos en ThingsBoard

**Síntomas:**
- Estado AI = 0 (conectado)
- Dirección IP obtenida correctamente
- No hay errores en la salida serie
- No aparecen datos en el panel de ThingsBoard

**Posibles causas:**

1. **Combinación incorrecta de protocolo y puerto**
   - COAP definido, pero se utiliza el puerto HTTP (80)
   - Modo HTTP, pero se utiliza un puerto CoAP (5683 o 5684)
   - Protocolo SSL (4) utilizado con CoAP sin cifrar

2. **Nombre de servidor incorrecto**
   - Se usa `thingsboard.cloud` para CoAPS (debería ser `coap.thingsboard.cloud`)
   - Se usa `coap.thingsboard.cloud` para HTTP (debería ser `thingsboard.cloud`)

3. **Token de acceso incorrecto**
   - Compruebe que el token corresponde a su dispositivo en ThingsBoard
   - Compruebe que no haya espacios ni caracteres adicionales

**Solución:**
- Utilice el modo HTTP (recomendado) con la configuración adecuada
- Compruebe que coincidan los tres parámetros: HOST, Port y protocol

#### La conexión agota el tiempo de espera

**Síntomas:**
- "Timeout or error. Trying again next wakeup."
- El mensaje se envía, pero no se recibe respuesta

**Posibles causas:**
- Congestión de la red
- Protocolo incorrecto para el puerto
- Cortafuegos que bloquea las conexiones

**Solución:**
- Aumente el valor de `TIMEOUT` en la configuración
- Compruebe que el protocolo corresponde a los requisitos de la ruta de acceso
- Revise los ajustes de APN de su operador

#### Falla la resolución DNS

**Síntomas:**
- Aparece "Sending DNS Lookup", pero no se recibe ninguna dirección IP
- No aparece el mensaje "Callback - IP Address"

**Posibles causas:**
- APN mal configurado
- La red no permite consultas DNS
- Problema con el firmware del XBee

**Solución:**
- Compruebe que el APN coincide con el de su operador
- Pruebe a especificar la dirección IP directamente en lugar del nombre de servidor (no recomendado para producción)
- Actualice el firmware del XBee

### Supervisión del estado de conexión

Añada mensajes de depuración para seguir el progreso de la conexión:

```cpp
#ifdef DEBUG > 0
    Serial.print(F("AI status = "));
    Serial.println(AIstatus, HEX);
    Serial.print(F("IP Address: "));
    Serial.println(IP, HEX);
#endif
```

## Formato de datos

### Carga JSON de HTTP

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
- `ts`: marca de tiempo en milisegundos desde la época Unix
- `h`: medición de altura o distancia
- `v`: tensión de la batería (en centivoltios)
- `t`: temperatura (en centésimas de grado Celsius)

### Carga de CoAP

CoAP utiliza el mismo formato JSON, pero encapsulado en la estructura de mensajes CoAP con opciones para la ruta de la API.

## Buenas prácticas

1. **Empiece con HTTP**: es más sencillo y fiable para las primeras pruebas.
2. **Supervise el estado AI**: compruebe siempre la conexión antes de enviar datos.
3. **Utilice los mensajes de depuración**: active la depuración completa (`DEBUG 2`) durante el desarrollo.
4. **Pruebe primero con curl**: compruebe que la ruta de ThingsBoard funciona antes de probar el hardware.
5. **Revise los tiempos de espera**: conceda tiempo suficiente para la conexión celular (600 s por defecto).
6. **Compruebe el APN**: un APN incorrecto es el error de configuración más habitual.

## Referencias

- [Página del producto DIGI XBee 4G LTE](https://www.digi.com/products/embedded-systems/digi-xbee/cellular-modems/xbee-cellular-lte-cat-1)
- [API HTTP para dispositivos de ThingsBoard](https://thingsboard.io/docs/reference/http-api/)
- [API CoAP para dispositivos de ThingsBoard](https://thingsboard.io/docs/reference/coap-api/)
- [Referencia de comandos de la API de XBee](https://www.digi.com/resources/documentation/digidocs/90001477/references)
