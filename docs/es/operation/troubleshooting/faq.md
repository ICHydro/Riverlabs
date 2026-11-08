# Preguntas frecuentes

!!! abstract "Descripción general"
    Respuestas a preguntas habituales sobre los registradores Riverlabs.

## Preguntas generales

### ¿Qué es un registrador Riverlabs?

Un **registrador Riverlabs** es un dispositivo de código abierto y bajo coste para medir el nivel del agua, diseñado para la investigación hidrológica. Utiliza sensores ultrasónicos o lidar para medir la distancia hasta la superficie del agua, guarda los datos en una tarjeta SD y, opcionalmente, los transmite mediante telemetría celular.

**Características principales:**
- Basado en dise&ntilde;os electronicos abiertos
- Alimentación por batería (Li-ion recargable)
- Almacenamiento de datos en tarjeta SD
- Telemetría celular opcional (LTE / LoRaWAN)
- Carcasa resistente a la intemperie

---

### Medición de distancia ultrasonica o lidar: ¿cuál debo elegir?

**Respuesta rápida:**

| Característica | Wari (ultrasónico) | Lidar |
|---------|-------------------|-------|
| **Alcance** | 0.3m - 5m | 0.05m - 40m |
| **Precisión** | ±10mm | ±25mm |
| **Autonomía de la batería** | 30-60 días | 15-30 días |
| **Coste** | Menor (~30 USD por sensor) | Mayor (~150 USD por sensor) |
| **Sensibilidad a la temperatura** | Sí (compensación integrada) | No |
| **Funciona con nieve/espuma** | No (absorben la señal) | Sí |
| **Más adecuado para** | Ríos, estanques (superficies estables) | Amplio rango, nieve, largas distancias |

**Elige ultrasónico si:**
- El rango de nivel del agua es <5m
- Tienes un presupuesto limitado
- Necesitas mayor autonomía
- La superficie del agua es estable

**Elige lidar si:**
- Necesitas un alcance >5m
- Mides sobre nieve o espuma
- Hay temperaturas extremas (el lidar no se ve afectado)
- La precisión es menos crítica (±25mm es aceptable)

Consulta la [guía de identificación del registrador](../quick-start/logger-identification.md) para una comparación detallada.

---

### ¿Puedo usar registradores Riverlabs en agua salada?

**Sí, con precauciones:**

**Sensor:** Tanto el ultrasónico como el lidar pueden medir la distancia *hasta* el agua salada (sin contacto).

**Carcasa:**
- Carcasas de plástico estándar: adecuadas temporalmente (días o semanas)
- A largo plazo (meses): utiliza una carcasa de grado marino
- Riesgo: corrosión de la electrónica y los conectores por salpicaduras salinas

**Montaje:**
- Elementos de fijación de acero inoxidable (grado marino 316)
- Evita el aluminio (se corroe en agua salada)

**Mantenimiento:**
- Enjuaga el registrador con agua dulce después de recuperarlo
- Comprueba mensualmente si hay corrosión
- Aplica grasa dieléctrica a los conectores

---

### ¿Cuánto dura la batería?

**Depende de la configuración:**

**Wari (ultrasónico), solo SD:**
- Registro cada 15 minutos: 30-60 días
- Registro cada 5 minutos: 15-30 días
- Registro cada minuto: 7-10 días

**Lidar, solo SD:**
- Registro cada 15 minutos: 15-30 días
- Registro cada 5 minutos: 7-15 días
- Registro cada minuto: 3-5 días

**Con telemetría (consumo adicional por transmisión):**
- Transmisión cada hora: reduce la autonomía un 30-40 %
- Transmisión diaria: reduce la autonomía un 10-20 %

**Factores:**
- Capacidad de la batería (2600mAh habitualmente)
- Temperatura (el frío reduce la capacidad)
- Antigüedad de la batería (se degrada con el tiempo)
- Intensidad de la señal de telemetría (una señal débil consume más)

Consulta la [guía de baterías y alimentación](../hardware/battery-power-guide.md) para cálculos detallados y consejos de optimización.

---

### ¿Puedo añadir alimentación solar?

El modelo wari cuenta con circuito electronico para carga solar, y esta activado o desactivado durante el ensamblaje. No se recomienda la activació por el usuario. Si la alimentación del panel solar original no es suficiente, se puede cambiar por otro panel mas grande, mientras que el voltaje del panel es de **6V**.

