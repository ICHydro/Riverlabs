# Guía de Mantenimiento

!!! abstract "Resumen"
    El mantenimiento regular asegura una operación confiable y a largo plazo de su registrador Riverlabs. Esta guía cubre revisiones de rutina, procedimientos de limpieza y programas de mantenimiento preventivo.

## Filosofía de Mantenimiento

**El mantenimiento preventivo es más económico que la reparación reactiva.**

Las revisiones regulares y sistemáticas detectan problemas antes de que causen pérdida de datos o fallo del equipo. Una visita de mantenimiento de 10 minutos cada 3–6 meses evita días de resolución de problemas y posibles brechas de datos.

---

## Descripción General del Programa de Mantenimiento

### Referencia Rápida

| Frecuencia      | Duración | Tareas                                                                              | Prioridad |
|-----------------|-----------|-------------------------------------------------------------------------------------|-----------|
| **Mensual**     | 1 min    | Revisión de datos: verificar datos y niveles de batería, descargar datos según sea apropiado | Media |
| **Trimestral**  | 5 min    | Inspección visual básica y limpieza externa                                         | Alta      |
| **Anual**       | 20 mins  | Limpieza externa, reemplazo de sílica y cambio de batería recomendado               | Alta      |

### Programación Flexible

Ajuste la frecuencia según:

- **Criticidad del despliegue** (investigación vs. operacional)
- **Dureza del entorno** (urbano vs. condiciones extremas)
- **Accesibilidad** (sitio de fácil vs. difícil acceso)
- **Disponibilidad de telemetría** (recuperación remota vs. manual de datos)
- **Antigüedad del registrador** (despliegue nuevo vs. establecido)

---

## Monitoreo Remoto (Trimestral)

### Si la Telemetría está Disponible

**Tiempo Requerido:**

- 1 minuto

**Tareas:**

1. **Verificar la Última Transmisión de Datos**

    - Datos recibidos en las últimas 24–48 horas
    - Sin brechas en el flujo de datos
    - Marcas de tiempo continuas y correctas

2. **Revisar el Voltaje de la Batería**

    - La tendencia del voltaje es estable/esperada
    - Sin caídas repentinas
    - Por encima de 3.5 V (planificar visita si se acerca a este nivel de agotamiento)

3. **Inspeccionar las Lecturas del Sensor**

    - Valores dentro del rango esperado
    - Sin lecturas atascadas (mismo valor repetidamente)
    - Varianza apropiada para las condiciones

4. **Verificar la Salud de la Telemetría (si aplica)**

    - Fuerza de señal adecuada
    - Conexión exitosa
    - Sin reintentos excesivos


!!! danger "Señales de Alerta que Requieren Visita al Sitio"
    - ⚠️ Sin datos durante >48 horas — probable pérdida de energía o desplazamiento del registrador.
    - ⚠️ Voltaje de la batería <3.4 V — drenaje de energía
    - ⚠️ Lecturas del sensor atascadas o erráticas
    - ⚠️ Señal de telemetría perdida

### Sin Telemetría

Si no hay telemetría, aumente la frecuencia de las visitas físicas al sitio (mensual en lugar de trimestral).

---

## Inspección del Sitio (Trimestral o Anual)

### Tiempo Requerido: 15–30 minutos

### Inspección Visual Externa

1. **Carcasa:**

    - Sin daños visibles ni grietas
    - Sellos intactos
    - Sin agua dentro de la carcasa (buscar condensación y verificar el color de la sílica interior)
    - Montaje seguro sin aflojamiento
    - Sin evidencia de manipulación o vandalismo
    - Verificar que no haya acumulación de agua en los sellos
    - Asegurar que no haya pelos o materiales de fibras en los sellos

2. **Ambiental:**

    - Vegetación despejada mantenida
    - Sin nuevas obstrucciones en el trayecto del sensor
    - Drenaje funcionando (sin acumulación de agua)
    - Sin nidos de animales/insectos visibles

3. **Sensor:**

    - Cara del sensor limpia
    - Sin telarañas o escombros
    - Sin daño físico
    - Cable seguro y sin daños


### Verificación Funcional Rápida

**Si es accesible sin abrir:**

- Actividad LED visible (cuando se presiona el botón RST)
- Sin patrones de LED de error (ver Patrones de LED)

### Descarga de Datos

Si es accesible y el tiempo lo permite:

- Descargar los datos de la tarjeta SD
- Verificar las mediciones recientes
- Verificar la integridad del archivo
- Comparar con datos remotos (si hay telemetría)

---

## Mantenimiento Anual

### Tiempo Requerido:

- 30–60 minutos

### Lista de Verificación Previa al Mantenimiento

**Traer:**

- Suministros de limpieza (ver sección de Limpieza)
- Baterías de repuesto si es necesario (cargadas)
- Destornillador de cabeza cruzada y herramientas básicas
- Multímetro
- Cable FTDI y laptop (en caso de que se necesiten actualizaciones de reloj/código — asegúrese de que Arduino y las bibliotecas requeridas estén instaladas)

### Inspección Detallada

**1. Condición Externa (10 min)**

1. **Integridad de la carcasa**

    - Sin grietas, arañazos ni daños
    - Todos los tornillos/sujetadores presentes y apretados
    - Juntas/sellos en buen estado
    - Prensaestopas apretados y sellados
    - Sin marcas de agua en el interior

