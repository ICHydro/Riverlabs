# Problemas habituales y soluciones

!!! abstract "Descripción general"
    Esta pagina provee una referencia de los problemas más frecuentes de los registradores Riverlabs. Organizada por síntoma → causa → solución para facilitar el diagnóstico.

## Cómo utilizar esta guía

1. **Busca entre los síntomas** el que corresponda a su problema
2. **Lee las causas probables** para comprenderlo
3. **Sigue los pasos de la solución** para resolverlo
4. **Consulta «Verificado por»** para confirmar su validez en situaciones reales

Cada problema incluye:
- 🔴 **Síntoma** - Lo que observas
- 🔍 **Causa probable** - Por qué ocurre
- ✅ **Solución** - Cómo resolverlo
- ⚙️ **Prevención** - Cómo evitarlo en el futuro

---

## Categoría: alimentación y batería

### Problema 1: El registrador no se enciende

**🔴 Síntoma:**
- No hay actividad LED con el interruptor en ON
- El registrador parece completamente apagado
- No hay salida serie

**🔍 Causas probables:**
1. **Batería completamente descargada** (<2.5V)
2. **Batería desconectada** o mal contacto
3. **Polaridad invertida** (batería colocada al revés)
4. **Interruptor averiado** (fallo mecánico)
5. **PCB dañada** (fallo de componente o pista)

**✅ Solución:**

**Paso 1: Comprueba la tensión de la batería**

- Desconecta la batería del registrador
- Mide con un multímetro: punta roja al cable rojo, negra al negro
- Lectura:
    - 3.7-4.2V: batería correcta → el problema está en el registrador (ve al paso 3)
    - 2.5-3.5V: batería descargada → cárgala o sustitúyela y repite la prueba
    - <2.5V: batería agotada → sustitúyela (probablemente no admita carga)
    - 0V: no hay batería o está totalmente averiada → sustitúyela

**Paso 2: Inspecciona el conector de la batería**

- ¿Está bien insertado el conector JST?
- ¿Hacen contacto los pines? (muévelo con cuidado: ¿se enciende de forma intermitente?)
- ¿Es correcta la polaridad? (cable rojo a + en la PCB; negro a -)
- Si está al revés: ¡DESCONÉCTALA INMEDIATAMENTE! Busca daños (componentes quemados, olor)

**Paso 3: Prueba el interruptor**

- Acciónalo varias veces (los contactos pueden estar sucios)
- Si puedes, puentea temporalmente los terminales para probarlo
- Si está roto, desuél­dalo y sustitúyelo o déjalo puenteado permanentemente

**Paso 4: Comprueba si la PCB está dañada**

- Inspección visual: ¿componentes quemados? ¿pistas rotas?
- Olor: ¿huele a componentes quemados?
- ¿Se calienta el regulador de tensión con la batería conectada? (fallo del regulador)
- Si hay daños: reparación profesional o sustitución del registrador

**⚙️ Prevención:**
- Supervisa el voltaje y sustituye la batería antes de que baje de 3.3V
- Utiliza conectores JST (polarizados, evitan invertir la conexión)
- Protege el registrador de los golpes (buena carcasa)
- Evita cortocircuitar la batería (causa daños permanentes)

---

### Problema 2: La batería se agota durante la noche

**🔴 Síntoma:**
- La batería se instala totalmente cargada por la tarde
- A la mañana siguiente está agotada o muy baja
- El registrador puede haber guardado unas pocas muestras o ninguna

**🔍 Causas probables:**
1. **No funciona el modo reposo** (el registrador trabaja continuamente)
2. **La telemetría se queda intentando conectarse** (el XBee nunca entra en reposo)
3. **Error del firmware** (nunca activa el reposo)
4. **Cortocircuito** (fallo de hardware)

**✅ Solución:**

**Paso 1: Comprueba el modo reposo en el código**
```cpp
// In main loop, should have:
LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
// Or similar sleep command

// For XBee:
pinMode(XBEE_SLEEP_PIN, OUTPUT);
digitalWrite(XBEE_SLEEP_PIN, HIGH); // Sleep XBee when not transmitting
```

**Paso 2: Mide el consumo de corriente**

- Necesita el uso de un multímetro con medición de corriente (modo mA)
- Desconecta la batería y conecta el amperímetro en serie
- Enciende el registrador
- Observa la corriente:
    - Reposo: <1 mA (bien)
    - Activo/midiendo: 50-100 mA (normal, brevemente)
    - >50 mA continuos: ¡no entra en reposo!

**Paso 3: Comprueba la salida serie de depuración**

- Conecta el cable FTDI
- Abre el monitor serie
- Comprueba si hay:
    - Mensajes «Entering sleep»; si no aparecen, no se activa el reposo
    - «XBee timeout» o similar; puede estar esperando al XBee
    - Salida continua; debería ser periódica

**Paso 4: Aísla la telemetría**

Si tiene XBee:

