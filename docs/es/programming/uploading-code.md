# Programación de los registradores Riverlabs

!!! Abstract "Resumen"
    En la sección anterior de este capítulo se explica cómo instalar el IDE de Arduino, las definiciones de placa y las bibliotecas necesarias. Una vez instalado todo, se puede conectar el registrador a un ordenador y compilar y cargar el código. Esta sección explica cómo hacerlo.

## Conectar el registrador al ordenador

!!! warning "Desconecta primero el sensor Maxbotix"
    Desconecta el sensor ultrasónico Maxbotix del registrador antes de programarlo. El sensor y el cable FTDI utilizan el mismo puerto serie, lo que provoca interferencias e impide cargar el código correctamente. Desenchufa el conector blanco.

Como se explica en la sección anterior, los registradores Wari no tienen puerto USB: necesitas un cable o una placa FTDI para conectarlos al conector de 6 pines situado en el lateral de la placa de circuito impreso. La placa lleva marcas para indicar la orientación correcta: el pin «GRN» debe coincidir con el cable verde, y el pin «BLK», con el cable negro.

<!-- insert picture -->

!!! danger "Comprueba la orientación"
    Si el cable FTDI se conecta en la orientación incorrecta, es posible que no puedas programar el registrador o que se dañen los componentes. Comprueba siempre las marcas GRN/BLK antes de conectarlo.

Conecta ahora el cable a un puerto USB del ordenador. Si el dispositivo reconoce correctamente el chip FTDI, aparecerá en el menú **Tools → Port** del IDE de Arduino. Los nombres de los puertos dependen del sistema operativo:

- **macOS:** `/dev/cu.usbserial-XXXXXXXX`
- **Linux:** `/dev/ttyUSB0` o `/dev/ttyACM0`
- **Windows:** `COM3`, `COM4`, etc.

Si no aparece ningún puerto, comprueba lo siguiente:

- Que los controladores FTDI estén instalados (solo en Windows)
- Que el cable funcione al conectarlo a otro puerto USB del ordenador

Para resolver problemas con más detalle, la [guía de FTDI de Sparkfun](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers) ofrece una explicación exhaustiva sobre el uso de estos cables.

## Seleccionar el puerto y configurar la placa en el IDE de Arduino

Asegúrate de seleccionar el puerto correcto en **Tools → Port**. Si hay otros dispositivos conectados al ordenador, pueden aparecer varios puertos, como «COM3» y «COM4». Para identificar el correcto, desconecta el cable FTDI, consulta la lista de puertos, vuelve a conectarlo y comprueba la lista de nuevo. El puerto COM asignado al cable FTDI debería desaparecer y volver a aparecer.

A continuación, selecciona las definiciones de placa correctas en el menú **Tools**:


1. **Board:** MiniCore → ATmega328
2. **Clock:** External 8 MHz
3. **BOD:** BOD 2.7V
4. **Variant:** 328P / 328PA
5. **Bootloader:** Yes (UART0)

Si estas opciones no aparecen en el menú, es posible que tengas que instalar las definiciones de placa de MiniCore mediante el gestor de placas.

!!! warning "La configuración de la placa es fundamental"
    La placa DEBE configurarse como **MiniCore → ATmega328** y el reloj como **Clock: External 8 MHz**. Una configuración incorrecta puede causar fallos al cargar el código o problemas durante el funcionamiento.

## Cargar el código de Riverlabs

### Ajustar el reloj

Solo es necesario realizar este paso si la placa es completamente nueva, si se ha instalado una pila de botón CR1220 nueva o si se ha retirado la pila. El reloj conservará la hora mientras la pila de botón permanezca instalada, incluso después de cargar código nuevo.

