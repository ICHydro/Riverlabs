# Programación de los registradores Riverlabs

## Introducción

!!! tip "El principio del hardware abierto"
    Los registradores Riverlabs estan basados en el principio del hardware abierto. Esto significa que tanto el diseño del hardware como el software que se ejecuta en él son completamente de código abierto. Por tanto, puedes adaptar el software (y el hardware, si te animas a ir un poco más allá) a tus necesidades. Una de las iniciativas de hardware abierto más populares es [Arduino](https://www.arduino.cc). El «ecosistema» Arduino consta de muchos componentes de hardware diferentes (placas, placas de expansión y otros periféricos) y una amplia variedad de herramientas de software, incluido un entorno de desarrollo integrado (IDE). El hardware de Riverlabs es totalmente compatible con el IDE de Arduino y, de hecho, recomendamos utilizar Arduino para programar los registradores por su facilidad de uso.

Para la mayoría de los usuarios, recomendamos utilizar el IDE de Arduino para programar los registradores Riverlabs. [Arduino](https://www.arduino.cc/) es un excelente ecosistema de [hardware de código abierto](https://en.wikipedia.org/wiki/Open-source_hardware), y el [IDE de Arduino](https://www.arduino.cc/en/software) es un entorno de desarrollo fácil de usar para escribir y compilar el código que se ejecuta en procesadores integrados como los de los registradores Riverlabs.

Si no le gusta el IDE de Arduino o le parece demasiado limitado, recomendamos utilizar [PlatformIO](https://platformio.org/). Es compatible con muchos IDE avanzados, como VSCode, Atom e incluso Emacs. Asegúrate de instalar [MiniCore para PlatformIO](https://github.com/MCUdude/MiniCore/blob/master/PlatformIO.md), ya que es el cargador de arranque y el núcleo de Arduino que utilizan los registradores Riverlabs.

Esperamos añadir a esta guía más detalles sobre el uso de PlatformIO, pero, por ahora, la documentación se centra en el IDE de Arduino.

## Requisitos de hardware

Una de las principales ventajas de urboot y otros cargadores de arranque similares es que no necesitas un programador de hardware costoso para programar la CPU. En su lugar, puedes conectar la placa directamente a un puerto USB, siempre que la CPU incorpore esta funcionalidad. No es el caso de la CPU ATmega328 que utilizan los registradores Riverlabs, por lo que necesitarás un componente adicional: un convertidor de USB a serie, también conocido como cable FTDI:

![Conexión del cable FTDI](../assets/images/FTDICable.png)<br>
*Cable FTDI con conector de 6 pines y cables identificados por colores*

Estos son algunos ejemplos de cables y placas adaptadoras:

- **Cable FTDI**: Conexión USB directa (p. ej., [cable FTDI de Sparkfun](https://www.sparkfun.com/products/9717))
- **Placa adaptadora FTDI**: Placa pequeña que requiere un cable micro-USB (p. ej., [FTDI Basic de Sparkfun](https://www.sparkfun.com/products/9873))

Los cables y las placas FTDI están disponibles en versiones de **3.3 V** y **5 V**. Los registradores Riverlabs son compatibles con ambas, pero se recomienda la de **3.3 V**.

!!! tip "Instalación de los controladores FTDI"
    Sigue [estas instrucciones](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers) para instalar los controladores FTDI en tu ordenador.

El cable FTDI (o una placa adaptadora equivalente) se conecta al conector serie de 6 pines de las placas Riverlabs. Presta atención a la orientación: el extremo «GRN» del conector debe coincidir con el cable verde y el extremo «BLK», con el cable negro.

!!! warning "Consideraciones sobre la alimentación" 
    El registrador tiene un interruptor de alimentación con la etiqueta «ON (Batt) / OFF (ftdi)». Esto significa que, cuando está en ON, el registrador recibe energía de la batería, como cabría esperar. En la posición OFF, recibe energía del cable FTDI si hay uno conectado; de lo contrario, no recibe alimentación.
    Esto permite programar el registrador sin batería, ya que se alimenta mediante el cable FTDI. También puedes programarlo con una batería instalada: tanto en la posición OFF (la batería está desconectada y la alimentación procede del cable FTDI) como en la posición ON (la alimentación procede de la batería).
    Este diseño también impide que la alimentación del cable FTDI y la de la batería se conecten al mismo tiempo, lo que podría dañar la batería si recibiera 5 V del puerto USB a través del cable FTDI.