- Retira el módulo XBee
- Prueba durante una noche registrando solo en SD
- Si la batería dura: problema de configuración del XBee (SM no configurado para reposo por pin)
- Si sigue agotándose: problema en el código o hardware del registrador

**⚙️ Prevención:**
- Prueba la autonomía en banco durante una noche antes de instalarlo
- Activa la depuración para comprobar el reposo
- Configura el reposo por pin del XBee (SM=1)
- Mide el consumo con un amperímetro durante el desarrollo

---

### Problema 3: El porcentaje de batería parece incorrecto

**🔴 Síntoma:**
- El registrador indica un 50 % de batería
- La siguiente lectura indica un 10 % o batería agotada
- O indica un 80 % durante días y se apaga de repente

**🔍 Causas probables:**
1. **Curva de tensión de Li-ion** (descarga no lineal)
2. **Temperaturas bajas** (caída de tensión)
3. **Batería vieja** (pérdida de capacidad)
4. **Error al medir la tensión** (imprecisión del ADC)

**✅ Solución:**

**Comprende la descarga de Li-ion:**
```
Li-ion batteries hold voltage until near end of life:

Voltaje   Carga remanente %
4.2V      100%
4.0V      80%
3.9V      60%
3.8V      40%
3.7V      30%
3.6V      20%
3.5V      10%
3.4V      5%
3.3V      1%
<3.3V     Riesgo de da&ntilde;o
```

**Nota:** El porcentaje no es lineal y depende de la temperatura y la carga.

**Paso 1: Utiliza umbrales de tensión, no porcentajes**

En lugar de: «Batería al 50 %»
Utiliza: «Tensión de la batería: 3.7V» (más preciso)

Umbrales de alerta:

- 3.5V: planifica la retirada durante la semana siguiente
- 3.4V: recógelo en un plazo de dos días
- 3.3V: recógelo inmediatamente (puede dejar de transmitir)

**Paso 2: Considera la temperatura**

El frío causa una caída temporal de tensión:

- Puede medirse 3.5V a 0°C
- La capacidad real podría ser mayor (al calentarse recupera tensión)
- No te alarmes por una única lectura baja con frío
- Retira el registrador si varias lecturas consecutivas son <3.4V

**Paso 3: Envejecimiento de la batería**

Las Li-ion se degradan con el tiempo:

- Nueva: capacidad total de 2600 mAh
- 1 año: ~90 % (2340 mAh)
- 2 años: ~80 % (2080 mAh)
- 3 años o más: <70 %; sustitúyela

Solución: sustituye las baterías anualmente en aplicaciones críticas

**⚙️ Prevención:**
- Configura alertas por tensión, no por porcentaje
- Sustituye las baterías cada año
- Recoge los registradores antes de llegar a una tensión crítica
- Almacena correctamente las baterías de repuesto (40-60 % de carga, lugar fresco)

---

## Categoría: sensor y calidad de los datos

### Problema 4: El sensor ultrasónico siempre indica el alcance máximo

**🔴 Síntoma:**
- Sensor siempre mide 5000mm (o el alcance máximo)
- El valor no cambia ni al mover el sensor

**🔍 Causas probables:**
1. **No vuelve el eco** (no hay objetivo o absorbe demasiado)
2. **Sensor sin alimentación** (cable desconectado)
3. **Sensor mal orientado** (apunta al cielo o al suelo)
4. **Sensor averiado** (poco frecuente)

**✅ Solución:**

**Paso 1: Escucha si el sensor emite clics**

- Enciende el registrador
- Acerca con cuidado el sensor al oído, sin tocar su cara frontal
- Deberías oír un «tic» o «clic» en cada medición (cada 15 segundos o según tu intervalo)
- Si hace clics: recibe alimentación y transmite → ve al paso 2
- Si no: no recibe alimentación → ve al paso 3

**Paso 2: Recibe alimentación, pero no hay eco**

Posibles causas:

1. No hay objetivo en la trayectoria del haz
    - Demasiado lejos: acércalo o usa lidar
    - Apunta al cielo: oriéntalo hacia el objetivo

2. El objetivo absorbe demasiado
    - Nieve reciente: muy absorbente
    - Espuma: absorbe los ultrasonidos
    - Solución: espera a que se derrita la nieve o utiliza lidar

3. El haz no alcanza un objetivo pequeño
    - Haz ultrasónico: cono de ~15-20°
    - Objetivo pequeño (p. ej., arroyo estrecho): puede no alcanzarlo
    - Solución: apunta con más cuidado o utiliza un sensor de haz más ancho

**Paso 3: Sensor sin alimentación**

- Desconecta el cable del sensor
- Comprueba la continuidad:
    - Entre los pines del cable y el conector: resistencia <1Ω
    - ¿Cable aplastado, cortado o corroído? Sustitúyelo
- Comprueba el conector de la PCB:
    - ¿Soldaduras intactas? (inspección visual)
    - ¿Hacen contacto los pines? Vuelve a insertar el cable
