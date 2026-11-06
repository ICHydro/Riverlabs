# Descarga de datos

Esta guía te acompaña en el proceso de descargar por primera vez los datos de tu registrador Riverlabs.

## Antes de empezar

El registrador almacena los datos en una tarjeta microSD. Para descargarlos, necesitarás:

- **Lector de tarjetas microSD** (USB o integrado en el ordenador)
- Acceso al registrador para retirar la tarjeta SD
- Un ordenador con un programa de hojas de cálculo (por ejemplo, Excel o Google Sheets)

## Paso 1: Retira la tarjeta SD de forma segura

!!! warning "Vuelca siempre los datos antes de retirar la tarjeta"
    Sigue estos pasos para evitar la pérdida de datos:

    1. Pulsa el botón **RESET** del registrador para volcar los datos almacenados temporalmente
    2. Espera a que el LED emita un pulso rojo (indica la transferencia de datos)
    3. Cuando se apague el LED, los datos se habrán volcado a la tarjeta microSD
    4. Presiona con cuidado la tarjeta SD y suéltala para expulsarla de la ranura
    5. Retira la tarjeta SD con cuidado

!!! tip "Volcado de datos"
    El volcado puede tardar varios segundos, según la cantidad de datos almacenados temporalmente. Nunca retires la tarjeta SD mientras el LED esté encendido.

## Paso 2: Introduce la tarjeta SD en el ordenador

1. Introduce la tarjeta microSD en el lector de tarjetas
2. Conecta el lector al ordenador (si es externo)
3. Espera a que el ordenador reconozca la tarjeta SD
4. Abre el explorador de archivos para localizar la unidad de la tarjeta SD

## Paso 3: Localiza los archivos de datos

El registrador crea archivos de datos diarios con el formato de nombre: **YYYYMMDD.CSV**

Por ejemplo:

- `20260103.CSV`: Datos del 3 de enero de 2026
- `20260104.CSV`: Datos del 4 de enero de 2026

Al abrir la tarjeta SD, verás un archivo CSV por cada día en que el registrador haya recogido datos.

## Paso 4: Copia los archivos de datos

1. Selecciona los archivos de datos que quieras descargar
2. Cópialos a una carpeta del ordenador
3. Recomendamos organizarlos por instalación o ubicación, por ejemplo:
   ```
   My Documents/
   └── Logger Data/
       └── Site A - Jan 2026/
           ├── 20260101.CSV
           ├── 20260102.CSV
           └── 20260103.CSV
   ```

!!! tip "Buena práctica"
    Copia los archivos en lugar de moverlos; así conservarás una copia de respaldo en la tarjeta SD hasta asegurarte de que los datos se han guardado correctamente.

## Paso 5: Abre y consulta los datos

Los archivos CSV pueden abrirse con cualquier programa de hojas de cálculo:

1. Abre tu aplicación de hojas de cálculo (por ejemplo, Excel o Google Sheets)
2. Abre el archivo CSV
3. Deberías ver las columnas con tus datos

### Cómo interpretar el formato de datos

Cada línea representa un período de medición y contiene las siguientes columnas:

| Columna | Contenido             | Ejemplo             | Descripción                                             |
|---------|-----------------------|---------------------|---------------------------------------------------------|
| 1       | Fecha/hora            | 2026/01/03 12:00:00 | Marca de tiempo en formato YYYY/MM/DD HH:MM:SS           |
| 2–11    | Lecturas de distancia | 2215, 2214, 2214... | 10 mediciones consecutivas sin procesar (mm)            |
| 12      | Tensión de la batería | 4100                | Tensión de la batería (mV)                               |
| 13      | Temperatura           | 1950                | Temperatura del registrador en 1/100°C (1950 = 19.50°C)  |

**Ejemplo de fila:**
```
2026/01/03 12:00:00, 2215, 2214, 2214, 2215, 2214, 2214, 2214, 2214, 2215, 2215, 4100, 1950
```

## Paso 6: Vuelve a colocar la tarjeta SD

Una vez que hayas confirmado que los datos se copiaron correctamente:

1. Expulsa la tarjeta SD del ordenador de forma segura
2. Vuelve a introducirla en el registrador:
    - Asegúrate de que esté orientada correctamente (con los contactos mirando hacia fuera y en dirección opuesta a la batería)
    - Empújala con cuidado hasta que encaje
3. Pulsa el botón **RESET** para reiniciar el registrador


## Solución de problemas

**El ordenador no reconoce la tarjeta SD:**

- Prueba con otro lector de tarjetas
- Comprueba si la tarjeta necesita formatearse (FAT o FAT32)
- Verifica que la tarjeta no esté protegida contra escritura

**No hay archivos de datos en la tarjeta:**

- Puede que el registrador no se haya iniciado correctamente
- Comprueba si está realizando mediciones (el LED debería parpadear periódicamente)
- Verifica que el reloj esté ajustado correctamente

**Archivos dañados o incompletos:**

- Puede indicar que se retiró la tarjeta SD sin volcar antes los datos
- Pulsa siempre RESET y espera a que se apague el LED antes de retirar la tarjeta
- Considera sustituir la tarjeta SD si los problemas persisten

## Siguientes pasos

Ahora que has descargado tus primeros datos:

- [Gestión de datos](../hardware/data-handling.md): administración detallada de la tarjeta SD
- [Problemas comunes](../troubleshooting/common-issues.md): solución de problemas relacionados con los datos
- [Guía de mantenimiento](../hardware/maintenance.md): programa de mantenimiento regular
- [Configuración de ThingsBoard](../../telemetry/thingsboard-configuration.md): configuración de la supervisión remota

## Tarjeta de referencia rápida

| Acción                         | Pasos                                             |
|--------------------------------|---------------------------------------------------|
| **Retirar la tarjeta SD**      | Pulsar RESET → Esperar al LED → Retirar la tarjeta |
| **Consultar los datos**        | Abrir un archivo .CSV en Excel/Google Sheets      |
| **Columnas de datos**          | FechaHora, 10x Distancia, Tensión, Temperatura    |
| **Volver a insertar la tarjeta** | Insertar la tarjeta → Pulsar RESET              |
| **Nombre de archivo**          | YYYYMMDD.CSV (p. ej., 20260103.CSV)               |