El reloj se puede ajustar con el programa «set_clock.ino» del repositorio de Riverlabs. Abre el monitor serie **antes* de cargar el programa para comprobar que el reloj se ajusta correctamente. Si no aparece nada en el monitor serie, comprueba que la velocidad esté configurada en **115200 baudios**. En [Instructables](https://www.instructables.com/id/HOW-TO-use-the-ARDUINO-SERIAL-MONITOR/) encontrarás un buen tutorial sobre el uso del monitor serie de Arduino.

!!! info "Zonas horarias diferentes"
     Si ajustas el reloj con este programa, se establecerá según la zona horaria de tu ordenador. Puedes cambiarla modificando la variable «TZ» del programa. Establece `TZ` con la diferencia horaria respecto de UTC, en horas (p. ej., `-5` para EST o `+1` para CET). En nuestras investigaciones hemos adoptado la práctica de ajustar siempre el reloj a UTC (GMT). Utilizamos registradores en distintas zonas horarias, y esto reduce el riesgo de confusiones. Si tienes un registrador Riverlabs preprogramado, es muy probable que el reloj ya esté configurado en UTC.

!!! tip "Compensación del tiempo de carga"
    Durante la compilación, el programa incorpora la hora del ordenador al código y, cuando este se ejecuta en el registrador, utiliza esa hora para ajustar el reloj. Entre ambos pasos transcurren aproximadamente **10 segundos**. El programa ya compensa ese retraso. Como depende del ordenador utilizado, puedes ajustar el valor si lo deseas:

La compensación del retraso se ajusta en esta línea del programa:
    
    ```cpp
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__) - TZ * 3600 + 10;
    ```

### Cargar el código del registrador

1. **Descarga el programa adecuado** para tu modelo de registrador. Estamos trabajando en unificar el código, pero, por el momento, cada modelo tiene el suyo. Consulta el README del repositorio para identificar el código correcto para tu modelo.

2. **Abre el programa** en el IDE de Arduino

3. **Configura los ajustes** del código (si es necesario):
    - Intervalo de medición
    - Calibración del sensor
    - Ajustes de telemetría

4. **Haz clic en el botón de carga** (icono de flecha →)

5. **Espera a que termine la carga**
    - La barra de progreso mostrará la compilación
    - Después aparecerá «Uploading...»
    - Mensaje de éxito: «Done uploading»

6. **Comprueba la carga:**
    - El LED debería encenderse durante aproximadamente 1 segundo
    - El registrador ya está programado y listo para su uso

7. **Vuelve a conectar el sensor** (si lo habías desconectado)

!!! success "Carga completada"
    Si el LED ha parpadeado, el registrador se ha programado correctamente. Ya puedes desconectar el cable FTDI e instalar el registrador.

---

## Diagnóstico mediante el puerto serie de hardware

Durante el funcionamiento, el registrador envía mensajes al puerto serie. Esto permite comprobar su funcionamiento y detectar problemas. Para verlos, deja el cable FTDI conectado al registrador y abre el monitor serie. Configura la velocidad en 115200 baudios. Al abrir el monitor serie, el IDE de Arduino reiniciará el registrador y verás los mensajes de diagnóstico desde el inicio. Es un comportamiento normal. Entre otros datos, aparecerán:

- Mensajes de arranque
- Lecturas del sensor
- Marcas de tiempo del reloj en tiempo real (RTC)
- Estado de la tarjeta SD
- Registros de transmisión de telemetría
- Mensajes de error

---

## Solución de problemas habituales al cargar el código

### Error: "This LowPower library only works on AVR processors"

**Causa:** Se ha seleccionado un tipo de placa incorrecto en el IDE de Arduino.

**Solución:**

- Ve a **Tools → Board → MiniCore**
- Selecciona **ATmega328**
- Comprueba que **Clock** esté configurado como **External 8 MHz**

---

### Error: "programmer is not responding" o "avrdude: stk500_getsync()"

**Causa:** Hay un problema de comunicación entre el ordenador y el registrador.

**Posibles soluciones:**

1. **Comprueba que el sensor esté desconectado**
    - ⚠️ **¡Es la causa más habitual!** El sensor Maxbotix DEBE estar desconectado durante la carga
    - Desenchufa el conector blanco de la placa

2. **Comprueba la orientación del cable FTDI**
    - Cable verde → marca «GRN»
    - Cable negro → marca «BLK» o lado de la tarjeta SD
    - Vuelve a insertar firmemente el cable FTDI

3. **Comprueba la configuración de la placa**
    - Placa: Arduino Pro o Pro Mini
    - Procesador: ATmega328P (3.3 V, 8 MHz)
    - Puerto correcto seleccionado

4. **Prueba otro puerto USB** del ordenador

5. **Comprueba la alimentación**
    - Interruptor en posición OFF (alimentación por FTDI)
    - O bien, batería instalada e interruptor en posición ON
    - El LED debería indicar que hay alimentación al conectarlo

---

### Error: "SdFat.h: No such file or directory"

**Causa:** Falta una biblioteca necesaria.

**Solución:**

- Instala la biblioteca que falta desde **Sketch → Include Library → Manage Libraries**
- Busca el nombre de la biblioteca (p. ej., «SdFat» o «RTC by Makuna»)
- Haz clic en **Install**
- Reinicia el IDE de Arduino

Este error puede producirse con cualquier biblioteca que falte:

- `LowPower.h` → Instala Rocketscream LowPower (requiere instalación manual)
- `RtcDS3231.h` → Instala RTC by Makuna
- `AltSoftSerial.h` → Instala AltSoftSerial (solo para los modelos con conectividad celular)

---

### El puerto no aparece en el menú Tools

**Causa:** No están instalados los controladores FTDI o no se detecta el cable.

**Soluciones:**

1. **Instala los controladores FTDI**
    - [Descárgalos del sitio web de FTDI](https://ftdichip.com/drivers/vcp-drivers/)
    - O sigue la [guía de Sparkfun](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers)

2. **Prueba otro cable FTDI** (puede que el cable esté defectuoso)

3. **Comprueba que el cable esté bien insertado** en el puerto USB

4. **Reinicia el IDE de Arduino** después de conectar el cable

---

### El código compila, pero no se ejecuta

**Posibles causas:**

1. **Velocidad o voltaje del procesador incorrectos**
    - Deben ser **3.3 V y 8 MHz**, no 5 V y 16 MHz
    - Vuelve a seleccionar el procesador correcto y carga el código otra vez

2. **Voltaje de la batería demasiado bajo**
    - Comprueba el voltaje de la batería con un multímetro
    - Para un funcionamiento fiable, debería ser superior a 3.5 V

3. **Has olvidado volver a conectar el sensor**
    - Vuelve a enchufar el conector blanco del sensor a la placa

---

### ¿Necesitas más ayuda?

- Consulta la guía de [problemas habituales](../operation/troubleshooting/common-issues.md)
- Revisa los [diagramas de diagnóstico](../operation/troubleshooting/diagnostic-flowcharts.md)
- Contacta con el servicio de asistencia: info@riverlabs.uk
