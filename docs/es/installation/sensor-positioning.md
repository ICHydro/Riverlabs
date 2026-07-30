# Guía de Posicionamiento del Sensor

!!! abstract "Resumen"
    El posicionamiento adecuado del sensor es fundamental para mediciones precisas. Esta guía proporciona información detallada sobre la colocación óptima del sensor, consideraciones de ángulo y requisitos de la superficie objetivo.

## Comprensión de los Tipos de Sensor

### Características del Haz Wari (Ultrasonido)

El sensor de ultrasonido emite un haz cónico que se extiende a medida que aumenta la distancia.

**Propiedades del Haz:**

- **Frecuencia:** Pulsos ultrasónicos de 42 kHz
- **Ángulo del Haz:** Cono de aproximadamente 15°
- **Crecimiento de la Huella:** ~26 cm de diámetro por metro de distancia
- **Reflexión:** Requiere superficie reflectante dentro del cono del haz

**Distancia vs. Huella:**

| Distancia | Diámetro del Haz | Área de Cobertura |
|-----------|------------------|-------------------|
| 0.5 m     | ~13 cm           | 0.013 m²          |
| 1.0 m     | ~26 cm           | 0.053 m²          |
| 2.0 m     | ~52 cm           | 0.212 m²          |
| 3.0 m     | ~80 cm           | 0.502 m²          |
| 5.0 m     | ~130 cm          | 1.327 m²          |

![Patrón del haz de ultrasonido](../assets/images/installation_US.png)

*El haz de ultrasonido se extiende en un patrón cónico*

### Características del Haz Lidar

El sensor lidar emite un haz láser muy enfocado con una divergencia mínima.

**Propiedades del Haz:**

- **Longitud de onda:** 905 nm infrarrojo cercano
- **Divergencia del Haz:** 8 milirradianes (~0.5°)
- **Tamaño del Punto:** Crecimiento mínimo con la distancia
- **Reflexión:** Requiere reflexión difusa del objetivo

**Distancia vs. Tamaño del Punto:**

| Distancia | Diámetro del Punto | Área de Cobertura |
|-----------|--------------------|--------------------|
| 1 m       | ~0.8 cm            | 0.0005 m²          |
| 5 m       | ~4 cm              | 0.0013 m²          |
| 10 m      | ~8 cm              | 0.005 m²           |
| 20 m      | ~16 cm             | 0.020 m²           |
| 35 m      | ~28 cm             | 0.062 m²           |

![Patrón del haz Lidar](../assets/images/installation_lidar.png)

*El haz Lidar mantiene un enfoque estrecho a lo largo de la distancia*

---

## Montaje Vertical (Configuración Estándar)


**Usando un Nivel:**

1. Fijar el nivel a la carcasa del registrador
2. Ajustar hasta que la burbuja esté centrada
3. Verificar desde múltiples ángulos
4. Asegurar el hardware de montaje
5. Verificar nuevamente después de apretar

**Usando Plomada:**

1. Colgar la línea de plomada desde el punto de montaje
2. Alinear el eje del sensor con el hilo
3. Verificar la alineación desde una perspectiva de 90°
4. Apretar manteniendo la posición

!!! tip "Método de Verificación"
    Después del montaje, tomar mediciones a una distancia conocida y comparar con la medición manual. Una diferencia de >2% sugiere desalineación.

---

## Montaje Inclinado (Solo para Lidar)

### Cuándo Usar el Montaje Inclinado

Considere el montaje inclinado cuando:

- El montaje vertical es físicamente imposible
- La geometría del puente/estructura lo requiere
- Las preocupaciones de acceso/seguridad impiden la instalación vertical

### Lidar: Capacidad de Montaje Inclinado

!!! success "El Lidar Admite Montaje Inclinado"
    El Lidar puede medir con precisión hasta 40° desde la vertical con una corrección mínima necesaria.

**Impacto del Ángulo en la Precisión:**