---

### ¿Es de código abierto? ¿Puedo modificar el código?

**Sí, es totalmente de código abierto:**

**Licencia:** Licencia MIT (permisiva)

**Código fuente:** https://github.com/ICHydro/Riverlabs

**Puedes:**
- ✅ Utilizarlo para cualquier fin (comercial o no comercial)
- ✅ Modificar el código según tus necesidades
- ✅ Compartir las modificaciones
- ✅ Contribuir mejoras al proyecto

**Deberías:**
- Incluir la licencia original en las obras derivadas
- Reconocer a los autores originales
- Compartir las mejoras (ayuda a la comunidad)

**Modificaciones habituales:**
- Cambiar el intervalo de registro
- Añadir sensores (temperatura, humedad)
- Modificar el formato de los datos
- Implementar ciclos de reposo personalizados
- Añadir protocolos de telemetría

---

### ¿Puedo utilizar otro sensor?

**Sí, modificando el código:**

**Sensores ultrasónicos:**
- MaxBotix MB7389 (predeterminado)
- Otros modelos MaxBotix (MB7092, MB7360, etc.)
- HC-SR04 (barato, pero menos fiable)

**Cambios necesarios en el código:**
- Asignación de pines
- Niveles de tensión (3.3V frente a 5V)
- Protocolo de comunicación (analógico, serie o I2C)

**Sensores lidar:**
- Garmin LIDAR-Lite v3 (predeterminado)
- TF-Luna (alternativa más barata)
- VL53L1X (solo corto alcance)

**Otros sensores de distancia:**
- Transductores de presión (para uso sumergido)
- Interruptores de flotador (solo niveles de encendido/apagado)

**Recomendación:** Utiliza los sensores predeterminados salvo que tengas necesidades específicas. Están bien probados y el código está listo para usar.

---

### ¿Qué tarjeta SD debo utilizar?

**Recomendaciones:**

**Capacidad:** 8-32GB (las tarjetas mayores suponen un gasto innecesario; el registrador no las llenará)

**Marca:** SanDisk, Samsung, Kingston (evita marcas desconocidas, con mayor tasa de fallos)

**Tipo:** SD estándar o microSD con adaptador (NO SDXC)

**Formato:** FAT32 (no exFAT ni NTFS)

**Ejemplo de cálculo:**
- Registro cada 15 minutos
- 100 bytes por muestra
- Muestras por día: 96
- Datos por día: 9.6 KB
- Tarjeta de 16GB: ~1000 años de datos (!!)
- En la práctica: 8GB es más que suficiente

**Evita:**
- Tarjetas >32GB (pueden utilizar exFAT en vez de FAT32)
- Tarjetas de velocidad ultralta (UHS-II): gasto innecesario para el registro
- MicroSD sin adaptador (difícil de manipular)

---

### ¿Cómo protejo el registrador del agua?

**Opciones de carcasa:**

**1. Carcasa con certificación IP67** (recomendada)
- Ejemplos: Hammond 1554, Bud Industries
- Tapa sellada mediante junta
- Prensaestopas para los cables
- Coste: 15-30 USD

**2. Impermeabilización casera**
- Recipiente de plástico para alimentos (solo a corto plazo)
- Sellado con silicona
- Cinta americana alrededor (añade cierta protección)
- Coste: 5 USD

**Entradas de cables:**
- Utiliza prensaestopas (lo mejor)
- O bien: perfora un orificio, pasa el cable y séllalo con silicona
- Alivio de tensión: haz un nudo dentro de la carcasa (evita tirones)

**Prueba:**
- Sumerge la carcasa en un cubo (¡sin componentes electrónicos dentro!)
- Espera una hora
- Ábrela y comprueba si ha entrado agua
- Repara las filtraciones antes del despliegue

**Desecante:**
- Añade una bolsita de gel de sílice dentro de la carcasa
- Absorbe la humedad (evita la condensación)
- Sustitúyela si está saturada (se vuelve rosa)

Consulta la [guía de instalación](../../installation/mounting-guide.md) para instrucciones detalladas de impermeabilización.

---

## Preguntas sobre telemetría

### ¿Necesito telemetría?

**Depende del uso previsto:**

