# Guía de Batería y Alimentación

!!! abstract "Resumen"
    Comprender el comportamiento de la batería y la gestión de energía es esencial para implementaciones de campo confiables. Esta guía cubre la selección de baterías y la interpretación del voltaje.

## Descripción General del Sistema de Batería

Los registradores Riverlabs usan un sistema de doble batería para la alimentación y el cronometraje.

![Comparación de tipos de batería](../../assets/images/Battery_Types.png)
*Tamaños de batería 18650 y 14500 con opciones de química LiPo y LiFePO4*


### Alimentación Principal: Batería 18650 o 14500

Los registradores Riverlabs admiten dos tamaños de batería con dos opciones de química:

| Tipo de Batería | Química                          | Voltaje Nominal | Capacidad       | Factor de Forma  | Notas                                          |
|-----------------|----------------------------------|-----------------|-----------------|------------------|------------------------------------------------|
| **18650**       | LiPo (Polímero de Litio)         | 3.7 V           | 2000–3000 mAh   | 18 mm × 65 mm    | Más común, alta capacidad                      |
| **18650**       | LiFePO4 (Fosfato de Hierro Litio)| 3.2 V           | 1500–2000 mAh   | 18 mm × 65 mm    | Más seguro, mayor vida de ciclo, menor voltaje |
| **14500**       | LiPo (Polímero de Litio)         | 3.7 V           | 600–800 mAh     | 14 mm × 50 mm    | Más pequeño, menor capacidad                   |
| **14500**       | LiFePO4 (Fosfato de Hierro Litio)| 3.2 V           | 400–600 mAh     | 14 mm × 50 mm    | Más seguro, factor de forma más pequeño        |

**Comparación de Química:**

| Característica              | LiPo (3.7 V)                    | LiFePO4 (3.2 V)                 |
|-----------------------------|--------------------------------|----------------------------------|
| **Rango de Voltaje**        | 4.2 V (llena)–3.0 V (vacía)    | 3.65 V (llena)–2.5 V (vacía)    |
| **Densidad de Energía**     | Mayor                          | Menor                            |
| **Seguridad**               | Buena (con protección)         | Excelente (muy estable)          |
| **Vida de Ciclo**           | 300–500 ciclos                 | 2000+ ciclos                     |
| **Tolerancia de Temperatura**| -20°C a 60°C                  | -20°C a 60°C                     |
| **Costo**                   | Menor                          | Ligeramente mayor                |
| **Mejor Para**              | Máximo tiempo de funcionamiento| Fiabilidad a largo plazo, seguridad |

**Información de Seguridad Crítica:**

!!! danger "Seguridad de la Batería"
    - ⚠️ Nunca use baterías dañadas o abolladas
    - ⚠️ Verifique la polaridad antes de la inserción
    - ⚠️ No cargar por debajo de 0°C
    - ⚠️ No exponer a calor >60°C
    - ⚠️ Reemplazar si está hinchada o con fugas
    - ⚠️ Desechar correctamente (reciclaje de baterías)

### Batería de Respaldo: Celda de Moneda CR1220

**Especificaciones:**

- **Tipo:** Celda de moneda de litio CR1220 (no recargable)
- **Voltaje Nominal:** 3.0 V
- **Capacidad:** ~40 mAh
- **Propósito:** Solo para respaldo del reloj en tiempo real
- **Vida Útil:** Típicamente 5–10 años

**Función:**

- Mantiene la hora del reloj cuando se retira la batería principal
- NO alimenta el registrador o el sensor
- El registrador funciona sin ella (pero pierde la hora)
- Esencial para aplicaciones de telemetría (hora UTC)

---

## Interpretación del Voltaje de la Batería

Comprender el voltaje de la batería ayuda a predecir la vida restante e identificar problemas.

### Voltaje vs. Estado de Carga

El voltaje de la batería indica la capacidad restante, pero los rangos de voltaje difieren entre LiPo y LiFePO4:

#### Baterías LiPo (Nominal 3.7 V)

