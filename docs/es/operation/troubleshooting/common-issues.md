# Problemas Comunes y Soluciones

!!! abstract "Resumen"
    Referencia completa de problemas frecuentes con registradores Riverlabs. Formato síntoma → causa → solución para una resolución de problemas rápida.

## Cómo Usar Esta Guía

1. **Escanear síntomas** para encontrar su problema
2. **Leer las causas probables** para entender por qué ocurre
3. **Seguir los pasos de solución** para arreglarlo
4. **Verificar "Verificado por"** para confirmación del mundo real

Cada problema incluye:
- 🔴 **Síntoma** - Lo que observa
- 🔍 **Causa Probable** - Por qué ocurre
- ✅ **Solución** - Cómo arreglarlo
- ⚙️ **Prevención** - Cómo evitarlo en el futuro

---

## Categoría: Energía y Batería

### Problema 1: El Registrador No Se Enciende

**🔴 Síntoma:**
- Sin actividad del LED cuando el interruptor de alimentación está en ON
- El registrador parece completamente muerto
- Sin salida serie

**🔍 Causas Probables:**
1. **Batería completamente descargada** (<2.5V)
2. **Batería desconectada** o contacto deficiente
3. **Polaridad inversa** (batería conectada al revés)
4. **Fallo del interruptor de alimentación** (mecánico)
5. **Daño del PCB** (fallo de componente, daño de traza)

**✅ Solución:**

**Paso 1: Verificar el voltaje de la batería**

- Desconectar la batería del registrador
- Medir con multímetro: Sonda roja al cable rojo, negro al negro
- Lectura:
    - 3.7-4.2V: Batería buena → problema está en el registrador (ir al Paso 3)
    - 2.5-3.5V: Batería descargada → cargar/reemplazar, volver a probar
    - <2.5V: Batería muerta → reemplazar (probablemente no acepte carga)
    - 0V: Sin batería o fallo completo → reemplazar

**Paso 2: Inspeccionar el conector de la batería**

- ¿Conector JST correctamente insertado?
- ¿Pines haciendo contacto? (mover suavemente, ¿se enciende de forma intermitente?)
- ¿Polaridad correcta? (cable rojo al pad + en el PCB, negro al -)
- Si está invertido: ¡DESCONECTAR INMEDIATAMENTE! Verificar si hay daños (componentes quemados, olor)

**Paso 3: Probar el interruptor de alimentación**

- Alternar el interruptor varias veces (puede tener contactos sucios)
- Si está disponible: Derivar el interruptor (cable temporal en los terminales del interruptor) para pruebas
- ¿Interruptor roto? Dessoldar y reemplazar, o puentear permanentemente

**Paso 4: Verificar daños en el PCB**

- Inspección visual: ¿componentes quemados? ¿trazas rotas?
- Olfato: ¿olor a electrónica quemada?
- ¿Regulador de voltaje caliente cuando la batería está conectada? (fallo del regulador)
- Si el PCB está dañado: Reparación profesional o reemplazo del registrador

**⚙️ Prevención:**
- Monitorear el voltaje de la batería, reemplazar antes de que baje de 3.3V
- Usar conectores JST (orientados, evitan la polaridad inversa)
- Proteger el registrador de daños físicos (buena carcasa)
- No cortocircuitar la batería (causa daño permanente)

---

### Problema 2: La Batería Se Agota Durante la Noche

**🔴 Síntoma:**
- Batería completamente cargada, instalada por la tarde
- A la mañana siguiente: Batería muerta o muy baja
- El registrador puede haber registrado algunas muestras o ninguna

**🔍 Causas Probables:**
1. **Modo de sueño no funcionando** (registrador ejecutándose continuamente)
2. **Telemetría atascada intentando conectarse** (XBee nunca duerme)
3. **Error de firmware** (sueño nunca activado)
4. **Cortocircuito** (fallo de hardware)

**✅ Solución:**

