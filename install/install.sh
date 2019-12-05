#!/bin/bash

lista="apache2 php mariadb-server python2.7 python-mysqldb"

clear

red=`tput setaf 1`
green=`tput setaf 2`
reset=`tput sgr0`

need="" 

if ! [ $(id -u) = 0 ]; then 
	echo "${red}---------------------------"
        echo ""
	echo "${red}I need to be an admin user" 
	echo ""
	echo "${red}---------------------------"
        exit 1 
fi

#-----------------------------------
#Funcion para comprobar si el paquete está instalado
#------------------------------------

echo "Testing if all needed packets are installed"
echo ""

function test {

	status=$(dpkg -s $1 2>&1 | grep "Status" | awk '{print $2}')

	
	#Comprobar si la variable está vacia (paquete nunca instalado

	if [[ -z "$status" ]]
	then
		status="nothing"
	fi

	#Comprobar si el paquete está instalado.

	if [ $status = "install" ]; then

		echo "${green}  Package" $1 "is installed!"
	else
		echo "${red} Package" $1 "is NOT installed!"
                need+=" "$1
	fi

}

function test_ori {

	dpkg -s $1 &> /dev/null

	if [ $? -eq 0 ]; then
    		echo "${green}  Package" $1 "is installed!"
	else
    		echo "${red} Package" $1 "is NOT installed!"
    		need+=" "$1
	fi
}

#-----------------------------------
#Bucle para comprobar paquetes
#------------------------------------


for variable in $lista
do
        test $variable
done

if [[ -z "$need" ]]
then
	echo ""
	echo "Everything it's ok!"
	echo ""
else
 
	echo ""
	echo ""
	#read -p "Do you want to install the missing packets? " -n 1 -r

	echo    # (optional) move to a new line

	#if [[ $REPLY =~ ^[Yy]$ ]]
	#then


		echo ""
		echo ""
		echo ""	
		echo "---------------------------------------"
		echo "Instaling needed packets"
		echo "---------------------------------------"

		for variable in $need
		do
			echo "${green}"
			echo ""
			echo "---------------------------------------"
        		echo "INSTALLING " $variable
			echo "---------------------------------------"
			echo ""
			apt install $variable -y
		done


#-----------------------------------
#Revision final de estado de paquetes
#------------------------------------

		echo "${green}"
		echo "---------------------------------------"
		for variable in $lista
		do
	        	test $variable
		done

		echo ""
		echo "---------------------------------------"

	#fi
fi