- Comprueba con el multímetro:
    - Mide la tensión en el conector del sensor mientras el registrador está activo
    - Debería haber 5V o 12V (según el modelo de sensor)
    - Si no hay tensión: fallo del circuito de alimentación de la PCB

**Paso 4: Sustituye el sensor**

Si nada funciona:

- Podría haber fallado internamente (raro, pero posible)
- Prueba con un sensor que funcione
- Si el nuevo funciona: el original está averiado
- Si ambos indican el máximo: el problema es de orientación, objetivo o entorno

**⚙️ Prevención:**
- Orienta cuidadosamente el sensor al instalarlo (prueba antes de irte)
- Protege el cable (fíjalo y evita puntos de aplastamiento)
- Elige un sensor adecuado al objetivo (lidar para nieve o espuma)
- Mantén limpia la cara del sensor

---

### Problema 5: Las lecturas de distancia derivan con el tiempo

**🔴 Síntoma:**
- Las distancias cambian 50-200mm a lo largo de varios días
- El nivel real del agua está estable (verificado visualmente)
- Hay una deriva lenta, no saltos bruscos

**🔍 Causas probables:**
1. **Cambios de temperatura** (el montaje se dilata o contrae)
2. **Crecimiento de vegetación** (obstruye el haz)
3. **Montaje flojo** (tornillos, bridas)
4. **Deriva de calibración** (rara)

**✅ Solución:**

**Paso 1: Comprueba la correlación con la temperatura**

- Representa distancia frente a temperatura a lo largo del tiempo
- Si hay una fuerte correlación (la distancia aumenta con la temperatura):
    - Causa: la estructura de montaje se dilata con el calor
    - Ejemplo: un soporte metálico se calienta al sol, se dilata y el sensor desciende 5mm
    - Solución:
        - a) Aceptar la variación natural (±50mm suele ser aceptable)
        - b) Utilizar un montaje térmicamente estable (menos metal)
        - c) Compensar la temperatura al procesar los datos

**Paso 2: Inspecciona el montaje**

- Revisa visualmente la instalación
- Comprueba:
    - ¿Bridas tensas? (pueden aflojarse)
    - ¿Tornillos y pernos apretados?
    - ¿Soporte doblado o desplazado?
- Solución: vuelve a apretar y refuerza el montaje

**Paso 3: Busca vegetación**

- Inspecciona la trayectoria del haz
- ¿Ha crecido vegetación (hierba, maleza, ramas)?
    - ¿Hojas nuevas en primavera?
    - ¿Algas en la superficie objetivo?
- Solución: poda la vegetación y limpia el objetivo
- Prevención: deja despejada la trayectoria y prevé el crecimiento estacional

**Paso 4: Comprueba la calibración**

No suele hacer falta, pero puedes verificarla:

- Mide una distancia conocida con una cinta (p. ej., 2000mm)
- Sitúa un objetivo a esa distancia
- La lectura debería coincidir con ±50mm
- Si difiere >100mm: quizá debas sustituir el sensor

**⚙️ Prevención:**
- Utiliza materiales de montaje térmicamente estables
- Mantén despejado el haz (poda trimestral)
- Asegura el montaje con fijaciones redundantes
- Ten en cuenta la dilatación estacional al analizar los datos

---

### Problema 6: El lidar devuelve valores negativos

**🔴 Síntoma:**
- A veces registra distancias negativas (p. ej., -150mm)
- La mayoría de las lecturas son positivas y correctas

**🔍 Causas probables:**
1. **Error de comunicación I2C** (datos corruptos)
2. **Tiempo de espera interpretado como negativo** (error de código)
3. **Desbordamiento de entero** (error matemático)

**✅ Solución:**

**Paso 1: Busca un patrón**

- ¿Cuándo aparecen los negativos?
    - Al azar: error I2C (paso 2)
    - Todas las lecturas a partir de cierto valor: desbordamiento (paso 3)
    - Durante o después de transmitir: interferencia del XBee en I2C (paso 4)

**Paso 2: Fiabilidad de la comunicación I2C**

I2C es sensible a:

- Longitud de cables (mantén <30cm)
- Interferencias electromagnéticas cercanas
- Conexiones flojas

Soluciones:

- a) Añade resistencias de pull-up (2.2kΩ o 4.7kΩ en SDA y SCL)
- b) Acorta los cables I2C
- c) Añade una ferrita al cable
- d) Apantalla el cable cerca de fuentes intensas de radiofrecuencia

<!--- WB: this needs review in the code. I think that the lidar values are returned in cm, not mm. It may also have implications for other parts of the code.

**Paso 3: Error de código: desbordamiento**
```cpp
// Bad code (potential overflow):
int distance = lidar.read(); // int = -32768 to 32767
// If actual distance > 32767mm, overflows to negative

// Good code:
unsigned int distance = lidar.read(); // unsigned = 0 to 65535
// Or:
long distance = lidar.read(); // long = much larger range
```
-->

