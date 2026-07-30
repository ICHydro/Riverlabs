# Descarga de Datos

Esta guía le explicará cómo descargar datos de su registrador Riverlabs por primera vez.

## Antes de Comenzar

Su registrador almacena datos en una tarjeta microSD. Para descargar datos, necesitará:

- **Lector de tarjeta MicroSD** (USB o integrado en su computadora)
- Acceso al registrador para retirar la tarjeta SD
- Una computadora con software de hoja de cálculo (p.ej., Excel o Google Sheets)

## Paso 1: Retirar la Tarjeta SD de Forma Segura

!!! warning "Siempre Vacíe los Datos Antes de Retirar"
    Siga estos pasos para evitar la pérdida de datos:

    1. Presione el botón **RESET** del registrador para vaciar los datos en búfer
    2. Espere a que el LED muestre un pulso rojo (indicando transferencia de datos)
    3. Una vez que el LED se apague, los datos se han vaciado a la tarjeta microSD
    4. Empuje suavemente y suelte la tarjeta SD para expulsarla del ranura
    5. Retire la tarjeta SD cuidadosamente

!!! tip "Vaciado de Datos"
    El proceso de vaciado puede tardar varios segundos dependiendo de la cantidad de datos en búfer. Nunca retire la tarjeta SD mientras el LED esté iluminado.

## Paso 2: Insertar la Tarjeta SD en su Computadora

1. Inserte la tarjeta microSD en su lector de tarjetas
2. Conecte el lector de tarjetas a su computadora (si es externo)
3. Espere a que su computadora reconozca la tarjeta SD
4. Abra su explorador de archivos/buscador para localizar la unidad de la tarjeta SD

## Paso 3: Localizar sus Archivos de Datos

El registrador crea archivos de datos diarios con el formato de nombre: **YYYYMMDD.CSV**

Por ejemplo:

- `20260103.CSV`: Datos del 3 de enero de 2026
- `20260104.CSV`: Datos del 4 de enero de 2026

Navegue a la tarjeta SD y verá un archivo CSV por cada día que el registrador recopiló datos.

## Paso 4: Copiar los Archivos de Datos

1. Seleccione los archivos de datos que desea descargar
2. Cópielos a una carpeta en su computadora
3. Recomendamos organizar por implementación o ubicación, por ejemplo:
   ```
   Mis Documentos/
   └── Datos del Registrador/
       └── Sitio A - Enero 2026/
           ├── 20260101.CSV
           ├── 20260102.CSV
           └── 20260103.CSV
   ```

!!! tip "Mejor Práctica"
    Siempre copie los archivos en lugar de moverlos, para mantener una copia de seguridad en la tarjeta SD hasta que esté seguro de que los datos están almacenados de forma segura.

## Paso 5: Abrir y Ver sus Datos

Los archivos CSV se pueden abrir con cualquier software de hoja de cálculo:

1. Abra su aplicación de hoja de cálculo (p.ej., Excel o Google Sheets)
2. Abra el archivo CSV
3. Debería ver columnas con sus datos

### Comprensión del Formato de Datos

Cada línea representa un período de medición con las siguientes columnas:

| Columna | Contenido          | Ejemplo             | Descripción                                        |
|---------|--------------------|---------------------|----------------------------------------------------|
| 1       | Fecha/Hora         | 2026/01/03 12:00:00 | Marca de tiempo en formato YYYY/MM/DD HH:MM:SS     |
| 2–11    | Lecturas de distancia | 2215, 2214, 2214... | 10 mediciones brutas consecutivas (mm)           |
| 12      | Voltaje de la batería | 4100             | Voltaje de la batería (mV)                         |
| 13      | Temperatura        | 1950                | Temperatura del registrador en 1/100°C (1950 = 19.50°C) |

**Fila de ejemplo:**
```
2026/01/03 12:00:00, 2215, 2214, 2214, 2215, 2214, 2214, 2214, 2214, 2215, 2215, 4100, 1950
```

## Paso 6: Devolver la Tarjeta SD

Una vez que haya confirmado que sus datos se han copiado correctamente:

1. Expulse la tarjeta SD de forma segura de su computadora
2. Vuelva a insertarla en el registrador:
    - Asegure la orientación correcta (contactos hacia afuera y lejos de la batería)
    - Empuje suavemente hasta que encaje en su lugar
3. Presione el botón **RESET** para reinicializar el registrador


## Resolución de Problemas

**La tarjeta SD no es reconocida:**

- Pruebe con un lector de tarjetas diferente
- Verifique si la tarjeta necesita formatearse (FAT o FAT32)
- Verifique que la tarjeta no esté protegida contra escritura

**No hay archivos de datos en la tarjeta:**

- Es posible que el registrador no se haya inicializado correctamente
- Verifique si el registrador está tomando mediciones (el LED debería parpadear periódicamente)
- Verifique que el reloj esté configurado correctamente

**Archivos corruptos o incompletos:**

- Puede indicar que la tarjeta SD fue retirada sin vaciar los datos
- Siempre presione RESET y espere a que el LED termine antes de retirar la tarjeta
- Considere reemplazar la tarjeta SD si los problemas persisten

## Próximos Pasos

Ahora que ha descargado correctamente sus primeros datos:

- [Manejo de Datos](../hardware/data-handling.md): Gestión detallada de la tarjeta SD
- [Problemas Comunes](../troubleshooting/common-issues.md): Resolución de problemas de datos
- [Guía de Mantenimiento](../hardware/maintenance.md): Programa de mantenimiento regular
- [Configuración de ThingsBoard](../../telemetry/thingsboard-configuration.md): Configurar monitoreo remoto

## Tarjeta de Referencia Rápida

| Acción                    | Pasos                                              |
|---------------------------|----------------------------------------------------|
| **Retirar tarjeta SD**    | Presionar RESET → Esperar LED → Retirar tarjeta    |
| **Ver datos**             | Abrir archivo .CSV en Excel/Google Sheets          |
| **Columnas de datos**     | FechaHora, 10x Distancia, Voltaje, Temperatura     |
| **Reinsertar tarjeta**    | Insertar tarjeta → Presionar RESET                 |
| **Nombre del archivo**    | YYYYMMDD.CSV (p.ej., 20260103.CSV)                 |
