# Referencia de Componentes Internos

!!! abstract "Resumen"
    Esta guía proporciona información detallada sobre los componentes internos de los registradores Riverlabs, sus ubicaciones, funciones y requisitos de mantenimiento.

!!! info "Se Necesitan Fotos del PCB"
    Esta sección se mejorará con fotografías anotadas del PCB y renderizados 3D. Las llamadas a los componentes se generarán usando Fusion 360 según se sugirió.

## Distribución de Componentes

### Descripción General de los Componentes Principales

El PCB del registrador Riverlabs contiene los siguientes componentes clave:

1. **Microcontrolador (CPU)**: Atmel Atmega328P
2. **Reloj en Tiempo Real (RTC)**: DS3231 RTC de precisión
3. **Ranura de Tarjeta SD**: Interfaz de tarjeta microSD (opcional)
4. **Cabezal de Programación FTDI**: Interfaz de programación serie de 6 pines
5. **Contactos de la Batería Principal**: p.ej., soporte de batería 18650
6. **Soporte de Batería de Respaldo**: Clip de celda de moneda CR1220
7. **Regulación de Energía**: Reguladores de voltaje y protección
8. **Zócalo XBee** (modelos con telemetría): Interfaz del módem celular
9. **Conector del Sensor**: Conexión al sensor de ultrasonido/lidar
10. **Indicadores LED**: Indicación de estado
11. **EEPROM**: Almacenamiento en búfer de datos externo

![Distribución de componentes del PCB - próximamente](../../assets/images/pcb-layout-annotated.png)<br>
*Distribución anotada del PCB mostrando las ubicaciones de los componentes*

---

## Detalles de Componentes

### 1. Microcontrolador (Atmega328P)

**Ubicación:**

- Centro del PCB
- IC cuadrado con muchos pines

**Función:**

- Procesador principal que ejecuta el cargador de arranque de Arduino
- Ejecuta el código de registro
- Controla todos los periféricos
- Gestiona los estados de energía (sueño/activo)

**Especificaciones:**

- Arquitectura AVR de 8 bits
- 32 KB de memoria flash (almacenamiento del programa)
- 2 KB de SRAM (memoria de trabajo)
- 1 KB de EEPROM (búfer de datos opcional)
- Velocidad de reloj de 16 MHz

**Serviciable por el Usuario:**

- No — no intente retirar ni reemplazar

### 2. Reloj en Tiempo Real (DS3231)

**Ubicación:**

- Cerca del soporte de batería de moneda

**Función:**

- Mantiene la hora precisa
- Genera alarmas de despertar
- Oscilador de cristal compensado por temperatura
- Mantiene la hora durante el retiro de la batería principal (si está instalado CR1220)

**Especificaciones:**

- Precisión: ±2 ppm (±1 minuto/año)
- Rango de temperatura: -40°C a +85°C
- Entrada de batería de respaldo para CR1220
- Interfaz de comunicación I2C

**Mantenimiento:**

- No requiere mantenimiento
- Alimentado por la batería de respaldo CR1220
- Mantiene la hora automáticamente

**Resolución de Problemas:**

- Si el reloj se resetea: Reemplazar la batería CR1220
- Si el reloj se desvía: Verificar el código para una correcta inicialización del RTC

### 3. Ranura de Tarjeta SD

**Ubicación:**

- Borde del PCB, zócalo metálico con resorte

**Función:**

- Almacenamiento de datos local
- Registra todas las mediciones con marcas de tiempo
- Extraíble para recuperación de datos

**Especificaciones:**

- Interfaz de tarjeta microSD
- Comunicación SPI
- Admite tarjetas SD y SDHC (hasta 32 GB)
- Sistema de archivos FAT32

**Mantenimiento:**

- Mantener los contactos limpios
- Asegurar que la tarjeta encaje en su lugar
- No forzar la inserción de la tarjeta
- Verificar regularmente el asiento de la tarjeta

**Problemas Comunes:**

