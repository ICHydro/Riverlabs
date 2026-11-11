# Guía de configuración de la telemetría

!!! abstract "Descripción general"
    Esta página explica de principio a fin cómo configurar la telemetría celular con módulos XBee para transmitir datos a distancia, desde la instalación del hardware hasta la primera transmisión correcta.

## Descripción general

Los registradores Riverlabs pueden incorporar telemetría celular para transmitir datos a distancia, reducir las visitas al lugar y permitir la supervisión en tiempo real.

**Necesitará:**

- Registrador Riverlabs con zócalo XBee (versión con telemetría)
- Módulo Digi XBee3 Cellular LTE-M/NB-IoT
- Tarjeta SIM activa con plan de datos
- Adaptador USB para configurar el XBee
- Programa XBee Studio (sustituye a XCTU)
- Cuenta de ThingsBoard o plataforma IoT compatible
- Ordenador para la configuración

**Tiempo necesario:** 1–2 horas la primera vez

---

## Arquitectura del sistema

### Cómo funciona la telemetría

```
┌─────────────┐
│ Registrador │ → Recoge datos del sensor
│  (Arduino)  │   Los escribe en la tarjeta SD
└──────┬──────┘
       │
       │ UART (serie)
       ↓
┌─────────────┐
│ Módulo XBee │ → Módem celular
│  (LTE-M)    │   Se conecta a la red
└──────┬──────┘
       │
       │ Red celular
       ↓
┌─────────────┐
│  Internet   │
└──────┬──────┘
       │
       │ MQTT
       ↓
┌─────────────┐
│ ThingsBoard │ → Plataforma IoT
│ o servidor  │   Almacena y muestra los datos
└─────────────┘
```

### Flujo de datos

1. **El registrador mide** los datos del sensor con la frecuencia configurada.
2. **Arduino procesa** los datos y los guarda en la tarjeta SD.
3. **Arduino envía** los datos al XBee mediante la interfaz serie.
4. **El XBee se conecta** a la red celular (LTE-M o NB-IoT).
5. **El XBee transmite** los datos al servidor configurado.
6. **El servidor recibe** y guarda los datos.
7. **El panel muestra** los datos al usuario.

---

## Descripción del módulo XBee

### XBee3 Cellular LTE-M/NB-IoT

**Especificaciones:**

| Característica | Especificación |
|----------------|----------------|
| **Tecnología** | LTE-M (Cat-M1) y NB-IoT |
| **Frecuencias** | Dependen de la región (compruebe el modelo) |
| **Alimentación** | Funcionamiento entre 3.3 y 4.3 V |
| **Consumo de corriente** | ~200 mA durante la transmisión |
| **Formato** | Montaje pasante o superficial |
| **Protocolos** | HTTP, HTTPS, CoAP, MQTT |
| **SIM** | Ranura para micro-SIM |

**Números de modelo:**

- **XB3-C-A2-UT-001**: LTE-M (EE. UU./Canadá)
- **XB3-C-A2-EU-001**: LTE-M/NB-IoT (Europa)
- **Consulte el sitio web de Digi** para su región

### XBee Studio frente a XCTU antiguo

!!! info "XBee Studio sustituye a XCTU"
    Digi ha sustituido XCTU por **XBee Studio** para los módulos XBee3. Si su documentación anterior menciona XCTU, los conceptos son los mismos, pero la interfaz ha cambiado.
    
    Descarga: [https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xbee-studio](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xbee-studio)

---

## Paso 1: Preparar la tarjeta SIM

### Elegir un proveedor de SIM

**Requisitos:**

- Buena cobertura en el lugar de instalación
- Tarifas de datos razonables

### Dimensionar el plan de datos

**Estime el consumo:**

**Datos típicos por transmisión:**

- Datos de medición: ~50–100 bytes
- Sobrecarga HTTP: ~200–500 bytes
- Total por transmisión: ~300–600 bytes
- Margen de seguridad: calcule 1 KB por transmisión

**Cálculo de datos mensuales:**

