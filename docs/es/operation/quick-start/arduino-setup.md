# Guía de Configuración de Arduino

Esta guía le ayudará a configurar el Arduino IDE (Entorno de Desarrollo Integrado) y preparar su computadora para programar su registrador Riverlabs.

### Componentes Principales

![Descripción general del Arduino IDE](../../assets/images/arduino1.jpg)
*El Arduino IDE mostrando la ventana del editor (arriba) y la ventana de información (abajo)*

**Ventana del Editor** (arriba)<br>

Aquí es donde escribe y edita su código. Arduino usa un lenguaje muy similar a C++. Cada programa (llamado "sketch") consta de dos funciones principales:

- `setup()`: Se ejecuta una vez cuando el registrador se enciende
- `loop()`: Se ejecuta repetidamente mientras el registrador está encendido

**Ventana de Información** (abajo)<br>

Esta ventana muestra la salida de compilación, el progreso de carga y cualquier error que ocurra durante el proceso.

**Botones importantes de la barra de herramientas**

- ✓ **Verificar**: Compila su código para verificar errores
- → **Cargar**: Compila y carga el código en su registrador
- **Monitor Serie**: Muestra la salida serie en tiempo real de su registrador

## ¿Qué es Arduino?

[Arduino](https://www.arduino.cc/) es un fantástico ecosistema de hardware de código abierto centrado en el Arduino IDE: un entorno de desarrollo de software fácil de usar para escribir código que se ejecuta en procesadores embebidos, como los de los registradores Riverlabs.

El equipo de Arduino desarrolló un cargador de arranque especial que le permite conectar placas compatibles con Arduino a su computadora sin necesidad de programadores de hardware especializados. Su registrador Riverlabs viene con este cargador de arranque preinstalado.

!!! tip "¿Nuevo en Arduino?"
    Si desea aprender más sobre Arduino, considere comprar una de las muchas placas de [Arduino.cc](https://www.arduino.cc/en/Main/Products), Sparkfun o Adafruit. El [sitio web de Arduino](https://www.arduino.cc/en/Guide/Environment) tiene excelentes tutoriales y documentación.

## Paso 1: Instalar el Arduino IDE

1. Visite la [página de Software de Arduino](https://www.arduino.cc/en/software)
2. Descargue el Arduino IDE para su sistema operativo (Windows, macOS o Linux)
3. Ejecute el instalador y siga las instrucciones de instalación
4. Una vez completada la instalación, inicie el Arduino IDE

## Paso 2: Instalar las Bibliotecas Requeridas

Su registrador Riverlabs requiere varias bibliotecas externas. La mayoría se pueden instalar a través del Administrador de Bibliotecas de Arduino.

### Instalar mediante el Administrador de Bibliotecas

1. Abra el Arduino IDE
2. Vaya a **Sketch → Include Library → Manage Libraries**
3. Busque e instale cada una de estas bibliotecas:
    - **RTC by Makuna**: Control del reloj en tiempo real
    - **SoftwareSerial**: Comunicación serie por software (para modelos celulares)
    - **SdFat by Bill Greiman**: Sistema de archivos de tarjeta SD
    - **AltSoftSerial**: Serie por software alternativo (para modelos celulares/lidar)

!!! note "Versión de SdFat"
    Asegúrese de instalar la biblioteca original **SdFat** escrita por **Bill Greiman**.

### Instalación Manual: Rocketscream LowPower

La biblioteca **Rocketscream LowPower** no está disponible en el Administrador de Bibliotecas y debe instalarse manualmente:

1. Descargue la biblioteca del [repositorio de Github](https://github.com/rocketscream/Low-Power)
2. Haga clic en el botón verde **Code** y seleccione **Download ZIP**
3. Extraiga el archivo ZIP
4. Mueva la carpeta extraída a su directorio de bibliotecas de Arduino:
    - **Windows:** `Documents\Arduino\libraries\`
    - **macOS:** `~/Documents/Arduino/libraries/`
    - **Linux:** `~/Arduino/libraries/`
5. Reinicie el Arduino IDE

!!! tip "Ayuda para la Instalación de Bibliotecas"
    Para instrucciones detalladas sobre la instalación manual de bibliotecas, consulte la [Guía de Bibliotecas de Arduino](https://www.arduino.cc/en/Guide/Libraries).

## Paso 3: Obtener un Cable FTDI

Los registradores Riverlabs no tienen puerto USB — usan una interfaz serie en su lugar. Necesitará un **conversor USB a Serie (nivel TTL)**, comúnmente llamado cable FTDI o placa FTDI.

**Opciones recomendadas:**

- **Cable FTDI**: Conexión USB directa ([Sparkfun FTDI Cable](https://www.sparkfun.com/products/9717))
- **Placa de Conexión FTDI**: Pequeña placa que requiere cable micro-USB ([Sparkfun FTDI Basic](https://www.sparkfun.com/products/9873))

**Selección de voltaje:**

Los cables FTDI vienen en versiones de 3.3 V o 5 V. Los registradores Riverlabs funcionan con ambos, pero **se recomienda 3.3 V**.

![Conexión del Cable FTDI](../../assets/images/FTDICable.png)<br>
*Cable FTDI mostrando el conector de 6 pines con cables codificados por colores*

## Paso 4: Instalar el Soporte de Placa MiniCore

Los registradores Riverlabs usan el paquete de hardware MiniCore, que proporciona mejor soporte para microcontroladores ATmega328:

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

!!! info "MiniCore"
    MiniCore es un paquete de hardware Arduino mantenido por la comunidad diseñado específicamente para ATmega328 y microcontroladores similares. Obtenga más información en el [repositorio de GitHub de MiniCore](https://github.com/MCUdude/MiniCore).

## Paso 5: Configurar el Arduino IDE para Registradores Riverlabs

Antes de cargar código, debe configurar el Arduino IDE con la configuración correcta de la placa:

### Configuración de la Placa

1. Abra el Arduino IDE
2. Vaya a **Tools → Board → MiniCore** y seleccione **ATmega328**
3. Configure los siguientes ajustes en el menú **Tools**:
    - **Clock:** External 8 MHz
    - **BOD:** BOD 2.7 V
    - **EEPROM:** EEPROM retained
    - **Compiler LTO:** LTO Disabled
    - **Variant:** 328P / 328PA
    - **Bootloader:** Yes (UART0)

!!! warning "Configuraciones Críticas"
    La placa DEBE configurarse en **MiniCore → ATmega328** con **Clock: External 8 MHz**. Usar configuraciones incorrectas causará fallos de carga o problemas de ejecución.

### Seleccionar el Puerto

Una vez que haya conectado su cable FTDI a su computadora:

1. Vaya a **Tools → Port**
2. Seleccione el puerto que aparece después de conectar el cable FTDI
3. Los nombres de los puertos varían según el sistema operativo:
    - **macOS:** `/dev/cu.usbserial-XXXXXXXX`
    - **Linux:** `/dev/ttyUSB0` o `/dev/ttyACM0`
    - **Windows:** `COM3`, `COM4`, etc.

Si no aparece ningún puerto, verifique que los controladores FTDI estén correctamente instalados.

!!! example "Prueba con Blink"
    Arduino viene con muchos sketches de ejemplo. El clásico ejemplo "Blink" (`File → Examples → 01.Basics → Blink`) puede adaptarse para registradores Riverlabs reemplazando `LED_BUILTIN` con:
    
    - `8` para registradores Wari
    - `A2` para registradores WMOnode
    
    ¡Esto hará parpadear el LED integrado, confirmando que su configuración funciona!

- **Número de Serie:** formato "RL0XXX" (p.ej., RL000123)
- **Tipo de Sensor:** Ultrasonido (Maxbotix) o lidar (Garmin)
- **Telemetría:** Verificar la presencia de módem celular XBee o radio LoRa

## Próximos Pasos

Ahora que tiene Arduino configurado, está listo para programar su registrador:

- [Guía de Inicio Rápido](quick-start.md): Flujo de trabajo de configuración completo
- [Carga de Código](../../programming/uploading-code.md): Instrucciones detalladas de carga con resolución de problemas
- [Identificación del Registrador](logger-identification.md): Encuentre el código correcto para su modelo de registrador

## Resolución de Problemas

**Problema: No aparece ningún puerto en Tools → Port**

- Verifique que los controladores FTDI estén instalados correctamente
- Pruebe con un puerto USB diferente en su computadora
- Intente reiniciar su computadora
- Pruebe con un cable USB diferente (si usa placa de conexión)

**Problema: Error "Board not found" durante la carga**

- Verifique la orientación del cable FTDI (marcas GRN/BLK)
- Asegúrese de que el sensor esté desconectado (conector blanco)
- Verifique la configuración correcta de la placa (p.ej., ATmega328P, 3.3 V, 8 MHz)
- Intente presionar el botón de reset en el registrador justo antes de cargar

**Problema: Las bibliotecas no se instalan**

- Asegúrese de tener conexión a internet
- Intente cerrar y reabrir el Administrador de Bibliotecas
- Para la instalación manual, verifique que la carpeta esté en el directorio correcto de bibliotecas
- Reinicie el Arduino IDE después de instalar las bibliotecas

## Recursos Relacionados

- [Documentación Oficial de Arduino](https://www.arduino.cc/en/Guide/HomePage)
- [Tutorial FTDI de Sparkfun](https://learn.sparkfun.com/tutorials/serial-communication)
- [Problemas Comunes](../troubleshooting/common-issues.md): Resolución de problemas de programación
