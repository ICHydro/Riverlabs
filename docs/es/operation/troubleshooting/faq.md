# Preguntas Frecuentes

!!! abstract "Resumen"
    Preguntas comunes sobre los registradores Riverlabs respondidas de forma clara y concisa. Desde especificaciones técnicas hasta consejos de despliegue en campo.

## Preguntas Generales

### ¿Qué es un registrador Riverlabs?

Un **registrador Riverlabs** es un dispositivo de monitoreo del nivel del agua de código abierto y bajo costo, diseñado para la investigación hidrológica. Usa sensores ultrasónicos o lidar para medir la distancia a la superficie del agua, registrando datos en tarjeta SD y transmitiendo opcionalmente a través de telemetría celular.

**Características principales:**
- Basado en Arduino (ATmega328P)
- Alimentado por batería (LiPo recargable)
- Almacenamiento de datos en tarjeta SD
- Telemetría celular opcional (XBee 3G/4G)
- Carcasa resistente a la intemperie
- Bajo costo (~$100-300 dependiendo de la configuración)

---

### Wari vs. Lidar - ¿cuál debo elegir?

**Respuesta rápida:**

| Característica | Wari (Ultrasónico) | Lidar |
|----------------|---------------------|-------|
| **Rango** | 0.3m - 5m | 0.05m - 40m |
| **Precisión** | ±10mm | ±25mm |
| **Vida de la batería** | 30-60 días | 15-30 días |
| **Costo** | Menor (~$30 sensor) | Mayor (~$150 sensor) |
| **Sensibilidad a la temperatura** | Sí (compensación integrada) | No |
| **Funciona en nieve/espuma** | No (absorbido) | Sí |
| **Mejor para** | Ríos, estanques (objetivos estables) | Rango amplio, nieve, largas distancias |

**Elija Wari si:**
- El rango del nivel del agua es <5m
- Presupuesto limitado
- Se necesita mayor vida de la batería
- Objetivo de agua líquida estable

**Elija Lidar si:**
- Necesita >5m de rango
- Midiendo a través de nieve o espuma
- Temperaturas extremas (Lidar no se ve afectado)
- La precisión es menos crítica (±25mm aceptable)

Consulte la [Guía de Identificación del Registrador](../quick-start/logger-identification.md) para una comparación detallada.

---

### ¿Puedo usar registradores Riverlabs en agua salada?

**Sí, con precauciones:**

**Sensor:** Tanto el ultrasónico como el lidar funcionan bien midiendo la distancia *hasta* el agua salada (medición sin contacto).

**Carcasa:**
- Carcasas de plástico estándar: OK para uso temporal (días-semanas)
- Para largo plazo (meses): Usar carcasa de grado marino
- Riesgo: Corrosión por salpicaduras de sal en la electrónica y los conectores

**Montaje:**
- Hardware de acero inoxidable (grado 316 marino)
- Evitar el aluminio (se corroe en agua salada)

**Mantenimiento:**
- Enjuagar el registrador con agua dulce después de recuperarlo
- Verificar si hay corrosión mensualmente
- Aplicar grasa dieléctrica en los conectores

**Recomendación:** Los registradores Riverlabs están diseñados principalmente para agua dulce. Para instalaciones permanentes en agua salada, considere equipos comerciales de grado marino.

---

### ¿Cuánto dura la batería?

**Depende de la configuración:**

**Wari (ultrasónico), solo SD:**
- Registro cada 15 minutos: 30-60 días
- Registro cada 5 minutos: 15-30 días
- Registro cada 1 minuto: 7-10 días

**Lidar, solo SD:**
- Registro cada 15 minutos: 15-30 días
- Registro cada 5 minutos: 7-15 días
- Registro cada 1 minuto: 3-5 días

**Con telemetría (agregar sobrecarga de transmisión):**
- Transmisión horaria: Reducir en 30-40%
- Transmisión diaria: Reducir en 10-20%

**Factores:**
- Capacidad de la batería (2600mAh típico)
- Temperatura (el frío reduce la capacidad)
- Antigüedad de la batería (se degrada con el tiempo)
- Intensidad de la señal de telemetría (señal débil = más energía)

Consulte la [Guía de Batería y Alimentación](../hardware/battery-power-guide.md) para cálculos detallados y consejos de optimización.

---

### ¿Puedo agregar energía solar?

**Técnicamente sí, pero es complicado:**

**Desafíos:**
1. **Se necesita controlador de carga** - LiPo requiere perfil de carga específico
2. **Integración impermeable** - Montaje del panel solar, penetraciones de cableado
3. **Latitudes del norte** - Solar insuficiente en invierno
4. **Costo** - Agrega $50-100, acercándose al costo de una solución comercial

**Recomendación:**
- Para la mayoría de los usuarios: Solo intercambiar baterías periódicamente (más simple, más económico)
- Para sitios remotos/inaccesibles: Considerar registradores solares comerciales
- Si DIY: Usar controlador de carga solar dedicado (p.ej., Adafruit Universal USB), panel de 5-10W

