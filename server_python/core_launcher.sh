#!/bin/bash
# -*- ENCODING: UTF-8 -*-

result=$(ps -A | grep "persia")

case $result in
		*persia*)
		      	if [[ $result == *"My long"* ]]; then
  				echo "OK"
			fi  
                    	
			;;
			

		*)
			
			echo “Se ha iniciado el progrograma Persia”
			python /home/pi/Desktop/repositorios/persia/server_python/persia_core.py &
			;;
	esac
