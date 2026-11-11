# Guía paso a paso para configurar XBee

!!! abstract "Descripción general"
    Esta guía visual explica paso a paso cómo configurar módulos Digi XBee Cellular para los registradores Riverlabs. Incluye XBee Studio (recomendado) y XCTU (anterior).
    
    **Compatibilidad:** XBee 3G, XBee Cellular LTE-M/NB-IoT y XBee3 Cellular

!!! info "Herramientas de configuración"
    La mayor parte de la configuración del XBee se realiza directamente en el código Arduino. Sin embargo, algunos parámetros deben ajustarse manualmente para que el registrador y el XBee puedan comunicarse:
    
    - **Ajustes de la interfaz serie** (velocidad en baudios, paridad y tiempos de espera)
    - **Modo de suspensión** (control mediante pin)
    - **Modo API** (con caracteres de escape)
    
    Puede utilizar cualquiera de estas herramientas:
    
    - **XBee Studio** (recomendado): interfaz moderna, más adecuada para XBee3
    - **XCTU**: herramienta anterior, compatible con todos los modelos XBee, incluidos los módulos 3G
    
    Ambas se descargan gratuitamente del [sitio web de Digi](https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools).

## Requisitos previos

Antes de empezar, compruebe que dispone de:

-  **Módulo XBee Cellular** (3G, LTE-M o NB-IoT)
-  **Tarjeta SIM activa** (insertada en el XBee)
-  **Adaptador USB** para el XBee (placa XBee Grove o similar)
-  **Ordenador** (Windows, macOS o Linux)
-  **XBee Studio** o **XCTU** instalado
-  **Datos del APN** del operador de la SIM

---

## Parte 1: Instalación de XBee Studio

### Descarga e instalación

**1. Vaya al sitio web de Digi:**

- URL: https://www.digi.com/products/embedded-systems/digi-xbee/digi-xbee-tools/xbee-studio
- O busque "XBee Studio download".

**2. Seleccione su plataforma:**

- Windows (64 bits)
- macOS
- Linux

**3. Descargue el instalador.**

**4. Instale el programa:**

**Windows:**

- Ejecute el instalador .exe.
- Acepte la licencia.
- Elija la carpeta de instalación.
- Instale los controladores cuando se le solicite.
- Finalice la instalación.

**macOS:**

- Abra el archivo .dmg.
- Arrastre XBee Studio a Aplicaciones.
- En el primer inicio: clic derecho > Abrir (para superar Gatekeeper).
- Los controladores se instalan automáticamente (quizá deba reiniciar).


**Linux:**
```bash
# Extraer el archivo
tar -xzf XBeeStudio-linux-x64.tar.gz

# Ejecutar el instalador o la AppImage
./XBeeStudio.AppImage

# Controladores (si hacen falta)
sudo apt-get install libusb-1.0-0
```

### Comprobar la instalación

1. Abra XBee Studio.
2. Compruebe que aparece la pantalla de bienvenida.
3. Consulte la versión (Help > About): para XBee3 Cellular debería ser 1.4.0 o posterior.

---

## Parte 2: Conexión del hardware

### Preparación física

**1. Inserte la SIM (si aún no lo ha hecho):**

!!! warning "Desconecte primero la alimentación"
    ¡El XBee NO debe estar conectado a la alimentación ni al USB cuando inserte o retire la SIM!

- Localice la ranura SIM (parte inferior del XBee).
- Observe la orientación indicada en el módulo.
- Inserte la micro-SIM (tamaño 3FF).
- Presione hasta que encaje y quede al ras.
- Compruebe que esté bien asentada.

**2. Monte el XBee en el adaptador USB:**

- Alinee cuidadosamente los pines.
- Presione con firmeza hasta asentarlo.
- Todos los pines deben encajar.
- El módulo debe quedar plano sobre el adaptador.

**3. Conecte el cable USB:**

- Conecte el adaptador al puerto USB del ordenador.
- Espere a que se reconozca el controlador.

