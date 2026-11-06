# Consideraciones ambientales

!!! abstract "Descripción general"
    Los factores ambientales influyen considerablemente en el rendimiento y la vida útil del registrador. Esta guía aborda el tiempo, el clima, la fauna y las características del emplazamiento para favorecer instalaciones duraderas.

## Clima y condiciones meteorológicas

### Gestión de la temperatura

#### Rangos de funcionamiento

| Componente | Mínimo | Máximo | Rango óptimo |
|------------|--------|--------|--------------|
| **Electrónica/CPU** | -20°C | +60°C | 0°C a +50°C |
| **Batería (descarga)** | -20°C | +60°C | +10°C a +40°C |
| **Batería (carga)** | 0°C | +45°C | +10°C a +35°C |
| **Tarjeta SD** | -25°C | +85°C | -10°C a +60°C |

#### Consideraciones para climas fríos

**Efectos de las bajas temperaturas:**

- **La capacidad de la batería disminuye** considerablemente
    - A 0°C: ~80 % de capacidad
    - A -10°C: ~60 % de capacidad
    - A -20°C: ~40 % de capacidad
- **Riesgo de condensación** al aumentar la temperatura

**Adaptaciones para climas fríos:**

❌ **Evitar:**

- Intentar cargar las baterías por debajo de 0°C
- Utilizar carga solar en condiciones de congelación
- Esperar una autonomía normal de la batería

**Efectos de las altas temperaturas:**

- **El estrés térmico de los componentes electrónicos** reduce su vida útil
- **La degradación de la batería** se acelera
- **La radiación solar** aumenta considerablemente la temperatura interna

**Adaptaciones para climas cálidos:**

✅ **Hacer:**

- Instalar protectores solares
- Reducir la frecuencia de registro para minimizar el tiempo de funcionamiento activo
- Comprobar el estado de la batería con mayor frecuencia

❌ **Evitar:**

- Montar el registrador bajo el sol directo de la tarde
- Usar carcasas de color oscuro
- Sellar por completo la carcasa sin prever ventilación
- Ignorar una batería hinchada (señal de peligro)

**Métodos de protección solar:**

![Diseño de protector solar](../assets/images/sun-shield-diagram.png)<br>
*Un protector solar sencillo reduce la temperatura interna entre 15 y 20°C*

1. **Protectores solares:**
    - Lámina de aluminio o plástico blanco
    - Montarla a 5–10 cm por encima del registrador
    - Permitir la circulación de aire por debajo
    - Inclinarla para bloquear el sol de la tarde
2. **Revestimiento reflectante:**
    - Pintar la carcasa de blanco
    - Usar cinta reflectante
    - Envolver el exterior con papel de aluminio
3. **Estructuras de sombra:**
    - Sombra natural de árboles o estructuras
    - Cubierta construida para este fin
    - Colocar el registrador en el lado sombreado de la estructura

### Precipitaciones y humedad

#### Lluvia y entrada de agua

1. **Entradas de cables**
    - Comprobar que los prensaestopas impermeables estén bien apretados
    - Aplicar sellador de silicona si se montan orientados hacia arriba, para evitar acumulaciones de agua
    - Guiar los cables hacia abajo (bucle de goteo)
2. **Juntas de la carcasa**
    - Comprobar que la junta esté en buen estado
    - Limpiar las superficies de sellado antes de cerrar
    - Aplicar una presión uniforme en todos los elementos de fijación
3. **Carcasa del sensor**
    - Evitar que se acumule agua sobre el sensor

**Prevención de acumulaciones de agua:**

![Diseño del drenaje](../assets/images/drainage-design.png)

- Inclinar la carcasa entre 5 y 10° para favorecer la escorrentía
- Hacer que los cables salgan por la parte inferior
- Evitar superficies horizontales donde pueda acumularse agua

#### Condensación

**Causas de la condensación:**

- Variaciones de temperatura (día/noche)
- Diferencia de humedad entre el interior y el exterior
- Humedad atrapada en una carcasa sellada
- Intercambio de aire de la carcasa debido a cambios de presión

**Medidas contra la condensación:**

- **Bolsas desecantes**
    - Renovar las bolsas de gel de sílice del interior de la carcasa
    - Sustituirlas cada 3–6 meses
    - Las bolsas con indicador muestran cuándo están saturadas (normalmente amarillo si están nuevas y verde si se han usado)
    - Usar desecante de calidad alimentaria (reutilizable)

## Factores biológicos

### Insectos y arañas

**Problemas habituales:**

- Telarañas sobre el sensor o la lente
- Insectos que anidan en la carcasa
- Telarañas que provocan lecturas falsas
- Obstrucción de los orificios de drenaje

**Prevención:**

✅ **Barreras físicas:**

- Malla fina sobre el sensor (sin interferir con el haz)
- Vaselina alrededor de los puntos de entrada
- Calendario de limpieza periódica

✅ **Repelentes químicos:**

- Pulverizar repelente de insectos alrededor del sensor (no sobre él)
- Colocar bolas de algodón con aceite de menta en la carcasa
- Usar un repelente comercial para arañas
- Volver a aplicarlo cada mes

**Específico para lidar:**

- Las arañas se sienten especialmente atraídas por la lente
- Revisarla semanalmente en épocas de abundancia de insectos
- Limpiarla con un cepillo suave o aire comprimido
- No tocar nunca la lente con los dedos

### Riesgo de inundación

**Durante una inundación:**

- El registrador puede quedar sumergido
- Los residuos arrastrados pueden causar daños por impacto
- Puede acumularse limo en el sensor
- El cable de alimentación puede sufrir daños

**Protección frente a inundaciones:**

1. **Altura de montaje**
    - Por encima del nivel de la crecida de período de retorno de 100 años, si es posible
    - Documentar el historial de inundaciones de la zona
    - Añadir un margen de seguridad de 1 m
    - Marcar en la estructura de montaje el nivel máximo de inundación conocido
2. **Protección frente a residuos arrastrados**
    - Montar el registrador detrás de una estructura, si es posible
    - Instalar un deflector aguas arriba
    - Colocar una jaula protectora alrededor del registrador
    - Usar un montaje aerodinámico (con el menor número posible de puntos donde puedan engancharse residuos)
3. **Inspección después de una inundación**
    - Realizar una inspección visual inmediata
    - Limpiar el sensor a fondo
    - Comprobar si ha entrado agua
    - Verificar la calibración

---

## Próximos pasos

- 🔋 [Guía de baterías y alimentación](../operation/hardware/battery-power-guide.md): Optimizar la alimentación según el clima
- 🔧 [Guía de mantenimiento](../operation/hardware/maintenance.md): Revisiones periódicas de los factores ambientales
- 🚨 [Problemas habituales](../operation/troubleshooting/common-issues.md): Problemas relacionados con el entorno
- 📞 Contactar con el equipo de asistencia: info@riverlabs.uk

---

!!! success "Instalación preparada para el entorno"
    Si se tienen en cuenta los factores ambientales y se adoptan las medidas necesarias, la instalación del registrador será resistente, fiable y duradera incluso en condiciones difíciles.