- Tarjeta no detectada: Volver a insertar la tarjeta y verificar el formateo
- Errores de escritura: Reemplazar la tarjeta (desgaste)
- Escrituras lentas: Usar una tarjeta más rápida (Clase 10)

### 4. Cabezal de Programación FTDI

**Ubicación:**

- Fila de 6 pines
- Generalmente en el borde del PCB

**Configuración de Pines:**
```
Lado GRN          Lado BLK
  ↓                  ↓
[GND][CTS][VCC][TXD][RXD][DTR]
```

**Marcas en la Placa:**

- La marca **"GRN"** indica el lado del cable verde
- La marca **"BLK"** indica el lado del cable negro
- En modelos celulares/lidar: El pin negro (GND) está en el lado de la ranura de tarjeta SD

**Función:**

- Interfaz de programación serie para carga de código
- Conectar cable FTDI para programación con Arduino IDE
- Comunicación con el monitor serie
- Configuración del reloj mediante la biblioteca RTC

**Uso:**

- **Orientación del Cable FTDI:** CRÍTICO
    - El cable verde se alinea con la marca "GRN"
    - El cable negro se alinea con la marca "BLK"
- **Voltaje:** Se admiten cables FTDI de 5 V o 3.3 V (se recomienda 3.3 V)
- **Auto-reset:** El pin DTR activa el reset automático para la programación
- **Alimentación:** Puede alimentar el registrador cuando está conectado (ver sección de Interruptor de Alimentación)

**Protección:**

- El cabezal no está protegido contra polaridad inversa
- ⚠️ **Verifique siempre la orientación del cable FTDI antes de conectar**
- Una conexión incorrecta puede dañar el registrador o impedir la programación

**Tipos de Cable:**