### Verificación de controladores

**Windows:**

1. Abra el Administrador de dispositivos (Win+X > Administrador de dispositivos).
2. Despliegue "Ports (COM & LPT)".
3. Busque "USB Serial Port (COMx)" y anote el número COM.
4. Si aparece "Unknown Device", reinstale XBee Studio (incluye controladores).

**macOS:**

1. Abra Terminal.
2. Enumere los dispositivos:
   ```bash
   ls /dev/tty.*
   ```
3. Busque `/dev/tty.usbserial-XXXXXXXX`.

**Linux:**

1. Compruebe el dispositivo:
   ```bash
   ls /dev/ttyUSB*
   ```
2. Añada su usuario al grupo dialout si no tiene permisos:
   ```bash
   sudo usermod -a -G dialout $USER
   # Cerrar sesión y volver a iniciarla
   ```

---

## Parte 3: Añadir el XBee a XBee Studio

### Paso 1: Añadir un dispositivo nuevo

**1. Abra XBee Studio.**

**2. Ventana principal:**

- Panel izquierdo: "Devices" (vacío al principio).
- Centro: indicaciones "Get Started".
- Derecha: paneles informativos.

**3. Haga clic en "+ Add Device"** (arriba a la izquierda o en el centro).

**4. Aparecerá el cuadro de conexión.**

### Paso 2: Seleccionar la conexión

**En el cuadro "Add Radio":**

**Tipo de conexión:**

- Seleccione **"USB"** (predeterminado).

**Puerto:**

- Windows: seleccione **"COM3"** (o el puerto COM que indique el Administrador de dispositivos).
- macOS/Linux: seleccione **/dev/ttyUSB0** (o el dispositivo correspondiente).

**Velocidad en baudios:**

- Deje **"9600"** (o "Auto" si está disponible).

!!! tip "¿No aparece el puerto?"
    - Haga clic en "Refresh".
    - Compruebe que el cable USB transmite datos y no solo carga.
    - Verifique que los controladores estén instalados.
    - Pruebe otro puerto USB.

**5. Haga clic en "Add Selected".**

### Paso 3: Primera lectura

**XBee Studio:**

- Se conecta al XBee.
- Lee la versión del firmware.
- Lee la configuración actual.
- Muestra información del módulo.

**Debería ver:**

- El dispositivo en el panel izquierdo.
- Datos del módulo (centro):
    - Modelo: XB3-C-A2-UT-001 (o similar).
    - Firmware: Cellular (p. ej., 11415).
    - Versión de hardware: 1E (o la actual).
- Pestañas de configuración.

**Si falla la conexión:**

- Revise el puerto COM elegido.
- Compruebe la velocidad (pruebe Auto o 9600).
- Verifique que el XBee esté bien encajado.
- Consulte la sección de solución de problemas.

---

## Parte 4: Pestañas de configuración

### Interfaz principal

**Panel izquierdo:**

- Lista de dispositivos añadidos.
- Haga clic en uno para seleccionarlo.

**Pestañas del panel central:**

**1. Radio Configuration**

- La pestaña más importante.
- Contiene todos los ajustes.

**2. Cellular Network**

- Estado de red.
- Intensidad de señal.
- Diagnóstico de conexión.

**3. MicroPython**

- Programación avanzada (innecesaria para el registrador).

**4. Firmware**

- Actualización del firmware.

**5. Terminal**

- Terminal serie para depuración.

**6. File System**

- Acceso a los archivos del XBee (avanzado).

### Recorrer la configuración de la radio

**Los ajustes se organizan en secciones desplegables:**

- **Addressing** (ID PAN, direcciones)
- **Serial Interfacing** (baudios, paridad)
- **I/O Settings** (configuración de pines)
- **Networking** (APN, conexión)
- **Security** (TLS, certificados)
- **RF Interfacing** (ajustes del módem celular)
- **Sleep Modes** (gestión de energía)
- **AT Command Options** (modo API)

**Cómo editarlos:**

