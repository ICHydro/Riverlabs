## Introducción

[Arduino](https://www.arduino.cc/) es un fantástico ecosistema de [hardware de código abierto](https://en.wikipedia.org/wiki/Open-source_hardware). Está centrado en el [Arduino IDE](https://www.arduino.cc/en/software), que es un entorno de desarrollo de software fácil de usar (y de código abierto) para escribir y compilar código de computadora que se ejecuta en procesadores embebidos como los que impulsan los registradores Riverlabs.

Para hacer esto posible, el equipo de Arduino ha desarrollado un cargador de arranque específico que permite conectar una placa compatible con Arduino a su computadora sin necesidad de programadores de hardware específicos. Si compra un registrador Riverlabs, este cargador de arranque ya estará instalado.

Si desea familiarizarse con Arduino, vale la pena comprar una de las [muchas placas](https://www.arduino.cc/en/Main/Products) de Arduino y otros proveedores como Sparkfun y Adafruit.

Existen muchos manuales excelentes para trabajar con Arduino, pero el mejor lugar es probablemente el [sitio web de Arduino](https://www.arduino.cc/en/Guide/Environment). Aquí solo damos una visión general muy breve, proporcionando el conocimiento mínimo para programar las placas Riverlabs. Pero le animamos a profundizar en Arduino si tiene tiempo.

Para instalar el Arduino IDE, simplemente vaya al [sitio web](https://www.arduino.cc/en/software) para descargarlo e instalarlo para su sistema operativo.

## Instalación del Soporte de Placa MiniCore

Los registradores Riverlabs usan el paquete de hardware **MiniCore**, que proporciona soporte optimizado para microcontroladores ATmega328:

1. Abra el Arduino IDE
2. Vaya a **File → Preferences**
3. En el campo "Additional Boards Manager URLs", agregue:
   ```
   https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json
   ```
4. Haga clic en **OK**
5. Vaya a **Tools → Board → Boards Manager**
6. Busque **MiniCore**
7. Haga clic en **Install** en la entrada de MiniCore por MCUdude
8. Cierre el Administrador de Placas

Para más información, visite el [repositorio de GitHub de MiniCore](https://github.com/MCUdude/MiniCore).

## Pasos de Programación

- Conectar la placa a la computadora
- Establecer el puerto correcto
- Establecer la placa correcta (MiniCore → ATmega328) y las opciones de placa (Clock: External 8 MHz)
- Compilar y cargar el código

Muchas placas se pueden conectar a la computadora a través de un cable USB. Las placas Riverlabs son ligeramente diferentes, ya que no tienen puerto USB, sino un puerto serie (UART) en su lugar. Por lo tanto, necesitará un convertidor de serie a USB. Consulte la sección sobre [carga de código con Arduino](uploading-code.md) para más detalles. Una vez hecho esto, los pasos restantes se pueden ejecutar a través de la interfaz gráfica del Arduino IDE.

## El Arduino IDE

La interfaz de usuario del Arduino IDE consta de dos ventanas principales. En la parte superior está el editor, en el que puede escribir y editar el código antes de cargarlo. Arduino usa un lenguaje de computadora muy similar a C++. La captura de pantalla a continuación muestra uno de los muchos ejemplos que vienen con la instalación, Blink.ino, que es un script corto para controlar un LED en una placa Arduino. Esto no funcionará de forma inmediata en las placas Riverlabs porque el LED de la placa está conectado a un pin diferente. Por lo tanto, necesitará reemplazar "LED_BUILTIN" con "8" para el Wari, y "A2" para el WMOnode.

Debajo del editor puede encontrar una ventana de información, que muestra la salida de los pasos de compilación y carga, incluidos los errores que puedan ocurrir durante este proceso.

De los menús, el menú "tool" es el más importante. Aquí es donde establece el puerto USB correcto al que está conectada la placa (elemento de menú "board"), y también deberá seleccionar el tipo correcto de placa, y a veces el tipo correcto de procesador. Las capturas de pantalla a continuación muestran dónde se pueden encontrar estos ajustes.

![Descripción general del Arduino IDE](../assets/images/arduino1.jpg "Descripción general del Arduino IDE")

![Configuración de la placa en Arduino](../assets/images/arduino2.jpg "Configuración del Arduino IDE")

¡Ahora está listo para proceder a [cargar el código en su registrador Riverlabs](uploading-code.md)!