**Paso 1: Verificar el modo de sueño en el código**
```cpp
// En el bucle principal, debería tener:
LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
// O un comando de sueño similar

// Para XBee:
pinMode(XBEE_SLEEP_PIN, OUTPUT);
digitalWrite(XBEE_SLEEP_PIN, HIGH); // Dormir XBee cuando no esté transmitiendo
```

**Paso 2: Medir el consumo de corriente**

- Equipo necesario: Multímetro con medición de corriente (modo mA)
- Desconectar la batería, conectar el amperímetro en serie
- Encender el registrador
- Observar la corriente:
    - Sueño: <1 mA (bueno)
    - Activo/midiendo: 50-100 mA (normal, breve)
    - Continuo >50 mA: ¡No está durmiendo! (problema)

**Paso 3: Verificar la salida de depuración serie**

- Conectar el cable FTDI
- Abrir el Monitor Serie
- Observar:
    - ¿Mensajes "Entering sleep"? (si no, el sueño no se activa)
    - ¿"XBee timeout" o similar? (atascado esperando el XBee)
    - ¿Salida continua? (debería ser periódica, no constante)

**Paso 4: Aislar la telemetría**

Si el registrador tiene XBee:

- Retirar el módulo XBee
- Probar durante la noche solo con registro en SD
- Si la vida de la batería es buena ahora: Problema de configuración del XBee (SM no establecido en pin sleep)
- Si la batería sigue agotándose: El problema está en el código/hardware del registrador

**⚙️ Prevención:**
- Probar la vida de la batería en una configuración de banco (durante la noche) antes del despliegue en campo
- Habilitar la salida de depuración para verificar el sueño
- Usar el modo de sueño pin del XBee (SM=1)
- Medir el consumo de corriente con amperímetro durante el desarrollo

---

### Problema 3: El Porcentaje de Batería Parece Incorrecto

**🔴 Síntoma:**
- El registrador informa 50% de batería
- Siguiente lectura: 10% o muerta
- O: Informa 80% durante días, luego muere repentinamente

**🔍 Causas Probables:**
1. **Curva de voltaje LiPo** (descarga no lineal)
2. **Bajas temperaturas** (caída de voltaje)
3. **Batería vieja** (capacidad reducida)
4. **Error de medición de voltaje** (inexactitud del ADC)

**✅ Solución:**

**Comprensión de la descarga LiPo:**
```
Las baterías LiPo mantienen el voltaje hasta cerca del final de su vida:

Voltaje   Porcentaje Restante Aproximado
4.2V      100%
4.0V      80%
3.9V      60%
3.8V      40%
3.7V      30%
3.6V      20%
3.5V      10%
3.4V      5%
3.3V      1% (emergencia, ¡recupere el registrador!)
<3.3V     Riesgo de daño
```

**Nota:** El porcentaje es no lineal y depende de la temperatura y la carga.

**Paso 1: Usar umbrales de voltaje, no porcentaje**

En lugar de:  "Batería al 50%"
Usar:         "Voltaje de la batería: 3.7V" (más preciso)

Umbrales de alerta:

- 3.5V: Planificar recuperación en la próxima semana
- 3.4V: Recuperar dentro de 2 días
- 3.3V: Recuperar inmediatamente (puede no transmitir)

**Paso 2: Compensación de temperatura**

El clima frío causa una caída de voltaje temporal:

- El voltaje medido puede leer 3.5V a 0°C
- La capacidad real puede ser mayor (se calienta → el voltaje se recupera)
- No pánico ante una sola lectura baja en frío
- Recuperar si múltiples lecturas consecutivas <3.4V

**Paso 3: Envejecimiento de la batería**

Las baterías LiPo se degradan con el tiempo:

- Nueva: Capacidad completa de 2600 mAh
- 1 año: ~90% de capacidad (2340 mAh)
- 2 años: ~80% de capacidad (2080 mAh)
- 3+ años: <70% de capacidad, reemplazar

Solución: Reemplazar las baterías anualmente para implementaciones críticas

**⚙️ Prevención:**
- Establecer alertas basadas en voltaje, no en porcentaje
- Reemplazar las baterías anualmente
- Recuperar los registradores antes de que el voltaje sea crítico
- Almacenar baterías de repuesto adecuadamente (40-60% de carga, lugar fresco)