1. Haga clic en una sección para desplegarla.
2. Haga clic en el ajuste que desea cambiar.
3. Introduzca un valor o elíjalo en la lista.
4. Los ajustes modificados aparecen en **negrita**.
5. Haga clic en **"Write"** para guardarlos en el XBee.

---

## Parte 5: Configuración imprescindible

### Resumen de los ajustes necesarios

**Objetivo:** configurar el XBee para:

1. Conectarse a la red celular.
2. Comunicarse por serie con Arduino.
3. Utilizar el modo API para transmitir datos.
4. Entrar en suspensión para ahorrar energía.

### Sección 1: Interfaz serie

**Despliegue "Serial Interfacing".**

!!! info "Ajustes serie críticos"
    Estos ajustes DEBEN coincidir exactamente para que el XBee se comunique con el registrador Riverlabs. Son los valores requeridos por la biblioteca xbee-arduino personalizada que utiliza el firmware.

| Ajuste | Parámetro | Valor | Descripción |
|--------|-----------|-------|-------------|
| **Velocidad en baudios** | BD | **9600** | Debe coincidir con el código Arduino |
| **Paridad** | NB | **0** (sin paridad) | Estándar |
| **Bits de parada** | SB | **1** | Un bit de parada |
| **Tiempo de empaquetado** | RO | **3** | Tiempo entre caracteres (3 ms) |
| **Entrega de transmisión** | TD | **0** | Desactiva el estado de entrega |
| **Umbral de control de flujo** | FT | **681** | Umbral del búfer UART |

**Cómo configurarlos:**

1. Haga clic en "BD (Baud Rate)".
2. Seleccione **"9600"**.
3. Establezca RO = **3**.
4. Establezca TD = **0**.
5. Establezca FT = **681** (hexadecimal: 0x2A9).
6. Compruebe NB = 0 y SB = 1 (valores normalmente predeterminados).
7. No pulse Write todavía (guarde todos los cambios juntos).

