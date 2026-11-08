# Programación de los registradores Riverlabs
 
## El IDE de Arduino

!!! tip "¿Es su primera vez con Arduino?"
    Si quiere saber más sobre Arduino, el [sitio web de Arduino](https://www.arduino.cc/en/Guide/Environment) ofrece excelentes tutoriales y documentación. Aunque puedes realizar muchos de sus ejemplos con los registradores Riverlabs, estos no siempre siguen los diseños estándar, por ejemplo, en lo que respecta al LED y las comunicaciones. Por eso, para aprender y experimentar, puede ser útil comprar una de las muchas placas Arduino disponibles a través de proveedores como [Arduino.cc](https://www.arduino.cc/en/Main/Products), Sparkfun o Adafruit.

Para instalar el IDE de Arduino, sigue las [instrucciones](https://www.arduino.cc/en/software/#ide) del sitio web de Arduino.

La interfaz del IDE de Arduino consta de dos ventanas principales. En la parte superior está el editor, donde puedes escribir y modificar el código antes de cargarlo. Arduino utiliza un lenguaje de programación muy parecido a C++. La captura de pantalla siguiente muestra Blink.ino, uno de los muchos ejemplos incluidos en la instalación. Es un programa breve que controla un LED en una placa Arduino. No funcionará directamente en las placas Riverlabs porque el LED de la placa está conectado a otro pin. Para probarlo, sustituye «LED_BUILTIN» por «A2».

Debajo del editor se encuentra la ventana de información, que muestra los resultados de la compilación y la carga, incluidos los errores que puedan producirse durante el proceso.

![Vista general del IDE de Arduino](../assets/images/arduino1.jpg "Vista general del IDE de Arduino")

**Botones importantes de la barra de herramientas**

- ✓ **Verificar**: Compila el código para comprobar si hay errores
- → **Cargar**: Compila y carga el código en el registrador
- **Monitor serie**: Muestra en tiempo real la salida serie del registrador

De los menús, el más importante es **Tools**: ahí se selecciona el puerto USB al que está conectada la placa, el tipo de placa correcto y, en ocasiones, el tipo de procesador.

![Configuración de la placa en Arduino](../assets/images/arduino2.jpg "Configuración del IDE de Arduino")

## Instalación de la compatibilidad con placas MiniCore

Los registradores Riverlabs utilizan el cargador de arranque y las definiciones de placa de **MiniCore**, que proporcionan compatibilidad optimizada con los microcontroladores ATmega328. Estas definiciones no se instalan de forma predeterminada con el IDE de Arduino, pero pueden añadirse fácilmente desde el gestor de placas:

1. Abre el IDE de Arduino
2. Ve a **File → Preferences**
3. En el campo «Additional Boards Manager URLs», añade:
   ```
   https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json
   ```
4. Haz clic en **OK**
5. Ve a **Tools → Board → Boards Manager**
6. Busca **MiniCore**
7. Haz clic en **Install** en la entrada de MiniCore de MCUdude
8. Cierra el gestor de placas

Para más información, visita el [repositorio de MiniCore en GitHub](https://github.com/MCUdude/MiniCore).

## Instalación de bibliotecas adicionales

El código de Riverlabs depende de varias bibliotecas externas. Todas pueden instalarse mediante el gestor de bibliotecas del IDE de Arduino:

1. Abre el IDE de Arduino
2. Ve a **Tools -> Manage Libraries** o haz clic en el icono de bibliotecas de la barra lateral izquierda.
3. Instala las siguientes bibliotecas externas:
    - **RTC by Makuna** de Michael C. Miller: Control del reloj en tiempo real
    - **SdFat** de Bill Greiman: Sistema de archivos de la tarjeta SD
    - **AltSoftSerial** de Paul Stoffregen: Puerto serie por software (para los modelos con conectividad celular)
    - **LowPower_LowPowerLab** de LowPowerLab: Funciones de suspensión
    - **LIDAR-Lite** de Garmin: Funciones del sensor lidar
    - **SPIMemory** de Prajwal Bhattaram: Funciones de la memoria FLASH


!!! note "Versión de SdFat"
    Asegúrate de instalar la biblioteca **SdFat** original, creada por **Bill Greiman**.

## Siguientes pasos

¡Ya puedes [compilar y cargar el código](uploading-code.md)!
