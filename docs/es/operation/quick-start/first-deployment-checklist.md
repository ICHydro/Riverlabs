# Lista de Verificación del Primer Despliegue

!!! success "Verificación Previa al Despliegue"
    Complete esta lista de verificación antes de desplegar su registrador en el campo. Tomarse el tiempo para hacerlo ahora evita despliegues fallidos y viajes desperdiciados.

## Resumen

Esta lista de verificación completa asegura que su registrador esté completamente probado y listo para el despliegue. Imprima esta página y marque los elementos a medida que los complete.

---

## Preparación del Hardware

### Instalación de la Batería

- Batería completamente cargada y polaridad verificada **antes** de la inserción
- Batería de respaldo CR1220 instalada
- Sin conexiones sueltas ni vibración

!!! tip "Verificación del Voltaje de la Batería"
    Use un multímetro para verificar el voltaje de la batería. Consulte la [Guía de Batería y Alimentación](../hardware/battery-power-guide.md) para la interpretación del voltaje.

### Tarjeta SD

El registrador viene con una tarjeta microSD de alta calidad preinstalada.

- Tarjeta MicroSD correctamente insertada en el ranura

!!! warning "No Reemplace la Tarjeta SD"
    La tarjeta microSD proporcionada ha sido probada para mayor fiabilidad. **No la reemplace** a menos que sea absolutamente necesario. Use la misma marca y especificación que la suministrada.

!!! info "Opciones de Respaldo de Datos"
    - **Memoria Flash:** Puede instalarse y usarse como respaldo adicional si es necesario
    - **EEPROM:** A veces puede recuperar datos perdidos en caso de que la tarjeta SD esté instalada incorrectamente

### Condición Física


- Carcasa intacta sin grietas ni daños
- Sellos y juntas en buen estado
- Los prensaestopas están apretados
- Sensor limpio y sin daños
- Lente/transductor del sensor libre de escombros
- Todos los tornillos y sujetadores están apretados
- Paquete de gel de sílice presente y fresco (no debe estar descolorido)

---

## Configuración del Software

### Configuración del Reloj

- Reloj en tiempo real configurado usando el script `set_clock.ino`
- Hora verificada en el Monitor Serie
- Zona horaria UTC utilizada (si la telemetría está habilitada)
- Reloj verificado preciso dentro de 1 minuto

### Script Principal

- Script correcto para el modelo de registrador cargado
    - `wari.ino` para registradores Wari Ultrasónico
    - `wari_lidar.ino` para registradores Wari Lidar
- Intervalo de medición configurado apropiadamente
  ```cpp
  #define INTERVAL 5  // Su intervalo elegido en minutos
  ```
- Número de lecturas por medición establecido
  ```cpp
  #define NREADINGS 10  // Valor típico
  ```
- Intervalo de vaciado de EEPROM configurado
  ```cpp
  #define FLUSHAFTER 288  // Lecturas antes de escritura en SD
  ```
- Modo DEBUG habilitado para pruebas iniciales
  ```cpp
  #define DEBUG  // Comentar para el despliegue
  ```

### Telemetría (Si Aplica)

- Módulo XBee instalado y seguro
- XBee configurado para modo API con escapes
- Configuración APN de red correcta para el proveedor de SIM
- Credenciales del dispositivo ThingsBoard ingresadas
- Intervalo de telemetría apropiado para la vida de la batería
- Tarjeta SIM instalada con plan de datos activo
- Transmisión de prueba exitosa

Consulte la [Guía de Configuración de Telemetría](../../telemetry/setup-guide.md) para configuración detallada.

---

## Prueba de Banco

### Encendido Inicial

- El registrador se enciende (se observa actividad del LED)
- Sin patrones de LED de error
- La salida serie muestra inicialización (si DEBUG está habilitado)
- El reloj muestra la hora correcta

### Prueba del Sensor

- Las mediciones del sensor devuelven lecturas válidas
- Las lecturas son consistentes y razonables
- La precisión verificada probando a una distancia conocida
- Múltiples lecturas muestran baja varianza

**Para Wari Ultrasónico:**

- Probar objetivos a varias distancias (0.5 m, 1 m, 2 m, etc.)
- Verificar que no hay interferencia de objetos cercanos
- Verificar que el haz tiene un camino libre

**Para Wari Lidar:**

- Probar en diferentes tipos de superficie
- Verificar mediciones de ángulo (si se usa)
- Confirmar lecturas desde 10 cm hasta el rango máximo esperado

### Registro de Datos

- Ejecutar el registrador durante un mínimo de 30 minutos
- El LED parpadea a los intervalos esperados (comparar con la salida serie cuando se toman lecturas)
- La tarjeta SD contiene el archivo de datos
- El archivo de datos es legible y está correctamente formateado
- El archivo de datos contiene marcas de tiempo correctas
- Los valores del sensor están en el rango esperado
- Sin entradas faltantes o corruptas

**Ejemplo del Formato de Datos Esperado:**
```
2025/12/27 14:30:00, 1250, 1248, 1252, 1249, 1251, 1250, 1248, 1251, 1249, 1250, 4150
```
*Muestra fecha/hora, 10 lecturas de distancia (mm) y voltaje de la batería (mV)*

### Prueba de Telemetría (Si Está Habilitada)

- Conexión inicial a la red exitosa
- Datos transmitidos al servidor
- Datos visibles en el panel de ThingsBoard
- Temporización de transmisión según la configuración
- Patrones de LED de conexión normales
- Sin intentos de conexión excesivos

## Referencia Rápida para la Resolución de Problemas

Si surgen problemas, consulte:

- [Problemas Comunes](../troubleshooting/common-issues.md): Soluciones rápidas
- [Diagramas de Diagnóstico](../troubleshooting/diagnostic-flowcharts.md): Diagnóstico sistemático
- [Preguntas Frecuentes](../troubleshooting/faq.md): Preguntas frecuentes

**Problemas Comunes Previos al Despliegue:**

| Síntoma               | Causa Probable           | Solución Rápida                              |
|-----------------------|--------------------------|----------------------------------------------|
| Sin actividad de LED  | Batería muerta/invertida | Verificar voltaje y polaridad                |
| Lecturas erráticas    | Sensor obstruido         | Limpiar sensor, verificar trayecto del haz   |
| Sin escrituras en SD  | Fallo de tarjeta SD      | Verificar tarjeta, considerar respaldo flash |
| Reloj reiniciado      | Sin batería de respaldo  | Instalar CR1220                              |
| Sin telemetría        | XBee no configurado      | Verificar modo API y configuración           |

!!! success "¡Listo para el Despliegue!"
    Si ha completado todos los elementos de esta lista de verificación, su registrador está listo para una operación de campo confiable. Guarde esta lista para referencia futura y úsela para despliegues posteriores.

## Próximos Pasos

- Proceder a la [Guía de Montaje](../../installation/mounting-guide.md)
- Configurar monitoreo en [ThingsBoard](../../telemetry/thingsboard-configuration.md)
- Revisar el [Programa de Mantenimiento](../hardware/maintenance.md)
