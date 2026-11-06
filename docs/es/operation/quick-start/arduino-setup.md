# Configuración de Arduino

## Introducción

!!! tip "El principio del hardware abierto"
    Los registradores Riverlabs se basan en el principio del hardware abierto. Esto significa que tanto el diseño del hardware como el software que se ejecuta en él son completamente de código abierto. Por tanto, puedes adaptar el software (y el hardware, si te animas a ir un poco más allá) a tus necesidades. Una de las iniciativas de hardware abierto más populares es [Arduino](https://www.arduino.cc). El «ecosistema» Arduino consta de numerosos componentes de hardware (placas, placas de expansión y otros periféricos) y una amplia gama de herramientas de software, incluido un entorno de desarrollo integrado (IDE). El hardware de Riverlabs es totalmente compatible con el IDE de Arduino y recomendamos utilizar Arduino para programar los registradores por su facilidad de uso. No obstante, existen muchas otras opciones, descritas en nuestro [manual de programación](../../programming/arduino-setup.md).

Esta sección ofrece un resumen rápido de los principales ajustes de Arduino y supone que ya tienes cierta familiaridad con esta plataforma. Encontrarás una explicación más completa del IDE y las herramientas de Arduino en el [manual de programación](../../programming/arduino-setup.md).

Ten en cuenta que los registradores Riverlabs no tienen puerto USB, sino que utilizan una interfaz serie. Necesitarás un convertidor de USB a serie (de nivel TTL), conocido habitualmente como cable o placa FTDI. Existen distintos tipos y modelos; entre las opciones más populares están el [cable FTDI original](https://www.sparkfun.com/products/9717) y placas adaptadoras como la [Sparkfun FTDI Basic](https://www.sparkfun.com/products/9873), que se utiliza con un cable USB convencional. Los cables y placas FTDI están disponibles en versiones de 3.3V y 5V. Los registradores Riverlabs funcionan con ambas, pero recomendamos 3.3V porque es la tensión interna de la CPU.


## Bibliotecas necesarias

El registrador Riverlabs necesita las siguientes bibliotecas externas:

- **RTC by Makuna** de Michael C. Miller: control del reloj de tiempo real
- **SdFat** de Bill Greiman: sistema de archivos de la tarjeta SD
- **AltSoftSerial** de Paul Stoffregen: comunicación serie por software (para modelos con conexión celular)
- **LowPower_LowPowerLab** de LowPowerLab: funciones de suspensión
- **LIDAR-Lite** de Garmin: funciones del sensor lidar
- **SPIMemory** de Prajwal Bhattaram: funciones de la memoria FLASH

Todas pueden instalarse mediante el gestor de bibliotecas del IDE de Arduino.

!!! note "Versión de SdFat"
    Asegúrate de instalar la biblioteca **SdFat** original, cuyo autor es **Bill Greiman**.

## Definiciones de placa necesarias

Los registradores Riverlabs utilizan las definiciones de placa **MiniCore**, que proporcionan compatibilidad optimizada con los microcontroladores ATmega328:

1. En el campo «Additional Boards Manager URLs», añade:
   ```
   https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json
   ```
2. Busca e instala **MiniCore by MCUdude** en el gestor de placas del IDE de Arduino.

Para más información, visita el [repositorio de MiniCore en GitHub](https://github.com/MCUdude/MiniCore).

## Configuración del IDE de Arduino

Antes de cargar el código, debes configurar el IDE de Arduino con los ajustes correctos de la placa:

### Ajustes de la placa

1. Ve a **Tools → Board → MiniCore** y selecciona **ATmega328**
2. Configura los siguientes ajustes en el menú **Tools**:
    - **Clock:** External 8 MHz
    - **BOD:** BOD 2.7 V
    - **EEPROM:** EEPROM retained
    - **Compiler LTO:** LTO Disabled
    - **Variant:** 328P / 328PA
    - **Bootloader:** Yes (UART0)

!!! warning "Ajustes fundamentales"
    La placa DEBE estar configurada como **MiniCore → ATmega328** con **Clock: External 8 MHz**. Una configuración incorrecta puede impedir la carga del código o causar problemas durante el funcionamiento.

## Recursos

- [Documentación oficial de Arduino](https://www.arduino.cc/en/Guide/HomePage)
- [Tutorial de FTDI de Sparkfun](https://learn.sparkfun.com/tutorials/serial-communication)
- [Problemas comunes](../troubleshooting/common-issues.md): solución de problemas de programación