!!! warning "Se necesita la biblioteca xbee-arduino ampliada"
    El código Riverlabs utiliza una versión ampliada de la [biblioteca xbee-arduino](https://github.com/ICHydro/xbee-arduino) con funciones de módem celular. Está incluida en los paquetes de código Riverlabs y no necesita instalarla aparte.

### Sección 2: Red

**Despliegue "Network" o "Mobile Network".**

!!! danger "Ajustes críticos"
    ¡Un APN incorrecto impide la conexión celular!

| Ajuste | Parámetro | Valor | Descripción |
|--------|-----------|-------|-------------|
| **Nombre del punto de acceso** | AN | **[APN de su operador]** | Facilitado por el proveedor de SIM |
| **Usuario del APN** | AU | **[Si se requiere]** | Normalmente vacío |
| **Contraseña del APN** | AV | **[Si se requiere]** | Normalmente vacía |
| **Tecnología de red** | NT | **0** (preferencia LTE-M) | O 1 (NB-IoT) |

**APN habituales:**

| Operador | APN |
|----------|-----|
| AT&T (EE. UU.) | `m2m.com.attwireless.com` |
| T-Mobile (EE. UU.) | `iot.t-mobile.com` |
| Verizon (EE. UU.) | `vzwinternet` |
| Vodafone (UE) | `iot.vodafone.com` |
| Hologram | `hologram` |

**Ejemplo de configuración:**
```
AN = m2m.com.attwireless.com
AU = (vacío)
AV = (vacío)
NT = 0
```

### Sección 3: Modo API

**Despliegue "AT Command Options" o "API".**

| Ajuste | Parámetro | Valor | Descripción |
|--------|-----------|-------|-------------|
| **Activación de API** | AP | **2** | Modo API con escapes |
| **Modo de salida API** | AO | **0** | Estándar |

!!! danger "¡El modo API debe ser 2!"
    El firmware Riverlabs **requiere el modo API 2** (con caracteres de escape).
    
    - **Modo 0** (transparente) = NO funcionará.
    - **Modo 1** (API sin escapes) = NO funcionará.  
    - **Modo 2** (API con escapes) = ✅ **OBLIGATORIO**.
    
    Con otro modo, el registrador no podrá comunicarse con el XBee y no habrá datos de telemetría.

**Cómo configurarlo:**

1. Haga clic en "AP (API Enable)".
2. Seleccione **"2 - API mode with escaped characters"**.
3. Compruebe que AO = 0.

### Sección 4: Modo de suspensión

**Despliegue "Sleep Modes" o "Power Management".**

| Ajuste | Parámetro | Valor | Descripción |
|--------|-----------|-------|-------------|
| **Modo de suspensión** | SM | **1** | Suspensión por pin [1] |
| **Opciones de suspensión** | SO | **0** | Estándar |

!!! info "Se requiere suspensión por pin"
    **SM debe ser 1 (suspensión por pin)** para una gestión adecuada de la energía con el registrador Riverlabs.

**Funcionamiento:**

- Arduino controla mediante un pin cuándo entra en suspensión el XBee.
- Pin 9 (Sleep_RQ/DTR):
    - **Alto** = XBee en suspensión (bajo consumo).
    - **Bajo** = XBee activo (consumo normal).
- Reduce considerablemente el consumo entre transmisiones.
- Es fundamental para una buena autonomía.

**Cómo configurarlo:**

1. Haga clic en "SM (Sleep Mode)".
2. Seleccione **"1 - Pin Sleep [1]"**.
3. Compruebe que SO = 0.

!!! warning "No utilice otros modos de suspensión"
    - **SM = 0** (desactivado) agotará rápidamente la batería.
    - **SM = 4** (suspensión cíclica) no funciona con la lógica de control del registrador.
    - **SM = 5** (suspensión cíclica con despertar por pin) no es compatible.

### Sección 5: Otros ajustes (opcionales, pero recomendados)

| Ajuste | Parámetro | Valor | Descripción |
|--------|-----------|-------|-------------|
| **Tiempo de guarda** | GT | **3E8** (1000 ms) | Retardo del modo de comandos |
| **Umbral de control de flujo** | FT | **681** | Búfer UART |
| **Opciones de transmisión** | TO | **0** | Estándar |

---

## Parte 6: Guardar la configuración

### Escritura en bloque

**Tras configurar TODOS los ajustes anteriores:**

1. **Revise los cambios:**
    - Los ajustes modificados aparecen en **negrita**.
    - Compruebe los valores críticos.

2. **Haga clic en "Write"** (parte superior de "Radio Configuration").
    - Suele estar en la barra de herramientas.
    - O use clic derecho sobre el dispositivo > "Write Settings".

3. **Espere a que termine:**
    - Indicador de progreso.
    - Mensaje "Write successful".
    - Los ajustes dejan de aparecer en negrita.

4. **Compruébelo:**
    - Haga clic en "Read" para actualizar.
    - Verifique que los ajustes coincidan con los valores previstos.

!!! warning "Guarde antes de desconectar"
    Los cambios NO se guardan hasta hacer clic en Write. Si desconecta antes, se perderán.

### Perfil de ajustes (opcional)

**Guarde la configuración para reutilizarla:**

1. **File > Export Profile**.
2. **Guárdelo como:** `Riverlabs_XBee3_Config.xpro`.
3. **Para usarlo después:**
    - File > Import Profile.
    - Aplíquelo a nuevos módulos XBee.
    - Así mantendrá la misma configuración en todos ellos.

---

## Parte 7: Prueba de conexión a la red

### Panel Cellular Network

**1. Abra la pestaña "Cellular Network".**

**2. Estado actual:**

- Registro en red: no registrado / registrado.
- Intensidad de señal (RSSI): valor en dBm.
- Dirección IP: ninguna o dirección asignada.
- Estado de conexión: desconectado / conectado.

**3. Haga clic en "Connect to Network".**

**4. Espere la conexión:**

- Puede tardar 30–90 segundos.
- Aparecerán mensajes de progreso:
    - "Searching for network..."
    - "Network found"
    - "Registering..."
    - "Connected"

**5. Indicadores de éxito:**

- ✅ Registro en red: **sí**.
- ✅ Intensidad de señal: **-90 dBm o mejor** (menos negativo = mayor señal).
- ✅ Dirección IP: **asignada** (p. ej., 10.x.x.x).
- ✅ Estado: **conectado**.

**6. Referencia de señal:**

| RSSI (dBm) | Calidad | Utilidad |
|------------|---------|----------|
| -50 a -70 | Excelente | Perfecta para uso continuo |
| -70 a -90 | Buena | Fiable para la mayoría de las aplicaciones |
| -90 a -105 | Regular | Utilizable, pero puede dar problemas |
| -105 a -115 | Mala | Marginal; muchos fallos |
| < -115 | Muy mala | Poco fiable |

### Problemas de conexión

**"No network found":**

**Causas:**

- SIM sin activar.
- Tecnología de red incorrecta (LTE-M frente a NB-IoT).
- Señal insuficiente en el lugar.

**Soluciones:**

1. Verifique con el operador que la SIM esté activa.
2. Cambie el parámetro NT:
    - NT=0 (LTE-M).
    - NT=1 (NB-IoT).
    - Guarde e intente conectarse de nuevo.
3. Acérquese a una ventana o salga al exterior.
4. Compruebe la antena externa, si existe.

**"Network found but registration failed":**

**Causas:**

- APN incorrecto.
- Problema de autenticación de la SIM.

**Soluciones:**

1. Compruebe la escritura del APN (¡distingue mayúsculas y minúsculas!).
2. Revise las credenciales AU/AV.
3. Pida al operador los datos del APN.
4. Pruebe el APN predeterminado (a menudo funciona).

**"Registered but no IP address":**

**Causas:**

- Fallo DHCP.
- Congestión de la red.

**Soluciones:**

1. Desconecte y vuelva a conectar.
2. Espere 2–3 minutos.
3. Reinicie el XBee (desconecte y vuelva a conectar el USB).
4. Consulte al operador por si hay problemas con la cuenta.

---

## Parte 8: Prueba de transmisión de datos

### Pestaña Terminal

**Pruebe un POST HTTP** para verificar la configuración completa:

1. **Abra la pestaña "Terminal".**

2. **Compruebe la conexión a la red** (paso anterior).

3. **Introduzca comandos API** (usuarios avanzados).

**Como alternativa, pruebe desde Arduino:**

1. **Retire el XBee del adaptador USB.**
2. **Instálelo en el registrador.**
3. **Cargue un sketch de prueba** (o el código definitivo).
4. **Conecte el monitor serie.**
5. **Observe los intentos de transmisión.**

**Secuencia esperada:**
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

**Si falla la transmisión:**

- Consulte [Solución de problemas de conexión](troubleshooting-connections.md).

---

## Parte 9: Finalizar la configuración

### Prueba de reinicio eléctrico

**Compruebe que se conserva la configuración:**

1. **Desconecte el USB** (retire el XBee del adaptador).
2. **Espere 10 segundos.**
3. **Reconecte el USB** (reinserte el XBee).
4. **Abra XBee Studio y añada el dispositivo.**
5. **Lea la configuración.**
6. **Compruebe todos los ajustes.**

Si se han restablecido:

 - La configuración no se guardó correctamente. Repita la parte 6.

### Etiquetar el módulo XBee

**Documente la configuración:**

- Use una etiquetadora o un rotulador permanente.
- Anote en el XBee (lejos de la antena):
    - "Configured DD/MM/YY"
    - "Logger: Wari-001"
    - El APN (si resulta útil).
- Evitará confundirlo con módulos sin configurar.

### Instalación en el registrador

**1. Apague el registrador** (desconecte la batería).

**2. Instale el XBee:**

- Alinee el pin 1 (busque la marca en la PCB).
- Presione suavemente, pero con firmeza.
- Compruebe que todos los pines encajen.
- El módulo debe quedar plano.

**3. Revise la antena:**

- Antena externa: conecte U.FL o equivalente.
- Antena integrada en la PCB: deje espacio libre (sin contacto con la carcasa).

**4. Reconecte la batería.**

**5. Compruebe el arranque:**

- El registrador arranca.
- El XBee se inicializa.
- La conexión a la red se establece en 60 segundos.

---

## Parte 10: Solución de problemas

### XBee Studio no detecta el módulo

**Síntoma:** no encuentra el dispositivo al añadirlo.

**Causas y soluciones:**

| Causa | Comprobación | Solución |
|-------|--------------|----------|
| Puerto incorrecto | Administrador de dispositivos (Windows) | Seleccione el puerto COM correcto |
| Velocidad incorrecta | Configuración previa de BD | Pruebe 9600, 115200 o Auto |
| Problema de controlador | Error en el Administrador de dispositivos | Reinstale XBee Studio y reinicie |
| Cable USB defectuoso | Cable solo de carga | Use un cable USB de datos |
| XBee mal asentado | Inspección física | Vuelva a encajarlo en el adaptador |
| Adaptador sin alimentación | LED del adaptador | Pruebe otro puerto USB o concentrador |

### No se guardan los ajustes

**Síntoma:** la configuración se restablece tras apagar y encender.

**Causas:**

- No se pulsó "Write".
- Falló la escritura (sin mostrar error).
- Firmware dañado (raro).

**Soluciones:**

1. Configure los ajustes.
2. Haga clic en "Write".
3. **Espere la confirmación.**
4. Haga clic en "Read" para verificar.
5. Apague y encienda, y vuelva a leer.
6. Si se siguen restableciendo, actualice el firmware (véase más abajo).

### Intensidad de señal deficiente

**Síntoma:** RSSI < -105 dBm.

**Soluciones:**

1. **Antena:**
    - Compruebe que esté conectada.
    - Pruebe una externa si utiliza la interna.
    - Revise posibles daños.
2. **Ubicación:**
    - Acérquelo a una ventana.
    - Eleve el dispositivo.
    - Aléjelo de estructuras metálicas.
3. **Red:**
    - Pruebe otra tecnología (NT=0 frente a NT=1).
    - Consulte el mapa de cobertura del operador.
    - Pruebe la SIM en un teléfono en el mismo lugar.

### Actualización del firmware

**Cuándo hacerlo:**

- Correcciones de Digi.
- Nuevas funciones.
- Mejoras de compatibilidad.

**Cómo actualizar:**

1. **Descargue el firmware:**
    - https://www.digi.com/support/productdetail?pid=5817
    - Seleccione XBee3 Cellular LTE-M/NB-IoT.
    - Elija la versión más reciente (p. ej., 11415 o posterior).

2. **En XBee Studio:**
    - Abra la pestaña "Firmware".
    - Haga clic en "Update Firmware".
    - Seleccione el archivo descargado (.ebin).
    - Haga clic en "Update".
    - Espere (5–10 minutos; **no desconecte el módulo**).
    - Compruebe el mensaje de confirmación.

3. **Verifique:**
    - Compruebe la versión de firmware (pestaña Radio Configuration).
    - Vuelva a configurar los ajustes (la actualización puede restablecerlos).

## Siguientes pasos

- [Configuración de ThingsBoard](thingsboard-configuration.md): preparar el panel
- [Solución de problemas de conexión](troubleshooting-connections.md): resolver problemas de telemetría
- [Guía de configuración de telemetría](setup-guide.md): visión general de la instalación
- [Guía de baterías y alimentación](../operation/hardware/battery-power-guide.md): optimizar la telemetría

---

!!! success "¡XBee configurado!"
    El XBee ya está configurado y listo para instalarlo en campo. Guarde el perfil de configuración para futuros módulos.