**Cálculo:**
- Consumo diario: ~50mAh (Wari, registro de 15 min)
- Panel solar necesario: ~2-5W (teniendo en cuenta nubes, invierno)
- Tamaño de la batería: 10,000+ mAh (para reserva invernal)

---

### ¿Es de código abierto? ¿Puedo modificar el código?

**¡Sí! Totalmente de código abierto:**

**Licencia:** Licencia MIT (permisiva)

**Código fuente:** https://github.com/ICHydro/Riverlabs

**Puede:**
- ✅ Usar para cualquier propósito (comercial o no comercial)
- ✅ Modificar el código para adaptarlo a sus necesidades
- ✅ Compartir modificaciones
- ✅ Contribuir mejoras de vuelta al proyecto

**Debería:**
- Incluir la licencia original en los derivados
- Acreditar a los autores originales
- Compartir mejoras (ayuda a la comunidad)

**Modificaciones comunes:**
- Cambiar el intervalo de registro
- Agregar sensores adicionales (temperatura, humedad)
- Modificar el formato de datos
- Implementar programas de sueño personalizados
- Agregar nuevos protocolos de telemetría

---

## Preguntas de Hardware

### ¿Puedo comprar registradores preensamblados?

**Estado actual:** Riverlabs es principalmente un proyecto DIY.

**Opciones:**

1. **Ensamblar usted mismo** (más económico, ~$100-150)
   - Comprar componentes
   - Seguir la guía de ensamblaje
   - Se requiere soldadura

2. **Asociaciones universitarias/de investigación** (para proyectos de investigación)
   - Contacto: ICHydro/desarrolladores originales
   - Pueden proporcionar unidades ensambladas para investigación colaborativa

3. **Alternativas comerciales** (si DIY no es factible)
   - HOBO Water Level Logger (~$500-1000)
   - Solinst Levelogger (~$1000-2000)
   - Sensores Campbell Scientific (~$2000+)

**Nota:** A medida que el proyecto crece, puede que estén disponibles kits ensamblados. Consulte GitHub para actualizaciones.

---

### ¿Qué componentes necesito?

**Configuración mínima de Wari:**

| Componente | Cant | Costo Est. |
|------------|------|------------|
| PCB ATmega328P (personalizado) | 1 | $30 |
| Sensor ultrasónico (MaxBotix MB7389) | 1 | $30 |
| RTC (DS3231) | 1 | $5 |
| Módulo de tarjeta SD | 1 | $3 |
| Batería LiPo (3.7V 2600mAh) | 1 | $10 |
| Tarjeta SD (8-32GB) | 1 | $10 |
| Carcasa (impermeable) | 1 | $15 |
| Programador FTDI | 1 | $10 |
| Varios (cables, conectores) | - | $10 |
| **Total** | | **~$123** |

**Agregar para telemetría:**
- Módulo XBee3 Cellular: $70
- Tarjeta SIM + plan de datos: $2-10/mes
- Adaptador USB (para configuración): $25

---

### ¿Puedo usar un sensor diferente?

**Sí, con modificaciones del código:**

**Sensores ultrasónicos:**
- MaxBotix MB7389 (predeterminado)
- Otros modelos MaxBotix (MB7092, MB7360, etc.)
- HC-SR04 (económico, pero menos fiable)

**Cambios de código necesarios:**
- Asignaciones de pines
- Niveles de voltaje (3.3V vs 5V)
- Protocolo de comunicación (analógico vs serial vs I2C)

**Sensores Lidar:**
- Garmin LIDAR-Lite v3 (predeterminado)
- TF-Luna (alternativa más económica)
- VL53L1X (solo corto alcance)

**Otros sensores de distancia:**
- Transductores de presión (para uso sumergido)
- Interruptores de flotador (solo niveles on/off)

**Recomendación:** Use los sensores predeterminados a menos que tenga requisitos específicos. Están bien probados y el código está listo para usar.

---

### ¿Qué tarjeta SD debo usar?

**Recomendaciones:**

**Capacidad:** 8-32GB (las tarjetas más grandes son un desperdicio de dinero, el registrador no las llenará)

**Marca:** SanDisk, Samsung, Kingston (evitar marcas genéricas, alta tasa de fallos)

**Tipo:** SD estándar o microSD con adaptador (NO SDXC)

**Formato:** FAT32 (no exFAT o NTFS)

**Cálculo de ejemplo:**
- Registro cada 15 minutos
- 100 bytes por muestra
- Muestras por día: 96
- Datos por día: 9.6 KB
- Tarjeta de 16GB: ~1000 años de datos (!!)
- Realista: 8GB es suficiente

**Evitar:**
- Tarjetas >32GB (pueden tener exFAT, no FAT32)
- Ultra alta velocidad (UHS-II) - gasto innecesario para registro
- Micro SD sin adaptador (difícil de manejar)

---

### ¿Cómo impermeabilizo el registrador?

**Opciones de carcasa:**

