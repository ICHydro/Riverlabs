# Referencia de componentes internos

!!! abstract "Descripción general"
    Esta pagina ofrece información detallada sobre los componentes internos de los registradores Riverlabs, su ubicación, función y necesidades de mantenimiento. No es conocimiento necesario para la operación de los equipos, pero puede ser util en el caso de reparaciones o alteraciones.

## Distribución de los componentes

### Descripción general de los componentes principales

La placa de circuito impreso (PCB) del registrador Riverlabs contiene los siguientes componentes principales:

1. **Microcontrolador (CPU)**: Atmel Atmega328P
2. **Reloj de tiempo real (RTC)**: RTC de precisión DS3231
3. **Ranura para tarjeta SD**: Interfaz para tarjeta microSD (opcional)
4. **Conector de programación FTDI**: Interfaz de programación serie de 6 pines
5. **Contactos de la batería principal**: P. ej., portapilas para batería 18650
6. **Portapilas de la batería de respaldo**: Soporte para pila de botón CR1220
7. **Regulación de alimentación**: Reguladores de tensión y protección
8. **Zócalo XBee** (modelos con telemetría): Interfaz para módem celular
9. **Conector del sensor**: Conexión para sensor ultrasónico o lidar
10. **Indicadores LED**: Indicación de estado
11. **EEPROM**: Almacenamiento temporal externo de datos

![Distribución de componentes de la PCB: próximamente](../../assets/images/pcb-layout-annotated.png)<br>
*Plano anotado de la PCB que muestra la ubicación de los componentes*

---

## Detalles de los componentes

### 1. Microcontrolador (Atmega328P)

**Ubicación:**

- Centro de la PCB
- Circuito integrado cuadrado con numerosos pines

**Función:**

- Procesador principal que ejecuta el gestor de arranque de Arduino
- Ejecuta el código de registro de datos
- Controla todos los periféricos
- Gestiona los estados de energía (reposo/activación)

**Especificaciones:**

- Arquitectura AVR de 8 bits
- 32 KB de memoria flash (almacenamiento del programa)
- 2 KB de SRAM (memoria de trabajo)
- 1 KB de EEPROM (búfer de datos opcional)
- Frecuencia de reloj de 16 MHz (pero operado a 8 MHz)

### 2. Reloj de tiempo real (DS3231)

**Ubicación:**

- Cerca del portapilas de botón

**Función:**

- Mantiene la hora con precisión
- Genera alarmas de activación
- Oscilador de cristal con compensación de temperatura
- Conserva la hora al retirar la batería principal (si hay una CR1220 instalada)

**Especificaciones:**

- Precisión: ±2 ppm (±1 minuto/año)
- Intervalo de temperatura: -40°C a +85°C
- Entrada de alimentación de respaldo para CR1220
- Interfaz de comunicación I2C

**Mantenimiento:**

- No requiere mantenimiento
- Alimentado por la batería de respaldo CR1220
- Conserva la hora automáticamente

**Resolución de problemas:**

- Si el reloj se reinicia: sustituya la batería CR1220
- Si el reloj se desfasa: compruebe que el código inicialice correctamente el RTC

### 3. Ranura para tarjeta SD

**Ubicación:**

- Borde de la PCB; zócalo metálico con resorte

**Función:**

- Almacenamiento local de datos
- Registra todas las mediciones con marcas de tiempo
- Tarjeta extraíble para recuperar los datos

**Especificaciones:**

- Interfaz para tarjeta microSD
- Comunicación SPI
- Admite tarjetas SD y SDHC (hasta 32 GB)
- Sistema de archivos FAT32

**Mantenimiento:**

- Mantenga limpios los contactos
- Compruebe que la tarjeta encaje correctamente
- No fuerce la inserción de la tarjeta
- Compruebe periódicamente que la tarjeta esté bien asentada

**Problemas frecuentes:**

- No se detecta la tarjeta: vuelva a insertarla y compruebe el formato
- Errores de escritura: sustituya la tarjeta (desgaste)
- Escritura lenta: utilice una tarjeta más rápida (clase 10)