**NECESITAS telemetría si:**
- El lugar es difícil o peligroso de acceder
- Se requiere vigilancia en tiempo real (alerta de inundaciones)
- Hay muchos registradores y no resulta práctico comprobar cada uno
- Quieres detectar rápidamente fallos del registrador

**NO necesitas telemetría si:**
- El lugar es accesible (se puede visitar mensualmente)
- Vas a analizar los datos después del evento (no en tiempo real)
- Tienes un presupuesto limitado (100 frente a 200 USD)
- No hay cobertura celular en el lugar

**Solución intermedia:** tarjeta SD + telemetría ocasional
- El registrador guarda en SD (alta frecuencia, fiable)
- Transmite parte de los datos (resumen horario)
- Los datos con resolución completa permanecen en la tarjeta SD

---

### ¿Cuántos datos utiliza la telemetría?

**Consumo típico:**

**Por transmisión:**
- Carga JSON: 50-100 bytes (solo datos)
- Sobrecarga HTTP: 200-500 bytes (cabeceras, establecimiento de conexión)
- Total: ~500-600 bytes por transmisión

**Ejemplos de consumo mensual:**

| Intervalo | Transmisiones/día | Datos diarios | Datos mensuales |
|----------|-------------------|------------|--------------|
| Cada medición (15 min) | 96 | 58 KB | 1.7 MB |
| Cada hora | 24 | 14 KB | 430 KB |
| Cada 4 horas | 6 | 3.6 KB | 110 KB |
| Cada día | 1 | 600 bytes | 18 KB |

**Plan recomendado:** 5-10 MB/mes (con margen suficiente)

---

## Preguntas sobre instalación en campo

### ¿Cómo oriento el sensor?

**Principios generales:**

**1. Apunta a la superficie del agua:**
- El haz debe llegar al agua (en todo el rango previsto de niveles)
- Evita orillas, rocas y vegetación

**2. La orientación vertical es ideal:**
- Apuntar hacia abajo elimina los errores de ángulo
- Si lo instalas en ángulo, utiliza trigonometría para corregirlo (consulta [posicionamiento del sensor](../../installation/sensor-positioning.md))

**3. Ten en cuenta los cambios de nivel:**
- Caudal bajo: ¿dónde estará el agua? (no apuntes demasiado alto)
- Caudal alto: evita que el sensor quede sumergido (instálalo suficientemente alto)

**4. Considera la anchura del haz:**
- Ultrasónico: cono de ~15-20°
- Lidar: haz de ~2°
- Evita que el haz alcance el puente o la estructura

**Prueba:**
- Toma mediciones en el lugar antes de la instalación definitiva
- Comprueba todo el rango previsto de niveles del agua
- Verifica que las lecturas sean razonables

---

### ¿Qué ocurre si el nivel del agua queda fuera del alcance del sensor?

**Dos situaciones:**

**1. Agua demasiado cerca (por debajo de la distancia mínima):**
- Mínimo del ultrasónico: ~0.3m
- Mínimo del lidar: ~0.05m
- **Solución:** Instala el sensor más alto

**2. Agua demasiado lejos (más allá del alcance máximo):**
- Máximo del ultrasónico: ~5m
- Máximo del lidar: ~40m
- **Síntomas:** El registrador muestra el alcance máximo (5000mm o 40000mm)
- **Solución:** Instala el sensor más bajo o utiliza uno de mayor alcance

**Cómo elegir el alcance:**
- Determina el rango de niveles del agua: caudal bajo y alto
- El alcance del sensor debe cubrir desde el nivel bajo hasta el alto, con margen
- Ejemplo:
  - Caudal bajo: 2m por debajo del sensor
  - Caudal alto: 0.5m por debajo del sensor
  - Alcance necesario: 0.5-2m → utiliza un ultrasónico (rango 0.3-5m)

**Margen de seguridad:** Reserva ±0.5m en cada extremo (no trabajes en los límites absolutos)

---

### ¿Cómo determino la altura de instalación?

**Altura de instalación** = distancia desde el sensor hasta el punto de referencia

**Puntos de referencia habituales:**
1. **Fondo del cauce** (lecho)
   - Mide desde el sensor hasta el lecho (punto más bajo previsto)
   - Profundidad del agua = altura de instalación - lectura del sensor

2. **Referencia topográfica fija** (hito)
   - Requiere equipo topográfico
   - Permite obtener una cota absoluta (p. ej., metros sobre el nivel del mar)
   - La mejor opción para estudios científicos