2. **Seguridad del montaje**

    - Hardware de montaje apretado
    - Sin corrosión en soportes/bridas
    - Las bridas no son frágiles (reemplazar si están degradadas)
    - Sin movimiento ni oscilación
    - Alineación todavía correcta

3. **Factores ambientales**

    - Vegetación podada (zonas libres mantenidas)
    - Sin nuevas obstrucciones cercanas
    - Protector solar intacto (si está presente)
    - Disuasivos de aves funcionando
    - Sin actividad animal

**2. Inspección Interna (10 min)**

Abrir la carcasa e inspeccionar:

1. **Sin humedad en el interior**

    - Desecante seco (reemplazar si está saturado)
    - Sin condensación en el PCB
    - Sin corrosión en los componentes

2. **Condición de la batería**

    - Sin hinchamiento ni daños
    - Contactos limpios y brillantes
    - Buena conexión mecánica

3. **Tarjeta SD**

    - Correctamente asentada
    - Sin daño físico
    - Datos recientes presentes

4. **Conexiones**

    - Cable del sensor seguro
    - Sin cables deshilachados
    - Uniones de soldadura intactas
    - XBee asentado (si está presente)

5. **Verificación visual del PCB**

    - Sin marcas de quemado
    - Sin corrosión
    - Todos los componentes presentes
    - Sin piezas sueltas que vibren

**3. Prueba Funcional (10 min)**

1. **Prueba de energía**

    - El registrador se enciende
    - La secuencia de LED es normal
    - Sin indicadores de error

2. **Verificación del reloj**

    - Hora precisa (dentro de 1 minuto)
    - Fecha correcta
    - Si está desviado: Reconfigurar usando el script `set_clock.ino`

3. **Prueba del sensor**

    - Tomar varias lecturas de prueba
    - Valores razonables y consistentes
    - Sin mensajes de error
    - Desviación estándar aceptable

4. **Prueba de la tarjeta SD**

    - Tarjeta legible
    - Datos recientes presentes
    - Archivos no corruptos
    - Espacio libre adecuado

5. **Prueba de telemetría** (si aplica)

    - Actividad LED del XBee
    - Transmisión exitosa reciente
    - Fuerza de señal adecuada


**4. Gestión de Datos (5 min)**

- Descargar todos los datos de la tarjeta SD
- Verificar la integridad de los datos
- Respaldar los datos (múltiples ubicaciones)
- Opcional: Reemplazar la tarjeta SD por una nueva formateada
- Documentar la recuperación de datos en el registro

## Resolución de Problemas Durante el Mantenimiento

### Problemas Comunes Encontrados Durante la Inspección

**1. Batería Baja/Muerta**

- **Solución:** Reemplazar la batería
- **Prevención:** Monitorear la tendencia del voltaje, reemplazar de forma proactiva

**2. Tarjeta SD Llena**

- **Solución:** Descargar los datos, eliminar archivos antiguos o reemplazar la tarjeta
- **Prevención:** Rastrear la tasa de acumulación de datos

**3. Reloj Reiniciado**

- **Solución:** Reemplazar CR1220, resetear el reloj con el script `set_clock.ino`
- **Prevención:** Reemplazar CR1220 anualmente

**4. Sensor Sucio/Obstruido**

- **Solución:** Limpiar según los procedimientos anteriores
- **Prevención:** Programa de limpieza más frecuente

**5. Montaje Suelto**

- **Solución:** Apretar/reemplazar los sujetadores
- **Prevención:** Usar fijador de roscas, verificar la condición de los sujetadores

**6. Corrosión en los Contactos**

- **Solución:** Limpiar con alcohol isopropílico, aplicar DeoxIT
- **Prevención:** Mantener la carcasa sellada, usar desecante

**7. Obstrucción de Vegetación**

- **Solución:** Podar la vegetación, ampliar la zona libre
- **Prevención:** Podado más frecuente, herbicida (con cuidado)

**8. Infiltración de Agua**

- **Solución:** Secar completamente, reemplazar las juntas, resellar
- **Prevención:** Verificar los sellos regularmente, mejorar el drenaje

**9. Sin Datos Recientes**

- **Solución:** Verificar el código en ejecución, verificar la tarjeta SD, probar el sensor
- **Prevención:** Alertas de monitoreo remoto

**10. Fallo de Telemetría**

- **Solución:** Verificar XBee, tarjeta SIM, señal, configuración
- **Prevención:** Monitorear la salud de la telemetría de forma remota


## Próximos Pasos

- 📖 [Lista de Verificación del Primer Despliegue](../quick-start/first-deployment-checklist.md): Verificación de la configuración inicial
- 🔋 [Guía de Batería y Alimentación](battery-power-guide.md): Monitoreo de la salud de la batería
- 🔧 [Componentes Internos](internal-components.md): Mantenimiento específico de componentes
- 🚨 [Resolución de Problemas](../troubleshooting/common-issues.md): Solución de problemas encontrados durante el mantenimiento

---

!!! success "Registrador Bien Mantenido = Datos Confiables"
    El mantenimiento consistente y documentado es la base del monitoreo exitoso a largo plazo. Una pequeña inversión de tiempo evita grandes problemas y garantiza una recopilación de datos continua y de alta calidad.