---

## Categoría: Calidad del Sensor y de los Datos

### Problema 4: El Sensor Ultrasónico Lee el Rango Máximo Constantemente

**🔴 Síntoma:**
- El registrador Wari siempre lee 5000mm (o el rango máximo)
- El valor nunca cambia, incluso al mover el sensor

**🔍 Causas Probables:**
1. **Sin eco devuelto** (sin objetivo, o el objetivo es demasiado absorbente)
2. **Sensor sin alimentación** (cable desconectado)
3. **Sensor mal orientado** (apuntando al cielo o al suelo)
4. **Sensor dañado** (poco común)

**✅ Solución:**

**Paso 1: Verificar si el sensor hace clic**

- Encender el registrador
- Sostener el sensor cerca del oído (con cuidado, sin tocar la cara)
- Debería escuchar: un "tic" o "clic" en cada medición (cada 15 segundos o su intervalo)
- Con clic: El sensor está alimentado y transmitiendo → ir al Paso 2
- Sin clic: El sensor no está alimentado → ir al Paso 3

**Paso 2: Sensor alimentado pero sin eco**

Causas de la ausencia de eco:

1. Sin objetivo en el trayecto del haz
    - Demasiado lejos: Acercarse más o usar Lidar
    - Apuntando al cielo: Reorientar hacia el objetivo

2. El objetivo es demasiado absorbente
    - Nieve (polvo fresco): Muy absorbente
    - Espuma: Absorbe el ultrasonido
    - Solución: Esperar a que la nieve se derrita, o usar Lidar

3. El ancho del haz es demasiado estrecho, perdiendo el objetivo
    - Haz de ultrasonido: ~cono de 15-20°
    - Objetivo pequeño (p.ej., arroyo delgado): Puede perderse
    - Solución: Apuntar más cuidadosamente, o usar un sensor de haz más ancho

**Paso 3: Sensor sin alimentación**

- Desconectar el cable del sensor del registrador
- Verificar la continuidad:
    - Pines del cable al conector: Debe tener resistencia <1Ω
    - ¿Cable dañado? (aplastado, cortado, corroído): Reemplazar el cable
- Verificar el conector del PCB:
    - ¿Uniones de soldadura intactas? (inspección visual)
    - ¿Pines haciendo contacto? (intentar volver a asentar el cable)
- Probar con multímetro:
    - Medir el voltaje en el conector del sensor cuando el registrador está activo
    - Debería ver 5V o 12V (dependiendo del modelo de sensor)
    - Sin voltaje: Fallo del circuito de alimentación en el PCB

**Paso 4: Reemplazo del sensor**

Si todo lo demás falla:

- El sensor puede haber fallado internamente (poco común pero posible)
- Prueba: Intercambiar el sensor con una unidad conocida en buen estado
- Si el nuevo sensor funciona: El sensor original está muerto
- Si el nuevo sensor también lee el rango máximo: El problema es la orientación/objetivo/entorno, no el sensor

**⚙️ Prevención:**
- Orientación cuidadosa durante la instalación (probar antes de abandonar el sitio)
- Proteger el cable de daños (enrutamiento seguro, evitar puntos de pellizco)
- Elegir el sensor apropiado para el material del objetivo (Lidar para nieve/espuma)
- Mantener la cara del sensor limpia (el polvo/escombros pueden bloquear el ultrasonido)

---

### Problema 5: Las Lecturas de Distancia se Desvían con el Tiempo

**🔴 Síntoma:**
- Los valores de distancia cambian en 50-200mm durante días
- Pero el nivel real del agua es estable (verificado visualmente)
- Deriva lenta, no saltos repentinos

**🔍 Causas Probables:**
1. **Cambios de temperatura** (la estructura de montaje del sensor se expande/contrae)
2. **Crecimiento de vegetación** (obstrucción del haz)
3. **Aflojamiento del montaje** (tornillos, bridas)
4. **Deriva de calibración del sensor** (poco común)