<!-- WB: Has this ever happened? Typically, measurements will not happen during a telemetry event. In any case, this is very advanced as very few users will alter the PCB in this way. If our designs are susceptible to such interference then it will be up to us to improve the design.

**Paso 4: Interferencia del XBee**

Si los negativos coinciden con la telemetría:

- La transmisión del XBee puede generar interferencias electromagnéticas
- Solución:
    - a) Separa los cables I2C de la antena XBee
    - b) Mide antes de transmitir (no durante)
    - c) Añade resistencias de pull-up I2C (mejoran la integridad de la señal)

-->

**Paso 3: Filtra durante el posprocesamiento**

Como último recurso (si no es práctico corregir el hardware):

- Elimina los valores negativos al analizar los datos
- Interpola: sustituye los negativos por el promedio de valores vecinos

**⚙️ Prevención:**
- Utiliza codigo adecuado
- Añade resistencias de pull-up I2C al diseño
- Prueba I2C antes de instalarlo (lee el sensor 1000 veces en un bucle y comprueba errores)

---

## Categoría: tarjeta SD y registro de datos

### Problema 7: La tarjeta SD aparece "corrupta" o "necesita formateo"

**🔴 Síntoma:**
- Retiras la tarjeta SD y la conectas al ordenador
- Windows indica «El disco necesita formatearse» o «Corrupto»
- Los archivos no se pueden leer

**🔍 Causas probables:**
1. **Apagado inadecuado** (tarjeta o alimentación retiradas durante una escritura)
2. **Batería agotada durante la escritura** (archivo no cerrado)
3. **Mala calidad de la tarjeta SD**
4. **Corrupción del sistema de archivos** (error de código)

**✅ Solución:**

**Paso 1: Intenta recuperar los datos**
```
Before formatting, try recovery:

Windows:
- Right-click drive → Properties → Tools → Check
- Let Windows scan and repair

Mac:
- Disk Utility → First Aid → Run

Linux:
- fsck command

Recovery software (if above fails):
- Recuva (free, Windows)
- PhotoRec (free, multi-platform)
- DiskDigger (free/paid)
```

**Paso 2: Formatea correctamente**
```
If recovery fails or you don't need data:

Windows:
- Right-click drive → Format
- File system: FAT32
- Allocation unit size: 32 kilobytes
- Quick Format: Unchecked (full format tests for bad sectors)

Mac:
- Disk Utility → Erase
- Format: MS-DOS (FAT)
- Scheme: Master Boot Record

Linux:
sudo mkfs.vfat -F 32 -S 512 -s 64 /dev/sdX1
```

**Paso 3: Prueba la tarjeta**
```
After formatting:
- Copy test file to card
- Verify file readable
- Remove and re-insert
- File still there? → Card OK
- File corrupted again? → Card failing, replace
```

<!-- This is really up to us to make sure that the code is as robust as possible. Ideally, we should not expect users to have to alter the code to solve problems


**Paso 4: Evita futuras corrupciones**
```
Code improvements:
1. Flush data after each write:
   dataFile.flush(); // Forces write to disk

2. Close file before sleep:
   dataFile.close();
   // Sleep
   dataFile = SD.open("data.csv", FILE_WRITE);

3. Add battery-low check:
   if(battery < 3.3) {
     dataFile.close();
     enterEmergencyShutdown();
   }
```

-->

**⚙️ Prevención:**
- No extraigas la tarjeta SD mientras el registrador escribe datos
- Comprueba que la batería tiene carga suficiente antes de instalarlo
- Utiliza tarjetas de calidad (SanDisk, Samsung, Kingston)
- Ejecuta flush() o close() regularmente en el código
- Retira los registradores antes de que la batería alcance un nivel crítico

---

### Problema 8: El CSV contiene caracteres extraños o formato incorrecto

**🔴 Síntoma:**
- Abres el CSV en Excel o Numbers
- Aparecen símbolos como `Ã`, `Â`, `â€™`
- Las columnas están desalineadas
- Los datos parecen corruptos

**🔍 Causas probables:**
1. **Codificación de caracteres incompatible** (UTF-8 frente a ASCII)
2. **Separador incorrecto** (coma frente a punto y coma)
3. **Comillas dentro de los datos** (interrumpen el análisis CSV)
4. **Líneas incompletas** (corte de alimentación durante la escritura)

**✅ Solución:**

**Paso 1: Prueba otra codificación**
```
Excel (Windows):
- Open Excel (don't double-click CSV)
- Data → From Text/CSV
- File Origin: UTF-8
- Delimiter: Comma
- Import

Excel (Mac):
- Similar: Data → Get External Data → Import Text File
- Select encoding: UTF-8 or Western (ASCII)

LibreOffice/OpenOffice:
- Opens CSV with import dialog automatically
- Character set: Unicode (UTF-8)
- Separated by: Comma
```