### 4. Conector de programación FTDI

**Ubicación:**

- Fila de 6 pines
- Normalmente en el borde de la PCB

**Configuración de pines:**
```
Lado GRN         Lado BLK
  ↓                  ↓
[GND][CTS][VCC][TXD][RXD][DTR]
```

**Marcas en la placa:**

- La marca **"GRN"** indica el lado del cable verde
- La marca **"BLK"** indica el lado del cable negro
- En los modelos celulares o lidar: el pin negro (GND) está del lado de la tarjeta SD

**Función:**

- Interfaz de programación serie para cargar código
- Conexión del cable FTDI para programar desde el IDE de Arduino
- Comunicación con el monitor serie
- Ajuste del reloj mediante la biblioteca RTC

**Uso:**

- **Orientación del cable FTDI:** FUNDAMENTAL
    - El cable verde se alinea con la marca "GRN"
    - El cable negro se alinea con la marca "BLK"
- **Tensión:** admite cables FTDI de 5 V o 3,3 V (se recomiendan 3,3 V)
- **Reinicio automático:** el pin DTR activa el reinicio automático para programar
- **Alimentación:** puede alimentar el registrador cuando está conectado (consulte la sección del interruptor de alimentación)

**Protección:**

- El conector no está protegido frente a la inversión de polaridad
- ⚠️ **Compruebe siempre dos veces la orientación del cable FTDI antes de conectarlo**
- Una conexión incorrecta puede dañar el registrador o impedir la programación

**Tipos de cable:**

