# ThingsBoard

El registrador WMOnode usa COAP como protocolo de transferencia (MQTT está en desarrollo). Este es un protocolo ampliamente utilizado que está implementado por varias pilas de software IoT. Una opción es [ThingsBoard](https://thingsboard.io/), que es un proyecto de código abierto con una versión comunitaria gratuita y una versión comercial más potente. También incluye una [versión demo](https://thingsboard.cloud) en línea que puede usar para probar su telemetría sin necesidad de instalar ningún software.

El código de Arduino está configurado por defecto con los ajustes del demo de ThingsBoard. Sin embargo, puede cambiar fácilmente los ajustes de acuerdo con su propia configuración y requisitos:

```cpp
#define HOST "demo.thingsboard.io"                // dirección de internet del servidor IoT al que reportar
#define ACCESSTOKEN "A1_TEST_TOKEN"               // token de acceso COAP
#define LOGGERID "MyLogger1"                      // ID del registrador. Establézcalo como prefiera
```
