# Posicionamiento de sensores

!!! abstract "Descripción general"
    El posicionamiento adecuado de los sensores es fundamental para obtener mediciones precisas. Esta guía ofrece información detallada sobre la ubicación óptima de los sensores más habituales de la plataforma Riverlabs.


## Sensor ultrasónico Maxbotix

### Instalación

El sensor ultrasónico emite un haz cónico que se ensancha a medida que aumenta la distancia. Su patrón específico se describe en la [ficha técnica de Maxbotix](https://maxbotix.com/pages/hrxl-maxsonar-wr-datasheet) y alcanza una anchura de hasta 60 cm. Por ello, recomendamos mantener una distancia mínima de 50 cm entre el centro de la trayectoria del haz y cualquier objeto:

![Patrón del haz ultrasónico](../assets/images/installation_US.png)

### Consideraciones sobre la reflectividad

Las mediciones ultrasónicas de distancia se basan en el tiempo de vuelo de un pulso sónico emitido por el sensor y reflejado por el objeto de destino. Como los materiales y las formas de los objetos pueden afectar a su reflectividad, también pueden influir en la calidad de la medición. Por lo general, el agua refleja bien el sonido, pero los siguientes factores pueden afectar a las lecturas:

- Oleaje y turbulencia: no afectan a la reflexión en sí, pero las variaciones resultantes del nivel del agua sí afectan a la lectura. Tomar varias mediciones y calcular su promedio puede aumentar la precisión.
- Residuos flotantes: algunos, como los materiales blandos, pueden reducir la reflectividad del objetivo. Los fragmentos de mayor tamaño también pueden modificar la superficie reflectante y provocar errores de medición.
- Viento: puede generar olas en la superficie del agua; si es fuerte, también puede hacer que se pierdan pulsos.

## Garmin Lidarlite v3HP

### Instalación

El sensor lidar emite un haz láser infrarrojo muy concentrado, con una divergencia mínima (8 milirradianes o ~0,5°). Esto da lugar, aproximadamente, a los siguientes diámetros y áreas iluminadas:

**Distancia y tamaño de la zona iluminada:**

| Distancia | Diámetro de la zona iluminada | Área cubierta |
|-----------|-------------------------------|---------------|
| 1 m       | ~0,8 cm                       | 0,0005 m²     |
| 5 m       | ~4 cm                         | 0,0013 m²     |
| 10 m      | ~8 cm                         | 0,005 m²      |
| 20 m      | ~16 cm                        | 0,020 m²      |
| 35 m      | ~28 cm                        | 0,062 m²      |

Gracias a lo estrecho del haz, el sensor lidar también puede instalarse en ángulo. Esto reduce algo la precisión (véase más abajo), pero puede ser una ventaja práctica, por ejemplo, cuando el montaje vertical es imposible o presenta problemas de acceso o seguridad. Nuestras pruebas indican que pueden utilizarse ángulos de hasta 40° con respecto a la vertical.

![Patrón del haz lidar](../assets/images/installation_lidar.png)

**Efecto del ángulo en la precisión:**

| Ángulo respecto a la vertical | Efecto en la precisión | Notas |
|-------------------------------|------------------------|-------|
| 0°–10°                        | Error <0,5 %           | Corrección insignificante |
| 10°–20°                       | Error <1 %             | Máximo recomendado |
| 20°–30°                       | Error <1,5 %           | Aceptable con corrección |
| 30°–40°                       | Error <2,5 %           | Máximo recomendado |
| >40°                          | Error >3 %             | No recomendado |

Cuando el sensor se monta en ángulo, la distancia medida difiere de la distancia vertical necesaria para convertir la medición bruta en profundidad del agua. La distancia vertical puede calcularse con la siguiente fórmula, donde «ángulo» es el ángulo entre la trayectoria del haz y la vertical.
   ```
   Distancia vertical = Distancia medida × cos(ángulo)
   ```

**Ejemplo de cálculo:**

```
Un sensor a 25° mide 10,5 m
Distancia vertical = 10,5 m × cos(25°)
Distancia vertical = 10,5 m × 0,906
Distancia vertical = 9,51 m
```

### Consideraciones sobre la reflectividad

Las propiedades de reflectividad del lidar son muy diferentes de las de un sensor ultrasónico. En condiciones de calma absoluta, el agua pura es muy transparente a la luz infrarroja y refleja muy poco. Esto cambia cuando la superficie está agitada o el agua contiene partículas de sedimento u otros materiales que reducen su transparencia. Una superficie agitada también dispersa el haz lidar, lo que aumenta la probabilidad de que parte de la señal regrese al sensor. Esto es especialmente importante en instalaciones en ángulo, donde una superficie perfectamente lisa puede reflejar la señal lejos del sensor, como lo haría un espejo.

Por tanto, un sensor lidar funcionará mejor al aire libre cuando el viento o la corriente agiten la superficie, o cuando el agua contenga sedimentos u otros materiales que aumenten su turbidez. La siguiente tabla ofrece un resumen:

| Buenas condiciones | Condiciones desfavorables |
|--------------------|--------------------------|
| Superficie del agua ligeramente agitada | Agua perfectamente clara y lisa (como un espejo) |
| Agua turbia o cargada de sedimentos | Agua muy limpia en condiciones de calma |
| Agua con partículas en suspensión | Agua muy transparente |
| Espuma o residuos flotantes en la superficie | Agua oscura sin material en suspensión |


## Próximos pasos

- 📐 [Guía de montaje](mounting-guide.md): Instalar el sensor en la posición elegida
- 🌡️ [Consideraciones ambientales](environmental-considerations.md): Factores específicos del emplazamiento
- 🔋 [Guía de baterías y alimentación](../operation/hardware/battery-power-guide.md): Planificación de la alimentación
- 🔧 [Mantenimiento](../operation/hardware/maintenance.md): Mantener el sensor en la posición correcta
