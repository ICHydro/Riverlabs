# Guía Paso a Paso de Configuración del XBee

!!! abstract "Resumen"
    Esta es una guía visual paso a paso para configurar módulos Digi XBee Cellular para registradores Riverlabs. Cubre tanto XBee Studio (recomendado) como XCTU (heredado).
    
    **Compatibilidad:** XBee 3G, XBee Cellular LTE-M/NB-IoT y XBee3 Cellular

!!! info "Herramientas de Configuración"
    La mayoría de la configuración del XBee se realiza directamente en el código de Arduino. Sin embargo, ciertos parámetros deben configurarse manualmente para garantizar una comunicación adecuada entre el registrador y el XBee:
    
    - **Configuración de la interfaz serie** (velocidad de baudios, paridad y tiempos de espera)
    - **Configuración del modo de sueño** (pin sleep)
    - **Modo API** (con caracteres escapados)
    
    Puede usar cualquiera de los dos:
    
    - **XBee Studio** (recomendado): Interfaz moderna, mejor para XBee3
    - **XCTU**: Herramienta heredada, funciona con todos los modelos XBee incluyendo módulos 3G más antiguos
    
    Ambas herramientas son gratuitas en el [sitio web de Digi](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools).

## Requisitos Previos

Antes de comenzar, asegúrese de tener:

-  **Módulo XBee Cellular** (3G, LTE-M o NB-IoT)
-  **Tarjeta SIM activa** (insertada en el XBee)
-  **Adaptador USB** para XBee (XBee Grove Board o similar)
-  **Computadora** (Windows, macOS o Linux)
-  **XBee Studio** o **XCTU** instalado
-  **Detalles del APN del operador** (del proveedor de SIM)

---

## Parte 1: Instalación de XBee Studio

### Descarga e Instalación

**1. Navegar al Sitio Web de Digi:**

- URL: https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xbee-studio
- O buscar "XBee Studio download"

**2. Seleccionar su Plataforma:**

- Windows (64-bit)
- macOS
- Linux

**3. Descargar el Instalador**

**4. Instalar:**

**Windows:**

- Ejecutar el instalador .exe
- Aceptar la licencia
- Elegir el directorio de instalación
- Instalar los controladores cuando se solicite
- Finalizar

**macOS:**

- Abrir el archivo .dmg
- Arrastrar XBee Studio a Aplicaciones
- Primer inicio: Clic derecho > Abrir (omitir Gatekeeper)
- Los controladores se instalan automáticamente (puede requerir reinicio)


**Linux:**
```bash
# Extraer el archivo
tar -xzf XBeeStudio-linux-x64.tar.gz

# Ejecutar el instalador o AppImage
./XBeeStudio.AppImage

# Controladores (si es necesario)
sudo apt-get install libusb-1.0-0
```

### Verificar la Instalación

1. Iniciar XBee Studio
2. Aparece la pantalla de bienvenida
3. Verificar la versión (Help > About): debe ser 1.4.0+ para soporte celular XBee3

---

## Parte 2: Conexión de Hardware

### Configuración Física

**1. Insertar la Tarjeta SIM (si aún no se ha hecho):**

!!! warning "Apagar Primero"
    ¡Asegúrese de que el XBee NO esté conectado a la alimentación o USB al insertar/retirar la SIM!

- Localizar la ranura de la SIM (parte inferior del XBee)
- Observar la orientación de la ranura (diagrama en el módulo)
- Insertar micro-SIM (tamaño 3FF)
- Empujar hasta que haga clic (a ras con el módulo)
- Verificar que esté correctamente asentada

**2. Montar el XBee en el Adaptador USB:**

- Alinear los pines cuidadosamente
- Presionar firmemente hasta que esté asentado
- Todos los pines deben estar en contacto
- El módulo se asienta plano contra el adaptador

**3. Conectar el Cable USB:**

- Adaptador al puerto USB de la computadora
- Esperar el reconocimiento del controlador

### Verificación del Controlador

**Windows:**

1. Abrir el Administrador de Dispositivos (Win+X > Administrador de Dispositivos)
2. Expandir "Puertos (COM y LPT)"
3. Buscar "USB Serial Port (COMx)" — anotar el número COM
4. Si "Dispositivo Desconocido": reinstalar XBee Studio (incluye controladores)