**Paso 2: Inspecciona el archivo sin procesar**
```
Use text editor (Notepad++, Sublime, VS Code):
- Open CSV as plain text
- Look for:
  - Proper line endings (should be LF or CRLF)
  - Consistent delimiters (all commas? all semicolons?)
  - Any binary characters (� symbols)

Example good CSV:
Date,Time,Distance_mm,Temp_C,Battery_V
2025-12-27,10:00:00,1234,22.5,3.85
2025-12-27,10:15:00,1256,23.1,3.84

Example bad CSV (missing delimiters):
Date,Time,Distance_mm,Temp_C,Battery_V
2025-12-2710:00:001234,22.5,3.85
```
<!-- Also here, suggesting users to edit code may open a can of worms. We should probably keep this content separate and make clear that it is only for users that develop their own code. It should not be needed as a solution to users that use our own code 

**Paso 3: Corrige el código si es necesario**
```cpp
// Ensure consistent delimiters:
dataFile.print(year);
dataFile.print(",");  // Not "," sometimes and ";" other times
dataFile.print(month);
dataFile.print(",");
// etc.

// Handle special characters:
// If data might contain commas, use quotes:
dataFile.print("\"");
dataFile.print(dataString);
dataFile.print("\"");
```
-->

**Paso 4: Recupera los datos parciales**
```
If file partially corrupted:
- Identify good rows vs. bad
- Manually fix or delete bad rows
- Or: Script to clean (Python pandas, etc.)
```

**⚙️ Prevención:**
- Escribe una fila de cabecera al iniciar por primera vez
- Utiliza separadores coherentes (la coma es habitual)
- Prueba la lectura de CSV con datos de ejemplo antes de instalarlo
- Vacía regularmente el búfer del archivo para completar las escrituras

---

## Categoría: telemetría

### Problema 9: ThingsBoard solo muestra datos antiguos

**🔴 Síntoma:**
- Hay datos en el panel de ThingsBoard
- Los datos más recientes tienen horas o días
- El registrador sigue funcionando (la SD contiene datos actuales)

**🔍 Causas probables:**
1. **Pérdida de red** (fuera de cobertura)
2. **SIM desactivada** (impago o problema del operador)
3. **Batería demasiado baja** (telemetría desactivada)
4. **Fallo del XBee** (hardware o configuración)

**✅ Solución:**

**Paso 1: Comprueba los datos recientes en la SD**
```
- Retrieve logger (or analyze last data on ThingsBoard)
- SD card shows recent measurements?
  - Yes: Logger working, telemetry failed → Go to Step 2
  - No: Logger stopped completely → See Flowchart 1 (Logger Won't Record)
```

**Paso 2: Comprueba la tensión de la batería**
```
- Check voltage in latest SD card data
- Voltage <3.5V?
  - Yes: Battery too low for reliable telemetry
  - Solution: Replace battery, retest
- Voltage >3.5V?
  - Telemetry failure for other reason → Go to Step 3
```

**Paso 3: Prueba la tarjeta SIM**
```
- Remove SIM from XBee
- Insert in phone (adapter if needed)
- Does phone get service?
  - No service: SIM deactivated
    - Solution: Contact carrier, reactivate, verify payment
  - Has service: SIM works → Go to Step 4
```

**Paso 4: Comprueba la señal en el lugar**
```
- Using phone (same carrier as SIM):
  - Test signal strength at logger location
  - Coverage adequate?
    - No coverage: Site moved out of coverage (tree fell, logger relocated?)
      - Solution: Move logger, or use external antenna
    - Coverage OK: XBee issue → Go to Step 5
```

**Paso 5: Revisa la configuración del XBee**
```
- Remove XBee, connect to XBee Studio
- Read configuration
- Critical settings:
  - AP = 2? (if not, set to 2)
  - AN = correct APN? (verify with carrier)
  - SM = 1? (pin sleep)
- Test connection in XBee Studio (Cellular Network tab)
  - Can connect? XBee works
    - Solution: Re-install in logger, check Arduino code
  - Can't connect? Network issue or XBee failure
    - Solution: Update firmware, or replace XBee
```

**⚙️ Prevención:**
- Configura avisos en ThingsBoard (sin datos durante >2 veces el intervalo esperado)
- Supervisa la tensión de forma remota
- Mantén la renovación automática del plan SIM
- Comprueba la señal antes de instalarlo

---

### Problema 10: La telemetría funciona de forma intermitente

**🔴 Síntoma:**
- Algunas transmisiones funcionan
- Otras fallan (tiempo de espera o código de error)
- Tasa de éxito: 50-80 %

**🔍 Causas probables:**
1. **Señal débil** (cobertura marginal)
2. **Congestión de red** (horas punta)
3. **Problema de antena** (conexión floja o daño)
4. **Problema de alimentación** (caída de tensión durante la transmisión)

**✅ Solución:**

**Paso 1: Analiza las correlaciones**

Busca patrones en los fallos:

