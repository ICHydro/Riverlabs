# Configuración de ThingsBoard

!!! abstract "Descripción general"
    Esta guía completa explica cómo configurar la plataforma IoT ThingsBoard para visualizar y supervisar los datos de Riverlabs, desde la creación de la cuenta hasta los paneles avanzados.

## ¿Qué es ThingsBoard?

[ThingsBoard](https://thingsboard.io/) es una plataforma IoT de código abierto para gestionar dispositivos y recopilar y visualizar datos. Es adecuada para los registradores Riverlabs porque ofrece:

- **Versión gratuita** (servidor de demostración)
- **Integración sencilla mediante API HTTP**
- **Potentes paneles de control** con widgets
- **Alertas y notificaciones**
- **Compatibilidad con varios usuarios**
- **Interfaz apta para móviles**
- **Opción de alojamiento propio** para usuarios avanzados

---

## Opciones de despliegue

### Opción 1: Servidor de demostración de ThingsBoard (recomendado para pruebas)

**Ventajas:**

- Gratuito
- No requiere configuración previa
- Adecuado para evaluar y aprender
- Incluye todas las funciones

**Inconvenientes:**

- Los datos se eliminan tras un periodo de inactividad
- Infraestructura compartida
- No se garantiza la disponibilidad
- No es adecuado para despliegues de producción

**URL:** https://demo.thingsboard.io

**Uso:** pruebas, aprendizaje y despliegues a corto plazo

### Opción 2: ThingsBoard Cloud

**Ventajas:**

- Alojamiento gestionado
- Disponibilidad garantizada
- Asistencia técnica
- Actualizaciones automáticas
- Preparado para producción

**Inconvenientes:**

- Cuotas mensuales (~$10–50+)
- Requiere suscripción

**URL:** https://thingsboard.cloud

**Uso:** seguimiento a largo plazo, despliegues críticos y varios registradores

### Opción 3: Alojamiento propio

**Ventajas:**

- Control total
- Sin cuotas mensuales tras la instalación
- Privacidad de los datos
- Configuración personalizada

**Inconvenientes:**

- Requiere conocimientos de administración de servidores
- Costes de infraestructura (AWS/DigitalOcean)
- Responsabilidad del mantenimiento
- Gestión de la seguridad

**Uso:** organizaciones con personal informático, numerosos registradores y necesidades específicas

!!! tip "Empiece por el servidor de demostración"
    Utilice el servidor de demostración para aprender ThingsBoard y probar su registrador. Migre a Cloud o a un servidor propio cuando domine la configuración.

---

## Creación de una cuenta de ThingsBoard

### Registro en el servidor de demostración

1. **Vaya a** https://demo.thingsboard.io.
2. **Haga clic en "Sign Up"** (arriba a la derecha).
3. **Rellene el formulario:**
    - Dirección de correo electrónico
    - Nombre y apellidos
    - Contraseña (elija una robusta y guárdela en un gestor de contraseñas)
4. **Revise su correo** para encontrar el enlace de activación.
5. **Haga clic en el enlace de activación.**
6. **Inicie sesión** en ThingsBoard.

!!! warning "Conservación de datos en el servidor de demostración"
    El servidor de demostración elimina periódicamente las cuentas inactivas. No lo utilice para despliegues a largo plazo sin acceder con regularidad.

### Registro en ThingsBoard Cloud

1. **Vaya a** https://thingsboard.cloud.
2. **Haga clic en "Try It Free"** o una opción equivalente.
3. **Complete el registro.**
4. **Elija un plan** (normalmente el gratuito para empezar).
5. **Confirme su correo electrónico.**
6. **Inicie sesión.**

---

## Añadir el registrador como dispositivo

### Paso 1: Crear el dispositivo

1. **Inicie sesión en ThingsBoard.**
2. **Abra "Devices"** en la barra lateral izquierda.
3. **Haga clic en "+ Add Device"** (arriba a la derecha).
4. **Rellene el formulario:**
    - **Name:** `Wari-001` (o el ID de su registrador)
    - **Label:** `Creek Monitoring - Site A` (ubicación descriptiva)
    - **Device Profile:** `Default` (o un perfil personalizado)
    - **Gateway:** déjelo desmarcado
5. **Haga clic en "Add".**
6. **El dispositivo quedará creado.**

### Paso 2: Obtener el token de acceso

**Después de crear el dispositivo:**

1. **El dispositivo aparecerá en la lista.**
2. **Haga clic en su nombre** para abrir los detalles.
3. **Abra la pestaña "Credentials".**
4. **Copie el "Access Token"** (una cadena larga, como `A1B2C3D4E5F6G7H8I9J0`).
5. **Guarde el token** de forma segura: lo necesitará en el código Arduino.

**Ejemplo de token:**
```
A1B2C3D4E5F6G7H8I9J0K1L2M3N4O5P6
```

!!! danger "Proteja el token"
    Este token autentica el registrador. Cualquiera que lo conozca podrá enviar datos como si fuera su dispositivo. No lo publique.

### Paso 3: Configurar el código Arduino

**Actualice el sketch Arduino** (p. ej., `wari_3G.ino`):

```cpp
// Antes:
#define SERVER_URL "http://demo.thingsboard.io/api/v1/PASTE_TOKEN_HERE/telemetry"

// Después (con su token):
#define SERVER_URL "http://demo.thingsboard.io/api/v1/A1B2C3D4E5F6G7H8I9J0K1L2M3N4O5P6/telemetry"
```

**Para ThingsBoard Cloud:**
```cpp
#define SERVER_URL "https://thingsboard.cloud/api/v1/YOUR_TOKEN/telemetry"
```

**Para un servidor propio:**
```cpp
#define SERVER_URL "http://your-server.com/api/v1/YOUR_TOKEN/telemetry"
```

### Paso 4: Probar la conexión

**Prueba manual con curl** (opcional):

```bash
curl -X POST \
  http://demo.thingsboard.io/api/v1/YOUR_TOKEN/telemetry \
  -H 'Content-Type: application/json' \
  -d '{"temperature":22.5, "distance_mm":1234}'
```

**Respuesta esperada:**
```json
{}
```

Una respuesta JSON vacía indica que la petición se realizó correctamente.

**En ThingsBoard:**

1. Abra el dispositivo.
2. Haga clic en la pestaña "Latest Telemetry".
3. Deberían aparecer los valores.

**Si no aparecen datos:** compruebe el token, la URL y la conexión de red.

---

## Estructura de los datos

### Formato JSON

Los registradores Riverlabs envían datos en formato JSON:

**Ejemplo de Wari (ultrasonidos):**
```json
{
  "distance_mm": 1234,
  "temperature_C": 22.5,
  "battery_V": 3.85,
  "timestamp": 1703688000
}
```

**Ejemplo de lidar:**
```json
{
  "distance_mm": 2456,
  "battery_V": 3.78,
  "signal_strength": 85,
  "timestamp": 1703688000
}
```

### Claves de telemetría

| Clave | Tipo | Descripción | Unidades |
|-------|------|-------------|----------|
| `distance_mm` | Entero | Distancia entre el sensor y el agua | Milímetros |
| `temperature_C` | Decimal | Temperatura del aire (solo Wari) | Grados Celsius |
| `battery_V` | Decimal | Tensión de la batería | Voltios |
| `signal_strength` | Entero | Calidad de la señal lidar | Escala arbitraria (0–100) |
| `timestamp` | Entero | Marca de tiempo Unix | Segundos desde la época Unix |

**ThingsBoard almacena:**

- **Telemetría:** series temporales (distancia, temperatura, etc.).
- **Atributos:** datos estáticos (modelo, ubicación y fecha de instalación).

---

## Creación de su primer panel de control

### Paso 1: Crear el panel

1. **Abra "Dashboards"** en la barra lateral.
2. **Haga clic en "+ Add Dashboard"** (arriba a la derecha).
3. **Introduzca el título:** `Logger Monitoring - Site A`.
4. **Haga clic en "Add".**
5. **Se creará un panel vacío.**

### Paso 2: Añadir widgets

**Abra el panel en modo de edición:**

1. Haga clic en el nombre del panel.
2. Haga clic en el **icono del lápiz** (arriba a la derecha).

**Añada el primer widget:**

1. **Haga clic en "+ Add Widget"** (centro o abajo a la derecha).
2. **Elija el grupo:** `Charts` (o `Gauges`, `Cards`, etc.).
3. **Elija el tipo:** `Timeseries Line Chart`.
4. **Configure la fuente de datos:**
    - **Type:** `Entity`
    - **Entity:** seleccione su dispositivo
    - **Data keys:** marque `distance_mm` y `battery_V`
5. **Configure el aspecto:**
    - **Title:** `Water Level & Battery`
    - **Time window:** últimos 7 días
    - **Line colors:** azul (distancia) y rojo (batería)
6. **Haga clic en "Add".**
7. **El widget aparecerá en el panel.**

**Cambiar tamaño y posición:**

- Arrastre el widget para moverlo.
- Arrastre los bordes o las esquinas para redimensionarlo.
- Añada más widgets.

### Paso 3: Otros widgets útiles

**1. Tarjeta de distancia actual**

- **Grupo:** `Cards`
- **Tipo:** `Simple Card`
- **Clave de datos:** `distance_mm`
- **Título:** `Current Distance`
- **Muestra:** el último valor de forma destacada

**2. Indicador de batería**

- **Grupo:** `Gauges`
- **Tipo:** `Radial Gauge`
- **Clave de datos:** `battery_V`
- **Valor mínimo:** 3.0
- **Valor máximo:** 4.2
- **Intervalos de color:**
    - 3.0–3.3: rojo (crítico)
    - 3.3–3.6: naranja (bajo)
    - 3.6–4.2: verde (bien)

**3. Gráfico de temperatura** (solo Wari)

- **Grupo:** `Charts`
- **Tipo:** `Timeseries Line Chart`
- **Clave de datos:** `temperature_C`
- **Ventana temporal:** últimas 24 horas

**4. Tarjeta de estado del dispositivo**

- **Grupo:** `Cards`
- **Tipo:** `Entity Table`
- **Muestra:** última actividad, atributos y estado

### Paso 4: Guardar el panel

1. **Haga clic en la marca de verificación** (arriba a la derecha) para salir del modo de edición.
2. **El panel se guarda** automáticamente.
3. **Comparta o guarde en favoritos** su URL para acceder rápidamente.

---

## Configuración avanzada de widgets

### Personalización de las ventanas temporales

**Ventana fija:**

- Última hora
- Últimas 24 horas
- Últimos 7 días
- Intervalo personalizado

**Ejemplo:** mostrar únicamente el horario laboral.

- Configuración: de 8:00 a. m. a 6:00 p. m.
- Útil para registradores solares.

### Añadir umbrales

**Líneas de alarma en los gráficos:**

1. Edite el widget (icono del lápiz).
2. Abra la pestaña "Advanced".
3. Añada **líneas de umbral:**
    - **Aviso de nivel alto de agua:** 500 mm (línea roja)
    - **Aviso de nivel bajo de agua:** 2000 mm (línea naranja)
    - **Batería crítica:** 3.3 V (línea roja)

### Agregación de datos

**Para intervalos largos** (meses), agregue los datos:

- **None:** mostrar todos los puntos (lento si hay muchos datos).
- **Average:** promedios diarios u horarios.
- **Min/Max:** mostrar extremos.
- **Sum:** total del periodo.

**Ejemplo:** nivel máximo diario del agua.

- Agregación: `Max`
- Intervalo: `1 day`

### Campos calculados

**Obtener valores nuevos de la telemetría:**

**Profundidad del agua** (altura de instalación menos distancia):

1. Añada el atributo del dispositivo: `install_height_mm = 3000`.
2. Utilice un widget con el cálculo:
   ```
   ${install_height_mm} - ${distance_mm}
   ```

**Porcentaje de batería:**

Estime el porcentaje a partir de la tensión:
```
(${battery_V} - 3.0) / (4.2 - 3.0) * 100
```

---

## Alarmas y notificaciones

### Creación de reglas de alarma

1. **Abra el dispositivo.**
2. **Haga clic en la pestaña "Alarms".**
3. **Haga clic en "+ Add Alarm Rule".**
4. **Configure la regla:**

**Ejemplo: alerta de batería baja**

- **Name:** `Low Battery Warning`
- **Type:** `battery_low`
- **Severity:** `Warning`
- **Condition:**
  ```
  $[battery_V] < 3.4
  ```
- **Schedule:** siempre activa
- **Action:** crear alarma

**Ejemplo: alerta de nivel alto de agua**

- **Name:** `Flood Warning`
- **Type:** `high_water`
- **Severity:** `Critical`
- **Condition:**
  ```
  $[distance_mm] < 500
  ```
  (Suponiendo que 500 mm sea el umbral de nivel alto.)

### Configuración de notificaciones

**Notificaciones por correo electrónico:**

1. **Abra "Rule Chains"** en la barra lateral.
2. **Abra "Root Rule Chain".**
3. **Añada un nodo "Send Email".**
4. **Configure SMTP:**
    - Servidor (Gmail, SendGrid, etc.)
    - Credenciales
    - Direcciones de los destinatarios
5. **Conecte los nodos de alarma** al nodo de correo.

**Notificaciones SMS** (mediante terceros):

- Integre Twilio o AWS SNS.
- Requiere ThingsBoard Cloud o alojamiento propio.
- Configúrelo en la cadena de reglas.

**Alarmas en el panel:**

- Añada el widget "Alarms" al panel.
- Muestra alarmas activas con colores según la gravedad.
- Haga clic para reconocerlas o cerrarlas.

---

## Exportación de datos

### Exportar datos históricos

1. **Abra el dispositivo.**
2. **Haga clic en la pestaña "Latest Telemetry".**
3. **Seleccione el intervalo de fechas.**
4. **Haga clic en el icono de exportación** (arriba a la derecha).
5. **Elija el formato:**
    - CSV (compatible con Excel)
    - JSON (para procesamiento)
6. **Descargue el archivo.**

**Formato CSV:**
```csv
Timestamp,distance_mm,battery_V,temperature_C
2025-12-27 10:00:00,1234,3.85,22.5
2025-12-27 10:15:00,1256,3.84,23.1
...
```

### Acceso a los datos mediante API

**Para acceso programático:**

```bash
curl -X GET \
  'http://demo.thingsboard.io/api/plugins/telemetry/DEVICE/YOUR_DEVICE_ID/values/timeseries?keys=distance_mm,battery_V&startTs=1703520000000&endTs=1703606400000' \
  -H 'X-Authorization: Bearer YOUR_JWT_TOKEN'
```

**Token JWT:** obténgalo en el perfil de usuario > ajustes de seguridad.

---

## Gestión de varios dispositivos

### Añadir varios registradores

**Repita para cada registrador:**

1. Añada el dispositivo (nombre único: `Wari-001`, `Wari-002`, etc.).
2. Obtenga el token de acceso.
3. Configure Arduino con el token correspondiente.
4. Despliéguelo.

### Grupos de dispositivos

**Organice por ubicación o tipo:**

1. **Abra "Entity Groups".**
2. **Cree el grupo:** `Creek Monitoring Sites`.
3. **Añada dispositivos** al grupo.
4. **Filtre los paneles** por grupo.

### Panel del conjunto de dispositivos

**Cree un panel general:**

1. **Añada el widget "Devices".**
2. **Entity type:** grupo de dispositivos.
3. **Seleccione el grupo:** `Creek Monitoring Sites`.
4. **Columnas:** nombre, última actividad, batería y última distancia.
5. **Reglas de color:** destaque dispositivos con batería baja o desconectados.

**Ejemplo de disposición:**

| Dispositivo | Ubicación | Última actividad | Batería | Distancia | Estado |
|-------------|-----------|------------------|---------|-----------|--------|
| Wari-001 | Sitio A | Hace 2 min | 3.85 V | 1234 mm | 🟢 Bien |
| Wari-002 | Sitio B | Hace 17 min | 3.45 V | 2456 mm | 🟡 Batería baja |
| Lidar-001 | Sitio C | Hace 3 horas | 3.12 V | -- | 🔴 Desconectado |

---

## Gestión de usuarios

### Añadir usuarios (solo Cloud o alojamiento propio)

El servidor de demostración no admite varios usuarios.

**En Cloud o en una instalación propia:**

1. **Abra "Users"** en la barra lateral.
2. **Haga clic en "+ Add User".**
3. **Rellene el formulario:**
    - Correo electrónico y nombre
    - Rol (Customer, Tenant, etc.)
4. **Asigne permisos:**
    - Solo lectura (para otras partes interesadas)
    - Acceso completo (para personal técnico)
5. **Envíe el correo** de activación.

### Compartir paneles

**Enlace público** (solo lectura):

1. Abra el panel.
2. Haga clic en el icono **"Share"** (arriba a la derecha).
3. Active **"Public"**.
4. Copie el enlace.
5. Comparta la URL con las partes interesadas.

**Ventaja:** compartir es fácil; no requiere inicio de sesión.  
**Inconveniente:** cualquier persona con el enlace puede verlo.

---

## Aplicación móvil ThingsBoard

### Instalación de la aplicación

**iOS:**

- App Store: busque "ThingsBoard Live".
- O visite https://apps.apple.com/app/thingsboard-live.

**Android:**

- Google Play: busque "ThingsBoard Live".
- O visite https://play.google.com/store/apps/details?id=org.thingsboard.demo.app.

### Configuración de la aplicación

1. **Abra la aplicación.**
2. **Elija el servidor:**
    - Demostración: `demo.thingsboard.io`
    - Cloud: `thingsboard.cloud`
    - Personalizado: URL de su servidor
3. **Inicie sesión** con sus credenciales.
4. **Los paneles** aparecerán en la pantalla de inicio.
5. **Tóquelos** para consultar los datos en tiempo real.

**Funciones móviles:**

- Ver paneles
- Consultar alarmas
- Exportar datos
- Recibir notificaciones push (si están configuradas)

---

## Solución de problemas

### No aparecen datos

**Lista de comprobación:**

1. **Compruebe que el registrador envía datos:**
    - Busque "HTTP 200 OK" en el monitor serie.
    - Compruebe que la tarjeta SD contiene datos recientes.
2. **Compruebe el token de acceso** en el código Arduino.
3. **Compruebe que la URL** corresponde a la instancia de ThingsBoard.
4. **Pruebe con curl** (petición POST manual).
5. **Compruebe la pestaña "Latest Telemetry"** del dispositivo, no solo el panel.
6. **Revise la conectividad** (¿está registrado el XBee?).

### Aparecen datos, pero el panel está vacío

1. **Revise la configuración del widget:**
    - ¿Se seleccionó el dispositivo correcto?
    - ¿Coinciden las claves con las de telemetría?
    - ¿Incluye la ventana temporal el periodo de los datos?
2. **Actualice el panel** (F5).
3. **Compruebe la consola del navegador** para detectar errores.

### Las alarmas no se activan

1. **Revise la sintaxis** de la condición de la regla.
2. **Compruebe que los valores** cumplen la condición.
3. **Verifique que la alarma anterior se haya cerrado** (una alarma ya activa no vuelve a dispararse).
4. **Pruebe con datos manuales** (POST con curl y valores que activen la alarma).

### Los datos llegan con retraso

- ThingsBoard suele procesarlos rápidamente (normalmente en menos de un segundo).
- El retraso suele deberse a la transmisión (tiempo de conexión del XBee).
- Compruebe la frecuencia de actualización automática del widget.

---

## Buenas prácticas

### Diseño del panel

**Haga lo siguiente:**

- Utilice títulos claros para los widgets.
- Destaque las métricas principales.
- Asigne colores según la gravedad (rojo = problema, verde = correcto).
- Indique cuándo se actualizaron los datos por última vez.
- Agrupe datos relacionados.

**Evite:**

- Saturar el panel con demasiados widgets.
- Utilizar escalas engañosas.
- Olvidar a los usuarios móviles (pruebe en un teléfono).

### Gestión de datos

- **Exporte datos periódicamente** como copia de seguridad ante problemas con la cuenta.
- **Documente los atributos del dispositivo** (ubicación, altura de instalación, etc.).
- **Use nombres coherentes** (`Wari-001`, no "logger at creek").
- **Revise las alarmas semanalmente** y ajuste los umbrales según la experiencia.

### Seguridad

- **Utilice HTTPS** en ThingsBoard Cloud o en una instalación propia.
- **Rote los tokens de acceso** si se ven comprometidos.
- **Limite los paneles públicos** a datos no sensibles.
- **Utilice contraseñas robustas.**
- **Active 2FA** si está disponible (Cloud o alojamiento propio).

---

## Costes

### Servidor de demostración

- **Coste:** gratuito.
- **Límites:** conservación de datos, ausencia de SLA y eliminación periódica.
- **Adecuado para:** pruebas y formación.

### ThingsBoard Cloud

- **Plan gratuito:**
    - 1 dispositivo
    - Número limitado de paneles
    - Soporte comunitario
- **Planes de pago:** ~$10–50+ al mes.
    - Más dispositivos
    - Funciones avanzadas
    - Asistencia técnica
    - SLA

### Alojamiento propio
- **Infraestructura:** $5–20+ al mes (DigitalOcean, AWS).
- **Tiempo de instalación:** 4–8 horas (primera vez).
- **Mantenimiento:** continuo.
- **Adecuado para:** organizaciones con recursos informáticos.

---

## Alternativas a ThingsBoard

Si ThingsBoard no responde a sus necesidades:

**Código abierto:**

- **Grafana + InfluxDB:** popular y potente, pero difícil de aprender.
- **Node-RED:** programación visual de flujos, flexible.
- **Emoncms:** centrado en la supervisión energética.

**Comerciales:**

- **Losant:** plataforma IoT empresarial.
- **Ubidots:** fácil de usar; precio moderado.
- **TagoIO:** adecuada para prototipos.

**Desarrollo propio:**

- Servidor personalizado con PHP/Python + MySQL.
- Google Sheets (mediante IFTTT o un script personalizado).

---

## Siguientes pasos

- [Guía paso a paso de XBee](xbee-setup-walkthrough.md): configuración detallada del XBee
- [Solución de problemas de conexión](troubleshooting-connections.md): resolver problemas de telemetría
- [Documentación de ThingsBoard](https://thingsboard.io/docs/): documentación oficial sobre funciones avanzadas
- [Guía de baterías y alimentación](../operation/hardware/battery-power-guide.md): supervisar el estado de la batería

---

!!! success "¡Panel listo!"
    El panel de ThingsBoard ya está configurado y muestra datos en tiempo real. Compruébelo cada día durante la primera semana para asegurarse de que funciona de forma estable.