```
Ejemplo: medición cada 15 minutos y transmisión cada hora
- Mediciones al día: 96 (cada 15 min)
- Transmisiones al día: 24 (cada hora)
- Datos al día: 24 × 1 KB = 24 KB
- Datos al mes: 24 KB × 30 = 720 KB ≈ 1 MB

Plan recomendado: 5-10 MB/mes
```

**Planes habituales:**

- 1 MB/mes: ~$2–5 al mes
- 10 MB/mes: ~$5–10 al mes
- 100 MB/mes: ~$10–15 al mes

!!! tip "Empiece con poco"
    Comience con el plan más pequeño; siempre puede ampliarlo. Muchos planes IoT no cobran por exceder el límite: simplemente dejan de funcionar.

### Instalación de la tarjeta SIM

**Tipo de SIM:**

- Micro-SIM (3FF)

**Pasos:**

1. **Consiga una SIM activada** del proveedor.

2. **Anote los ajustes de APN** facilitados por el operador:
    - Nombre del APN (p. ej., `m2m.com.attwireless.com`)
    - Usuario (si se requiere)
    - Contraseña (si se requiere)

3. **Localice la ranura SIM** del módulo XBee:
    - Normalmente está en la parte inferior.
    - Puede tener una bandeja con muelle.

4. **Inserte la SIM:**
    - Respete la orientación (esquina recortada).
    - Presione hasta que encaje.
    - Compruebe que esté completamente asentada.

5. **Anote el ICCID** (número de la SIM para identificarla).

---

## Paso 2: Configurar el XBee

### Preparar el hardware

**Necesitará:**

- Módulo XBee con la SIM instalada
- Adaptador USB XBee (p. ej., placa de desarrollo Digi XBee Grove o similar)
- Cable USB
- Ordenador con XBee Studio instalado

**Conexión:**

1. Inserte el XBee en el adaptador USB.
2. Conecte el adaptador al ordenador por USB.
3. Espere a que se instalen los controladores (Windows).
4. Anote el puerto COM o el dispositivo (consulte el Administrador de dispositivos o las preferencias del sistema).

### Configuración de XBee Studio

#### Primera conexión

1. **Abra XBee Studio.**
2. **Haga clic en "Add Device".**
3. **Seleccione la conexión USB.**
4. **Elija el puerto COM correcto.**
5. **Haga clic en "Add Device".**
6. **XBee Studio leerá la configuración del módulo.**

#### Ajustes imprescindibles

Recorra las pestañas y configure lo siguiente:

**1. Ajustes de red**

| Ajuste | Parámetro | Valor | Notas |
|--------|-----------|-------|-------|
| **Red móvil** | AN (Access Point Name) | [APN de su operador] | p. ej., `m2m.com.attwireless.com` |
| **Usuario del APN** | AU | [Si se requiere] | Déjelo vacío si no hace falta |
| **Contraseña del APN** | AV | [Si se requiere] | Déjelo vacío si no hace falta |
| **Tecnología de red** | NT | LTE-M (o NB-IoT) | Debe corresponder a la SIM |

**2. Ajustes serie**

| Ajuste | Parámetro | Valor | Notas |
|--------|-----------|-------|-------|
| **Velocidad en baudios** | BD | 9600 | Debe coincidir con el código Arduino |
| **Paridad** | NB | Sin paridad | Estándar |
| **Bits de parada** | SB | 1 | Estándar |
| **Bits de datos** | -- | 8 | Predeterminado |

**3. Modo API**

| Ajuste | Parámetro | Valor | Notas |
|--------|-----------|-------|-------|
| **Activar API** | AP | 2 | Modo API con caracteres de escape |
| **Modo de salida API** | AO | 0 | Estándar |

!!! danger "El modo API es imprescindible"
    El firmware del registrador requiere **modo API 2 (con caracteres de escape)**. AP=1 (sin escapes) o AP=0 (transparente) impedirán la comunicación.

**4. Gestión de energía**

| Ajuste | Parámetro | Valor | Notas |
|--------|-----------|-------|-------|
| **Modo de suspensión** | SM | 1 | Suspensión por pin |
| **Opciones de suspensión** | SO | 0 | Estándar |