**macOS:**

1. Abrir Terminal
2. Listar dispositivos:
   ```bash
   ls /dev/tty.*
   ```
3. Buscar `/dev/tty.usbserial-XXXXXXXX`

**Linux:**

1. Verificar el dispositivo:
   ```bash
   ls /dev/ttyUSB*
   ```
2. Agregar usuario al grupo dialout (si se deniega el permiso):
   ```bash
   sudo usermod -a -G dialout $USER
   # Cerrar sesión y volver a iniciarla
   ```

---

## Parte 3: Agregar el XBee a XBee Studio

### Paso 1: Agregar Nuevo Dispositivo

**1. Iniciar XBee Studio**

**2. Ventana Principal:**

- Panel izquierdo: "Devices" (actualmente vacío)
- Centro: Mensajes de "Get Started"
- Derecha: Paneles de información

**3. Hacer clic en "+ Add Device"** (botón en la parte superior izquierda o central)

**4. Aparece el Diálogo de Conexión**

### Paso 2: Seleccionar el Método de Conexión

**En el diálogo "Add Radio":**

**Tipo de Conexión:**

- Seleccionar: **"USB"** (predeterminado)

**Puerto:**

- Windows: Seleccionar **"COM3"** (o su puerto COM del Administrador de Dispositivos)
- macOS/Linux: Seleccionar **/dev/ttyUSB0** (o su dispositivo)

**Velocidad de Baudios:**

- Dejar en **"9600"** (o "Auto" si está disponible)

!!! tip "¿No Puede Ver su Puerto?"
    - Intentar hacer clic en el botón "Refresh"
    - Verificar el cable USB (con capacidad de datos, no solo de carga)
    - Verificar que los controladores estén instalados
    - Intentar con un puerto USB diferente

**5. Hacer clic en "Add Selected"**

### Paso 3: Lectura Inicial

**XBee Studio:**

- Se conecta al XBee
- Lee la versión del firmware
- Lee la configuración actual
- Muestra la información del módulo

**Debería ver:**

- Dispositivo aparece en el panel izquierdo
- Detalles del módulo (centro):
    - Modelo: XB3-C-A2-UT-001 (o similar)
    - Firmware: Cellular (p.ej., 11415)
    - Versión de hardware: 1E (o actual)
- Aparecen las pestañas de configuración

**Si la conexión falla:**

- Verificar la selección del puerto COM
- Verificar la velocidad de baudios (probar Auto o 9600)
- Asegurar que el XBee esté correctamente asentado
- Ver la sección de resolución de problemas

---

## Parte 4: Descripción General de las Pestañas de Configuración

### Interfaz Principal de Configuración

**Panel Izquierdo:**

- Lista de dispositivos agregados
- Hacer clic en el dispositivo para seleccionarlo

**Pestañas del Panel Central:**

**1. Radio Configuration**

- La pestaña más importante
- Todos los ajustes aquí

**2. Cellular Network**

- Estado de la red
- Intensidad de la señal
- Diagnósticos de conexión

**3. MicroPython**

- Para programación avanzada (no necesario para el registrador)

**4. Firmware**

- Actualizar firmware

**5. Terminal**

- Terminal serie para depuración

**6. File System**

- Acceder al sistema de archivos del XBee (avanzado)

### Navegación por la Configuración de Radio

**Los ajustes están organizados en secciones contraíbles:**

- **Addressing** (ID de PAN, direcciones)
- **Serial Interfacing** (velocidad de baudios, paridad)
- **I/O Settings** (configuración de pines)
- **Networking** (APN, configuración de conexión)
- **Security** (TLS, certificados)
- **RF Interfacing** (ajustes del módem celular)
- **Sleep Modes** (gestión de energía)
- **AT Command Options** (modo API)

**Cómo editar:**

1. Hacer clic en la sección para expandirla
2. Hacer clic en el nombre del ajuste para editar
3. Escribir el nuevo valor o seleccionar del desplegable
4. El ajuste se vuelve **negrita** cuando se modifica
5. Debe hacer clic en **"Write"** para guardar en el XBee

