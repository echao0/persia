#! /bin/bash
### BEGIN INIT INFO
# Provides: Temp_Arduino
# Required-Start: $local_fs $remote_fs $network
# Required-Stop: $local_fs $remote_fs $network
# Default-Start: 2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: Script para controlar desde Arduino Temp desde la Rpi.
# Description: by _echao_
### END INIT INFO

case $1 in
start)
PROCESS=$(ps -ef | grep "Temp_Rpi_Arduino" | grep -v "grep" | awk '{print $2}')

echo $PROCESS

if [ $PROCESS‚> "1"‚ ];

then

echo ‚El servicio Temp_Tpi_Arduino ya esta ejecutandose

else

#sudo python /var/www/html/temp/python/Temp_Rpi_Arduino.py &
echo Menor
fi

;;
stop)
PROCESS=ps -ef | grep "Temp_Rpi_Arduino" | grep -v "grep" | awk '{print $2}'
sudo kill $PROCESS
;;
esac
exit 0