**5. Ajustes adicionales**

| Ajuste | Parámetro | Valor | Notas |
|--------|-----------|-------|-------|
| **Tiempo de espera** | RO | 3 | Tiempo de espera entre caracteres |
| **TD** | TD | 0 | Retardo de transmisión |
| **FT** | FT | 681 | Umbral de control de flujo |

#### Comprobar la configuración

1. **Haga clic en "Read" para actualizar los ajustes actuales.**
2. **Compruebe que todos los parámetros coincidan con los anteriores.**
3. **Haga clic en "Write" para guardarlos en el módulo.**
4. **Espere la confirmación.**
5. **Vuelva a hacer clic en "Read" para verificar el guardado.**

### Prueba de conexión a la red

**Pruebe la conexión celular:**

1. En XBee Studio, abra **"Cellular Network"**.
2. Haga clic en **"Connect to Network"**.
3. Espere la conexión (puede tardar 30–60 segundos).
4. Compruebe:
    - Registrado en la red: sí
    - Intensidad de señal: >-100 dBm (cuanto menos negativa, mejor)
    - Dirección IP asignada

**Solución de problemas de conexión:**

| Problema | Causa probable | Solución |
|----------|----------------|----------|
| No encuentra la red | SIM inactiva o APN incorrecto | Consulte al operador y compruebe el APN |
| Señal débil | Ubicación | Acerque el dispositivo a una ventana o sitúelo más alto |
| Tiempo de conexión agotado | Tecnología de red incompatible | Cambie entre LTE-M y NB-IoT |
| Autenticación fallida | Credenciales de APN incorrectas | Compruebe usuario y contraseña |

---

## Paso 3: Configurar el código Arduino

### Parámetros del código

La telemetría se configura en el sketch Arduino (p. ej., `wari_3G.ino`).

**Definiciones principales:**

```cpp
// Configuración de telemetría
#define TELEMETRY_INTERVAL 12 // Transmitir cada N mediciones
#define SERVER_URL "http://demo.thingsboard.io/api/v1/[YOUR_TOKEN]/telemetry"
// Ajustes XBee (deben coincidir con la configuración del XBee)
#define XBEE_BAUD 9600
```

**Adáptelos a su instalación:**

1. **TELEMETRY_INTERVAL**
    - Determina la frecuencia de transmisión.
    - Ejemplo: con mediciones cada 15 minutos, un intervalo de 4 transmite cada hora.
    - Un intervalo mayor prolonga la duración de la batería.

2. **SERVER_URL**
    - Ruta de ThingsBoard o del servidor.
    - Incluya el token de autenticación en la URL.
    - Consulte [Configuración de ThingsBoard](thingsboard-configuration.md).

3. **XBEE_BAUD**
    - Debe coincidir con BD del XBee.
    - Valor predeterminado: 9600.

### Cargar el código

1. **Conecte el registrador** con un cable FTDI.
2. **Abra el IDE de Arduino.**
3. **Cargue el sketch adecuado:**
    - `wari_3G.ino` para Wari con telemetría.
    - `wari_lidar_cellular.ino` para lidar con telemetría.
4. **Compruebe los ajustes** anteriores.
5. **Compile** y compruebe si hay errores.
6. **Cargue** el código en el registrador.
7. **Abra el monitor serie** (115200 baudios si DEBUG está activado).
8. **Compruebe los mensajes** de inicialización.

---

## Paso 4: Pruebas de integración

### Prueba en banco antes del despliegue

**Preparación:**

1. Registrador con XBee y SIM instalados.
2. Batería cargada y dispositivo encendido.
3. Monitor serie conectado (opcional, pero recomendado).
4. Cerca de una ventana para mejorar la cobertura.

**Procedimiento:**

1. **Encienda el dispositivo:**
    - El registrador arranca.
    - Inicializa el XBee.
    - El XBee se conecta a la red (observe el LED).

2. **Espere una medición:**
    - El registrador toma mediciones.
    - Las guarda en la tarjeta SD.
    - Prepara el paquete de telemetría.

