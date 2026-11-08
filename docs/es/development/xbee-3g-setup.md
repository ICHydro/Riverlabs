## Configuraciones clave

### Interfaz serie:

```
BD: 9600
NB: sin paridad
SB: un bit de parada
RO: 3
TD: 0
FT: 681
AP: modo API con escapes
```

### Comandos de suspensión:

```
SM: suspensión mediante pin [1]
```

## Radios XBee

Los registradores Wari vienen equipados con un zócalo para XBee, que puede utilizarse con radios compatibles con DIGI XBee.

Los Digi XBee son una serie de dispositivos de radio populares para aplicaciones integradas. La gama XBee abarca numerosas tecnologías y protocolos de radio diferentes, como Zigbee. Esto proporciona una gran flexibilidad a la hora de diseñar soluciones de red para aplicaciones específicas, tanto para redes extensas como locales, y para actualizar dispositivos. Además de ser equipos extremadamente sólidos y fiables, se integran fácilmente con Arduino gracias a la biblioteca xbee-arduino. Hemos ampliado la [biblioteca original](https://github.com/andrewrapp/xbee-arduino) con la funcionalidad necesaria para interactuar con los módems celulares más recientes, que utilizan una versión ligeramente ampliada de la API original. Nuestra versión está disponible en nuestra [página de GitHub](https://github.com/ICHydro/xbee-arduino), pero también está incluida en el paquete WMOnode de esta página de GitHub, por lo que no es necesario instalarla por separado.

Para la telemetría a través de la red celular, utilizamos el [DIGI XBee Cellular](https://www.digi.com/products/embedded-systems/digi-xbee/cellular-modems/digi-xbee-cellular). Existen versiones con los protocolos especializados LTE-M o NB-IoT, tanto como una versión LTE que es compatible con la mayoria de las redes 4G y 5G.

La mayor parte de la configuración del XBee Cellular se realiza directamente en el código de Arduino. Sin embargo, es necesario configurar algunos parámetros manualmente para garantizar que el registrador pueda comunicarse con el XBee. La forma más sencilla de hacerlo es mediante el [software XCTU gratuito de DIGI](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xctu). Aquí ofrecemos una breve descripción general, pero para consultar toda la funcionalidad del software XCTU, recomendamos utilizar el [manual original](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xctu).

## Programación de un XBee con XCTU

### Conexión del XBee al ordenador

Los dispositivos XBee disponen de varias interfaces digitales, entre ellas UART y SPI. La interfaz serie UART es la más práctica y también es la que utilizan los registradores Riverlabs. El XBee puede programarse completamente mediante código de Arduino, pero resulta más cómodo programarlo primero conectándolo directamente a un ordenador.

Digi comercializa una placa de desarrollo, la [XBIB](https://www.digi.com/products/models/xbib-u-dev), que incluye todas las funciones necesarias y cuyo uso se describe en el manual del XBee. Sin embargo, puede utilizarse cualquier adaptador USB-serie, y existen muchos adaptadores de este tipo. La mayoría se basan en el chip FTDI, como el [Sparkfun XBee Explorer](https://www.sparkfun.com/products/11812). Si utiliza Windows, es posible que necesite instalar los controladores FTDI, o configurar algunos permisos en Linux. Sparkfun dispone de un [manual excelente](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers/all) si tiene problemas porque su PC no reconoce el adaptador serie.

También es importante tener en cuenta que los adaptadores FTDI suelen estar disponibles en versiones de 3,3 V y 5 V. Los XBee son dispositivos estrictamente de 3,3 V, por lo que nunca debe utilizar adaptadores FTDI de 5 V, ya que podría dañar el XBee. Algunos adaptadores tienen un interruptor para seleccionar el voltaje. En ese caso, asegúrese de configurarlo correctamente.

[Foto del XBee conectado mediante un adaptador FTDI]

### Uso del software XCTU

La forma más práctica de programar un XBee mediante un PC es utilizar el software XCTU de Digi. XCTU puede descargarse gratuitamente desde el [sitio web de Digi](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xctu).

XCTU también está bien documentado en el manual de usuario, por lo que aquí nos centramos únicamente en los pasos relevantes para utilizar los registradores Riverlabs. Los pasos principales para conectar el XBee se muestran en las Figuras 2 y 3.

![Captura de pantalla de XCTU 1](../assets/images/XCTU1.jpg "Captura de pantalla de XCTU 1")

![Captura de pantalla de XCTU 2](../assets/images/XCTU2.jpg "Captura de pantalla de XCTU 2")
