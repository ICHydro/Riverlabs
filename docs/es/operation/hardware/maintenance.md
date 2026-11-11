# Guía de mantenimiento

El mantenimiento periódico permite que su registrador Riverlabs funcione de forma fiable a largo plazo. Esta guía ofrece recomendaciones para las revisiones habituales, los procedimientos de limpieza y la planificación del mantenimiento preventivo.

## Plan de mantenimiento general

El siguiente plan de mantenimiento es orientativo. Puede ajustarse según la importancia de los datos, la facilidad (o dificultad) de acceso al lugar, las condiciones locales (p. ej., acumulación de suciedad o riesgo de daños) y la disponibilidad de telemetría (que permite comprobar el funcionamiento y los datos a distancia):

| Frecuencia recomendada | Tareas                                                                    | Prioridad |
|------------------------|---------------------------------------------------------------------------|-----------|
| **Mensual**            | Revisar los datos y el nivel de batería; descargar los datos si procede  | Media     |
| **Trimestral**         | Inspección visual básica y limpieza exterior                             | Alta      |
| **Anual**              | Limpieza exterior; se recomienda sustituir el gel de sílice y la batería | Alta      |

## Supervisión remota

### Si hay telemetría disponible

**Tareas:**

1. **Comprobar la última transmisión de datos**

    - Se han recibido datos en las últimas 24–48 horas
    - No hay interrupciones en la serie de datos
    - Las marcas de tiempo son continuas y correctas

2. **Revisar la tensión de la batería**

    - La tendencia de la tensión es estable o la esperada
    - No hay caídas bruscas
    - Está por encima de 3.5 V (planifique una visita si se acerca a este nivel de descarga)

3. **Examinar las lecturas del sensor**

    - Los valores están dentro del intervalo esperado
    - No hay lecturas bloqueadas (el mismo valor repetido)
    - La variación es adecuada para las condiciones

4. **Comprobar el estado de la telemetría (si corresponde)**

    - La intensidad de la señal es suficiente
    - La conexión se establece correctamente
    - No hay demasiados reintentos


!!! danger "Señales de que puede ser necesaria una visita al lugar"
    - ⚠️ No se reciben datos durante más de 48 horas: posible pérdida de alimentación o desplazamiento del registrador
    - ⚠️ Tensión de la batería <3.4 V: descarga de la batería
    - ⚠️ Lecturas del sensor bloqueadas o erráticas
    - ⚠️ Pérdida de la señal de telemetría

### Sin telemetría

Si no hay telemetría, se sugiere realizar visitas presenciales más frecuentes (p. ej., mensuales).

---

## Inspección en el lugar (trimestral o anual)

### Inspección visual exterior

1. **Carcasa:**

    - Sin daños visibles ni grietas
    - Juntas intactas
    - Sin agua dentro de la carcasa (busque condensación y compruebe el color de la sílice del interior)
    - Fijación firme, sin holguras
    - Sin indicios de manipulación indebida o vandalismo
    - Compruebe que no se acumule agua sobre las juntas
    - Asegúrese de que no haya pelos ni fibras atravesando las juntas

2. **Entorno:**

    - Se mantiene despejada la vegetación
    - No hay nuevos obstáculos en la trayectoria del sensor
    - El drenaje funciona (no se acumula agua)
    - No hay nidos visibles de animales o insectos

3. **Sensor:**

    - La superficie del sensor está limpia
    - No hay telarañas ni residuos
    - No presenta daños físicos
    - El cable está bien sujeto y no presenta daños


### Comprobación funcional rápida

**Si puede hacerse sin abrir la carcasa:**

- Se observa actividad del LED (al pulsar el botón RST)
- No aparecen patrones de error en el LED (consulte Patrones de LED)

### Descarga de datos

Si se puede acceder al equipo y hay tiempo:

- Descargue los datos de la tarjeta SD
- Compruebe las mediciones recientes
- Compruebe la integridad de los archivos
- Compárelos con los datos remotos (si hay telemetría)

---

## Mantenimiento anual

### Lista de preparativos

**Lleve:**

- Material de limpieza (consulte la sección Limpieza)
- Baterías de repuesto, si son necesarias (cargadas)
- Destornillador de estrella y herramientas básicas
- Multímetro
- Cable FTDI y ordenador portátil (por si debe actualizar el reloj o el código; asegúrese de tener instalados Arduino y las bibliotecas necesarias)

### Inspección detallada

**1. Estado exterior (10 min)**

1. **Integridad de la carcasa**

    - Sin grietas, arañazos ni daños
    - Todos los tornillos y elementos de fijación están presentes y apretados
    - Las juntas están en buen estado
    - Los prensaestopas están apretados y sellados
    - No hay marcas de agua en el interior

2. **Seguridad del montaje**

    - Los elementos de fijación están apretados
    - No hay corrosión en soportes ni bridas
    - Las bridas no están quebradizas (sustitúyalas si están deterioradas)
    - No hay movimiento ni bamboleo
    - La alineación sigue siendo correcta