¿Por la hora del día?
- Fallos nocturnos: congestión de red (todos en casa, usando red)
- Fallos durante el día: Posibles pero menos comunes
- Aleatorio: Problema de intensidad de señal

¿Por voltaje de batería?
- Las fallas aumentan a medida que baja el voltaje: Problema de potencia
- Sin correlación: Problema de red

¿Por temperatura?
- Fallos en frío: caída de voltaje en la batería en frío
- Sin correlación: Otro problema


**Paso 2: Comprueba la intensidad de señal**

Si es posible (depuración en XBee Studio o en serie):
- Comprobar RSSI durante transmisiones exitosas o fallidas
- RSSI menor a -100 dBm: Señal débil, alta tasa de fallo
- Solución: antena externa, reubicar el registrador más arriba/ubicación más clara
- RSSI > -95 dBm pero aún fallos: No fuerza de señal, pasar al Paso 3


**Paso 3: Inspecciona la antena**

- Inspeccionar la conexión de la antena (si es externa)
- ¿Conector U.FL bien colocado? (pequeño, frágil)
- ¿Cable de antena dañado? (revisa si hay dobladuras, roturas)
- Antena interna de la PCB: ¿Espacio libre? (Sin contacto con la carcasa/metal)
- Solución: volver a colocar el conector, cambiar la antena si está dañado

**Paso 4: Añade reintentos en el codigo**
```cpp
// In Arduino code:
bool sendData() {
  int attempts = 0;
  while(attempts < 3) {
    if(transmitXBee()) {
      return true; // Success
    }
    delay(30000); // Wait 30 seconds
    attempts++;
  }
  return false; // Failed after 3 attempts
}
```

**Paso 5: Cambia el horario de transmisión**

Si los fallos se correlacionan con las horas punta:
- Tiempo de transmisión de marcha:
- En lugar de cada hora en punto (09:00, 10:00, 11:00)
- Desplazamiento: Minuto aleatorio (09:17, 10:42, 11:23)
- Reduce la congestión de la red (no todos transmiten simultáneamente)


**⚙️ Prevención:**
- Instala el equipo donde haya buena señal (prueba primero con un teléfono)
- Utiliza una antena externa si la cobertura es limitada
- Implementa reintentos con espera exponencial
- Supervisa la tasa de éxito en ThingsBoard (debería ser >90 %)

---

## Categoría: Arduino y firmware

### Problema 11: Fallo de carga «programmer is not responding» o «avrdude: stk500_getsync()»

**🔴 Síntoma:**
- Intentas cargar un sketch
- Arduino IDE muestra «programmer is not responding» u otro error avrdude
- La carga falla al 0 % o durante la comunicación

**🔍 Causas probables:**
1. **Sensor conectado durante la carga** (CRÍTICO: el más frecuente en Wari ultrasónico)
2. **Cable FTDI invertido** (verde/negro intercambiados)
3. **Configuración de placa incorrecta** (no MiniCore ATmega328)
4. **Configuración de reloj incorrecta** (no External 8 MHz)
5. **Cable FTDI mal colocado**
6. **Problema de alimentación** (insuficiente o inexistente)

**✅ Solución:**

**Paso 1: Desconecta el sensor (CRÍTICO para Wari ultrasónico)**

⚠️ CAUSA MAS PROBABLE ⚠️

Para registradores ultrasónicos Wari con sensor Maxbotix:
- Desconectar el CONECTOR BLANCO del registrador
- El sensor y FTDI utilizan el mismo puerto serie
- El sensor causa interferencias que impiden la subida
- Una vez completada la subida, reconecta el sensor

Esto NO se aplica a los modelos Lidar.


**Paso 2: Comprueba la orientación del FTDI**

Check board markings:
- Green wire → "GRN" marking
- Black wire → "BLK" marking
- On cellular/lidar: Black wire on SD card side

If reversed:
- Disconnect immediately
- Rotate 180°
- Reconnect firmly


**Paso 3: Comprueba la configuración de Arduino IDE**

Tools → Board → MiniCore: "ATmega328"
Tools → Clock: "External 8 MHz"

NOT: "Arduino Pro or Pro Mini"  ← Old/Wrong!


**Paso 4: Comprueba la alimentación**

- Power switch to OFF (powered by FTDI)
- Or battery installed and switch ON
- LED should briefly light when reset button pressed


**Paso 5: Vuelve a insertar el cable FTDI**

- Disconnect FTDI from logger
- Check all pins aligned
- Press firmly onto header
- Should sit flush


**Paso 6: Prueba otro puerto USB**

- Some USB ports provide insufficient current
- Try USB 2.0 port (more reliable than USB 3.0)
- Avoid USB hubs


**⚙️ Prevención:**
- **Desconecta siempre el sensor antes de programar** (Wari ultrasónico)
- Marca la orientación en el cable FTDI
- Revisa los ajustes de la placa antes de cada carga
- Ten a mano la lista de conexiones FTDI

---

### Problema 12: Fallo de carga «Serial Port Already in Use»