---

## Parte 5: Configuración Esencial

### Descripción General de los Ajustes Requeridos

**Objetivo:** Configurar el XBee para:

1. Conexión de red celular
2. Comunicación serie con Arduino
3. Modo API para la transmisión de datos
4. Modo de sueño para ahorro de energía

### Sección 1: Interfaz Serie

**Expandir la sección "Serial Interfacing"**

!!! info "Configuraciones Serie Críticas"
    Estos ajustes DEBEN coincidir exactamente para que el XBee se comunique con el registrador Riverlabs. Estos valores son requeridos por la biblioteca xbee-arduino personalizada usada en el firmware del registrador.

| Ajuste                     | Parámetro | Valor             | Descripción                  |
|----------------------------|-----------|-------------------|------------------------------|
| **Velocidad de Baudios**   | BD        | **9600**          | Debe coincidir con el código Arduino |
| **Paridad**                | NB        | **0** (Sin Paridad)| Estándar, sin bit de paridad |
| **Bits de Parada**         | SB        | **1**             | Un bit de parada             |
| **Timeout de Empaquetado** | RO        | **3**             | Timeout de caracteres (3 ms) |
| **Entrega de Transmisión** | TD        | **0**             | Deshabilitar estado de entrega |
| **Umbral de Control de Flujo** | FT   | **681**           | Umbral del búfer UART        |

**Cómo configurar:**

1. Hacer clic en "BD (Baud Rate)"
2. Desplegable: Seleccionar **"9600"**
3. Establecer RO = **3**
4. Establecer TD = **0**
5. Establecer FT = **681** (hexadecimal: 0x2A9)
6. Verificar NB = 0, SB = 1 (normalmente predeterminados)
7. No hacer clic en Write todavía (agrupar todos los cambios)