3. **Observe la transmisión:**
    - Actividad del LED del XBee (si está visible).
    - Mensajes serie de depuración (si están activados):
        ```
        Connecting to network...
        Network connected
        Sending data...
        HTTP POST: 200 OK
        Transmission successful
        ```

4. **Compruebe la recepción:**
    - Revise el panel de ThingsBoard.
    - Los datos deberían aparecer en 1–2 minutos.
    - Compruebe fecha, hora y valores.

**Tiempos previstos:**

- Arranque: 5–10 segundos.
- Conexión de red: 30–60 segundos (primera vez); 5–15 segundos (posteriores).
- Transmisión: 5–10 segundos.
- Total: alrededor de 1 minuto por transmisión.

### Si fallan las pruebas

**No hay conexión de red:**

- Compruebe que la SIM esté activa.
- Compruebe la señal (pruebe cerca de una ventana o fuera).
- Revise el APN.
- Observe los patrones del LED del XBee.

**La red conecta, pero no llegan datos:**

- Compruebe la URL del servidor.
- Revise el token de autenticación.
- Busque códigos de error en el monitor serie.
- Pruebe la ruta del servidor con curl o Postman.

**Llegan datos, pero los valores son incorrectos:**

- Compruebe cómo procesa los datos el servidor.
- Revise el formato JSON.
- Compare con la tarjeta SD.

---

## Paso 5: Instalación en campo

### Lista previa al despliegue

- [ ] Prueba en banco correcta (3 o más transmisiones)
- [ ] El servidor recibe y muestra los datos correctamente
- [ ] Batería completamente cargada
- [ ] Señal aceptable en el lugar (pruebe con un teléfono)
- [ ] Intervalo de telemetría adecuado para la autonomía
- [ ] Copia en tarjeta SD operativa (si falla la telemetría)
- [ ] Antena bien sujeta y orientada

### Comprobación de la señal en el lugar

Antes de la instalación definitiva:

1. **Lleve un teléfono del mismo operador** que la SIM.
2. **Compruebe la señal** en el lugar de instalación.
3. **Tenga en cuenta la posición de la antena:**
    - Suele funcionar mejor en vertical.
    - A mayor altura, mejor señal.
    - Aléjela de estructuras metálicas.
    - Es preferible una visión despejada del cielo.

**Señal mínima requerida:**

- LTE-M/NB-IoT: -110 dBm como mínimo; -90 dBm es buena señal.
- Las señales más débiles pueden funcionar, pero agotan la batería antes.

### Verificación tras la instalación

**En la primera hora:**

- [ ] Se recibe la primera transmisión
- [ ] Los datos aparecen en el panel
- [ ] Los valores son razonables

**En las primeras 24 horas:**

- [ ] Hay transmisiones periódicas
- [ ] No hay lagunas de datos
- [ ] La tensión de la batería es estable
- [ ] La intensidad de señal es constante

**Primera semana:**

- [ ] Revise los datos cada día
- [ ] Vigile el ritmo de descarga de la batería
- [ ] Compruebe si hay fallos de transmisión
- [ ] Ajuste el intervalo si es necesario

---

## Consumo eléctrico

### Presupuesto energético de la telemetría

**Consumo del XBee:**

- Suspensión: <1 mA.
- Conectado en reposo: ~10 mA.
- Transmisión: ~200 mA (5–10 segundos).

**Efecto en la duración de la batería:**

Sin telemetría (solo SD):

- Wari: ~30–60 días (medición cada 15 min).
- Lidar: ~15–30 días (medición cada 15 min).

Con telemetría (transmisión cada hora):

- Wari: ~20–40 días (medición cada 15 min).
- Lidar: ~10–20 días (medición cada 15 min).

**Estrategias de optimización:**

1. **Reduzca las transmisiones:**
    - Mida cada 15 min y transmita cada hora.
    - Autonomía 4 veces mayor que al transmitir cada medición.

2. **Programe las transmisiones:**
    - Envíe datos una o dos veces al día.
    - Ahorra mucha energía.
    - La tarjeta SD conserva los datos con resolución completa.

