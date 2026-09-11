# Configuración de ThingsBoard

!!! abstract "Resumen"
    Esta es una guía completa para configurar la plataforma IoT ThingsBoard para la visualización y el monitoreo de datos de Riverlabs, desde la creación de la cuenta hasta los paneles avanzados.

## ¿Qué es ThingsBoard?

[ThingsBoard](https://thingsboard.io/) es una plataforma IoT de código abierto para la gestión de dispositivos, la recopilación de datos y la visualización. Es ideal para los registradores Riverlabs porque:

- **Nivel gratuito disponible** (servidor demo)
- **API HTTP fácil** de integrar
- **Paneles potentes** con widgets
- **Alertas y notificaciones**
- **Soporte multi-usuario**
- **Compatible con dispositivos móviles**
- **Opción de auto-alojamiento** para usuarios avanzados

---

## Opciones de Despliegue

### Opción 1: Servidor Demo de ThingsBoard (Recomendado para Pruebas)

**Ventajas:**

- Gratuito
- Sin configuración necesaria
- Bueno para evaluación y aprendizaje
- Incluye todas las funciones

**Desventajas:**

- Los datos se eliminan después de la inactividad
- Infraestructura compartida
- Sin tiempo de actividad garantizado
- No para despliegues de producción

**URL:** https://demo.thingsboard.io

**Caso de Uso:** Pruebas, aprendizaje y despliegues a corto plazo

### Opción 2: ThingsBoard Cloud

**Ventajas:**

- Hosting gestionado
- Tiempo de actividad garantizado
- Soporte técnico
- Actualizaciones automáticas
- Listo para producción

**Desventajas:**

- Cuotas mensuales (~$10–50+)
- Se requiere suscripción

**URL:** https://thingsboard.cloud

**Caso de Uso:** Monitoreo a largo plazo, despliegues críticos y múltiples registradores

### Opción 3: Auto-Alojado

**Ventajas:**

- Control total
- Sin cuotas mensuales (después de la configuración)
- Privacidad de datos
- Configuración personalizada

**Desventajas:**

- Requiere habilidades de gestión de servidores
- Costos de infraestructura (AWS/DigitalOcean)
- Responsabilidad de mantenimiento
- Gestión de seguridad

**Caso de Uso:** Organizaciones con personal de TI, muchos registradores y requisitos personalizados

!!! tip "Comenzar con el Servidor Demo"
    Use el servidor demo para aprender ThingsBoard y probar su registrador. Migre a Cloud o auto-alojado una vez que esté seguro de la configuración.

---

## Creación de una Cuenta en ThingsBoard

### Registro en el Servidor Demo

1. **Navegar a** https://demo.thingsboard.io
2. **Hacer clic en "Sign Up"** (parte superior derecha)
3. **Completar el formulario de registro:**
    - Dirección de correo electrónico
    - Nombre y apellido
    - Contraseña (use una contraseña segura y guárdela en un gestor de contraseñas)
4. **Revisar el correo electrónico** para el enlace de activación
5. **Hacer clic en el enlace de activación**
6. **Iniciar sesión** en ThingsBoard

!!! warning "Retención de Datos del Servidor Demo"
    El servidor demo elimina periódicamente las cuentas inactivas. No usar para despliegues a largo plazo sin acceso regular.

### Registro en ThingsBoard Cloud

1. **Navegar a** https://thingsboard.cloud
2. **Hacer clic en "Try It Free"** o similar
3. **Completar el registro**
4. **Seleccionar el plan** (normalmente el nivel gratuito para empezar)
5. **Confirmar el correo electrónico**
6. **Iniciar sesión**

---

## Agregar su Registrador como Dispositivo

### Paso 1: Crear Dispositivo

1. **Iniciar sesión en ThingsBoard**
2. **Navegar a "Devices"** (barra lateral izquierda)
3. **Hacer clic en "+ Add Device"** (parte superior derecha)
4. **Completar el formulario del dispositivo:**
    - **Name:** `Wari-001` (o su ID de registrador)
    - **Label:** `Creek Monitoring - Site A` (ubicación descriptiva)
    - **Device Profile:** `Default` (o perfil personalizado)
    - **Gateway:** Dejar sin marcar
5. **Hacer clic en "Add"**
6. **¡Dispositivo creado!**

### Paso 2: Obtener el Token de Acceso

**Después de crear el dispositivo:**

1. **El dispositivo aparece en la lista**
2. **Hacer clic en el nombre del dispositivo** para abrir los detalles
3. **Navegar a la pestaña "Credentials"**
4. **Copiar el "Access Token"** (cadena larga como `A1B2C3D4E5F6G7H8I9J0`)
5. **Guardar el token** de forma segura (lo necesitará para el código de Arduino)

**Token de Ejemplo:**
```
A1B2C3D4E5F6G7H8I9J0K1L2M3N4O5P6
```

!!! danger "Mantener el Token Seguro"
    Este token autentica su registrador. Cualquiera que lo tenga puede enviar datos como su dispositivo. ¡No compartir públicamente!

### Paso 3: Configurar el Código de Arduino

**Actualizar su sketch de Arduino** (p.ej., `wari_3G.ino`):

```cpp
// Anterior:
#define SERVER_URL "http://demo.thingsboard.io/api/v1/PASTE_TOKEN_HERE/telemetry"

// Nuevo (con su token):
#define SERVER_URL "http://demo.thingsboard.io/api/v1/A1B2C3D4E5F6G7H8I9J0K1L2M3N4O5P6/telemetry"
```

**Para ThingsBoard Cloud:**
```cpp
#define SERVER_URL "https://thingsboard.cloud/api/v1/SU_TOKEN/telemetry"
```

**Para Auto-Alojado:**
```cpp
#define SERVER_URL "http://su-servidor.com/api/v1/SU_TOKEN/telemetry"
```

### Paso 4: Probar la Conexión

**Prueba Manual con curl** (opcional):

```bash
curl -X POST \
  http://demo.thingsboard.io/api/v1/SU_TOKEN/telemetry \
  -H 'Content-Type: application/json' \
  -d '{"temperature":22.5, "distance_mm":1234}'
```

**Respuesta esperada:**
```json
{}
```

¡Respuesta JSON vacía = éxito!

**En ThingsBoard:**

1. Navegar al dispositivo
2. Hacer clic en la pestaña "Latest Telemetry"
3. Los valores deben aparecer

**Si no aparecen datos:** Verificar el token, la URL y la conectividad de red.

---

## Comprensión de la Estructura de Datos

### Formato JSON

Los registradores Riverlabs envían datos en formato JSON:

**Ejemplo Wari (Ultrasónico):**
```json
{
  "distance_mm": 1234,
  "temperature_C": 22.5,
  "battery_V": 3.85,
  "timestamp": 1703688000
}
```

**Ejemplo Lidar:**
```json
{
  "distance_mm": 2456,
  "battery_V": 3.78,
  "signal_strength": 85,
  "timestamp": 1703688000
}
```

### Claves de Telemetría

| Clave             | Tipo    | Descripción                     | Unidades              |
|-------------------|---------|---------------------------------|-----------------------|
| `distance_mm`     | Entero  | Distancia sensor-agua           | Milímetros            |
| `temperature_C`   | Flotante| Temperatura del aire (solo Wari)| Celsius               |
| `battery_V`       | Flotante| Voltaje de la batería           | Voltios               |
| `signal_strength` | Entero  | Calidad de señal lidar          | Arbitrario (0–100)    |
| `timestamp`       | Entero  | Marca de tiempo Unix            | Segundos desde epoch  |

**ThingsBoard almacena:**

- **Telemetría:** Datos de series temporales (p.ej., distancia, temperatura, etc.)
- **Atributos:** Datos estáticos (modelo del dispositivo, ubicación y fecha de instalación)

---

## Creación de su Primer Panel

### Paso 1: Crear Panel

1. **Navegar a "Dashboards"** (barra lateral izquierda)
2. **Hacer clic en "+ Add Dashboard"** (parte superior derecha)
3. **Ingresar el título del panel:** `Logger Monitoring - Site A`
4. **Hacer clic en "Add"**
5. **Panel creado** (actualmente vacío)

### Paso 2: Agregar Widgets

**Abrir el panel en modo de edición:**

1. Hacer clic en el nombre del panel para abrirlo
2. Hacer clic en el **icono de lápiz** (modo de edición, parte superior derecha)

**Agregar su primer widget:**

1. **Hacer clic en "+ Add Widget"** (centro o parte inferior derecha)
2. **Seleccionar el paquete de widgets:** `Charts` (o `Gauges`, `Cards`, etc.)
3. **Seleccionar el tipo de widget:** `Timeseries Line Chart`
4. **Configurar la fuente de datos:**
    - **Type:** `Entity`
    - **Entity:** Seleccionar su dispositivo
    - **Data keys:** Marcar `distance_mm`, `battery_V`
5. **Configurar la apariencia:**
    - **Title:** `Nivel de Agua y Batería`
    - **Time window:** Últimos 7 días
    - **Colores de línea:** Azul (distancia), Rojo (batería)
6. **Hacer clic en "Add"**
7. **El widget aparece en el panel**

**Cambiar tamaño y posición:**

- Arrastrar el widget para moverlo
- Arrastrar los bordes/esquinas para cambiar el tamaño
- Agregar más widgets

### Paso 3: Widgets Adicionales Útiles

**1. Tarjeta de Distancia Actual**

- **Paquete:** `Cards`
- **Tipo:** `Simple Card`
- **Clave de datos:** `distance_mm`
- **Título:** `Distancia Actual`
- **Muestra:** Último valor grande y claro

**2. Medidor de Batería**

- **Paquete:** `Gauges`
- **Tipo:** `Radial Gauge`
- **Clave de datos:** `battery_V`
- **Valor mínimo:** 3.0
- **Valor máximo:** 4.2
- **Rangos de color:**
    - 3.0–3.3: Rojo (crítico)
    - 3.3–3.6: Naranja (bajo)
    - 3.6–4.2: Verde (bueno)

**3. Gráfico de Temperatura** (solo Wari)

- **Paquete:** `Charts`
- **Tipo:** `Timeseries Line Chart`
- **Clave de datos:** `temperature_C`
- **Ventana de tiempo:** Últimas 24 horas

**4. Tarjeta de Estado del Dispositivo**

- **Paquete:** `Cards`
- **Tipo:** `Entity Table`
- **Muestra:** Última actividad, atributos y estado

### Paso 4: Guardar el Panel

1. **Hacer clic en el ícono de verificación** (parte superior derecha) para salir del modo de edición
2. **El panel se guarda** automáticamente
3. **Compartir o marcar** la URL para acceso rápido

---

## Configuración Avanzada de Widgets

### Personalización de Ventanas de Tiempo

**Ventana Fija:**

- Última hora
- Últimas 24 horas
- Últimos 7 días
- Rango personalizado

**Ejemplo:** Mostrar solo horas de trabajo

- Configurar: 8:00 AM–6:00 PM
- Útil para registradores alimentados por energía solar

### Agregar Umbrales

**Niveles de Alarma en Gráficos:**

1. Editar widget (ícono de lápiz)
2. Navegar a la pestaña "Advanced"
3. Agregar **líneas de umbral:**
    - **Advertencia de agua alta:** 500 mm (línea roja)
    - **Advertencia de agua baja:** 2000 mm (línea naranja)
    - **Batería crítica:** 3.3 V (línea roja)

### Agregación de Datos

**Para períodos de tiempo largos** (meses), agregar datos:

- **Ninguna:** Mostrar todos los puntos (lento para conjuntos de datos grandes)
- **Promedio:** Promedios diarios/horarios
- **Mín/Máx:** Mostrar extremos
- **Suma:** Total durante el período

**Ejemplo:** Nivel máximo diario del agua

- Agregación: `Max`
- Intervalo: `1 día`

### Campos Calculados

**Derivar nuevos valores de la telemetría:**

**Profundidad del Agua** (altura de instalación - distancia):

1. Agregar atributo del dispositivo: `install_height_mm = 3000`
2. Usar widget con cálculo:
   ```
   ${install_height_mm} - ${distance_mm}
   ```

**Porcentaje de Batería:**

Estimar % del voltaje:
```
(${battery_V} - 3.0) / (4.2 - 3.0) * 100
```

---

## Alarmas y Notificaciones

### Creación de Reglas de Alarma

1. **Navegar al dispositivo**
2. **Hacer clic en la pestaña "Alarms"**
3. **Hacer clic en "+ Add Alarm Rule"**
4. **Configurar la regla:**

**Ejemplo: Alerta de Batería Baja**

- **Name:** `Low Battery Warning`
- **Type:** `battery_low`
- **Severity:** `Warning`
- **Condition:**
  ```
  $[battery_V] < 3.4
  ```
- **Schedule:** Siempre activo
- **Action:** Crear alarma

**Ejemplo: Alerta de Agua Alta**

- **Name:** `Flood Warning`
- **Type:** `high_water`
- **Severity:** `Critical`
- **Condition:**
  ```
  $[distance_mm] < 500
  ```
  (Asumiendo 500mm = umbral de agua alta)

### Configuración de Notificaciones

**Notificaciones por Correo Electrónico:**

1. **Navegar a "Rule Chains"** (barra lateral izquierda)
2. **Abrir "Root Rule Chain"**
3. **Agregar nodo "Send Email"**
4. **Configurar los ajustes SMTP:**
    - Servidor (Gmail, SendGrid, etc.)
    - Credenciales
    - Direcciones de destinatarios
5. **Conectar nodos de alarma** al nodo de correo electrónico

**Notificaciones SMS** (a través de servicio de terceros):

- Integrar con Twilio, AWS SNS
- Requiere ThingsBoard Cloud o auto-alojado
- Configurar en Rule Chain

**Alarmas en el Panel:**

- Agregar widget de "Alarms" al panel
- Muestra alarmas activas con colores de severidad
- Hacer clic para reconocer/borrar

---

## Exportación de Datos

### Exportación de Datos Históricos

1. **Navegar al dispositivo**
2. **Hacer clic en la pestaña "Latest Telemetry"**
3. **Seleccionar el rango de fechas**
4. **Hacer clic en el ícono de exportar** (parte superior derecha)
5. **Elegir el formato:**
    - CSV (compatible con Excel)
    - JSON (para procesamiento)
6. **Descargar el archivo**

**Formato CSV:**
```csv
Timestamp,distance_mm,battery_V,temperature_C
2025-12-27 10:00:00,1234,3.85,22.5
2025-12-27 10:15:00,1256,3.84,23.1
...
```

### Acceso a Datos por API

**Para acceso programático:**

```bash
curl -X GET \
  'http://demo.thingsboard.io/api/plugins/telemetry/DEVICE/SU_DEVICE_ID/values/timeseries?keys=distance_mm,battery_V&startTs=1703520000000&endTs=1703606400000' \
  -H 'X-Authorization: Bearer SU_TOKEN_JWT'
```

**Token JWT:** Obtener desde el perfil de usuario > Configuración de seguridad

---

## Gestión de Múltiples Dispositivos

### Agregar Múltiples Registradores

**Repetir para cada registrador:**

1. Agregar dispositivo (nombre único: `Wari-001`, `Wari-002`, etc.)
2. Obtener el token de acceso
3. Configurar Arduino con el token respectivo
4. Desplegar

### Grupos de Dispositivos

**Organizar por ubicación o tipo:**

1. **Navegar a "Entity Groups"**
2. **Crear grupo:** `Creek Monitoring Sites`
3. **Agregar dispositivos** al grupo
4. **Filtrar paneles** por grupo

### Panel de Flota

**Crear un panel de resumen:**

1. **Agregar widget de "Devices"**
2. **Tipo de entidad:** Grupo de dispositivos
3. **Seleccionar grupo:** `Creek Monitoring Sites`
4. **Columnas:** Nombre, Última actividad, Batería, Última distancia
5. **Reglas de color:** Resaltar dispositivos con batería baja o sin conexión

**Ejemplo de diseño:**

| Dispositivo | Ubicación | Última vez visto | Batería | Distancia | Estado              |
|-------------|-----------|------------------|---------|-----------|---------------------|
| Wari-001    | Sitio A   | hace 2 min       | 3.85 V  | 1234 mm   | 🟢 OK               |
| Wari-002    | Sitio B   | hace 17 min      | 3.45 V  | 2456 mm   | 🟡 Batería baja     |
| Lidar-001   | Sitio C   | hace 3 horas     | 3.12 V  | --        | 🔴 Sin conexión     |

---

## Gestión de Usuarios

### Agregar Usuarios (Solo Cloud/Autoalojado)

El servidor de demostración no admite múltiples usuarios.

**En Cloud/Autoalojado:**

1. **Navegar a "Users"** (barra lateral izquierda)
2. **Hacer clic en "+ Add User"**
3. **Completar formulario:**
    - Correo electrónico, nombre
    - Rol (Customer, Tenant, etc.)
4. **Asignar permisos:**
    - Solo lectura (para partes interesadas)
    - Acceso completo (para técnicos)
5. **Enviar correo** de activación

### Compartir Paneles

**Enlace público** (solo lectura):

1. Abrir el panel
2. Hacer clic en el icono **"Share"** (arriba a la derecha)
3. Activar **"Public"**
4. Copiar el enlace
5. Compartir la URL con las partes interesadas

**Ventajas:** Fácil de compartir, no requiere inicio de sesión  
**Desventajas:** Cualquier persona con el enlace puede ver los datos

---

## Aplicación Móvil de ThingsBoard

### Instalación de la Aplicación

**iOS:**

- App Store: Buscar "ThingsBoard Live"
- O: https://apps.apple.com/app/thingsboard-live

**Android:**

- Google Play: Buscar "ThingsBoard Live"
- O: https://play.google.com/store/apps/details?id=org.thingsboard.demo.app

### Configuración de la Aplicación

1. **Abrir la aplicación**
2. **Seleccionar servidor:**
    - Demo: `demo.thingsboard.io`
    - Cloud: `thingsboard.cloud`
    - Personalizado: URL de tu servidor
3. **Iniciar sesión** con las credenciales
4. Los **paneles** aparecen en la pantalla de inicio
5. **Tocar para ver** datos en tiempo real

**Funciones móviles:**

- Ver paneles
- Revisar alarmas
- Exportar datos
- Notificaciones push (si está configurado)

---

## Solución de Problemas

### No Aparecen Datos

**Lista de verificación:**

1. **Verificar que el registrador esté enviando datos**
    - Revisar el Monitor Serial para "HTTP 200 OK"
    - Verificar que la tarjeta SD tenga datos recientes
2. **Comprobar el token de acceso** en el código de Arduino
3. **Verificar la URL del servidor** que coincida con la instancia de ThingsBoard
4. **Probar con curl** (solicitud POST manual)
5. **Revisar la pestaña "Latest Telemetry"** del dispositivo (no solo el panel)
6. **Inspeccionar la conectividad de red** (¿XBee registrado?)

### Los Datos Aparecen pero el Panel Está en Blanco

1. **Revisar la configuración del widget:**
    - ¿Está seleccionado el dispositivo correcto?
    - ¿Las claves de datos coinciden con las claves de telemetría?
    - ¿La ventana de tiempo incluye el período de datos?
2. **Actualizar el panel** (F5)
3. **Revisar la consola del navegador** para ver errores

### Las Alarmas No Se Activan

1. **Verificar la sintaxis de la condición** de la regla de alarma
2. **Comprobar que los valores de telemetría** cumplan la condición
3. **Asegurarse de que la alarma esté limpia** (no se volverá a activar si ya está activa)
4. **Probar con datos manuales** (POST con curl con valores que activen la alarma)

### Datos Retrasados

- ThingsBoard procesa datos rápidamente (generalmente < 1 segundo)
- El retraso suele estar en la transmisión (tiempo de conexión XBee)
- Revisar la tasa de actualización del widget (intervalo de actualización automática)

---

## Buenas Prácticas

### Diseño del Panel

**Hacer:**

- Usar títulos de widgets claros
- Mostrar las métricas clave de forma prominente
- Codificar por colores según gravedad (rojo = malo, verde = bueno)
- Incluir contexto temporal (última actualización)
- Agrupar datos relacionados

**No hacer:**

- Sobrecargar con demasiados widgets
- Usar escalas engañosas
- Olvidar a los usuarios móviles (probar en el teléfono)

### Gestión de Datos

- **Exportar datos regularmente** (copia de seguridad en caso de problemas con la cuenta)
- **Documentar atributos del dispositivo** (ubicación de instalación, altura, etc.)
- **Usar nombres coherentes** (Wari-001, no "registrador en el arroyo")
- **Revisar alarmas semanalmente** (ajustar umbrales según la experiencia)

### Seguridad

- **Usar HTTPS** para ThingsBoard Cloud/autoalojado
- **Rotar tokens de acceso** si se ven comprometidos
- **Limitar el uso compartido de paneles públicos** a datos no sensibles
- **Usar contraseñas seguras**
- **Activar 2FA** si está disponible (Cloud/autoalojado)

---

## Consideraciones de Costo

### Servidor de Demostración

- **Costo:** Gratuito
- **Límites:** Retención de datos, sin SLA, eliminación periódica
- **Ideal para:** Pruebas, educación

### ThingsBoard Cloud

- **Nivel gratuito:** 
    - 1 dispositivo
    - Paneles limitados
    - Soporte comunitario
- **Planes de pago:** ~$10–50+/mes
    - Más dispositivos
    - Funciones avanzadas
    - Soporte técnico
    - SLA

### Autoalojado
- **Infraestructura:** $5–20+/mes (DigitalOcean, AWS)
- **Tiempo de configuración:** 4–8 horas (primera vez)
- **Mantenimiento:** Continuo
- **Ideal para:** Organizaciones con recursos de TI

---

## Alternativas a ThingsBoard

Si ThingsBoard no se adapta a tus necesidades:

**Código abierto:**

- **Grafana + InfluxDB:** Popular, potente, curva de aprendizaje pronunciada
- **Node-RED:** Programación de flujos visual, flexible
- **Emoncms:** Enfocado en monitoreo de energía

**Comercial:**

- **Losant:** Plataforma IoT empresarial
- **Ubidots:** Fácil de usar, precios moderados
- **TagoIO:** Bueno para prototipos

**DIY:**

- Servidor personalizado con PHP/Python + MySQL
- Google Sheets (mediante IFTTT o script personalizado)

---

## Próximos Pasos

- [Guía de configuración de XBee](xbee-setup-walkthrough.md): Configuración detallada de XBee
- [Solución de problemas de conexión](troubleshooting-connections.md): Resolver problemas de telemetría
- [Documentación de ThingsBoard](https://thingsboard.io/docs/): Documentación oficial para funciones avanzadas
- [Guía de batería y alimentación](../operation/hardware/battery-power-guide.md): Monitorear el estado de la batería

---

!!! success "¡Panel listo!"
    Tu panel de ThingsBoard está ahora configurado y mostrando datos en tiempo real. ¡Compruébalo diariamente durante la primera semana para asegurarte de que funciona de manera estable!