**🔴 Síntoma:**
- Intentas cargar un sketch
- Arduino IDE indica «Serial port busy» o «Access denied»

**🔍 Causas probables:**
1. **Monitor serie abierto** (bloquea el puerto)
2. **Otro programa utiliza el puerto** (PuTTY, Coolterm, etc.)
3. **Carga anterior sin terminar** (puerto bloqueado)
4. **Problema del controlador** (Windows)

**✅ Solución:**

**Paso 1: Cierra el monitor serie**

- In Arduino IDE: Close Serial Monitor window
- Wait 2 seconds
- Retry upload


**Paso 2: Busca otros programas**

Windows:
- Ctrl+Shift+Esc (Task Manager)
- Look for: PuTTY, Coolterm, Serial Monitor, Python scripts
- End task

Mac:
- Activity Monitor
- Search for serial terminal programs
- Quit

Linux:
- ps aux | grep tty
- kill [PID]


**Paso 3: Desconecta y vuelve a conectar**

- Unplug FTDI cable
- Wait 5 seconds
- Reconnect
- Retry upload


**Paso 4: Reinicia Arduino IDE**

- Close Arduino IDE completely
- Reopen
- Retry upload


**Paso 5: Reinicia el ordenador** (si nada más funciona)

**⚙️ Prevención:**
- Cierra el monitor serie antes de subir código
- No uses varios programas serie a la vez
- Desconecta el FTDI cuando termines

---

### Problema 13: El sketch compila, pero el registrador no hace nada

**🔴 Síntoma:**
- La carga termina correctamente (100 %, «Done uploading»)
- El LED parpadea una vez al encender
- Después no registra, no se ilumina y no hay salida serie

**🔍 Causas probables:**
1. **Placa equivocada** (compilación para otro microcontrolador)
2. **Bootloader corrupto** (raro)
3. **Reloj mal configurado** (el código se ejecuta con tiempos incorrectos)
4. **Bucle infinito en setup()** (el código se bloquea)

**✅ Solución:**

**Paso 1: Comprueba la selección de placa**
```
Arduino IDE:
- Tools → Board → MiniCore: Should be "ATmega328"
- Tools → Clock: Should be "External 8 MHz"
- If wrong: Select correct, re-upload
```

**Paso 2: Prueba un programa sencillo de parpadeo**
```cpp
// Upload simplest possible code to verify functionality:
void setup() {
  pinMode(13, OUTPUT); // Or whatever pin has LED
}

void loop() {
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  delay(1000);
}

// Should blink every second
// If doesn't blink: Hardware or bootloader issue
// If blinks: Logger is OK, issue is in main code
```

**Paso 3: Activa la salida serie de depuración**
```cpp
void setup() {
  Serial.begin(115200);
  Serial.println("Logger starting...");
  Serial.println("Initializing RTC...");
  // etc.
}

// Connect Serial Monitor, check for output
// No output: Serial initialization issue or bootloader problem
// Has output: Follow debug messages to find where code fails
```

**Paso 4: Busca bucles infinitos**
```cpp
// Common mistake:
void setup() {
  while(!rtc.begin()) {
    // Stuck here forever if RTC not connected!
    delay(1000);
  }
}

// Better:
void setup() {
  int attempts = 0;
  while(!rtc.begin() && attempts < 10) {
    delay(1000);
    attempts++;
  }
  if(attempts == 10) {
    // RTC failed, but continue with default time
    Serial.println("RTC failed");
  }
}
```

**Paso 5: Graba el bootloader** (si el hardware funciona, pero el código no)
```
Requires:
- Another Arduino as ISP programmer
- Or: AVR ISP programmer

Process:
- Arduino IDE → File → Examples → ArduinoISP
- Upload ArduinoISP to programmer Arduino
- Connect programmer to target logger (ISP header)
- Tools → Programmer → Arduino as ISP
- Tools → Burn Bootloader
- Wait for completion
- Re-upload your sketch
```

**⚙️ Prevención:**
- Comprueba la selección de placa antes de subir el código
- Prueba primero con un sketch sencillo y conocido
- Añade salida serie de depuración durante el desarrollo
- Evita los bucles infinitos (usa tiempos de espera)

---

### Problema 14: Errores de compilación: faltan bibliotecas

**🔴 Síntoma:**
- Arduino IDE muestra errores en rojo
- Mensaje: `fatal error: [LibraryName].h: No such file or directory`
- Ejemplos:
  - `SdFat.h: No such file or directory`
  - `LowPower.h: No such file or directory`
  - `RtcDS3231.h: No such file or directory`
  - `AltSoftSerial.h: No such file or directory`

**🔍 Causas probables:**
1. **Biblioteca necesaria no instalada**
2. **Está instalada, pero Arduino no la encuentra**
3. **Versión incorrecta de la biblioteca**
4. **Nombre de carpeta incorrecto**

**✅ Solución:**

