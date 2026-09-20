# Consideraciones Ambientales

!!! abstract "Resumen"
    Los factores ambientales afectan significativamente el rendimiento y la vida útil del registrador. Esta guía cubre consideraciones climáticas, meteorológicas, de vida silvestre y específicas del sitio para implementaciones exitosas a largo plazo.

## Clima y Condiciones Meteorológicas

### Gestión de Temperatura

#### Rangos de Operación

| Componente              | Mínimo | Máximo | Rango Óptimo   |
|-------------------------|--------|--------|----------------|
| **Electrónica/CPU**     | -20°C  | +60°C  | 0°C a +50°C    |
| **Batería (Descarga)**  | -20°C  | +60°C  | +10°C a +40°C  |
| **Batería (Carga)**     | 0°C    | +45°C  | +10°C a +35°C  |
| **Tarjeta SD**          | -25°C  | +85°C  | -10°C a +60°C  |

#### Consideraciones en Climas Fríos

**Impacto de las Bajas Temperaturas:**

- **La capacidad de la batería se reduce** significativamente
    - A 0°C: ~80% de capacidad
    - A -10°C: ~60% de capacidad
    - A -20°C: ~40% de capacidad
- Riesgo de **condensación** al calentarse

**Adaptaciones para Climas Fríos:**

❌ **No hacer:**

- Intentar cargar baterías por debajo de 0°C
- Usar carga solar en condiciones de helada
- Esperar vida útil normal de la batería

**Impacto de las Altas Temperaturas:**

- **El estrés térmico de la electrónica** reduce la vida útil
- **La degradación de la batería** se acelera
- **La ganancia solar** eleva significativamente la temperatura interna

**Adaptaciones para Climas Cálidos:**

✅ **Hacer:**

- Instalar protectores solares
- Reducir la frecuencia de registro para minimizar el tiempo activo
- Monitorear la salud de la batería con mayor frecuencia

❌ **No hacer:**

- Montar en sol directo de la tarde
- Usar carcasas de color oscuro
- Sellar completamente sin un plan de ventilación
- Ignorar el hinchamiento de la batería (señal de peligro)

**Métodos de Protección Solar:**

![Diseño de protector solar](../assets/images/sun-shield-diagram.png)<br>
*Un protector solar simple reduce la temperatura interna en 15–20°C*

1. **Protectores Solares:**
    - Lámina de aluminio o plástico blanco
    - Montar 5–10 cm por encima del registrador
    - Permitir circulación de aire por debajo
    - Inclinar para bloquear el sol de la tarde
2. **Recubrimiento Reflectante:**
    - Pintar la carcasa de blanco
    - Usar cinta reflectante
    - Envolver externamente con papel de aluminio
3. **Estructuras de Sombra:**
    - Sombra natural de árboles/estructuras
    - Toldo construido específicamente
    - Posicionar en el lado sombreado de la estructura

### Precipitación y Humedad

#### Lluvia e Infiltración de Agua

1. **Entradas de Cables**
    - Verificar que los prensaestopas estén bien apretados
    - Aplicar sellador de silicona si el montaje está hacia arriba para evitar acumulación
    - Enrutar los cables hacia abajo (bucle antigoteo)
2. **Sellos de la Carcasa**
    - Verificar que la junta esté en buen estado
    - Limpiar las superficies de sellado antes de cerrar
    - Presión uniforme en todos los sujetadores
3. **Alojamiento del Sensor**
    - Sin acumulación de agua en el sensor

**Prevención de Acumulación de Agua:**

![Diseño de drenaje](../assets/images/drainage-design.png)

- Inclinar la carcasa 5–10° para el escurrimiento
- Los cables salen por la parte inferior
- Sin superficies horizontales que recolecten agua

#### Condensación

**Causas de Condensación:**

- Ciclos de temperatura (día/noche)
- Diferencial de humedad (interior/exterior)
- Carcasa sellada que atrapa la humedad
- "Respiración" de la carcasa con cambios de presión

**Medidas Anti-Condensación:**

- **Paquetes Desecantes**
    - Renovar los paquetes de gel de sílice dentro de la carcasa
    - Reemplazar cada 3–6 meses
    - El tipo indicador muestra cuándo está saturado (normalmente amarillo fresco, verde usado)
    - Usar desecante de grado alimenticio (reutilizable)

## Factores Biológicos

### Insectos y Arañas

**Problemas Comunes:**

- Telarañas sobre el sensor/lente
- Insectos anidando en la carcasa
- Telarañas provocando lecturas falsas
- Obstrucción de los orificios de drenaje

**Prevención:**

✅ **Barreras Físicas:**

- Malla fina sobre el sensor (no debe afectar el haz)
- Vaselina alrededor de los puntos de entrada
- Programa de limpieza regular

✅ **Disuasivos Químicos:**

- Spray repelente de insectos alrededor (no sobre) el sensor
- Bolitas de algodón con aceite de menta dentro de la carcasa
- Disuasivo comercial para arañas
- Reaplicar mensualmente

**Específico para Lidar:**

- Las arañas se sienten particularmente atraídas por el lente
- Inspección semanal en temporada de alta actividad de insectos
- Cepillo suave o aire comprimido para limpiar
- Nunca tocar el lente con los dedos

### Riesgo de Inundación

**Durante Inundaciones:**

- El registrador puede quedar sumergido
- Daños por impacto de escombros
- Sedimentación del sensor
- Daños en el cable de alimentación

**Protección contra Inundaciones:**

1. **Altura de Montaje**
    - Por encima del nivel de inundación centenario si es posible
    - Documentar el historial de inundaciones del área
    - Agregar 1 m de margen de seguridad
    - Marcar la inundación máxima conocida en el soporte
2. **Protección contra Escombros**
    - Montar detrás de una estructura si es posible
    - Deflector de escombros aguas arriba
    - Jaula protectora alrededor del registrador
    - Montaje aerodinámico (mínimos puntos de enganche)
3. **Inspección Post-Inundación**
    - Verificación visual inmediata
    - Limpiar el sensor a fondo
    - Verificar infiltración de agua
    - Verificar calibración

---

## Próximos Pasos

- 🔋 [Guía de Batería y Alimentación](../operation/hardware/battery-power-guide.md): Optimizar para su clima
- 🔧 [Guía de Mantenimiento](../operation/hardware/maintenance.md): Revisiones regulares de factores ambientales
- 🚨 [Problemas Comunes](../operation/troubleshooting/common-issues.md): Problemas relacionados con el entorno
- 📞 Contacte soporte: info@riverlabs.uk

---

!!! success "Implementación Lista para el Entorno"
    Al considerar y prepararse para los factores ambientales, su implementación del registrador será robusta, confiable y duradera a pesar de las condiciones desafiantes.
