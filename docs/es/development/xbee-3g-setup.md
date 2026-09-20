## Configuraciones Clave

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

### Comandos de sueño:

```
SM: pin sleep [1]
```

## Radios XBee

Los registradores WMOnode vienen con un zócalo xbee, que puede usarse para radios compatibles con DIGI XBee.

Los DIGI XBee son una serie de dispositivos de radio populares para aplicaciones embebidas. La gama XBee cubre muchas tecnologías de radio y protocolos diferentes, como Zigbee. Esto le proporciona una gran flexibilidad al diseñar soluciones de red para aplicaciones particulares, tanto redes amplias como locales, y para actualizar los dispositivos. Además de ser piezas de equipo extremadamente sólidas y fiables, se integran perfectamente con Arduino gracias a la biblioteca xbee-arduino. Hemos extendido la [biblioteca original](https://github.com/andrewrapp/xbee-arduino) con la funcionalidad necesaria para interactuar con los módems celulares más nuevos, que usan una versión ligeramente extendida de la API original. Nuestra versión está disponible en nuestra [página de github](https://github.com/ICHydro/xbee-arduino) pero también ya está incluida en el paquete WMOnode en esta página de github, por lo que no necesita instalarla por separado.

Para la telemetría a través de la red celular, utilizamos el [DIGI XBee Cellular 3G](https://www.digi.com/products/embedded-systems/digi-xbee/cellular-modems/digi-xbee-cellular-3g). Digi también tiene soluciones para 4G, pero estas se basan en los protocolos LTE-M o NB-IoT, los cuales aún no son muy comunes. Sin embargo, debería ser sencillo actualizarlos, aunque esta guía se centra en el módem 3G.

La mayor parte de la configuración del XBee Celular se realiza directamente en el código de Arduino. Sin embargo, es necesario establecer algunos parámetros manualmente para garantizar que el registrador pueda comunicarse con el XBee. Esto puede hacerse más fácilmente con el software gratuito [XCTU de DIGI](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xctu). Aquí proporcionamos un breve resumen, pero para explorar la funcionalidad completa del software XCTU, remitimos al [manual original](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xctu).

## Programación de un XBee con XCTU

### Conexión del XBee a su Computadora

Los dispositivos XBee vienen con algunas interfaces digitales diferentes, incluyendo UART y SPI. La interfaz serie UART es la más conveniente para trabajar, y también es la que usan los registradores Riverlabs. El XBee se puede programar completamente a través del código de Arduino, pero es más conveniente programarlo primero conectándolo directamente a una computadora.

Digi vende una placa de desarrollo, el [XBIB](https://www.digi.com/products/models/xbib-u-dev), que viene con todas las prestaciones y su uso se describe en el manual de XBee. Sin embargo, se puede usar cualquier adaptador USB a serie y existen muchos adaptadores. La mayoría de ellos están basados en el chip FTDI, como el [Sparkfun Xbee Explorer](https://www.sparkfun.com/products/11812). Es posible que necesite instalar los controladores FTDI si usa Windows, o configurar algunos permisos en Linux. Sparkfun tiene un [excelente manual](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers/all) si tiene problemas con que su PC no reconozca el adaptador serie.

También es importante tener en cuenta que los adaptadores FTDI normalmente vienen en versiones de 3.3V y 5V. Los XBee son estrictamente dispositivos de 3.3V, por lo que nunca debe usar adaptadores FTDI de 5V o podría dañar su XBee. Algunos adaptadores tienen un interruptor para establecer el voltaje. En ese caso, asegúrese de que esté configurado correctamente.

[Foto de XBee conectado a través del adaptador FTDI]

### Uso del Software XCTU

Es más conveniente usar el software XCTU de Digi para programar un XBee a través de un PC. XCTU se puede descargar de forma gratuita desde el [sitio web de Digi](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xctu).

XCTU también está bien documentado en el manual del usuario, por lo que aquí nos centramos únicamente en los pasos relevantes para operar los registradores Riverlabs. Los pasos principales para conectarse al XBee se describen en las Figuras 2 y 3.

![Captura de pantalla de XCTU 1](../assets/images/XCTU1.jpg "Captura de pantalla de XCTU 1")

![Captura de pantalla de XCTU 2](../assets/images/XCTU2.jpg "Captura de pantalla de XCTU 2")