**Paso 1: Instala mediante el gestor de bibliotecas**
```
Arduino IDE:
1. Sketch → Include Library → Manage Libraries
2. Search for library name (e.g., "SdFat")
3. Find correct library:
   - "RTC" by Makuna
   - "SdFat" by Bill Greiman  
   - "AltSoftSerial" by Paul Stoffregen
4. Click Install
5. Wait for installation
6. Restart Arduino IDE
7. Retry compilation
```

**Paso 2: Instalación manual (Rocketscream LowPower)**
```
The LowPower library requires manual installation:

1. Go to https://github.com/rocketscream/Low-Power
2. Click Code → Download ZIP
3. Extract ZIP file
4. Rename folder to "Low-Power" (if needed)
5. Move folder to Arduino libraries directory:
   - Windows: Documents\Arduino\libraries\
   - Mac: ~/Documents/Arduino/libraries/
   - Linux: ~/Arduino/libraries/
6. Restart Arduino IDE
7. Verify: Sketch → Include Library → Low-Power should appear
```

**Paso 3: Comprueba la estructura de carpetas**
```
Correct structure:
Arduino/libraries/Low-Power/
  ├── LowPower.h
  ├── LowPower.cpp
  └── examples/

Wrong structure (won't work):
Arduino/libraries/Low-Power-master/Low-Power/
  └── ...

Fix: Move inner folder up one level
```

**Paso 4: Comprueba la compatibilidad**
```
Error: "This LowPower library only works on AVR processors"

Solution:
- Board must be: MiniCore → ATmega328
- Clock must be: External 8 MHz
- NOT: Arduino Uno, Mega, or other non-AVR boards
```

**Bibliotecas necesarias para los registradores Riverlabs:**

| Biblioteca | Autor | Instalación | Uso |
|---------|--------|---------------------|----------|
| RTC | Makuna | Gestor de bibliotecas | Reloj en tiempo real |
| SdFat | Bill Greiman | Gestor de bibliotecas | Acceso a tarjeta SD |
| Rocketscream LowPower | Rocketscream | Manual (GitHub) | Modos de reposo |
| SoftwareSerial | Arduino | Integrada | Modelos celulares |
| AltSoftSerial | Paul Stoffregen | Gestor de bibliotecas | Modelos celulares |

**⚙️ Prevención:**
- Instala todas las bibliotecas antes de la primera compilación
- Manténlas actualizadas, pero prueba el código tras actualizarlas
- Documenta las versiones que funcionan
- Conserva copias de seguridad de esas versiones

---

## Tabla de consulta rápida

| Síntoma | Causa más probable | Solución rápida | Referencia |
|---------|-------------------|-----------|----------|
| No se enciende | Batería agotada | Sustituye la batería | Problema 1 |
| Batería agotada en una noche | No entra en reposo | Revisa el código de reposo | Problema 2 |
| Alcance máximo | No hay objetivo o está mal orientado | Reorienta el sensor | Problema 4 |
| Deriva de distancia | Dilatación térmica | Acéptala o compénsala | Problema 5 |
| Lidar con valores negativos | Error I2C | Añade resistencias de pull-up | Problema 6 |
| SD corrupta | Apagado incorrecto | Recupera datos y formatea | Problema 7 |
| Caracteres extraños en CSV | Codificación incompatible | Importa como UTF-8 | Problema 8 |
| Datos antiguos en ThingsBoard | Problema de red o SIM | Revisa señal y SIM | Problema 9 |
| Telemetría intermitente | Señal débil | Antena externa | Problema 10 |
| Fallo de carga «not responding» | Sensor conectado | Desconecta el sensor | Problema 11 |
| Puerto serie ocupado | Monitor serie abierto | Cierra el monitor | Problema 12 |
| Carga correcta, sin funcionamiento | Placa equivocada | Revisa la configuración | Problema 13 |
| Falta una biblioteca | No está instalada | Instálala | Problema 14 |

---

## ¿Sigues sin resolverlo?

Si tu problema no aparece aquí:

1. **Consulta los [diagramas de diagnóstico](diagnostic-flowcharts.md)** - Rutas visuales de diagnóstico
2. **Revisa las [preguntas frecuentes](faq.md)** - Casos poco comunes
3. **Busca en GitHub Issues** - Puede que alguien haya tenido el mismo problema
4. **Contacta con soporte** - Incluye síntomas, pruebas realizadas y registros serie

---

## Próximos pasos

- 📊 [Diagramas de diagnóstico](diagnostic-flowcharts.md) - Árboles visuales
- ❓ [Preguntas frecuentes](faq.md) - Respuestas a preguntas habituales
- 🔧 [Guía de mantenimiento](../hardware/maintenance.md) - Mantenimiento preventivo
- 📞 Soporte - [GitHub Issues](https://github.com/ICHydro/Riverlabs/issues)

---

!!! success "La mayoría de los problemas son sencillos"
    El 80 % se debe a batería baja, tarjeta SD sin formatear o XBee mal configurado. ¡Comprueba primero lo básico!