**✅ Solución:**

**Paso 1: Verificar la correlación con la temperatura**

- Trazar la distancia vs. la temperatura a lo largo del tiempo
- Si hay una correlación fuerte (la distancia aumenta cuando la temperatura aumenta):
    - Causa: La estructura de montaje se expande con el calor
    - Ejemplo: El montaje metálico se calienta al sol, se expande, el sensor se mueve 5mm hacia abajo
    - Solución: 
        - a) Aceptar como variación natural (±50mm generalmente aceptable)
        - b) Usar montaje térmicamente estable (minimizar el metal)
        - c) Compensar por temperatura en el postprocesamiento

**Paso 2: Inspeccionar el montaje**

- Inspección visual de la instalación del registrador
- Verificar:
    - ¿Bridas apretadas? (pueden aflojarse con el tiempo)
    - ¿Tornillos/pernos apretados?
    - ¿Soporte doblado o movido?
- Solución: Reapretar, reforzar el montaje

**Paso 3: Verificar si hay vegetación**

- Inspeccionar el trayecto del haz
- ¿Vegetación creciente? (hierba, maleza, ramas)
    - ¿Nuevas hojas en primavera?
    - ¿Algas creciendo en la superficie objetivo?
- Solución: Podar la vegetación, limpiar el objetivo
- Prevención: Instalar con un trayecto del haz despejado, anticipar el crecimiento estacional

**Paso 4: Verificación de la calibración del sensor**

Raramente necesario, pero para verificar:

- Medir una distancia conocida con cinta métrica (p.ej., 2000mm)
- Colocar el objetivo a esa distancia
- La lectura del registrador debe coincidir ±50mm
- Si difiere en >100mm: El sensor puede necesitar reemplazo

**⚙️ Prevención:**
- Usar materiales de montaje térmicamente estables
- Mantener el trayecto del haz despejado (podar la vegetación trimestralmente)
- Asegurar el montaje con sujetadores redundantes
- Considerar la expansión/contracción estacional en el análisis de datos

---

### Problema 6: El Lidar Devuelve Valores Negativos

**🔴 Síntoma:**
- El registrador lidar registra ocasionalmente distancias negativas (p.ej., -150mm)
- La mayoría de las lecturas son positivas y correctas

**🔍 Causas Probables:**
1. **Error de comunicación I2C** (corrupción de datos)
2. **Tiempo de espera interpretado como negativo** (error de código)
3. **Desbordamiento de entero** (error matemático en el código)

**✅ Solución:**

**Paso 1: Verificar el patrón**

- ¿Cuándo ocurren los negativos?
    - Aleatoriamente: Error I2C (ir al Paso 2)
    - Todas las lecturas después de cierto valor: Desbordamiento (ir al Paso 3)
    - Durante/después de la transmisión: XBee interfiriendo con I2C (ir al Paso 4)

**Paso 2: Confiabilidad de la comunicación I2C**

I2C es sensible a:

- Longitud del cable (mantener <30cm)
- Interferencia (EMI de fuentes cercanas)
- Conexiones deficientes (cables sueltos)

Soluciones:

- a) Agregar resistencias pull-up (2.2kΩ o 4.7kΩ en SDA y SCL)
- b) Acortar los cables I2C
- c) Agregar ferrita en el cable
- d) Blindar el cable si está cerca de una fuente RF fuerte

**Paso 3: Error de código - desbordamiento**
```cpp
// Código incorrecto (desbordamiento potencial):
int distance = lidar.read(); // int = -32768 a 32767
// Si la distancia real > 32767mm, se desborda a negativo

// Código correcto:
unsigned int distance = lidar.read(); // sin signo = 0 a 65535
// O:
long distance = lidar.read(); // long = rango mucho mayor
```

**Paso 4: Interferencia del XBee**

Si los negativos ocurren durante/después de la telemetría:

- La transmisión del XBee puede causar EMI
- Solución:
    - a) Separar los cables I2C de la antena del XBee
    - b) Medir antes de la transmisión (no durante)