3. **Referencia arbitraria**
   - «Parte superior del tablero del puente»
   - «Nivel del suelo junto al registrador»
   - Es coherente, pero no se puede trasladar a otras ubicaciones

**Medición:**
- Utiliza una cinta métrica, un telémetro láser o equipo topográfico
- Mide cuando el agua esté estable y sea posible medirla
- Registra la altura de instalación y la fecha
- Ten en cuenta que el lecho puede cambiar (erosión o sedimentación)

**Documentación:**
- Dibuja un esquema con el registrador, el sensor, el agua y la referencia
- Fotografía la instalación con la cinta métrica visible
- Registra las coordenadas GPS

---

### ¿Puedo instalarlo en invierno o con heladas?

**Sí, pero plantea dificultades:**

**Efectos del frío:**

**1. Reducción de la capacidad de la batería:**
- 0°C: ~80 % de capacidad
- -20°C: ~50 % de capacidad
- **Solución:** Batería más grande o visitas más frecuentes

**2. Riesgo por congelación de la LiPo:**
- La LiPo se congela a ~-40°C
- Cargarla a bajas temperaturas es peligroso
- **Solución:** No la cargues por debajo de 0°C

**3. Hielo en el sensor:**
- El hielo bloquea los ultrasonidos
- El lidar puede funcionar a través del hielo (hasta cierto punto)
- **Solución:** Resistencia calefactora (compleja) o utilizar lidar

**4. Condensación:**
- Los ciclos de temperatura producen humedad dentro de la carcasa
- **Solución:** Desecante dentro de una carcasa con certificación IP67

**5. Nieve:**
- La nieve reciente absorbe los ultrasonidos (el registrador muestra el alcance máximo)
- El lidar funciona mejor, pero también se ve afectado
- **Solución:** Para medir bajo el hielo, utiliza un transductor de presión sumergido

**Recomendación para inviernos duros:**
- Instálalo a finales de otoño (antes de las heladas)
- Recupéralo a principios de primavera (tras el deshielo)
- O utiliza una carcasa calefactada y alimentación de red (ya no será una solución de bajo coste)

---

### ¿Con qué frecuencia debo visitar el lugar?

**Depende de:**
1. Autonomía de la batería (consulta la guía de baterías)
2. Capacidad de la tarjeta SD (normalmente no es un problema)
3. Riesgo de pérdida o daños (vandalismo, inundaciones)
4. Importancia de los datos

**Calendarios habituales:**

**Con telemetría:**
- Primera visita (tras una semana): verificar que funciona
- Visitas mensuales: comprobar el estado físico
- Cambio de batería: cuando la tensión sea <3.5V (supervisión remota)

**Sin telemetría:**
- Primera visita (tras una semana): verificar que funciona
- Visitas mensuales: revisar datos y cambiar la tarjeta SD si hace falta
- Cambio de batería: mensualmente o según la autonomía prevista

**Lugares de alto riesgo:**
- Visitas semanales al principio
- Considera un montaje más robusto o medidas de protección

**Lugares remotos:**
- Planifica según el acceso estacional (puede ser imposible en invierno)
- Instala en primavera y retira en otoño

---

### ¿Qué hago si roban o pierdo el registrador?

**Prevención:**

**1. Disimúlalo:**
- Pinta el registrador para integrarlo en el entorno (marrón o verde)
- Escóndelo tras la vegetación
- Evita colores llamativos o etiquetas visibles

**2. Montaje seguro:**
- Cable de seguridad (como un candado de bicicleta)
- Fíjalo a la estructura con tornillos antimanipulación

**3. Señalización:**
- Pequeña etiqueta: «Equipo de investigación; por favor, no manipular»
- Datos de contacto (correo electrónico, no teléfono)
- Algunas personas la respetarán, aunque también puede llamar la atención

**4. Elección del lugar:**
- Una propiedad privada (con permiso) es más segura que un espacio público
- Elige ubicaciones menos visibles

**Recuperación:**

**1. Números de serie:**
- Anota el ICCID de la SIM, el número de serie del XBee y el ID del registrador
- Puede ayudar con el seguro o el seguimiento por el operador

**2. Seguro:**
- Comprueba si el equipo de investigación está cubierto
- Documenta su valor con facturas

**3. Copias de seguridad:**
- Con telemetría: los datos están a salvo en ThingsBoard
- Solo SD: se pierden los datos

