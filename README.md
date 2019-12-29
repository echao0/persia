# persia
Proyecto IoT personal de automatización de hogar y conectividad con Amazon Alexa para control por voz.
El proyecto está basado en modulos ESP8266 como dispositivos finales y con posibilidad de actualización OTA, 
Rpi como servidor central donde se aloja la base de datos en SQL + el core en Python + la web en PHP, 
Javascript utilizada como gestor multiplataforma.
Para la conexión con Amazon Alexa, se han utilizado LAMBDA de AMAZON AWS creando nuevos Skill para Amazon ECHO. 
Ademas de la adaptación de ESP8266 como dispositivo reconocible por Alexa.
Actualmente está completada la conversión de la calefacción para control por voz total y control de temperatura en el hogar, 
también la automatización de persianas así como la actuación automática bajo programación.

Actualmente esta abierto el proyecto para servidor MQTT así como la creación de un contenedor en Docker para facilitar la instalación de todo lo necesario.
