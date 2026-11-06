# Thingsboard

El registrador WMOnode utiliza COAP como protocolo de transferencia (MQTT está en desarrollo). Se trata de un protocolo ampliamente utilizado e implementado por varias plataformas de software de IoT. Una opción es [Thingsboard](https://thingsboard.io/), un proyecto de código abierto con una versión comunitaria gratuita y una versión comercial más avanzada. También incluye una [versión de demostración en línea](https://thingsboard.cloud) que puede utilizar para probar su telemetría sin necesidad de instalar ningún software.

El código de Arduino está configurado de forma predeterminada con los ajustes de la demostración de Thingsboard. Sin embargo, puede cambiar fácilmente los ajustes según su propia configuración y sus requisitos:

```cpp
#define HOST "demo.thingsboard.io"                // dirección de Internet del servidor de IoT al que se enviarán los datos
#define ACCESSTOKEN "A1_TEST_TOKEN"               // token de acceso COAP
#define LOGGERID "MyLogger1"                      // ID del registrador. Establézcalo como prefiera
```