#! /bin/sh
# /etc/init.d/persianas-init
 
### BEGIN INIT INFO
# Provides:          detector-init
# Required-Start:    $all
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Script de ejemplo de arranque automático
# Description:       Script para arrancar el detector de presencia
### END INIT INFO
 
/var/www/html/temp/python/temp_rpi_arduino.py
 
# Dependiendo de los parámetros que se le pasen al programa se usa una opción u otra
case "$1" in
  start)
    echo "Arrancando persianas-init"
    # Aquí hay que poner el programa que quieras arrancar automáticamente
    /var/www/html/temp/python/./temp_rpi_arduino.py
    ;;
  stop)
    echo "Deteniendo persianas-init"
    kill $(echo $(ps ax | grep temp_rpi_arduino.py ) | cut --fields=1 -d" ")
    ;;
  *)
    echo "Modo de uso: /etc/init.d/persianas-init {start|stop}"
    exit 1
    ;;
esac
 
exit 0
