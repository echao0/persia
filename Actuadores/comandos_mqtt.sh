#!/bin/bash
# Bash Menu Script Example

PS3='Please enter your choice: '
options=("persia/log" "persia/temp" "topic" "order" "info-general" "getInfo" "Quit")
select opt in "${options[@]}"
do
    case $opt in
        "persia/log")
	    clear
            echo "persia/log"
	      set $(cat pass.txt) && mosquitto_sub -h 127.0.0.1 -t persia/log -q 2 -u $1 -P $1
            ;;
        "persia/temp")
            clear
            echo "persia/temp"
           set $(cat pass.txt) && mosquitto_sub -h 127.0.0.1 -t persia/temp -q 2 -u $1 -P $1
            ;;
        "topic")
        clear
        echo "nombre del topic"
        read varname
        set $(cat pass.txt) && mosquitto_sub -h 127.0.0.1 -t $varname -q 2 -u $1 -P $1
        ;;

        "order")
        clear
        echo "nombre del dispositivo"
        read name
        echo "nombre del orden (getInfo , getTemp, restart, test, up, down, stop, )"
        read order

        set $(cat pass.txt) && mosquitto_pub -h 192.168.3.181 -d -t persia/$name/order -m "{\"order\": \"$order\",\"value2\": 40}" -u $1 -P $1
        ;;


        "info-general")
            clear
            echo "you chose choice $REPLY which is $opt"
            set $(cat pass.txt) && mosquitto_pub -h 192.168.3.181 -d -t persia/general -m "{\"order\": \"getInfo\",\"value2\": 40}" -u $1 -P $1
            ;;

        "getInfo")
        clear
        echo "nombre del dispositivo"
        read varname
        set $(cat pass.txt) && mosquitto_pub -h 192.168.3.181 -d -t persia/$varname/order -m "{\"order\": \"getInfo\",\"value2\": 40}" -u $1 -P $1
        ;;

        "Quit")
            break
            ;;
        *) echo "invalid option $REPLY";;
    esac
done