<table>
<thead>
<tr>
<th>Voltaje</th>
<th>Estado de Carga</th>
<th>Estado</th>
<th>Acción</th>
</tr>
</thead>
<tbody>
<tr style="background-color: #22c55e; color: white;">
<td><strong>4.20 V</strong></td>
<td>100%</td>
<td>Nueva/Completamente Cargada</td>
<td>Óptimo</td>
</tr>
<tr style="background-color: #4ade80; color: white;">
<td><strong>4.10 V</strong></td>
<td>~95%</td>
<td>Excelente</td>
<td>Operación normal</td>
</tr>
<tr style="background-color: #86efac;">
<td><strong>4.00 V</strong></td>
<td>~85%</td>
<td>Muy Bueno</td>
<td>Operación normal</td>
</tr>
<tr style="background-color: #bbf7d0;">
<td><strong>3.90 V</strong></td>
<td>~75%</td>
<td>Bueno</td>
<td>Operación normal</td>
</tr>
<tr style="background-color: #d9f99d;">
<td><strong>3.80 V</strong></td>
<td>~60%</td>
<td>Regular</td>
<td>Monitorear</td>
</tr>
<tr style="background-color: #fde047;">
<td><strong>3.70 V</strong></td>
<td>~45%</td>
<td>Nominal</td>
<td>Planificar reemplazo</td>
</tr>
<tr style="background-color: #fbbf24;">
<td><strong>3.60 V</strong></td>
<td>~30%</td>
<td>Bajo</td>
<td>Reemplazar pronto</td>
</tr>
<tr style="background-color: #fb923c;">
<td><strong>3.50 V</strong></td>
<td>~20%</td>
<td>Muy Bajo</td>
<td>Reemplazar inmediatamente</td>
</tr>
<tr style="background-color: #f97316; color: white;">
<td><strong>3.40 V</strong></td>
<td>~10%</td>
<td>Crítico</td>
<td><strong>Reemplazar ahora</strong></td>
</tr>
<tr style="background-color: #dc2626; color: white;">
<td><strong>3.30 V</strong></td>
<td>~5%</td>
<td>Emergencia</td>
<td><strong>Fallo inminente</strong></td>
</tr>
<tr style="background-color: #991b1b; color: white;">
<td><strong>3.00 V</strong></td>
<td>0%</td>
<td>Agotado</td>
<td>El registrador puede detenerse</td>
</tr>
<tr style="background-color: #000000; color: white;">
<td><strong>&lt;3.00 V</strong></td>
<td>Sobredescargado</td>
<td>Dañado</td>
<td>La batería puede estar arruinada</td>
</tr>
</tbody>
</table>

#### Baterías LiFePO4 (Nominal 3.2 V)

<table>
<thead>
<tr>
<th>Voltaje</th>
<th>Estado de Carga</th>
<th>Estado</th>
<th>Acción</th>
</tr>
</thead>
<tbody>
<tr style="background-color: #22c55e; color: white;">
<td><strong>3.65 V</strong></td>
<td>100%</td>
<td>Nueva/Completamente Cargada</td>
<td>Óptimo</td>
</tr>
<tr style="background-color: #4ade80; color: white;">
<td><strong>3.40 V</strong></td>
<td>~95%</td>
<td>Excelente</td>
<td>Operación normal</td>
</tr>
<tr style="background-color: #86efac;">
<td><strong>3.30 V</strong></td>
<td>~75%</td>
<td>Muy Bueno</td>
<td>Operación normal</td>
</tr>
<tr style="background-color: #bbf7d0;">
<td><strong>3.25 V</strong></td>
<td>~50%</td>
<td>Bueno</td>
<td>Operación normal</td>
</tr>
<tr style="background-color: #d9f99d;">
<td><strong>3.20 V</strong></td>
<td>~40%</td>
<td>Regular</td>
<td>Monitorear</td>
</tr>
<tr style="background-color: #fbbf24;">
<td><strong>3.10 V</strong></td>
<td>~20%</td>
<td>Bajo</td>
<td>Reemplazar pronto</td>
</tr>
<tr style="background-color: #fb923c;">
<td><strong>3.00 V</strong></td>
<td>~10%</td>
<td>Muy Bajo</td>
<td>Reemplazar inmediatamente</td>
</tr>
<tr style="background-color: #f97316; color: white;">
<td><strong>2.90 V</strong></td>
<td>~5%</td>
<td>Crítico</td>
<td><strong>Reemplazar ahora</strong></td>
</tr>
<tr style="background-color: #dc2626; color: white;">
<td><strong>2.70 V</strong></td>
<td>~2%</td>
<td>Emergencia</td>
<td><strong>Fallo inminente</strong></td>
</tr>
<tr style="background-color: #991b1b; color: white;">
<td><strong>2.50 V</strong></td>
<td>0%</td>
<td>Agotado</td>
<td>El registrador se detendrá</td>
</tr>
<tr style="background-color: #000000; color: white;">
<td><strong>&lt;2.50 V</strong></td>
<td>Sobredescargado</td>
<td>Dañado</td>
<td>La batería puede estar arruinada</td>
</tr>
</tbody>
</table>

!!! info "Curva de Descarga Más Plana"
    Las baterías LiFePO4 mantienen el voltaje de manera más consistente durante la descarga. Se mantienen alrededor de 3.2–3.3 V durante la mayor parte de su capacidad, luego caen rápidamente cuando se agotan.

### Lectura del Voltaje de la Batería

**Desde los Datos del Registrador:**

- Voltaje registrado con cada medición
- Normalmente en la última columna del archivo de datos
- Valor en milivoltios (p.ej., 3850 = 3.85 V)

**Línea de Datos de Ejemplo:**
```
2025/12/27 14:30:00, 1250, 1248, 1252, 1249, 1251, 1250, 1248, 1251, 1249, 1250, 3850
                                                                                      ^^^^
                                                                                Voltaje de la batería (mV)
```

**Con Multímetro:**

1. Configurar el multímetro en voltaje DC
2. Acceder a los terminales de la batería (puede requerir abrir el registrador)
3. Sonda roja al terminal +
4. Sonda negra al terminal -
5. Leer el voltaje (debe mostrar 3.0–4.2 V)

