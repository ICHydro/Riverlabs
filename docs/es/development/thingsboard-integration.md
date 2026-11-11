# Thingsboard

El código de transmisión disponible en GitHub utiliza MQTT como protocolo de transferencia. Se trata de un protocolo ampliamente utilizado e implementado por varias plataformas de software de IoT. Una opción es [Thingsboard](https://thingsboard.io/), un proyecto de código abierto con una versión gratuita de uso limitado. También ofrece una [versión de demostración en línea](https://thingsboard.cloud) que puede utilizar para probar la telemetría sin necesidad de instalar ningún software.

El código de Arduino está configurado de forma predeterminada con los ajustes de la demostración de Thingsboard. Sin embargo, puede modificarlos fácilmente según su propia configuración y sus requisitos:

```cpp
#define HOST "demo.thingsboard.io"                // dirección de Internet del servidor de IoT al que se enviarán los datos
#define ACCESSTOKEN "A1_TEST_TOKEN"               // token de acceso COAP
#define LOGGERID "MyLogger1"                      // ID del registrador. Establézcalo como prefiera
```