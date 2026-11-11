# Descarga de datos

Esta guía le acompaña en el proceso de descargar los datos del registrador Riverlabs.

## Antes de empezar

El registrador almacena los datos en una tarjeta microSD. Para descargarlos, necesitará:

- **Lector de tarjetas microSD** (USB o integrado en el ordenador)
- Acceso al registrador para retirar la tarjeta SD
- Un ordenador con un programa de hojas de cálculo (por ejemplo, Excel o Google Sheets)

## Paso 1: Retire la tarjeta SD de forma segura

!!! warning "Vuelque siempre los datos antes de retirar la tarjeta"
    Siga estos pasos para evitar la pérdida de datos:

    1. Pulse el botón **RESET** del registrador para volcar los datos almacenados temporalmente
    2. Espere a que el LED emita un pulso rojo (indica la transferencia de datos)
    3. Cuando se apague el LED, los datos se habrán volcado a la tarjeta microSD
    4. Presione con cuidado la tarjeta SD y suéltela para expulsarla de la ranura
    5. Retire la tarjeta SD con cuidado

!!! tip "Volcado de datos"
    El volcado puede tardar varios segundos, según la cantidad de datos almacenados temporalmente. Nunca retire la tarjeta SD mientras el LED esté encendido.

## Paso 2: Introduzca la tarjeta SD en el ordenador

1. Introduzca la tarjeta microSD en el lector de tarjetas
2. Conecte el lector al ordenador (si es externo)
3. Espere a que el ordenador reconozca la tarjeta SD
4. Abra el explorador de archivos para localizar la unidad de la tarjeta SD

## Paso 3: Localice los archivos de datos

El registrador crea archivos de datos diarios con el formato de nombre: **YYYYMMDD.CSV**

Por ejemplo:

- `20260103.CSV`: Datos del 3 de enero de 2026
- `20260104.CSV`: Datos del 4 de enero de 2026

Al abrir la tarjeta SD, verá un archivo CSV por cada día en que el registrador haya recogido datos.

## Paso 4: Copie los archivos de datos

1. Seleccione los archivos de datos que quiera descargar
2. Cópielos a una carpeta del ordenador
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
    Copie los archivos en lugar de moverlos; así conservará una copia de respaldo en la tarjeta SD hasta asegurarse de que los datos se han guardado correctamente.

## Paso 5: Abra y consulte los datos

Los archivos CSV pueden abrirse con cualquier programa de hojas de cálculo:

1. Abra su aplicación de hojas de cálculo (por ejemplo, Excel o Google Sheets)
2. Abra el archivo CSV
3. Debería ver las columnas con sus datos

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

## Paso 6: Vuelva a colocar la tarjeta SD

Una vez que haya confirmado que los datos se copiaron correctamente:

1. Expulse la tarjeta SD del ordenador de forma segura
2. Vuelva a introducirla en el registrador:
    - Asegúrese de que esté orientada correctamente (con los contactos mirando hacia fuera y en dirección opuesta a la batería)
    - Empújela con cuidado hasta que encaje
3. Pulse el botón **RESET** para reiniciar el registrador


## Solución de problemas

**El ordenador no reconoce la tarjeta SD:**

- Pruebe con otro lector de tarjetas
- Compruebe si la tarjeta necesita formatearse (FAT o FAT32)
- Verifique que la tarjeta no esté protegida contra escritura

**No hay archivos de datos en la tarjeta:**

- Puede que el registrador no se haya iniciado correctamente
- Compruebe si está realizando mediciones (el LED debería parpadear periódicamente)
- Verifique que el reloj esté ajustado correctamente

**Archivos dañados o incompletos:**

- Puede indicar que se retiró la tarjeta SD sin volcar antes los datos
- Pulse siempre RESET y espere a que se apague el LED antes de retirar la tarjeta
- Considere sustituir la tarjeta SD si los problemas persisten

## Siguientes pasos

Ahora que ha descargado sus primeros datos:

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