3. **Transmisión adaptable:**
    - Transmita más durante los eventos.
    - Reduzca la frecuencia cuando las condiciones sean estables.
    - Requiere un código más complejo.

Consulte la [Guía de baterías y alimentación](../operation/hardware/battery-power-guide.md) para obtener cálculos detallados.

---

## Seguridad

### Seguridad de los datos

**Seguridad del transporte:**

- Use HTTPS en lugar de HTTP cuando sea posible.
- ThingsBoard admite TLS.
- Puede añadir sobrecarga de datos y procesamiento.

**Autenticación:**

- Tokens de dispositivo (ThingsBoard).
- Claves API.
- No incluya credenciales fijas en código público.

**Seguridad de la SIM:**

- Use un APN de IP fija si está disponible.
- Proteja la SIM con PIN.
- Vigile consumos de datos inusuales.

### Seguridad física

- El XBee y la antena pueden ser objeto de robo.
- Considere ocultarlos o utilizar carcasas protectoras.
- Documente los números de serie.

---

## Supervisión y mantenimiento

### Qué supervisar

**A diario (alertas automáticas):**

- [ ] Datos recibidos en las últimas 24 horas
- [ ] Tendencia de la tensión de la batería
- [ ] Intensidad de señal

**Cada semana:**

- [ ] Porcentaje de transmisiones correctas
- [ ] Calidad de los datos
- [ ] Consumo de datos de la SIM

**Cada mes:**

- [ ] Estado de la batería
- [ ] Datos restantes del plan
- [ ] Disponibilidad de actualizaciones de firmware

### Problemas habituales de telemetría

Consulte [Solución de problemas de conexión](troubleshooting-connections.md) para un diagnóstico detallado.

**Referencia rápida:**

| Síntoma | Comprobación rápida | Solución probable |
|---------|---------------------|------------------|
| Sin transmisiones | ¿SIM activa? ¿Hay señal? | Consulte al operador o cambie la ubicación |
| Transmisiones intermitentes | ¿Varía la señal? | Mejore la posición de la antena |
| Gran consumo de batería | ¿Se transmite demasiado? | Reduzca la frecuencia |
| Datos dañados | ¿Falla el procesamiento en el servidor? | Compruebe el formato JSON |

---

## Otras plataformas

Aunque esta guía se centra en ThingsBoard, los módulos XBee3 admiten diversas plataformas:

- **Servidores MQTT** (Mosquitto, HiveMQ, AWS IoT)
- **API REST HTTP** (servidores propios)
- **Servidores CoAP**
- **Azure IoT Hub**
- **AWS IoT Core**
- **Google Cloud IoT**

Los principios de configuración son similares; consulte la documentación de cada plataforma para conocer sus rutas y métodos de autenticación.

---

## Resumen de costes

**Costes únicos:**

- Módulo XBee3 Cellular: $60–80.
- Adaptador USB para configuración: $20–30.
- Activación de SIM: $0–20.

**Costes recurrentes:**

- Plan de datos: $2–15 al mes.
- Alojamiento del servidor (si no usa la versión gratuita de ThingsBoard): $0–10 al mes.

**Total del primer año:**

- Hardware: ~$100.
- Servicio: ~$50–180.
- **Total: ~$150–280.**

Compárelo con:

- Coste de las visitas al lugar (transporte y tiempo).
- Lagunas de datos debidas a visitas poco frecuentes.
- Valor de la supervisión en tiempo real.

---

## Siguientes pasos

- [Configuración de ThingsBoard](thingsboard-configuration.md): preparar el panel
- [Guía paso a paso de XBee](xbee-setup-walkthrough.md): guía detallada de XBee Studio con capturas
- [Solución de problemas de conexión](troubleshooting-connections.md): resolver problemas de telemetría

---

!!! success "¡Telemetría configurada!"
    Con la telemetría celular en funcionamiento, ya puede supervisar el registrador a distancia y en tiempo real. No tendrá que desplazarse únicamente para comprobar si funciona.