- [Sparkfun FTDI Cable 3.3 V](https://www.sparkfun.com/products/9717): Conexión USB directa
- [Sparkfun FTDI Basic Breakout 3.3 V](https://www.sparkfun.com/products/9873): Requiere cable micro-USB

### 4b. Pines Serie de Depuración (DBG)

**Ubicación:**

- Cabezal de 2 pines cerca del borde del PCB, etiquetado "DBG"

**Configuración de Pines:**
```
[GND] [TX]
```

**Función:**

- Salida de depuración serie de hardware
- Monitorear la actividad del registrador en tiempo real sin reprogramar
- Ver lecturas del sensor, escrituras en SD y estado de telemetría
- NO requiere conexión al cabezal FTDI principal

**Cómo Usar los Pines de Depuración:**

**Configuración de Hardware:**

1. **Equipo necesario:**
    - Cable FTDI o adaptador USB a serie
    - 2× cables puente de placa de pruebas hembra a hembra

2. **Conexiones:**
   ```
   DBG GND del registrador → Cable negro del FTDI (GND)
   DBG TX del registrador  → Cable amarillo del FTDI (RX/Entrada Serie)
   ```

3. **NO conectar:**
    - FTDI al cabezal principal de programación
    - Alimentación desde FTDI (el registrador debe estar alimentado por batería)

4. **Alimentación:**
    - El registrador debe estar alimentado por batería (interruptor en ON)

**Configuración de Software:**

1. Conectar el cable FTDI al ordenador
2. Abrir el Monitor Serie de Arduino IDE (Herramientas → Monitor Serie)
3. Establecer la velocidad de baudios en **115200**
4. Reiniciar el registrador (presionar el botón de reset)
5. La salida de depuración aparecerá en el Monitor Serie

**La salida de depuración incluye:**

- Mensajes de arranque e inicialización
- Marca de tiempo del RTC en cada medición
- Lecturas del sensor (crudas y procesadas)
- Estado de escritura en tarjeta SD
- Estado de conexión de telemetría
- Registros de comunicación XBee
- Mensajes de error y advertencias

!!! tip "Esencial para la resolución de problemas"
    Los pines DBG son invaluables para diagnosticar problemas en campo. Puedes conectarlos mientras el registrador está desplegado para ver qué está ocurriendo sin retirar la tarjeta SD ni reprogramar.

**Mensajes de depuración comunes:**

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

### 5. Contactos de la Batería Principal

**Ubicación:**

- Contactos con resorte o portapilas

**Función:**

- Conexión de alimentación principal
- Interfaz para batería 18650
- Terminales positivo y negativo

**Mantenimiento:**

- **Mensual:** Inspeccionar por corrosión
- **Según sea necesario:** Limpiar con alcohol isopropílico
- **Verificar:** Tensión adecuada del resorte
- **Comprobar:** Sin oxidación ni acumulación

**Prevención de la corrosión:**

- Capa fina de DeoxIT en los contactos
- Asegurarse de que los terminales de la batería estén limpios
- Mantener el compartimento seco

### 6. Portapilas de Respaldo (CR1220)

**Ubicación:**

- Soporte circular pequeño cerca del CI RTC

**Función:**

- Alimenta el RTC cuando se retira la batería principal
- Mantiene el registro del tiempo
- No alimenta el registrador

**Instalación:**

- El lado positivo (+) normalmente mira hacia arriba
- Presionar firmemente hasta que encaje
- Debe quedar enrasado en el soporte

**Vida útil:**

- Típicamente 5–10 años
- Reemplazar anualmente en aplicaciones críticas
- Reemplazar si el reloj se reinicia al retirar la batería principal

### 7. Interruptor de Alimentación

**Ubicación:**

- Borde de la PCB, interruptor deslizante o de palanca

**Etiqueta:**

- "ON (Batt) / OFF (ftdi)"

**Función:**

- Controla la alimentación de la batería principal al registrador
- Permite la alimentación por FTDI cuando está en OFF
- Función de seguridad que evita alimentación simultánea por batería + USB

**Posiciones del interruptor:**

| Posición       | Batería      | Alimentación FTDI | Caso de uso                              |
|----------------|--------------|-------------------|------------------------------------------|
| **ON (Batt)**  | Conectada    | Bloqueada         | Operación normal, despliegue en campo    |
| **OFF (ftdi)** | Desconectada | Habilitada        | Programación mediante cable FTDI         |

**Cómo funciona:**

!!! info "Diseño de alimentación inteligente"
    El interruptor de alimentación está diseñado para evitar conectar accidentalmente tanto la batería como la alimentación USB (mediante FTDI) de forma simultánea, lo que podría dañar la batería o los componentes.

**Posición ON (Batería):**

- La batería alimenta todo el registrador
- El registrador funciona con normalidad
- El cable FTDI puede conectarse igualmente para programar
- El registrador recibe alimentación de la batería, no del FTDI
- El LED mostrará operación normal

**Posición OFF (FTDI):**

- La batería está desconectada del circuito
- Si se conecta el cable FTDI: El registrador se alimenta por USB
- Si no se conecta el FTDI: El registrador no tiene alimentación
- Conveniente para programar sin batería instalada
- Evita daños a la batería por la alimentación USB

**Opciones de programación:**

Puedes programar el registrador en tres configuraciones:

1. **Interruptor en OFF, sin batería, FTDI conectado**
    - La opción más segura para la primera programación
    - El registrador se alimenta completamente por USB/FTDI
    - Enfoque más común

2. **Interruptor en OFF, batería instalada, FTDI conectado**
    - Batería desconectada pero presente
    - El registrador se alimenta por USB/FTDI
    - La batería permanece segura

3. **Interruptor en ON, batería instalada, FTDI conectado**
    - La batería alimenta el registrador
    - El FTDI solo proporciona conexión de datos
    - Útil si el cable FTDI no puede suministrar suficiente corriente

**Buenas prácticas:**

✅ **HACER:**

- Poner el interruptor en OFF al programar (recomendado)
- Verificar la posición del interruptor antes de conectar el FTDI
- Usar el interruptor para apagar completamente el registrador

❌ **NO HACER:**

- Preocuparse por confusión en la posición del interruptor — el diseño evita daños
- Dejar el interruptor en ON durante el almacenamiento prolongado (agota la batería)

**Resolución de problemas:**

| Problema                         | Verificar                                                     |
|----------------------------------|---------------------------------------------------------------|
| Sin alimentación con interruptor en ON  | Batería agotada, comprobar voltaje                     |
| Sin alimentación con interruptor en OFF | Cable FTDI no conectado o sin pines de datos           |
| No se puede cargar código        | Probar posición OFF, verificar orientación del FTDI           |

### 8. Circuito de Regulación de Alimentación

**Ubicación:**

- Componentes cerca de los contactos de la batería

**Componentes:**

- Reguladores de tensión (suministro lógico de 3.3 V)
- Diodos de protección
- Condensadores de filtrado
- CIs de gestión de energía

**Función:**

- Convierte la batería de 3.7 V al nivel lógico de 3.3 V
- Suministra alimentación estable a la electrónica
- Protege contra la polaridad inversa (en algunos modelos)
- Protección contra sobrecorriente

**Reparable por el usuario:**

- No — no modificar

### 8. Conector XBee (Solo Modelos con Telemetría)

**Ubicación:**

- Conector prominente, normalmente en un extremo de la PCB

**Función:**

- Aloja el módem celular XBee
- Conector pasante para el módulo XBee
- Pines de alimentación y comunicación

**Instalación del XBee:**

- Alinear la muesca del XBee con el conector
- Presionar firmemente hasta asentarlo
- Todos los pines deben estar en el conector
- No debe moverse

**Extracción:**

- Mover suavemente de lado a lado mientras se tira
- Nunca tirar de la antena
- Comprobar si hay pines doblados

**Mantenimiento:**

- Mantener el conector libre de polvo
- Comprobar si hay pines doblados
- Asegurarse de que la tarjeta SIM esté instalada en el XBee

### 9. Conector del Sensor

**Ubicación:**

- Conector para el cable del sensor externo

**Tipos:**

- **Wari:** Terminal de tornillo de 3–4 pines o JST
- **Lidar:** Conector JST de 6 pines

**Función:**

- Alimentación al sensor
- Comunicación de datos
- Referencia de tierra

**Mantenimiento:**

- Comprobar que la conexión esté apretada
- Inspeccionar por corrosión
- Verificar que no haya cables rotos
- Alivio de tensión adecuado

**Configuración de pines (típica):**

**Wari (MB7389):**
```
1. +5V (alimentación)
2. Salida serie
3. Entrada serie (a veces sin usar)
4. GND
```

**Lidar (Garmin):**
```
1. Habilitación de alimentación
2. Control de modo
3. +5V
4. I2C SDA
5. I2C SCL
6. GND
```

### 10. Indicadores LED

**Ubicación:**

- LEDs de montaje en orificio o superficie

**LEDs comunes:**

| LED            | Color         | Función                    |
|----------------|---------------|----------------------------|
| **Alimentación** | Rojo/verde  | Registrador encendido      |
| **Actividad**  | Amarillo/azul | Medición en curso          |
| **Error**      | Rojo          | Condición de error         |
| **Telemetría** | Azul/verde    | Actividad XBee             |

**Comportamiento de los LEDs:**

**Operación normal:**

- Destello breve durante la medición
- Encendido continuo durante la lectura del sensor
- Destellos rápidos durante la escritura en SD

**Patrones de error:**

- Parpadeo rápido: Error en la tarjeta SD
- Encendido continuo: Atascado en estado de error
- Sin actividad: Problema de alimentación/código

**Ahorro de energía:**

- LEDs desactivados en modo de despliegue
- Activar el modo DEBUG para ver actividad

### 11. EEPROM Externa (Algunos Modelos)

**Ubicación:**

- CI de 8 pines cerca del slot de la tarjeta SD

**Función:**

- Almacenamiento intermedio de datos
- Guarda las mediciones en memoria antes de escribir en SD
- Prolonga la vida útil de la tarjeta SD
- Evita la pérdida de datos durante problemas con la SD

**Especificaciones:**

- Típicamente 24LC512 o similar
- Capacidad de 64 KB
- Interfaz I2C
- Memoria no volátil

**Funcionamiento:**

- Las mediciones se escriben primero en la EEPROM
- Se envían en lote a la tarjeta SD periódicamente
- Intervalo de volcado configurable por el usuario

**Reparable por el usuario:**

- No

---

## Componentes Adicionales

### Condensadores

**Propósito:**

- Suavizado del suministro de alimentación
- Filtrado de ruido
- Circuitos de temporización

**Mantenimiento:**

- No requiere mantenimiento por parte del usuario

### Resistencias

**Propósito:**

- Pull-up/pull-down en señales
- Limitación de corriente para LEDs
- Divisores de tensión

**Mantenimiento:**

- No requiere mantenimiento por parte del usuario

### Cristales/Osciladores

**Cristal de 8 MHz (Externo):**

- Reloj principal para el microcontrolador
- Determina la precisión del tiempo

**Mantenimiento:**

- No requiere mantenimiento por parte del usuario

---

## Versiones de PCB

### Identificar la Versión de tu PCB

**Buscar:**

- Número de versión serigrafiado en la PCB (p. ej., "v2.1")
- Código de fecha
- Logo/texto de Riverlabs

**Versiones comunes:**

| Versión           | Características              | Notas                       |
|-------------------|------------------------------|-----------------------------|
| **Wari v1**       | Registro básico              | Diseño original             |
| **Wari v2.0**     | + EEPROM, alimentación mejorada | Mayor fiabilidad          |
| **Wari v2.1**     | + Optimizaciones de telemetría | Producción actual          |
| **Lidar/WMOnode** | Compatibilidad con sensor Lidar | Amplificador 5V de mayor potencia |

---

## Seguridad y Manipulación

### Protección ESD

!!! warning "La electricidad estática puede dañar la electrónica"
    
**Precauciones:**

- Tocar metal con conexión a tierra antes de manipular la PCB
- Evitar manipular en condiciones secas
- Usar pulsera antiestática si está disponible
- Guardar en bolsa antiestática cuando no esté instalada

### Manipulación Física

**Hacer:**

- ✅ Manipular solo por los bordes
- ✅ Usar las herramientas adecuadas
- ✅ Trabajar sobre una superficie no conductora
- ✅ Mantener alejado de líquidos

**No hacer:**

- ❌ Tocar los pines de los componentes
- ❌ Aplicar presión sobre los componentes
- ❌ Exponer a la humedad
- ❌ Dejar caer o golpear

### Limpieza

**Si es necesario:**

- Apagar y retirar las baterías
- Usar alcohol isopropílico al 90%+
- Cepillo suave (cepillo de dientes o cepillo antiestático)
- Dejar secar completamente antes de volver a montar
- No usar agua ni limpiadores

---

## Prueba de Componentes

### Inspección Visual

**Comprobar:**

- [ ] Sin daños físicos
- [ ] Sin pines doblados
- [ ] Sin corrosión
- [ ] Sin marcas de quemado
- [ ] Uniones de soldadura intactas
- [ ] Sin grietas en la PCB

### Pruebas Funcionales

**Prueba de alimentación:**

1. Instalar batería nueva
2. Verificar actividad del LED
3. Comprobar voltaje con multímetro

**Prueba de comunicación:**

1. Conectar cable FTDI
2. Abrir el Monitor Serie
3. Cargar sketch de prueba
4. Verificar salida serie

**Prueba del sensor:**

1. Conectar el sensor
2. Ejecutar código de prueba del sensor
3. Verificar lecturas razonables
4. Comprobar mensajes de error

**Prueba de tarjeta SD:**

1. Insertar tarjeta formateada
2. Ejecutar durante varias mediciones
3. Retirar y leer el archivo de datos
4. Verificar formato y contenido

**Prueba del RTC:**

1. Configurar el reloj
2. Retirar la batería principal (con batería de respaldo instalada)
3. Esperar 1 minuto
4. Reinstalar la batería principal
5. Comprobar la hora — debe ser precisa

---

## Modificaciones y Actualizaciones

### Actualizaciones de Firmware

**Seguras y recomendadas:**

- Cargar nuevo código mediante FTDI
- Modificar parámetros configurables por el usuario
- Actualizar a la última versión estable

**Procedimiento:**

1. Descargar el nuevo firmware
2. Verificar compatibilidad con la versión de hardware
3. Cargar mediante Arduino IDE
4. Probar exhaustivamente antes del despliegue

### Modificaciones de Hardware

!!! danger "Las modificaciones de hardware son responsabilidad del usuario"
    Las modificaciones a la PCB, componentes o circuitos pueden dañar el registrador.

---

## Resolución de Problemas de Componentes

### Por Síntoma

**Sin alimentación:**

- Comprobar voltaje de la batería
- Inspeccionar los contactos de la batería (doblar hacia adentro si es necesario)
- Verificar el LED de alimentación (si está presente)
- Comprobar cortocircuitos (cable suelto o daño en la PCB)

**El reloj se reinicia:**

- Reemplazar la batería de respaldo CR1220 (debe medir 3 VDC)
- Comprobar el portapilas del RTC
- Verificar que el código configure correctamente el reloj

**Errores de tarjeta SD:**

- Limpiar los contactos de la tarjeta
- Probar con una tarjeta diferente
- Comprobar si hay daños en el slot

**Errores del sensor:**

- Comprobar que el conector esté bien fijado
- Verificar la continuidad del cable
- Probar el sensor por separado (en otro registrador funcional si es posible)
- Comprobar 5 V en el pin de alimentación (solo lidar)

**Fallo en la comunicación serie:**

- Verificar la orientación del cable FTDI
- Comprobar si hay pines doblados en el cabezal
- Probar con un cable FTDI diferente
- Probar con el sketch de Blink simple

---

## Piezas de Repuesto

### Componentes Reemplazables por el Usuario

| Componente           | Número de pieza      | Fuente  | Nivel de habilidad |
|----------------------|----------------------|---------|--------------------|
| **Batería 18650**    | Ver Guía de batería  | Minorista | Fácil            |
| **Batería CR1220**   | CR1220               | Minorista | Fácil            |
| **Tarjeta MicroSD**  | Compatible con FAT32 | Minorista | Fácil            |
| **Módulo XBee**      | XBee3 Cellular LTE-M | Digi    | Moderado           |

### Requieren Reemplazo Profesional

- Microcontrolador
- CI RTC
- Reguladores de tensión
- Cualquier componente de montaje superficial
- Pistas de PCB dañadas

**Contactar a Riverlabs para reparaciones:** info@riverlabs.uk

---

## Documentación

### Esquemático de la PCB

!!! info "Esquemáticos de código abierto"
    Los esquemáticos completos de la PCB están disponibles en el [repositorio de GitHub](https://github.com/ICHydro/Riverlabs). Contactar info@riverlabs.uk para más detalles.

### Lista de Materiales (BOM)

Las listas completas de componentes están disponibles en la [documentación del repositorio](https://github.com/ICHydro/Riverlabs).

---

## Próximos Pasos

- 🔋 [Guía de batería y alimentación](battery-power-guide.md): Detalles del sistema de alimentación
- 🛠️ [Guía de mantenimiento](maintenance.md): Procedimientos de mantenimiento regular
- 🚨 [Resolución de problemas](../troubleshooting/common-issues.md): Problemas específicos de componentes
- 💻 [Configuración de Arduino](../quick-start/arduino-setup.md): Programación y firmware

---

!!! note "Evolución de la PCB"
    Riverlabs mejora continuamente los diseños de PCB. Aunque las ubicaciones de los componentes pueden variar ligeramente entre versiones, la funcionalidad principal y la programación se mantienen constantes.