**1. Carcasa con clasificación IP67** (recomendado)
- Ejemplo: Hammond 1554, Bud Industries
- Tapa sellada con junta
- Prensaestopas para cables
- Costo: $15-30

**2. Impermeabilización DIY**
- Recipiente de plástico para alimentos (solo a corto plazo)
- Sellar con silicona
- Envolver en cinta adhesiva (agrega algo de protección)
- Costo: $5

**Penetraciones de cables:**
- Usar prensaestopas (mejor)
- O: Taladrar orificio, pasar cable, sellar con silicona
- Alivio de tensión: Atar un nudo dentro de la carcasa (evita el jalón)

**Prueba:**
- Sumergir la carcasa en un cubo (¡sin electrónica dentro!)
- Esperar 1 hora
- Abrir, verificar si hay entrada de agua
- Arreglar las fugas antes de desplegar

**Desecante:**
- Agregar paquete de gel de sílice dentro de la carcasa
- Absorbe la humedad (evita la condensación)
- Reemplazar si está saturado (se vuelve rosa)

Consulte la [Guía de Instalación](../../installation/mounting-guide.md) para instrucciones detalladas de impermeabilización.

---

### ¿Puedo usar baterías alcalinas en lugar de LiPo?

**No recomendado, pero posible con modificaciones:**

**Desafíos:**
1. **Voltaje:** Alcalinas = 1.5V por celda, necesita 2-3 celdas = 3-4.5V (funciona, pero el voltaje cae más rápido)
2. **Sin recarga:** Solo desechables
3. **Conector:** Necesita soporte de batería (en lugar de JST para LiPo)
4. **Cambios de código:** Calibración de lectura del voltaje de la batería

**Ventajas:**
- Ampliamente disponible
- Sin riesgo de incendio (más seguras que LiPo)
- Mayor vida útil en almacenamiento

**Recomendación:** Use 3× pilas AA de litio (no alcalinas) si debe evitar LiPo:
- Energizer Ultimate Lithium
- 1.5V × 3 = 4.5V nominal
- Mejor rendimiento en frío que las alcalinas
- Mayor duración que las alcalinas

**Cambio de código necesario:**
```cpp
// Calibración del divisor de voltaje para 3× AA (4.5V máximo en lugar de 4.2V)
float batteryVoltage = analogRead(BATTERY_PIN) * (4.5 / 1023.0) * 2.0;
```

---

## Preguntas de Telemetría

### ¿Necesito telemetría?

**Depende de su caso de uso:**

**NECESITA telemetría si:**
- El sitio es difícil/peligroso de acceder
- Se requiere monitoreo en tiempo real (alerta de inundación)
- Múltiples registradores, verificar cada uno es imprácticamente
- Desea detectar el fallo del registrador rápidamente

**NO necesita telemetría si:**
- Sitio accesible (fácil de visitar mensualmente)
- Análisis post-evento (no en tiempo real)
- Presupuesto limitado ($100 vs $200)
- Sin cobertura celular en el sitio

**Opción de compromiso:** Tarjeta SD + telemetría ocasional
- El registrador graba en SD (alta frecuencia, confiable)
- Transmitir subconjunto de datos (resumen horario)
- Datos de resolución completa en la tarjeta SD

---

### ¿Qué operadoras celulares funcionan?

**Requisitos:** Soporte LTE-M o NB-IoT

**Estados Unidos:**
- ✅ AT&T (mejor cobertura LTE-M)
- ✅ T-Mobile (LTE-M, en expansión)
- ✅ Verizon (Cat-M1)
- ❌ Sprint (fusionado con T-Mobile)

**Europa:**
- ✅ Vodafone M2M
- ✅ Telefonica
- ✅ Deutsche Telekom
- ✅ Orange

**Reino Unido:**
- ✅ EE
- ✅ Vodafone
- ✅ O2

**Global/Multi-operadora:**
- ✅ Hologram (itinerancia en múltiples operadoras)
- ✅ Twilio Super SIM
- ✅ 1NCE

**Verificar cobertura:** Use el mapa de cobertura de la operadora para LTE-M/NB-IoT (no LTE regular) en su sitio de despliegue.

---

### ¿Cuántos datos usa la telemetría?

**Uso típico:**

**Por transmisión:**
- Payload JSON: 50-100 bytes (solo datos)
- Sobrecarga HTTP: 200-500 bytes (encabezados, handshake)
- Total: ~500-600 bytes por transmisión

**Ejemplos de uso mensual:**

| Intervalo | Transmisiones/día | Datos diarios | Datos mensuales |
|-----------|-------------------|---------------|-----------------|
| Cada medición (15 min) | 96 | 58 KB | 1.7 MB |
| Cada hora | 24 | 14 KB | 430 KB |
| Cada 4 horas | 6 | 3.6 KB | 110 KB |
| Diario | 1 | 600 bytes | 18 KB |

**Plan recomendado:** 5-10 MB/mes (con amplio margen)

**Costo:** ~$2-10/mes dependiendo de la operadora y el plan