!!! tip "Caída de Voltaje Durante la Medición"
    El voltaje cae brevemente durante la medición activa debido al alto consumo de corriente. El voltaje registrado se mide normalmente durante el sueño (más preciso para estimar la capacidad).

!!! info "La Primera Lectura Puede Ser Baja"
    En algunos registradores, la primera lectura de voltaje después del encendido puede parecer más baja que las lecturas posteriores. Esto ocurre porque los condensadores del circuito de medición no se han cargado completamente. Las lecturas posteriores mostrarán el voltaje correcto una vez que el circuito se haya estabilizado (generalmente después de la segunda o tercera medición).

---

## Carga Solar

### Especificaciones del Panel Solar

**Configuración Solar Típica:**

- Panel: 5–10 W, 6 V
- Controlador de Carga: Compatible con Li-ion 3.7 V
- Cable: Resistente a la intemperie, con alivio de tensión
- Montaje: Ajustable para el ángulo del sol

!!! warning "Limitaciones de la Carga Solar"
    - Solo carga por encima de 0°C
    - Requiere buena exposición al sol, no adecuado para ubicaciones muy sombreadas
    - Puede no mantenerse al día con el uso de telemetría

---

## Almacenamiento de Baterías

### Almacenamiento a Largo Plazo

**Condiciones Óptimas de Almacenamiento:**

- **Voltaje:** 3.7–3.8 V para LiPo (50% de carga) o 3.2–3.3 V para LiFePO4
- **Temperatura:** 15–20°C
- **Humedad:** <60%
- **Ubicación:** Fresco, seco, alejado de objetos metálicos

**Duración del Almacenamiento:**

**Baterías LiPo:**

| Voltaje al Almacenar | Tiempo hasta Autodescarga a 3.0 V |
|----------------------|-----------------------------------|
| 4.2 V (llena)        | ~12–18 meses                      |
| 3.8 V (50%)          | 18–24 meses                       |
| 3.4 V (baja)         | 6–12 meses                        |

**Baterías LiFePO4:**

| Voltaje al Almacenar | Tiempo hasta Autodescarga a 2.5 V |
|----------------------|-----------------------------------|
| 3.65 V (llena)       | ~18–24 meses                      |
| 3.2 V (50%)          | 24–36 meses                       |
| 2.9 V (baja)         | 12–18 meses                       |

!!! warning "Verificar las Baterías Almacenadas"
    Las baterías en almacenamiento deben verificarse cada 3–6 meses.
    
    - **LiPo:** Recargar a 3.7–3.8 V si está por debajo de 3.4 V
    - **LiFePO4:** Recargar a 3.2–3.3 V si está por debajo de 2.9 V

### Eliminación

**¡Nunca tire las baterías en la basura regular!**

**Eliminación Correcta:**

1. Descargar a <3.0 V (usar en el registrador hasta agotarse)
2. Cubrir los terminales con cinta eléctrica
3. Llevar al centro de reciclaje de baterías
4. Muchos minoristas aceptan baterías 18650 para reciclaje
5. Consultar las instalaciones locales de residuos peligrosos

---

## Resolución de Problemas de Alimentación

### El Registrador No Se Enciende

**Verificar:**

1. Voltaje de la batería >3.3 V
2. Polaridad correcta de la batería
3. Contactos de la batería limpios y en contacto
4. Sin daño físico al registrador
5. Probar con una batería conocida en buen estado

### Drenaje Rápido de la Batería

**Posibles Causas:**

| Síntoma                         | Causa Probable                                        | Solución                                             |
|---------------------------------|-------------------------------------------------------|------------------------------------------------------|
| Nueva batería se agota en días  | Telemetría excesiva o fallo al entrar en modo de sueño | Reducir la frecuencia de telemetría y recargar el firmware |
| Empeoramiento gradual           | Envejecimiento de la batería                          | Reemplazar la batería                                |

### Errores en las Lecturas de Voltaje

**Lecturas de Voltaje Inconsistentes:**

- Contacto deficiente: Limpiar terminales
- Problema de código: Recargar el firmware
- Batería muriendo: Reemplazar

**Sin Voltaje Registrado:**

- Verificar el formato del archivo de datos
- Verificar la medición de voltaje en el código
- Probar ADC con multímetro

---

## Próximos Pasos

-  [Componentes Internos](internal-components.md): Comprensión del hardware de su registrador
-  [Guía de Mantenimiento](maintenance.md): Procedimientos de mantenimiento regular
-  [ThingsBoard](../../telemetry/thingsboard-configuration.md): Monitorear el voltaje de la batería de forma remota
-  [Resolución de Problemas](../troubleshooting/common-issues.md): Problemas relacionados con la alimentación

---

!!! success "Gestión de Energía Dominada"
    Con la selección correcta de batería, el monitoreo y la optimización, sus implementaciones de registrador pueden lograr una operación confiable durante semanas o meses con una sola carga de batería.