| Ángulo desde la Vertical | Impacto en Precisión | Notas                         |
|--------------------------|----------------------|-------------------------------|
| 0°–10°                   | <0.5% de error       | Corrección insignificante     |
| 10°–20°                  | <1% de error         | Máximo recomendado            |
| 20°–30°                  | <1.5% de error       | Aceptable con corrección      |
| 30°–40°                  | <2.5% de error       | Máximo recomendado            |
| >40°                     | >3% de error         | No recomendado                |

**Pasos de Instalación:**

1. **Medir y registrar el ángulo** usando transportador o inclinómetro
2. **Marcar el ángulo en el registrador** para referencia futura
3. **Aplicar corrección de ángulo** en el procesamiento de datos:
   ```
   Distancia Vertical = Distancia Medida × cos(ángulo)
   ```
4. **Documentar a fondo** en los registros de instalación

**Ejemplo de Cálculo:**

```
Sensor a 25° de ángulo mide 10.5m
Distancia vertical = 10.5m × cos(25°)
Distancia vertical = 10.5m × 0.906
Distancia vertical = 9.51m
```

## Consideraciones sobre la Superficie Objetivo

### Superficies de Agua

La condición de la superficie del agua afecta significativamente la confiabilidad de la medición.

#### Ultrasonido en Agua

**Condiciones Excelentes:**

- Agua tranquila con ligeras ondas
- Agua ligeramente turbulenta
- Agua con carga de sedimentos
- Agua con espuma/escombros

**Condiciones Deficientes:**

- Superficie perfectamente lisa como un espejo
- Turbulencia intensa/aguas blancas
- Superficie del agua en ángulo con el haz
- Superficie cubierta por escombros flotantes

**Consejos de Optimización:**

- Tomar múltiples lecturas y promediar
- Aumentar el número de lecturas en condiciones turbulentas
- Evitar el montaje donde el viento crea patrones de olas consistentes
- Considerar el momento de medición (períodos de calma)

#### Lidar en Agua

**Condiciones Excelentes:**

- Superficie de agua ligeramente rugosa
- Agua turbia/con carga de sedimentos
- Agua con partículas en suspensión
- Espuma o escombros en la superficie

**Condiciones Deficientes:**

- Agua perfectamente clara y lisa (como un espejo)
- Agua muy limpia en condiciones tranquilas
- Agua altamente transparente
- Agua oscura sin material en suspensión

**Por qué el Agua Lisa/Clara es Difícil para el Lidar:**

- La reflexión especular (tipo espejo) refleja el haz en otra dirección
- El agua transparente permite que el haz penetre la superficie
- Requiere reflexión difusa para una medición confiable
- El agua natural generalmente tiene suficiente turbidez

**Optimización:**

- La mayoría de los ríos/arroyos naturales funcionan bien
- El agua de laboratorio/limpia puede ser problemática
- Probar en el sitio antes de la instalación permanente
- Considerar el ultrasonido para aplicaciones con agua muy limpia

### Superficies No Acuáticas

Ambos sensores pueden medir objetivos no acuáticos:

**Buenos Objetivos:**

- Concreto (acabado rugoso)
- Madera
- Suelo/tierra
- Materiales granulares
- Superficies con textura

**Objetivos Desafiantes:**

- Metal liso (reflexión especular)
- Vidrio o espejos
- Superficies altamente pulidas
- Materiales transparentes
- Materiales muy oscuros y no reflectantes

---

## Próximos Pasos

- 📐 [Guía de Montaje](mounting-guide.md): Instale su posición de sensor optimizada
- 🌡️ [Consideraciones Ambientales](environmental-considerations.md): Factores específicos del sitio
- 🔋 [Guía de Batería y Alimentación](../operation/hardware/battery-power-guide.md): Planificación de energía
- 🔧 [Mantenimiento](../operation/hardware/maintenance.md): Mantener el sensor correctamente posicionado

---

!!! success "Posicionamiento Óptimo Logrado"
    Con una atención cuidadosa al posicionamiento, los espacios libres y las consideraciones del objetivo, su registrador proporcionará mediciones precisas y confiables durante años de operación.