- [Cable FTDI Sparkfun de 3,3 V](https://www.sparkfun.com/products/9717): Conexión USB directa
- [Placa adaptadora FTDI Basic de Sparkfun de 3,3 V](https://www.sparkfun.com/products/9873): Requiere un cable micro-USB

### 4b. Pines serie de depuración (DBG)

**Ubicación:**

- Conector de 2 pines cerca del borde de la PCB, etiquetado "DBG"

**Configuración de pines:**
```
[GND] [TX]
```

**Función:**

- Salida serie de depuración por hardware
- Permite supervisar la actividad del registrador en tiempo real sin reprogramarlo
- Permite ver las lecturas del sensor, las escrituras en la SD y el estado de la telemetría
- NO requiere conectarse al conector principal FTDI

**Cómo usar los pines de depuración:**

**Preparación del hardware:**

1. **Equipo necesario:**
    - Cable FTDI o adaptador USB a serie
    - 2 cables de puente hembra-hembra para placa de pruebas

2. **Conexiones:**
   ```
   Logger DBG GND → FTDI Black wire (GND)
   Logger DBG TX  → FTDI Yellow wire (RX/Serial In)
   ```

3. **NO conecte:**
    - El FTDI al conector principal de programación
    - La alimentación del FTDI (el registrador debe alimentarse con batería)

4. **Alimentación:**
    - El registrador debe estar alimentado por batería (interruptor en ON)

**Configuración del software:**

1. Conecte el cable FTDI al ordenador
2. Abra el monitor serie del IDE de Arduino (Tools → Serial Monitor)
3. Ajuste la velocidad a **115200 baudios**
4. Reinicie el registrador (pulse el botón de reinicio)
5. La salida de depuración aparecerá en el monitor serie

**La salida de depuración incluye:**

- Mensajes de arranque e inicialización
- Marca de tiempo del RTC en cada medición
- Lecturas del sensor (brutas y procesadas)
- Estado de escritura en la tarjeta SD
- Estado de la conexión de telemetría
- Registros de comunicación XBee
- Mensajes de error y advertencias

!!! tip "Esencial para resolver problemas"
    Los pines DBG son muy útiles para diagnosticar problemas sobre el terreno. Puede conectarlos con el registrador ya instalado para ver qué ocurre sin retirar la tarjeta SD ni reprogramarlo.

**Mensajes de depuración habituales:**

```
--- Wari Logger Boot ---
RTC Init: OK
SD Card Init: OK
Time: 2026-01-03 14:35:22
Sensor: 1.234m
SD Write: OK
XBee Wake
TX: Data sent
XBee Sleep
Entering sleep...
```

### 5. Contactos de la batería principal

**Ubicación:**

- Contactos con resorte o portabaterías

**Función:**

- Conexión de alimentación principal
- Interfaz para batería 18650
- Terminales positivo y negativo

**Mantenimiento:**

- **Mensualmente:** compruebe si hay corrosión
- **Cuando sea necesario:** limpie con alcohol isopropílico
- **Compruebe:** que la tensión del resorte sea adecuada
- **Verifique:** que no haya oxidación ni depósitos

**Prevención de la corrosión:**

- Aplique una capa fina de DeoxIT a los contactos
- Asegúrese de que los terminales de la batería estén limpios
- Mantenga seco el compartimento

### 6. Portapilas de la batería de respaldo (CR1220)

**Ubicación:**

- Pequeño soporte circular cerca del circuito integrado RTC

**Función:**

- Alimenta el RTC cuando se retira la batería principal
- Mantiene la hora
- No alimenta el registrador

**Instalación:**

- El lado positivo (+) suele quedar hacia arriba
- Presione firmemente hasta que encaje
- Debe quedar a ras del soporte

**Vida útil:**

- Normalmente de 5 a 10 años
- Sustitúyala cada año en aplicaciones críticas
- Sustitúyala si el reloj se reinicia al retirar la batería principal

### 7. Interruptor de alimentación

**Ubicación:**

- Borde de la PCB; interruptor deslizante o de palanca

**Etiqueta:**

 - "ON (Batt) / OFF (ftdi)"

**Función:**

- Controla la alimentación del registrador desde la batería principal
- Permite alimentarlo desde el FTDI para programarlo cuando está en OFF
- Evita que la batería y el USB suministren energía simultáneamente

**Posiciones del interruptor:**

| Posición      | Batería       | Alimentación FTDI | Uso                                |
|---------------|---------------|-------------------|------------------------------------|
| **ON (Batt)** | Conectada     | Bloqueada         | Uso normal, despliegue en campo    |
| **OFF (ftdi)**| Desconectada  | Habilitada        | Programación mediante cable FTDI   |

**Cómo funciona:**

!!! info "Diseño inteligente de alimentación"
    El interruptor evita conectar accidentalmente a la vez la alimentación de la batería y la del USB (a través del FTDI), lo que podría dañar la batería o los componentes.

**Posición ON (batería):**

- La batería alimenta todo el registrador
- El registrador funciona normalmente
- El cable FTDI puede permanecer conectado para programar
- El registrador se alimenta de la batería, no del FTDI
- El LED indicará el funcionamiento normal

**Posición OFF (FTDI):**

- La batería queda desconectada del circuito
- Si el cable FTDI está conectado: el registrador se alimenta por USB
- Si el FTDI no está conectado: el registrador no recibe alimentación
- Facilita la programación sin instalar una batería
- Evita que la alimentación USB dañe la batería

**Opciones de programación:**

Puede programar el registrador en tres configuraciones:

1. **Interruptor en OFF, sin batería y con FTDI conectado**
    - La opción más segura para la primera programación
    - El registrador se alimenta por completo desde USB/FTDI
    - Es la opción más habitual

2. **Interruptor en OFF, con batería instalada y FTDI conectado**
    - La batería está presente, pero desconectada
    - El registrador se alimenta desde USB/FTDI
    - La batería permanece protegida

3. **Interruptor en ON, con batería instalada y FTDI conectado**
    - La batería alimenta el registrador
    - El FTDI solo proporciona la conexión de datos
    - Útil si el cable FTDI no puede suministrar suficiente corriente

**Buenas prácticas:**

✅ **HAGA LO SIGUIENTE:**

- Ponga el interruptor en OFF al programar (recomendado)
- Compruebe la posición del interruptor antes de conectar el FTDI
- Utilice el interruptor para apagar completamente el registrador

❌ **EVITE LO SIGUIENTE:**

- Preocuparse por confundir las posiciones: el diseño evita daños
- Dejar el interruptor en ON durante un almacenamiento prolongado (agota la batería)

**Resolución de problemas:**

| Problema                    | Qué comprobar                                         |
|-----------------------------|------------------------------------------------------|
| Sin alimentación en ON      | Batería agotada; compruebe la tensión                |
| Sin alimentación en OFF     | Cable FTDI no conectado o ausencia de pines de datos |
| No se puede cargar el código| Pruebe OFF; compruebe la orientación del FTDI        |

### 8. Circuito de regulación de alimentación

**Ubicación:**

- Componentes próximos a los contactos de la batería

**Componentes:**

- Reguladores de tensión (alimentación lógica de 3.3 V)
- Diodos de protección
- Condensadores para estabilización
- Circuitos integrados de gestión de energía

**Función:**

- Convierte los 3.7 V de la batería en 3.3 V para la lógica
- Proporciona alimentación estable a la electrónica
- Protege contra la inversión de polaridad (en algunos modelos)
- Protección frente a sobrecorriente

### 8. Zócalo XBee (solo modelos con telemetría)

**Ubicación:**

 - Zócalo destacado, a menudo en un extremo de la PCB

**Función:**

- Aloja el módem celular XBee
- Zócalo de inserción para el módulo XBee
- Pines de alimentación y comunicación

**Instalación del XBee:**

- Alinee la muesca del XBee con el zócalo
- Presione firmemente hasta que quede asentado
- Todos los pines deben entrar en el zócalo
- No debe moverse

**Extracción:**

- Muévalo suavemente de lado a lado mientras tira de él
- Nunca tire de la antena
- Compruebe si hay pines doblados

**Mantenimiento:**

- Mantenga el zócalo libre de polvo
- Compruebe si hay pines doblados
- Asegúrese de que la tarjeta SIM esté instalada en el XBee

### 9. Conector del sensor

**Ubicación:**

 - Conector para el cable del sensor externo

**Tipos:**

- **Wari:** bornera de tornillo de 3–4 pines o conector JST
- **Lidar:** conector JST de 6 pines

**Función:**

- Alimentación del sensor
- Comunicación de datos
- Referencia de tierra

**Mantenimiento:**

- Compruebe que la conexión esté firme
- Inspeccione si hay corrosión
- Verifique que no haya cables rotos
- Compruebe que el cable tenga suficiente alivio de tensión

**Configuración de pines (típica):**

**Wari (MB7389):**
```
1. +5V (power)
2. Serial Out
3. Serial In (sometimes unused)
4. GND
```

**Lidar (Garmin):**
```
1. Power Enable
2. Mode Control
3. +5V
4. I2C SDA
5. I2C SCL
6. GND
```

### 10. Indicadores LED

**Ubicación:**

 - LED de montaje pasante o superficial

**LED habituales:**

| LED            | Color         | Función                       |
|----------------|---------------|-------------------------------|
| **Alimentación** | Rojo/verde  | Registrador encendido         |
| **Actividad**  | Amarillo/azul | Medición en curso             |
| **Error**      | Rojo          | Estado de error               |
| **Telemetría** | Azul/verde    | Actividad del XBee            |

**Comportamiento de los LED:**

**Funcionamiento normal:**

- Destello breve durante la medición
- Luz fija durante la lectura del sensor
- Destellos rápidos durante la escritura en la SD

**Patrones de error:**

- Parpadeo rápido: error de la tarjeta SD
- Luz fija continua: bloqueado en un estado de error
- Sin actividad: problema de alimentación o código

**Ahorro de energía:**

- Los LED se desactivan en el modo de despliegue
- Active el modo DEBUG para ver la actividad

### 11. EEPROM externa (algunos modelos)

**Ubicación:**

- Circuito integrado de 8 pines cerca de la ranura para tarjeta SD

**Función:**

- Almacenamiento temporal de datos
- Guarda las mediciones en memoria antes de escribirlas en la SD
- Prolonga la vida útil de la tarjeta SD
- Evita la pérdida de datos si hay problemas con la SD

**Especificaciones:**

- Normalmente 24LC512 o similar
- Capacidad de 64 KB
- Interfaz I2C
- Memoria no volátil

**Funcionamiento:**

- Las mediciones se escriben primero en la EEPROM
- Se escriben periódicamente en la tarjeta SD en lotes
- El usuario puede configurar el intervalo de volcado

---

## Componentes adicionales

### Condensadores

**Finalidad:**

- Estabilización de la alimentación
- Filtrado de ruido
- Circuitos de temporización

**Mantenimiento:**

- No requieren mantenimiento por parte del usuario

### Resistencias

**Finalidad:**

- Resistencias de polarización de señales
- Limitación de corriente para LED
- Divisores de tensión

**Mantenimiento:**

 - No requieren mantenimiento por parte del usuario

### Cristales/osciladores

**Cristal de 8 MHz (externo):**

- Reloj principal del microcontrolador
- Determina la precisión de la temporización

**Mantenimiento:**

 - No requiere mantenimiento por parte del usuario

---

## Versiones de la PCB

### Cómo identificar la versión de su PCB

**Busque:**

- Número de versión serigrafiado en la PCB (p. ej., "v2.1")
- Código de fecha
- Logotipo o texto de Riverlabs

**Versiones habituales:**

| Versión           | Características                      | Notas                       |
|-------------------|--------------------------------------|-----------------------------|
| **Wari v1**       | Registro básico de datos             | Diseño original             |
| **Wari v2.0**     | + EEPROM, alimentación mejorada      | Mayor fiabilidad            |
| **Wari v2.1**     | + Optimización de telemetría         | Producción actual           |
| **Lidar/WMOnode** | Compatibilidad con sensor lidar      | Elevador de 5 V de mayor potencia |

---

## Seguridad y manipulación

### Protección contra descargas electrostáticas

!!! warning "La electricidad estática puede dañar los componentes electrónicos"
    
**Precauciones:**

- Toque un objeto metálico conectado a tierra antes de manipular la PCB
- Evite manipularla en ambientes secos
- Use una pulsera antiestática si dispone de una
- Guárdela en una bolsa antiestática cuando no esté instalada

### Manipulación física

**Haga lo siguiente:**

- ✅ Sujétela solo por los bordes
- ✅ Utilice herramientas adecuadas
- ✅ Trabaje sobre una superficie no conductora
- ✅ Manténgala alejada de líquidos

**Evite lo siguiente:**

- ❌ Tocar los pines de los componentes
- ❌ Presionar los componentes
- ❌ Exponerla a la humedad
- ❌ Dejarla caer o golpearla

### Limpieza

**Si es necesario:**

- Apague el dispositivo y retire las baterías
- Use alcohol isopropílico de al menos el 90 %
- Use un cepillo suave (de dientes o antiestático)
- Deje secar completamente al aire antes de volver a montarlo
- No use agua ni productos de limpieza

---

## Pruebas de los componentes

### Inspección visual

**Compruebe que:**

- [ ] No haya daños físicos
- [ ] No haya pines doblados
- [ ] No haya corrosión
- [ ] No haya marcas de quemaduras
- [ ] Las soldaduras estén intactas
- [ ] La PCB no tenga grietas

### Pruebas funcionales

**Prueba de alimentación:**

1. Instale una batería nueva
2. Compruebe la actividad del LED
3. Mida la tensión con un multímetro

**Prueba de comunicación:**

1. Conecte el cable FTDI
2. Abra el monitor serie
3. Cargue un programa de prueba
4. Compruebe la salida serie

**Prueba del sensor:**

1. Conecte el sensor
2. Ejecute el código de prueba del sensor
3. Compruebe que las lecturas sean razonables
4. Compruebe si hay mensajes de error

**Prueba de la tarjeta SD:**

1. Inserte una tarjeta formateada
2. Ejecute varias mediciones
3. Retire la tarjeta y lea el archivo de datos
4. Compruebe el formato y el contenido

**Prueba del RTC:**

1. Ajuste el reloj
2. Retire la batería principal (con la batería de respaldo instalada)
3. Espere 1 minuto
4. Vuelva a instalar la batería principal
5. Compruebe la hora: debe ser correcta

---

## Modificaciones y actualizaciones

### Actualizaciones del firmware

**Seguras y recomendadas:**

- Cargue código nuevo mediante FTDI
- Modifique los parámetros configurables por el usuario
- Actualice a la última versión estable

**Procedimiento:**

1. Descargue el firmware nuevo
2. Compruebe su compatibilidad con la versión del hardware
3. Cárguelo mediante el IDE de Arduino
4. Pruébelo a fondo antes de instalarlo en campo

### Modificaciones del hardware

!!! danger "Las modificaciones del hardware son responsabilidad del usuario"
    Las modificaciones de la PCB, sus componentes o sus circuitos pueden dañar el registrador.

---

## Resolución de problemas de los componentes

### Según el síntoma

**Sin alimentación:**

- Compruebe la tensión de la batería
- Inspeccione los contactos de la batería (dóblelos hacia dentro si es necesario)
- Compruebe el LED de alimentación (si existe)
- Busque cortocircuitos (cables sueltos o daños en la PCB)

**El reloj se reinicia:**

- Sustituya la batería de respaldo CR1220 (debe indicar 3 V CC)
- Compruebe el portapilas del RTC
- Verifique que el código ajuste correctamente el reloj

**Errores de la tarjeta SD:**

- Limpie los contactos de la tarjeta
- Pruebe otra tarjeta
- Compruebe si el zócalo está dañado

**Errores del sensor:**

- Compruebe que el conector esté bien sujeto
- Verifique la continuidad del cable
- Pruebe el sensor por separado (en otro registrador que funcione, si es posible)
- Compruebe que haya 5 V en el pin de alimentación (solo lidar)

**Fallo de comunicación serie:**

- Compruebe la orientación del cable FTDI
- Compruebe si hay pines doblados en el conector
- Pruebe otro cable FTDI
- Pruebe con un programa sencillo de parpadeo

---

## Piezas de repuesto

### Componentes sustituibles por el usuario

| Componente            | Número de pieza       | Proveedor       | Dificultad |
|-----------------------|-----------------------|-----------------|------------|
| **Batería 18650**     | Consulte la guía de baterías | Comercio | Fácil     |
| **Batería CR1220**    | CR1220                | Comercio        | Fácil      |
| **Tarjeta microSD**   | Cualquiera compatible con FAT32 | Comercio | Fácil |
| **Módulo XBee**       | XBee3 Cellular        | Digi            | Moderada   |

### Sustitución a cargo de profesionales

- Microcontrolador
- Circuito integrado RTC
- Reguladores de tensión
- Cualquier componente de montaje superficial
- Pistas dañadas de la PCB

**Contacte con Riverlabs para reparaciones:** info@riverlabs.uk

---

## Documentación

### Esquema de la PCB

!!! info "Esquemas de código abierto"
    Escriba a info@riverlabs.uk para obtener más información.

### Lista de materiales (BOM)

Escriba a info@riverlabs.uk para obtener más información.

---

## Próximos pasos

- 🔋 [Guía de baterías y alimentación](battery-power-guide.md): Detalles del sistema de alimentación
- 🛠️ [Guía de mantenimiento](maintenance.md): Procedimientos de mantenimiento periódico
- 🚨 [Resolución de problemas](../troubleshooting/common-issues.md): Problemas de los componentes
- 💻 [Configuración de Arduino](../quick-start/arduino-setup.md): Programación y firmware

---

!!! note "Evolución de la PCB"
    Riverlabs mejora continuamente el diseño de sus PCB. Aunque la ubicación de los componentes puede variar ligeramente entre versiones, las funciones principales y el proceso de programación siguen siendo los mismos.