# Programación de los registradores Riverlabs

!!! abstract "Resumen"
    En la sección anterior de este capítulo se explicó cómo instalar el IDE de Arduino, las definiciones de placa y las bibliotecas necesarias. Con todo instalado, ya se puede conectar el registrador a un ordenador para compilar y cargar el código. En esta sección se explica cómo hacerlo.

## Conectar el registrador al ordenador

!!! warning "Desconecte primero el sensor Maxbotix"
    Desconecte el sensor ultrasónico Maxbotix del registrador antes de programarlo. El sensor y el cable FTDI utilizan el mismo puerto serie, lo que provoca interferencias e impide cargar el código correctamente. Desenchufe el conector blanco.

Como se explicó en la sección anterior, los registradores Wari no tienen puerto USB, por lo que se necesita un cable o una placa FTDI para conectarlos al conector de 6 pines situado en el lateral de la placa de circuito impreso. La placa lleva marcas para indicar la orientación correcta: el pin «GRN» debe coincidir con el cable verde, y el pin «BLK», con el cable negro.

<!-- insert picture -->

!!! danger "Compruebe la orientación"
    Si el cable FTDI se conecta en la orientación incorrecta, es posible que no pueda programar el registrador o que se dañen los componentes. Compruebe siempre las marcas GRN/BLK antes de conectarlo.

Conecte ahora el cable a un puerto USB del ordenador. Si el dispositivo reconoce correctamente el chip FTDI, aparecerá en el menú **Tools → Port** del IDE de Arduino. Los nombres de los puertos dependen del sistema operativo:

- **macOS:** `/dev/cu.usbserial-XXXXXXXX`
- **Linux:** `/dev/ttyUSB0` o `/dev/ttyACM0`
- **Windows:** `COM3`, `COM4`, etc.

Si no aparece ningún puerto, compruebe lo siguiente:

- Que los controladores FTDI estén instalados (solo en Windows)
- Que el cable funcione al conectarlo a otro puerto USB del ordenador

Para resolver problemas con más detalle, la [guía de FTDI de Sparkfun](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers) ofrece una explicación exhaustiva sobre el uso de estos cables.

## Seleccionar el puerto y configurar la placa en el IDE de Arduino

Asegúrese de seleccionar el puerto correcto en **Tools → Port**. Si hay otros dispositivos conectados al ordenador, pueden aparecer varios puertos, como «COM3» y «COM4». Para identificar el correcto, desconecte el cable FTDI, consulte la lista de puertos, vuelva a conectarlo y compruebe la lista de nuevo. El puerto COM asignado al cable FTDI debería desaparecer y volver a aparecer.

A continuación, seleccione las definiciones de placa correctas en el menú **Tools**:


1. **Board:** MiniCore → ATmega328
2. **Clock:** External 8 MHz
3. **BOD:** BOD 2.7 V
4. **Variant:** 328P / 328PA
5. **Bootloader:** Yes (UART0)

Si estas opciones no aparecen en el menú, es posible que tenga que instalar las definiciones de placa de MiniCore mediante el gestor de placas.

!!! warning "La configuración de la placa es fundamental"
    La placa DEBE configurarse como **MiniCore → ATmega328** y el reloj como **Clock: External 8 MHz**. Una configuración incorrecta puede causar fallos al cargar el código o problemas durante el funcionamiento.

## Cargar el código de Riverlabs

### Ajustar el reloj

Solo es necesario realizar este paso si la placa es completamente nueva, si se ha instalado una pila de botón CR1220 nueva o si se ha retirado la pila. El reloj conservará la hora mientras la pila de botón permanezca instalada, incluso después de cargar código nuevo.