3. **Factores ambientales**

    - Vegetación recortada (se mantienen las zonas despejadas)
    - No hay nuevos obstáculos cercanos
    - El protector solar está intacto (si existe)
    - Los elementos disuasorios para aves funcionan
    - No hay actividad de animales

**2. Inspección interior (10 min)**

Abra la carcasa e inspeccione:

1. **Ausencia de humedad en el interior**

    - El desecante está seco (sustitúyalo si está saturado)
    - No hay condensación en la PCB
    - No hay corrosión en los componentes

2. **Estado de la batería**

    - No presenta hinchazón ni daños
    - Los contactos están limpios y brillantes
    - La conexión mecánica es buena

3. **Tarjeta SD**

    - Está insertada correctamente
    - No presenta daños físicos
    - Contiene datos recientes

4. **Conexiones**

    - El cable del sensor está bien sujeto
    - No hay cables deshilachados
    - Las soldaduras están intactas
    - El XBee está bien asentado (si existe)

5. **Inspección visual de la PCB**

    - Sin marcas de quemaduras
    - Sin corrosión
    - Todos los componentes están presentes
    - No hay piezas sueltas que hagan ruido

**3. Pruebas funcionales (10 min)**

1. **Prueba de alimentación**

    - El registrador se enciende
    - La secuencia de los LED es normal
    - No aparecen indicadores de error

2. **Comprobación del reloj**

    - La hora es correcta
    - La fecha es correcta
    - Si no lo es: vuelva a ajustarlo con el programa `set_clock.ino`

3. **Prueba del sensor**

    - Tome varias lecturas de prueba
    - Los valores son razonables y coherentes
    - No hay mensajes de error
    - La desviación estándar es aceptable

4. **Prueba de la tarjeta SD**

    - La tarjeta puede leerse
    - Hay datos recientes
    - Los archivos no están dañados
    - Hay espacio libre suficiente

5. **Prueba de telemetría** (si corresponde)

    - Actividad del LED del XBee
    - Una transmisión reciente se realizó correctamente
    - La intensidad de la señal es suficiente


**4. Gestión de datos (5 min)**

- Descargue todos los datos de la tarjeta SD
- Compruebe la integridad de los datos
- Haga copias de seguridad de los datos (en varios lugares)
- Opcional: sustituya la tarjeta SD por otra recién formateada
- Documente la recuperación de datos en el registro

## Resolución de problemas durante el mantenimiento

### Problemas frecuentes detectados en la inspección

**1. Batería baja o agotada**

- **Solución:** sustituya la batería
- **Prevención:** supervise la tendencia de la tensión y sustitúyala de forma preventiva

**2. Tarjeta SD llena**

- **Solución:** descargue los datos, elimine archivos antiguos o sustituya la tarjeta
- **Prevención:** controle la velocidad de acumulación de datos

**3. Reloj reiniciado**

- **Solución:** sustituya la CR1220 y vuelva a ajustar el reloj con el programa `set_clock.ino`
- **Prevención:** sustituya la CR1220 cada año

**4. Sensor sucio u obstruido**

- **Solución:** límpielo siguiendo los procedimientos anteriores
- **Prevención:** limpie con mayor frecuencia

**5. Montaje flojo**

- **Solución:** apriete o sustituya los elementos de fijación
- **Prevención:** utilice fijador de roscas y revise el estado de las fijaciones

**6. Corrosión en los contactos**

- **Solución:** límpielos con alcohol isopropílico y aplique DeoxIT
- **Prevención:** mantenga sellada la carcasa y utilice desecante

**7. Vegetación que obstruye**

- **Solución:** pode la vegetación y amplíe la zona despejada
- **Prevención:** pode con mayor frecuencia; use herbicida con precaución

**8. Entrada de agua**

- **Solución:** seque completamente, sustituya las juntas y vuelva a sellar
- **Prevención:** revise las juntas periódicamente y mejore el drenaje

**9. No hay datos recientes**

- **Solución:** compruebe que el código se ejecute, revise la tarjeta SD y pruebe el sensor
- **Prevención:** configure alertas de supervisión remota

**10. Fallo de telemetría**

- **Solución:** compruebe el XBee, la tarjeta SIM, la señal y la configuración
- **Prevención:** supervise a distancia el estado de la telemetría

## Próximos pasos

- 📖 [Lista de comprobación para el primer despliegue](../quick-start/first-deployment-checklist.md): Verificación de la configuración inicial
- 🔋 [Guía de baterías y alimentación](battery-power-guide.md): Supervisión del estado de la batería
- 🔧 [Componentes internos](internal-components.md): Mantenimiento específico de los componentes
- 🚨 [Resolución de problemas](../troubleshooting/common-issues.md): Solución de los problemas detectados durante el mantenimiento
