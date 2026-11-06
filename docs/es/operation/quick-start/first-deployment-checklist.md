# Lista de verificación para la primera instalación

## Objetivo

Esta lista te ayuda a comprobar que el registrador está listo para instalarse.

### Por dónde empezar

Para comprobar el funcionamiento del registrador, pulsa el botón de reinicio. No hay riesgo de perder datos ni de cambiar ningún ajuste: reiniciar el registrador simplemente vuelve a iniciar su software interno.

Cuando el registrador se inicia o reinicia, realiza algunas comprobaciones internas, como verificar el acceso a la tarjeta SD o al módem de telemetría. Si las comprobaciones se completan correctamente, el LED rojo parpadeará una vez. Normalmente se trata de un destello breve (< 1s), pero puede durar más si el registrador encuentra datos en la memoria interna y los vuelca en la tarjeta SD (consulta la sección de [gestión de datos](../hardware/data-handling.md)). Cuando termina, pulsar de nuevo el botón de reinicio produce un destello breve, porque todos los datos ya se han volcado.

Una secuencia de 3x3 destellos indica que hay un problema.

Si no se produce ningún destello rojo, el registrador no arranca, probablemente debido a un problema de alimentación.
x
### Batería y alimentación

- El registrador se enciende (se observa actividad en el LED)
- La batería está completamente cargada y se ha comprobado la polaridad. (Consulta la [guía de baterías y alimentación](../hardware/battery-power-guide.md) para interpretar la tensión).
- La pila de respaldo CR1220 está instalada
- No hay conexiones sueltas ni piezas que se muevan dentro de la carcasa
- Si se utiliza un panel solar, comprueba que esté correctamente conectado y que el LED de carga se encienda con la luz solar.

### Tarjeta SD (si corresponde)

- La tarjeta microSD está bien colocada en la ranura
- No se observan patrones de error en el LED (3 x 3 destellos)

### Carcasa del registrador

- La carcasa está intacta, sin grietas ni daños
- Las juntas y sellos están en buen estado
- Los prensaestopas están bien apretados
- El sensor está limpio y no presenta daños
- La lente o el transductor del sensor no tiene residuos
- Todos los tornillos y elementos de fijación están apretados
- Hay un sobre de gel de sílice en buen estado

### Telemetría (si corresponde)

- El módulo XBee está instalado y bien sujeto
- XBee está configurado en modo API con caracteres de escape
- La configuración APN de la red es correcta para el proveedor de la SIM
- Se han introducido las credenciales del dispositivo de ThingsBoard
- El intervalo de telemetría es adecuado para la duración de la batería
- Hay una tarjeta SIM instalada con un plan de datos activo
- La transmisión de prueba se ha realizado correctamente

Consulta la [guía de configuración de telemetría](../../telemetry/setup-guide.md) para obtener instrucciones detalladas.

### Salida del registrador

Puedes comprobar la salida del registrador leyendo la tarjeta SD, verificando los datos recibidos por telemetría o conectando un cable serie y observando los mensajes de depuración. Se recomienda dejarlo funcionar durante 30 minutos y recoger varias lecturas antes de llevarlo al lugar de instalación.

- El reloj indica la hora correcta
- Las mediciones del sensor devuelven lecturas válidas
- Las lecturas son coherentes y razonables
- Varias lecturas muestran poca variación
- El LED parpadea a los intervalos esperados
- La tarjeta SD contiene un archivo de datos
- El archivo de datos puede leerse y tiene el formato correcto
- Los valores del sensor están dentro del rango esperado
- No faltan registros ni hay entradas dañadas

**Para Wari ultrasónico:**

- Probar objetivos a varias distancias (0.5 m, 1 m, 2 m, etc.)
- Verificar que los objetos cercanos no causen interferencias
- Comprobar que el haz tenga una trayectoria despejada

**Para Wari lidar:**

- Probar con distintos tipos de superficie
- Verificar las mediciones angulares (si se usan)
- Confirmar las lecturas desde 10 cm hasta el alcance máximo esperado


## Referencia rápida para solucionar problemas

Si surgen problemas, consulta:

- [Problemas comunes](../troubleshooting/common-issues.md): soluciones rápidas
- [Diagramas de diagnóstico](../troubleshooting/diagnostic-flowcharts.md): diagnóstico sistemático
- [Preguntas frecuentes](../troubleshooting/faq.md): respuestas a preguntas habituales

**Problemas comunes antes de la instalación:**

| Síntoma                 | Causa probable                  | Solución rápida                                         |
|------------------------|---------------------------------|---------------------------------------------------------|
| El LED no se enciende  | Batería agotada o invertida     | Comprobar la tensión y la polaridad                      |
| Lecturas erráticas     | Sensor obstruido                | Limpiar el sensor y comprobar la trayectoria del haz     |
| No se escribe en la SD | Fallo de la tarjeta SD          | Comprobar la tarjeta; considerar respaldo en memoria flash |
| El reloj se reinicia  | Falta la pila de respaldo       | Instalar una CR1220                                      |
| No hay telemetría      | XBee no está configurado        | Verificar el modo API y los ajustes                      |

## Siguientes pasos

- Continúa con la [guía de montaje](../../installation/mounting-guide.md)
- Configura la supervisión en [ThingsBoard](../../telemetry/thingsboard-configuration.md)
- Revisa el [programa de mantenimiento](../hardware/maintenance.md)