**Asumir el riesgo:**
- El menor coste de los registradores hace que la pérdida sea más asumible
- Presupuesta una tasa de pérdida del 10 % en proyectos con varios registradores

---

## Preguntas sobre datos y análisis

### ¿En qué formato se guardan los datos?

**CSV (valores separados por comas)**

**Archivo de ejemplo: `2025-12-27.CSV`**

```csv
Date,Time,Distance_mm,Temperature_C,Battery_V
2025-12-27,10:00:00,1234,22.5,3.85
2025-12-27,10:15:00,1256,23.1,3.84
2025-12-27,10:30:00,1278,23.8,3.84
```

**Campos:**
- **Date:** AAAA-MM-DD
- **Time:** HH:MM:SS (24 horas)
- **Distance_mm:** Lectura del sensor (milímetros)
- **Temperature_C:** Temperatura del aire (solo Wari, Celsius)
- **Battery_V:** Tensión de la batería (voltios)

**Apertura:**
- Excel/Google Sheets: abre directamente el archivo .CSV
- Python: `pandas.read_csv()`
- R: `read.csv()`
- MATLAB: `readtable()`

---

### ¿Cómo convierto la distancia en profundidad del agua?

**Profundidad del agua = altura de instalación - distancia medida por el sensor**

**Ejemplo:**
- Registrador montado a 3000mm sobre el lecho
- El sensor mide 1500mm (distancia a la superficie del agua)
- Profundidad = 3000mm - 1500mm = 1500mm = 1.5m

**En Excel:**
```excel
=($A$1 - B2)
```
Donde:
- A1: altura de instalación (constante)
- B2: lectura del sensor (esta fila)

**En Python:**
```python
import pandas as pd
df = pd.read_csv('data.csv')
installation_height = 3000  # mm
df['water_depth_mm'] = installation_height - df['Distance_mm']
```

**Unidades:**
- Mantén unidades coherentes (todo en mm)
- Para convertir a metros: divide por 1000

---

### ¿Qué precisión tienen los datos?

**Precisión de los sensores:**

**Wari (ultrasónico):**
- Especificación del fabricante: ±10mm
- En condiciones reales: ±20mm

**Lidar:**
- Especificación del fabricante: ±25mm
- En condiciones reales: ±30-60mm

**Fuentes de error:**
1. **Precisión del sensor:** Limitación intrínseca
2. **Temperatura:** Afecta a la velocidad del sonido (ultrasónico) o al índice de refracción del aire (lidar)
3. **Movimiento del montaje:** Viento, dilatación térmica
4. **Ángulo del haz:** Si no es perfectamente vertical
5. **Superficie objetivo:** Agua turbulenta, espuma, ondas
6. **Entorno:** Lluvia, nieve, niebla

**Para mejorar la precisión:**
- Usa Wari cuando se necesite precisión absoluta (<10mm)
- Monta el sensor verticalmente (elimina el error angular)
- Fíjalo de forma estable (reduce el movimiento)
- Busca una superficie tranquila (usa un pozo tranquilizador en aguas turbulentas)
- Compensa la temperatura (Wari la mide automáticamente)

**Precisión suficiente para:**
- Vigilancia de inundaciones (cambios de 0.5m)
- Relaciones nivel-caudal
- Detección de presencia o ausencia

**No adecuado para:**
- Topografía de precisión (<1mm)
- Derechos legales sobre el agua (utiliza equipos certificados)

---

### ¿Dónde puedo encontrar ejemplos de código?

**Repositorio de GitHub:** https://github.com/ICHydro/Riverlabs

**Sketches principales:**
- `wari_v2.1/wari_v2.1.ino` - Registrador ultrasónico (versión más reciente)
- `wari_lidar/wari_lidar.ino` - Registrador lidar
- `wari_3G/wari_3G.ino` - Con telemetría 3G
- `wari_4G/wari_4G.ino` - Con telemetría 4G LTE-M

**Utilidades:**
- `set_clock/set_clock.ino` - Ajuste de la hora del RTC

**Bibliotecas utilizadas:**
- RTClib (Adafruit)
- SdFat (Bill Greiman)
- LowPower (Rocket Scream)
- XBee (Digi)
- Wire (I2C, integrada)

**Documentación:**
- Los comentarios del código explican su funcionamiento
- README.md en cada directorio
- ¡Este sitio de documentación!