!!! warning "Se Requiere la Biblioteca xbee-arduino Extendida"
    El código Riverlabs usa una versión extendida de la [biblioteca xbee-arduino](https://github.com/ICHydro/xbee-arduino) con funcionalidad de módem celular. Esta biblioteca personalizada está incluida en los paquetes de código Riverlabs, por lo que no necesita instalarla por separado.

### Sección 2: Red

**Expandir la sección "Network" o "Mobile Network"**

!!! danger "Ajustes Críticos"
    ¡Un APN incorrecto impide la conexión celular!

| Ajuste                  | Parámetro | Valor                   | Descripción       |
|-------------------------|-----------|-------------------------|-------------------|
| **Nombre del Punto de Acceso** | AN | **[Su APN del operador]** | Del proveedor de SIM |
| **Nombre de Usuario APN** | AU    | **[Si es necesario]**   | Normalmente en blanco |
| **Contraseña APN**      | AV        | **[Si es necesario]**   | Normalmente en blanco |
| **Tecnología de Red**   | NT        | **0** (LTE-M preferido) | O 1 (NB-IoT)      |

**APNs Comunes:**

| Operadora        | APN                       |
|------------------|---------------------------|
| AT&T (EE.UU.)    | `m2m.com.attwireless.com` |
| T-Mobile (EE.UU.)| `iot.t-mobile.com`        |
| Verizon (EE.UU.) | `vzwinternet`             |
| Vodafone (UE)    | `iot.vodafone.com`        |
| Hologram         | `hologram`                |

**Ejemplo de Configuración:**
```
AN = m2m.com.attwireless.com
AU = (en blanco)
AV = (en blanco)
NT = 0
```

### Sección 3: Modo API

**Expandir la sección "AT Command Options" o "API"**

| Ajuste              | Parámetro | Valor | Descripción             |
|---------------------|-----------|-------|-------------------------|
| **Habilitar API**   | AP        | **2** | Modo API con escapes    |
| **Modo de Salida API** | AO     | **0** | Estándar                |

!!! danger "¡El Modo API Debe Ser 2 — Crítico!"
    El firmware de Riverlabs **requiere el Modo API 2** (modo API con caracteres escapados).
    
    - **Modo 0** (Transparente) = NO funcionará
    - **Modo 1** (API sin escapes) = NO funcionará  
    - **Modo 2** (API con escapes) = ✅ **REQUERIDO**
    
    Usar el modo incorrecto impedirá que el registrador se comunique con el XBee, resultando en la ausencia de datos de telemetría.

**Cómo configurar:**

1. Hacer clic en "AP (API Enable)"
2. Desplegable: Seleccionar **"2 - API mode with escaped characters"**
3. Verificar AO = 0

### Sección 4: Modo de Sueño

**Expandir la sección "Sleep Modes" o "Power Management"**

| Ajuste              | Parámetro | Valor | Descripción     |
|---------------------|-----------|-------|-----------------|
| **Modo de Sueño**   | SM        | **1** | Pin Sleep [1]   |
| **Opciones de Sueño**| SO       | **0** | Estándar        |

!!! info "Se Requiere el Modo de Sueño Pin"
    **SM debe configurarse en 1 (Pin Sleep)** para una gestión correcta de la energía con el registrador Riverlabs.

**Operación del Pin Sleep:**

- Arduino controla cuándo duerme el XBee a través de un pin de hardware
- Pin 9 (Sleep_RQ/DTR): 
    - **Alto** = XBee duerme (bajo consumo)
    - **Bajo** = XBee se despierta (activo)
- Ahorra energía significativa entre transmisiones
- Esencial para una larga vida de la batería

**Cómo configurar:**

1. Hacer clic en "SM (Sleep Mode)"
2. Desplegable: Seleccionar **"1 - Pin Sleep [1]"**
3. Verificar SO = 0

!!! warning "No usar otros modos de suspensión"
    - **SM = 0** (Desactivado) agotará la batería rápidamente
    - **SM = 4** (Cyclic Sleep) no funcionará con la lógica de control del registrador
    - **SM = 5** (Cyclic Sleep with Pin Wake) no es compatible

### Sección 5: Configuración Adicional (Opcional pero Recomendada)

| Configuración              | Parámetro | Valor              | Descripción          |
|----------------------------|-----------|--------------------|----------------------|
| **Guard Time**             | GT        | **3E8** (1000 ms)  | Retardo modo comando |
| **Flow Control Threshold** | FT        | **681**            | Búfer UART           |
| **Transmit Options**       | TO        | **0**              | Estándar             |

---

## Parte 6: Escribir la Configuración

### Escritura por lotes

**Después de configurar TODAS las opciones anteriores:**

1. **Revisar los cambios:**
    - Los ajustes modificados aparecen en **negrita**
    - Verificar que todos los ajustes críticos hayan cambiado

2. **Hacer clic en el botón "Write"** (parte superior de la pestaña Radio Configuration)
    - El botón suele estar en la barra de herramientas
    - O: Clic derecho en el dispositivo > "Write Settings"

3. **Esperar a que termine la escritura:**
    - Indicador de progreso
    - Mensaje "Write successful"
    - Los ajustes ya no aparecen en negrita

4. **Verificar:**
    - Hacer clic en el botón "Read" para actualizar
    - Confirmar que todos los ajustes coinciden con los valores esperados

!!! warning "Escribir antes de desconectar"
    Los cambios NO se guardan hasta hacer clic en Write. Si se desconecta sin escribir, los cambios se pierden.

### Perfil de configuración (Opcional)

**Guardar la configuración para uso futuro:**

1. **File > Export Profile**
2. **Guardar como:** `Riverlabs_XBee3_Config.xpro`
3. **Usar más adelante:**
    - File > Import Profile
    - Aplicar a nuevos módulos XBee
    - Garantiza consistencia en toda la flota

---

## Parte 7: Prueba de Conexión de Red

### Usar el Panel de Red Celular

**1. Navegar a la pestaña "Cellular Network"**

**2. Visualización del estado actual:**

- Registro en red: Not registered / Registered
- Intensidad de señal (RSSI): valor en dBm
- Dirección IP: (ninguna) o IP asignada
- Estado de conexión: Disconnected / Connected

**3. Hacer clic en el botón "Connect to Network"**

**4. Esperar la conexión:**

- Puede tardar 30–90 segundos
- Aparecen mensajes de progreso
- Observar:
    - "Searching for network..."
    - "Network found"
    - "Registering..."
    - "Connected"

**5. Indicadores de éxito:**

- ✅ Red registrada: **Sí**
- ✅ Intensidad de señal: **-90 dBm o mejor** (menos negativo = más fuerte)
- ✅ Dirección IP: **Asignada** (p. ej., 10.x.x.x)
- ✅ Estado de conexión: **Connected**

**6. Guía de intensidad de señal:**

| RSSI (dBm)    | Calidad    | Usabilidad                            |
|---------------|------------|---------------------------------------|
| -50 a -70     | Excelente  | Perfecta para uso continuo            |
| -70 a -90     | Buena      | Fiable para la mayoría de aplicaciones |
| -90 a -105    | Aceptable  | Usable pero puede tener problemas     |
| -105 a -115   | Deficiente | Marginal, alta tasa de fallos         |
| < -115        | Muy mala   | Poco fiable                           |

### Resolución de Problemas de Conexión

**"No se encontró red":**

**Causas:**

- SIM no activada
- Tecnología de red incorrecta (LTE-M vs NB-IoT)
- Ubicación con señal débil

**Soluciones:**

1. Verificar que la SIM esté activa (contactar al operador)
2. Intentar cambiar el parámetro NT:
    - NT=0 (LTE-M)
    - NT=1 (NB-IoT)
    - Escribir y reintentar la conexión
3. Mover a una ventana o al exterior
4. Comprobar que la antena esté conectada (si es externa)

**"Red encontrada pero registro fallido":**

**Causas:**

- APN incorrecto
- Problema de autenticación de la SIM

**Soluciones:**

1. Verificar la ortografía del APN (¡distingue mayúsculas y minúsculas!)
2. Comprobar las credenciales AU/AV
3. Contactar al operador para obtener los detalles del APN
4. Probar con el APN predeterminado (suele funcionar)

**"Registrado pero sin dirección IP":**

**Causas:**

- Fallo de DHCP
- Congestión de red

**Soluciones:**

1. Desconectar y volver a conectar
2. Esperar 2–3 minutos
3. Reiniciar el XBee (retirar/reinsertar el USB)
4. Contactar al operador (posible problema con la cuenta)

---

## Parte 8: Prueba de Transmisión de Datos

### Usar la Pestaña Terminal

**Probar HTTP POST** (para verificar la configuración completa):

1. **Navegar a la pestaña "Terminal"**

2. **Asegurarse de estar conectado a la red** (del paso anterior)

3. **Introducir comandos API** (usuarios avanzados):

**Alternativamente, probar desde Arduino:**

1. **Retirar el XBee del adaptador USB**
2. **Instalarlo en el registrador**
3. **Cargar el sketch de prueba** (o el código de producción)
4. **Conectar el monitor serie**
5. **Observar los intentos de transmisión**

**Flujo esperado:**
```
Initializing XBee...
XBee ready
Connecting to network...
Network connected
Reading sensor...
Distance: 1234 mm
Preparing HTTP request...
Sending to ThingsBoard...
Response: 200 OK
Transmission successful!
```

**Si la transmisión falla:**

- Ver [Resolución de problemas de conexión](troubleshooting-connections.md)

---

## Parte 9: Finalizar la Configuración

### Prueba de ciclo de alimentación

**Verificar que la configuración persiste:**

1. **Desconectar el USB** (retirar el XBee del adaptador)
2. **Esperar 10 segundos**
3. **Volver a conectar el USB** (reinsertar en el adaptador)
4. **Abrir XBee Studio, añadir el dispositivo**
5. **Leer la configuración**
6. **Verificar que todos los ajustes** siguen siendo correctos

Si los ajustes se restablecen:

- La configuración no se escribió correctamente. Repetir la Parte 6.

### Etiquetar el módulo XBee

**Documentar la configuración:**

- Usar una etiquetadora o marcador permanente
- Escribir en el XBee (alejado de la antena):
    - "Configurado DD/MM/AA"
    - "Registrador: Wari-001"
    - APN (si es útil)
- Evita confusiones con módulos sin configurar

### Instalación en el Registrador

**1. Apagar el registrador** (desconectar la batería)

**2. Instalar el XBee:**

- Alinear el pin 1 (buscar la marca en la PCB)
- Presionar suave pero firmemente
- Asegurarse de que todos los pines encajen
- El módulo debe quedar enrasado

**3. Comprobar la antena:**

- Antena externa: Conectar U.FL o similar
- Antena PCB interna: Asegurarse de que haya espacio libre (sin contacto con la carcasa)

**4. Volver a conectar la batería**

**5. Probar el arranque:**

- El registrador arranca
- El XBee se inicializa
- Conexión de red en menos de 60 segundos

---

## Parte 10: Resolución de Problemas

### XBee Studio No Detecta el Módulo

**Síntoma:** No se encuentra el dispositivo al añadirlo

**Causas y soluciones:**

| Causa                      | Verificar                        | Solución                              |
|----------------------------|----------------------------------|---------------------------------------|
| Puerto seleccionado incorrecto | Administrador de dispositivos (Windows) | Seleccionar el puerto COM correcto |
| Velocidad de baudios incorrecta | Config. anterior cambió BD   | Probar 9600, 115200 o Auto            |
| Problema de controlador    | Administrador de dispositivos muestra error | Reinstalar XBee Studio, reiniciar |
| Cable USB defectuoso       | Cable solo de carga              | Usar cable USB con datos              |
| XBee no bien insertado     | Inspección física                | Reinsertar el XBee en el adaptador    |
| Problema de alimentación del adaptador | LED en el adaptador   | Probar otro puerto/hub USB            |

### Ajustes No se Guardan

**Síntoma:** La configuración se restablece tras un ciclo de alimentación

**Causas:**

- No se hizo clic en el botón "Write"
- La escritura falló (sin mensaje de error)
- Corrupción del firmware (poco frecuente)

**Soluciones:**

1. Configurar los ajustes
2. Hacer clic en "Write"
3. **Esperar la confirmación**
4. Hacer clic en "Read" para verificar
5. Reiniciar el ciclo de alimentación y leer de nuevo
6. Si sigue restableciendo: Actualizar el firmware (ver a continuación)

### Intensidad de Señal Débil

**Síntoma:** RSSI < -105 dBm

**Soluciones:**

1. **Antena:**
    - Asegurarse de que esté bien conectada
    - Probar con antena externa si se usa la interna
    - Comprobar si hay daños
2. **Ubicación:**
    - Mover a una ventana
    - Elevar el dispositivo
    - Alejarlo de estructuras metálicas
3. **Red:**
    - Probar diferente tecnología (NT=0 vs NT=1)
    - Comprobar el mapa de cobertura del operador
    - Probar la SIM en un teléfono en la misma ubicación

### Actualización de Firmware

**Cuándo es necesario:**

- Correcciones de errores de Digi
- Nuevas funciones
- Mejoras de compatibilidad

**Cómo actualizar:**

1. **Descargar el firmware:**
    - https://www.digi.com/support/productdetail?pid=5817
    - Seleccionar XBee3 Cellular LTE-M/NB-IoT
    - Elegir la última versión (p. ej., 11415 o más reciente)

2. **En XBee Studio:**
    - Navegar a la pestaña "Firmware"
    - Hacer clic en "Update Firmware"
    - Navegar al archivo descargado (.ebin)
    - Hacer clic en "Update"
    - Esperar (5–10 minutos, **no desconectar**)
    - Mensaje de éxito

3. **Verificar:**
    - Comprobar la versión de firmware (pestaña Radio Configuration)
    - Reconfigurar los ajustes (la actualización de firmware puede restablecerlos)

## Próximos Pasos

- [Configuración de ThingsBoard](thingsboard-configuration.md): Configurar el panel
- [Resolución de problemas de conexión](troubleshooting-connections.md): Solucionar problemas de telemetría
- [Guía de configuración de telemetría](setup-guide.md): Resumen completo de la configuración
- [Guía de batería y alimentación](../operation/hardware/battery-power-guide.md): Optimizar para telemetría

---

!!! success "¡XBee configurado!"
    Tu XBee está ahora correctamente configurado y listo para el despliegue en campo. ¡Guarda el perfil de configuración para futuros módulos!