El reloj se puede ajustar con el programa «set_clock.ino» del repositorio de Riverlabs. Abra el monitor serie **antes* de cargar el programa para comprobar que el reloj se ajusta correctamente. Si no aparece nada en el monitor serie, compruebe que la velocidad esté configurada en **115200 baudios**. En [Instructables](https://www.instructables.com/id/HOW-TO-use-the-ARDUINO-SERIAL-MONITOR/) encontrará un buen tutorial sobre el uso del monitor serie de Arduino.

!!! info "Zonas horarias diferentes"
     Si ajusta el reloj con este programa, se establecerá según la zona horaria de su ordenador. Puede cambiarla modificando la variable «TZ» del programa. Establezca `TZ` con la diferencia horaria respecto de UTC, en horas (p. ej., `-5` para EST o `+1` para CET). En nuestras investigaciones hemos adoptado la práctica de ajustar siempre el reloj a UTC (GMT). Utilizamos registradores en distintas zonas horarias, y esto reduce el riesgo de confusiones. Si tiene un registrador Riverlabs preprogramado, es muy probable que el reloj ya esté configurado en UTC.

!!! tip "Compensación del tiempo de carga"
    Durante la compilación, el programa incorpora la hora del ordenador al código y, cuando este se ejecuta en el registrador, utiliza esa hora para ajustar el reloj. Entre ambos pasos transcurren aproximadamente **10 segundos**. El programa ya compensa ese retraso. Como depende del ordenador utilizado, puede ajustar el valor si lo desea:

La compensación del retraso se ajusta en esta línea del programa:
    
    ```cpp
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__) - TZ * 3600 + 10;
    ```

### Cargar el código del registrador

1. **Descargue el programa adecuado** para su modelo de registrador. Estamos trabajando en unificar el código, pero, por el momento, cada modelo tiene el suyo. Consulte el README del repositorio para identificar el código correcto para su modelo.

2. **Abra el programa** en el IDE de Arduino

3. **Configure los ajustes** del código (si es necesario):
    - Intervalo de medición
    - Calibración del sensor
    - Ajustes de telemetría

4. **Haga clic en el botón de carga** (icono de flecha →)

5. **Espere a que termine la carga**
    - La barra de progreso mostrará la compilación
    - Después aparecerá «Uploading...»
    - Mensaje de éxito: «Done uploading»

6. **Compruebe la carga:**
    - El LED debería encenderse durante aproximadamente 1 segundo
    - El registrador ya está programado y listo para su uso

7. **Vuelva a conectar el sensor** (si lo había desconectado)

!!! success "Carga completada"
    Si el LED ha parpadeado, el registrador se ha programado correctamente. Ya puede desconectar el cable FTDI e instalar el registrador.

---

## Diagnóstico mediante el puerto serie de hardware

Durante el funcionamiento, el registrador envía mensajes al puerto serie. Esto permite comprobar su funcionamiento y detectar problemas. Para verlos, deje el cable FTDI conectado al registrador y abra el monitor serie. Configure la velocidad en 115200 baudios. Al abrir el monitor serie, el IDE de Arduino reiniciará el registrador y verá los mensajes de diagnóstico desde el inicio. Es un comportamiento normal. Entre otros datos, aparecerán:

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

- Vaya a **Tools → Board → MiniCore**
- Seleccione **ATmega328**
- Compruebe que **Clock** esté configurado como **External 8 MHz**

---

### Error: "programmer is not responding" o "avrdude: stk500_getsync()"

**Causa:** Hay un problema de comunicación entre el ordenador y el registrador.

**Posibles soluciones:**

1. **Compruebe que el sensor esté desconectado**
    - ⚠️ **¡Es la causa más habitual!** El sensor Maxbotix DEBE estar desconectado durante la carga
    - Desenchufe el conector blanco de la placa

2. **Compruebe la orientación del cable FTDI**
    - Cable verde → marca «GRN»
    - Cable negro → marca «BLK» o lado de la tarjeta SD
    - Vuelva a insertar firmemente el cable FTDI

3. **Compruebe la configuración de la placa**
    - Placa: Arduino Pro o Pro Mini
    - Procesador: ATmega328P (3.3 V, 8 MHz)
    - Puerto correcto seleccionado

4. **Pruebe otro puerto USB** del ordenador

5. **Compruebe la alimentación**
    - Interruptor en posición OFF (alimentación por FTDI)
    - O bien, batería instalada e interruptor en posición ON
    - El LED debería indicar que hay alimentación al conectarlo

---

### Error: "SdFat.h: No such file or directory"

**Causa:** Falta una biblioteca necesaria.

**Solución:**

- Instale la biblioteca que falta desde **Sketch → Include Library → Manage Libraries**
- Busque el nombre de la biblioteca (p. ej., «SdFat» o «RTC by Makuna»)
- Haga clic en **Install**
- Reinicie el IDE de Arduino

Este error puede producirse con cualquier biblioteca que falte:

- `LowPower.h` → Instale Rocketscream LowPower (requiere instalación manual)
- `RtcDS3231.h` → Instale RTC by Makuna
- `AltSoftSerial.h` → Instale AltSoftSerial (solo para los modelos con conectividad celular)

---

### El puerto no aparece en el menú Tools

**Causa:** No están instalados los controladores FTDI o no se detecta el cable.

**Soluciones:**

1. **Instale los controladores FTDI**
    - [Descárguelos del sitio web de FTDI](https://ftdichip.com/drivers/vcp-drivers/)
    - O siga la [guía de Sparkfun](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers)

2. **Pruebe otro cable FTDI** (puede que el cable esté defectuoso)

3. **Compruebe que el cable esté bien insertado** en el puerto USB

4. **Reinicie el IDE de Arduino** después de conectar el cable

---

### El código compila, pero no se ejecuta

**Posibles causas:**

1. **Velocidad o voltaje del procesador incorrectos**
    - Deben ser **3.3 V y 8 MHz**, no 5 V y 16 MHz
    - Vuelva a seleccionar el procesador correcto y cargue el código otra vez

2. **Voltaje de la batería demasiado bajo**
    - Compruebe el voltaje de la batería con un multímetro
    - Para un funcionamiento fiable, debería ser superior a 3.5 V

3. **Ha olvidado volver a conectar el sensor**
    - Vuelva a enchufar el conector blanco del sensor a la placa

---

### ¿Necesita más ayuda?

- Consulte la guía de [problemas habituales](../operation/troubleshooting/common-issues.md)
- Revise los [diagramas de diagnóstico](../operation/troubleshooting/diagnostic-flowcharts.md)
- Contacte con el servicio de asistencia: info@riverlabs.uk