---

## Preguntas rápidas para solucionar problemas

### El registrador no se enciende

→ Consulta [problema 1: el registrador no se enciende](common-issues.md#problema-1-el-registrador-no-se-enciende)

**Comprobación rápida:** ¿Batería >3.3V? ¿Conectada correctamente? ¿Interruptor en ON?

---

### La tarjeta SD indica «corrupta»

→ Consulta [problema 7: tarjeta SD corrupta](common-issues.md#problema-7-la-tarjeta-sd-aparece-corrupta-o-necesita-formateo)

**Solución rápida:** Prueba un programa de recuperación y después formatéala como FAT32.

---

### El sensor siempre indica su alcance máximo

→ Consulta [problema 4: el sensor siempre indica su alcance máximo](common-issues.md#problema-4-el-sensor-ultrasonico-siempre-indica-el-alcance-maximo)

**Comprobación rápida:** ¿Emite clics el sensor? ¿Apunta al objetivo? ¿Está dentro del alcance?

---

### No llegan datos de telemetría

→ Consulta [resolución de problemas de conexión de telemetría](../../telemetry/troubleshooting-connections.md)

**Comprobación rápida:** ¿SIM activa? ¿Buena señal? ¿API Mode = 2? ¿Token correcto?

---

### La batería se agota demasiado rápido

→ Consulta [problema 2: la batería se agota durante la noche](common-issues.md#problema-2-la-bateria-se-agota-durante-la-noche)

**Comprobación rápida:** ¿Entra en reposo el registrador? ¿Intervalo de telemetría razonable? ¿Batería antigua?

---

## Contribuciones y comunidad

### ¿Cómo puedo contribuir?

**Formas de contribuir:**

**1. Mejoras del código:**
- Corregir errores
- Añadir funciones
- Optimizar
- Enviar solicitudes de incorporación de cambios en GitHub

**2. Documentación:**
- Corregir errores
- Añadir ejemplos
- Traducir a otros idiomas

**3. Diseños de hardware:**
- Mejorar las PCB
- Diseñar carcasas (imprimibles en 3D)
- Proponer soluciones de montaje

**4. Pruebas y comentarios:**
- Instalar registradores y comunicar problemas
- Compartir fotos y datos de las instalaciones
- Redactar casos de estudio

**5. Apoyo a la comunidad:**
- Responder preguntas en GitHub Issues
- Compartir tu experiencia

**Para empezar:**
- Crea una bifurcación del repositorio
- Haz tus cambios
- Envía una solicitud de incorporación
- Coméntalo en el sistema de incidencias

---

### ¿Dónde puedo obtener ayuda?

**Recursos:**

**1. Esta documentación:**
- Guías completas para la mayoría de situaciones
- Función de búsqueda (arriba a la derecha)

**2. GitHub Issues:**
- https://github.com/ICHydro/Riverlabs/issues
- Busca incidencias existentes (quizá ya haya una respuesta)
- Crea una incidencia con estos datos:
  - Configuración del registrador
  - Descripción del problema
  - Qué has intentado
  - Salida del monitor serie
  - Fotos

**3. Grupo de investigación:**
- Contacto: grupo ICHydro (si estás vinculado)
- Colaboraciones académicas

**4. Comunidad:**
- (En el futuro: foro o chat cuando crezca la comunidad)

**Antes de preguntar:**
- Lee detenidamente la documentación
- Sigue los diagramas de diagnóstico
- Consulta los problemas habituales
- Reúne información de diagnóstico (registros serie, fotos)

---

## Próximos pasos

- 🚀 [Guía de inicio rápido](../quick-start/quick-start.md) - Empieza ahora
- 📊 [Diagramas de diagnóstico](diagnostic-flowcharts.md) - Solución visual de problemas
- 📋 [Problemas habituales](common-issues.md) - Soluciones detalladas
- 🛠️ [Mantenimiento del hardware](../hardware/maintenance.md) - Mantén el registrador en buen estado
- 📡 [Configuración de telemetría](../../telemetry/setup-guide.md) - Añade supervisión remota

---

!!! question "¿Todavía tienes preguntas?"
    Si no encuentras la respuesta aquí, consulta [GitHub Issues](https://github.com/ICHydro/Riverlabs/issues) o crea una nueva incidencia con tu pregunta. ¡La comunidad puede ayudarte!
