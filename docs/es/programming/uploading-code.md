## Carga del Código

### Obtener las Bibliotecas Requeridas

El código usa las siguientes bibliotecas externas que deben instalarse por separado:

* **RTC by Makuna**: Control del reloj en tiempo real
* **SoftwareSerial**: Comunicación serie por software (modelos celulares)
* **SdFat**: Sistema de archivos de tarjeta SD (usar la versión original de Bill Greiman)
* **AltSoftSerial**: Serie por software alternativo (solo para modelos celulares/lidar)
* **Rocketscream LowPower**: Modos de sueño de bajo consumo

Las primeras 4 bibliotecas se pueden instalar a través del Administrador de Bibliotecas de Arduino:

1. Abrir Arduino IDE
2. Ir a **Sketch → Include Library → Manage Libraries**
3. Buscar cada biblioteca por nombre
4. Hacer clic en **Install**

!!! note "Versión de SdFat"
    Use la biblioteca SdFat original de Bill Greiman. Si aparecen múltiples versiones, seleccione la escrita por Bill Greiman.

La biblioteca **Rocketscream LowPower** no está disponible a través del Administrador de Bibliotecas. Necesitará:

1. Descargar el código de la [página de Github](https://github.com/rocketscream/Low-Power)
2. Extraer el archivo ZIP
3. Mover la carpeta a su directorio de bibliotecas de Arduino
4. Reiniciar el Arduino IDE

Consulte la [Instalación Manual](https://www.arduino.cc/en/Guide/Libraries) en el sitio web de Arduino para instrucciones detalladas.

### Consideraciones de Energía

El registrador tiene un interruptor de alimentación, que está etiquetado "ON (Batt) / OFF (ftdi)". Esto significa que el registrador recibe alimentación de la batería cuando está encendido (como es de esperar). En el estado apagado, el registrador recibirá alimentación del cable ftdi, si hay uno conectado, y de lo contrario no tendrá ninguna alimentación.

Esto hace posible programar el registrador sin una batería (ya que el registrador obtendrá energía del cable ftdi), lo cual es conveniente. Pero también puede programar el registrador cuando hay una batería insertada, ya sea en la posición OFF (cuando la batería está desconectada, y el registrador obtiene energía del cable ftdi) y en la posición ON (cuando el registrador obtendrá energía de la batería).

Este diseño también hace imposible conectar la alimentación ftdi y la alimentación de la batería al mismo tiempo, lo que puede dañar la batería (ya que puede forzarse con alimentación de 5 V del puerto USB a través del cable ftdi).

### Obtener un Cable FTDI

Para minimizar el costo de los componentes, los registradores Riverlabs no tienen conexión USB. En su lugar, usan una interfaz serie que requiere un **convertidor USB a Serie (nivel TTL)**. Las herramientas más comunes son:

- **Cable FTDI**: Conexión USB directa (p.ej., [Sparkfun FTDI Cable](https://www.sparkfun.com/products/9717))
- **Placa de Conexión FTDI**: Pequeña placa que requiere cable micro-USB (p.ej., [Sparkfun FTDI Basic](https://www.sparkfun.com/products/9873))

Los cables/placas FTDI vienen en versiones de **3.3 V** o **5 V**. Los registradores Riverlabs son compatibles con ambos, pero **se recomienda 3.3 V**.

!!! tip "Instalación de Controladores FTDI"
    Siga [estas instrucciones](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers) para instalar los controladores FTDI en su computadora.

### Conectar el Registrador a su Computadora

!!! warning "Desconectar el Sensor Primero"
    **CRÍTICO:** Desconecte el sensor ultrasónico Maxbotix del registrador antes de programar. El sensor y el cable FTDI usan el mismo puerto serie, lo que causará interferencias e impedirá la carga exitosa del código. Desconecte el conector blanco.

**Pasos de Conexión:**

1. **Retirar o desconectar la batería** (recomendado para las primeras cargas)
    - Coloque el interruptor de alimentación en posición "OFF" si la batería está instalada
   
2. **Identificar la orientación de los pines FTDI** en la placa del registrador:
    - Buscar las marcas **"GRN"** y **"BLK"** en la placa
    - Para modelos celulares/lidar: El pin negro (GND) está en el lado del ranura de tarjeta SD

3. **Conectar el cable/placa FTDI a los pines FTDI:**
    - Cable verde → lado "GRN"
    - Cable negro → lado "BLK"
    - Cable amarillo → RX
    - Cable naranja → TX
   
4. **Conectar el cable/placa FTDI al puerto USB de su computadora**

!!! danger "Verificar la Orientación Dos Veces"
    Una orientación FTDI incorrecta puede impedir la programación o potencialmente dañar componentes. Verifique siempre las marcas GRN/BLK antes de conectar.

### Configurar la Placa Correcta en el Arduino IDE

**Configuración de la Placa** (en el menú **Tools**):

1. **Board:** MiniCore → ATmega328
2. **Clock:** External 8 MHz
3. **BOD:** BOD 2.7V
4. **Variant:** 328P / 328PA
5. **Bootloader:** Yes (UART0)

!!! warning "La Configuración de la Placa es Crítica"
    La placa DEBE configurarse en **MiniCore → ATmega328** con **Clock: External 8 MHz**. Usar configuraciones incorrectas puede causar fallos de carga o problemas de ejecución.

!!! info "Instalación de MiniCore"
    Si no ve MiniCore en su lista de placas, necesita instalarlo a través del Administrador de Placas. Agregue esta URL en File → Preferences → Additional Boards Manager URLs:
    ```
    https://mcudude.github.io/MiniCore/package_MCUdude_MiniCore_index.json
    ```
    Luego instale MiniCore desde Tools → Board → Boards Manager.

**Seleccionar el Puerto:**

1. Ir a **Tools → Port**
2. Seleccionar el puerto que aparece después de conectar el cable FTDI
3. Los nombres de los puertos varían según el sistema operativo:
    - **macOS:** `/dev/cu.usbserial-XXXXXXXX`
    - **Linux:** `/dev/ttyUSB0` o `/dev/ttyACM0`
    - **Windows:** `COM3`, `COM4`, etc.

Si no aparece ningún puerto:

- Verificar que los controladores FTDI estén instalados
- Intentar con un puerto USB diferente en su computadora
- Verificar la conexión del cable FTDI al registrador

Para resolución de problemas detallada, consulte la [Guía FTDI de Sparkfun](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers).

### Configurar el Reloj

Este paso solo es necesario cuando se coloca una nueva batería de moneda CR1220 o cuando se ha retirado la batería. El reloj retendrá la hora siempre que la batería de moneda esté en su lugar, incluso si se carga código nuevo.

El reloj se puede configurar usando el script de ejemplo proporcionado por la biblioteca RTC. En el Arduino IDE vaya a File -> Examples -> Rtc by Makuna -> RS3231_Simple.ino. Abra el script. Abra un Monitor Serie, establezca la velocidad de baudios en 57600, y haga clic en el botón "upload". Si todo va bien, el monitor debería mostrar la hora correcta cada 10 segundos.

Un buen tutorial sobre cómo usar el Monitor Serie en Arduino se puede encontrar en [Instructables](https://www.instructables.com/id/HOW-TO-use-the-ARDUINO-SERIAL-MONITOR/).

!!! info "Nota de Zona Horaria"
    Los registradores Riverlabs están originalmente programados en la zona horaria **UTC (GMT)**. Si configura el reloj nuevamente, se establecerá según la zona horaria de su computadora.

!!! tip "Compensar el Retraso de Carga"
    Hay aproximadamente un **retraso de 10 segundos** entre cuando se compila el código y cuando el microcontrolador lo ejecuta. Esto significa que el reloj estará 10 segundos atrasado.
    
    **Para compensar**, modifique el script DS3231_Simple:
    
    ```cpp
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__) + 10;
    ```
    
    **Para diferentes zonas horarias**, use esta fórmula:
    
    ```cpp
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__) - TZ * 3600 + 10;
    ```
    
    Reemplace `TZ` con su desplazamiento de tiempo en horas (p.ej., `-5` para EST, `+1` para CET).

### Cargar el Código del Registrador

1. **Descargue el sketch apropiado** para su modelo de registrador:
    - `wari.ino`: Registradores Wari Ultrasónico
    - `wari_lidar.ino`: Registradores Wari Lidar
    - `wari_3G.ino`: Wari con telemetría celular 3G
    - `wari_4G.ino`: Wari con telemetría celular 4G

2. **Abra el sketch** en el Arduino IDE

3. **Configure los ajustes** en el código (si es necesario):
    - Intervalo de medición
    - Calibración del sensor
    - Configuración de telemetría

4. **Haga clic en el botón Upload** (icono de flecha →)

5. **Espere a que la carga se complete**
    - La barra de progreso mostrará la compilación
    - Luego aparecerá "Uploading..."
    - Mensaje de éxito: "Done uploading"

6. **Verifique la carga:**
    - El LED debería iluminarse durante aproximadamente 1 segundo
    - El registrador ya está programado y listo

7. **Vuelva a conectar el sensor** (si lo desconectó antes)

!!! success "Carga Completa"
    ¡Si el LED parpadeó, su registrador está programado correctamente! Ahora puede desconectar el cable FTDI y desplegar su registrador.

---

## Depuración con Puerto Serie de Hardware

Para depuración avanzada, puede usar los **pines DBG (depuración)** en la placa del registrador para monitorear la salida serie sin reprogramar.

### Configuración de Hardware

**Lo que Necesita:**

- Cable FTDI o placa de conexión
- 2× cables puente de placa de pruebas (hembra a hembra o macho a hembra)

**Conexión:**

1. Localice los **pines DBG** en la placa del registrador (generalmente cabezal de 2 pines cerca del borde)
2. Conecte usando cables puente:
    - **Pin DBG GND** → **Cable negro** del cable FTDI
    - **Pin DBG TX** → **Cable amarillo (RX)** del cable FTDI
3. Deje el cable FTDI **sin conectar** al cabezal FTDI principal
4. Alimente el registrador normalmente (batería ON)

### Ver la Salida de Depuración

1. Conectar el cable FTDI a la computadora
2. Abrir el **Monitor Serie** del Arduino IDE (Tools → Serial Monitor)
3. Establecer la velocidad de baudios en **115200**
4. Resetear el registrador presionando el botón de reset
5. La información de depuración aparecerá en el monitor

!!! tip "Velocidad de baudios del Monitor Serie de depuración"
    La salida de depuración usa **115200 baudios**. Asegúrate de que el Monitor Serie esté configurado con esta velocidad, de lo contrario verás texto ilegible.

**Qué verás:**

- Mensajes de arranque
- Lecturas del sensor
- Marcas de tiempo del RTC
- Estado de la tarjeta SD
- Registros de transmisión de telemetría
- Mensajes de error

Esto es de gran utilidad para diagnosticar problemas en campo o durante el desarrollo.

---

## Solución de Problemas Comunes de Carga

### Error: "This LowPower library only works on AVR processors"

**Causa:** Tipo de placa incorrecto seleccionado en Arduino IDE.

**Solución:**

- Ir a **Tools → Board → MiniCore**
- Seleccionar **ATmega328**
- Verificar que **Clock** esté configurado en **External 8 MHz**

---

### Error: "programmer is not responding" o "avrdude: stk500_getsync()"

**Causa:** Problema de comunicación entre el ordenador y el registrador.

**Soluciones a probar:**

1. **Comprobar que el sensor está desconectado**
    - ⚠️ **¡Problema más común!** El sensor Maxbotix DEBE estar desenchufado durante la carga
    - Desenchufar el conector blanco de la placa

2. **Verificar la orientación del cable FTDI**
    - Cable verde → marcado "GRN"
    - Cable negro → marcado "BLK" o lado de la tarjeta SD
    - Volver a insertar el cable FTDI firmemente

3. **Comprobar la configuración de la placa**
    - Placa: Arduino Pro or Pro Mini
    - Procesador: ATmega328P (3.3 V, 8 MHz)
    - Puerto correcto seleccionado

4. **Probar un puerto USB diferente** en el ordenador

5. **Comprobar la alimentación**
    - Interruptor de alimentación en posición OFF (alimentado por FTDI)
    - O batería instalada e interruptor en ON
    - El LED debe indicar alimentación al conectar

---

### Error: "SdFat.h: No such file or directory"

**Causa:** Librería requerida no instalada.

**Solución:**

- Instalar la librería que falta mediante **Sketch → Include Library → Manage Libraries**
- Buscar el nombre de la librería (p. ej., "SdFat", "RTC by Makuna")
- Hacer clic en **Install**
- Reiniciar Arduino IDE

Este error puede ocurrir con cualquier librería que falte:

- `LowPower.h` → Instalar Rocketscream LowPower (requiere instalación manual)
- `RtcDS3231.h` → Instalar RTC by Makuna
- `AltSoftSerial.h` → Instalar AltSoftSerial (solo modelos celulares)

---

### El Puerto No Aparece

**Causa:** Controladores FTDI no instalados o cable no detectado.

**Soluciones:**

1. **Instalar los controladores FTDI**
    - [Descargar desde el sitio de FTDI](https://ftdichip.com/drivers/vcp-drivers/)
    - O seguir la [guía de Sparkfun](https://learn.sparkfun.com/tutorials/how-to-install-ftdi-drivers)

2. **Probar con un cable FTDI diferente** (el cable puede estar defectuoso)

3. **Comprobar que el cable esté completamente insertado** en el puerto USB

4. **Reiniciar Arduino IDE** después de conectar el cable

---

### El Código Compila pero No Funciona

**Posibles causas:**

1. **Velocidad/voltaje del procesador incorrecto**
    - Debe ser **3.3 V, 8 MHz** y no 5 V, 16 MHz
    - Volver a seleccionar el procesador correcto y recargar

2. **Voltaje de la batería demasiado bajo**
    - Comprobar el voltaje de la batería con un multímetro
    - Debe ser >3.5 V para un funcionamiento fiable

3. **Olvidaste volver a conectar el sensor**
    - Enchufar el conector blanco del sensor de nuevo en la placa

---

### ¿Necesitas más ayuda?

- Consultar la guía de [Problemas comunes](../operation/troubleshooting/common-issues.md)
- Revisar los [Diagramas de flujo de diagnóstico](../operation/troubleshooting/diagnostic-flowcharts.md)
