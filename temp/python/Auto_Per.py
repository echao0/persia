#!/usr/bin/env python

#application: [CONTROL DE ARDUINO TEMP]
#version: 5.1
#runtime: python
#api_version: 3
#autor: _echao_
#
#descripcion: Conexion con codigo_envio_temperatura , conexion con SQL y graba datos cada x minutos ;
#			  Cada noche a las 00:00 borra la base de datos parciales y guarda el max y el min en la tabla historico
#			  Se pueden pasar los siguiente argumentos
#					-v 		Vervose mode
#					-t 10	Tiempo en segundos de espera (aun no configurado)	
#
#
#V2   Insertar con fecha a la que se ha producido el max y el min y parcial del dia
#V3   Incorpora el control de arduino alive, actualiza el estado en la DB Temperatura.Dispositivos
#V4	  Corrige el error al introducir la temperatura media, no introducia fecha (linea 127)
#	  Corrige el error al introducir el status del dispositivo, no introducia fecha (linea 179, 187)
#	  Anado funcion date_time(), que devuelve la fecha actual (null) o la de ayer (1)
#	  Anado la ip del dispositivo cuando actualizo en DB el estado ONLINE - OFFLINE
#V5	  Creo funcion get_insert_temp(): para recoger e insertar la informacion de la temperatura cuando el dispositivo esta ONLINE
#	  Solo actualizo la temperatura en DB.Parciales si arduino esta ONLINE			  

#data = raw_input("Selecciona una de las opciones : ");
                
from socket import *
import SocketServer
import socket

from decimal import Decimal
import time 									# Se importa para poder leer la hora
import os 										# Se importa para poder borrar pantalla de linux
import sys 										# Se importa para poder pasar paremetros en la llamada
import MySQLdb 									# Server Connection to MySQL
import datetime									# Para poder consrguir la fecha de ayer
import threading								# Para poder realizar varios Hilos

import argparse                                                                 #biblioteca para argumentos
 
parser = argparse.ArgumentParser()
parser.add_argument("-v", "--verbose", action="store_true")
python_args = parser.parse_args()
    
#------- Configuracion IP Arduino ---------- #
#antes del inicio del programa llamo a la funcion ip_dispositivos  que consulta en SQL las ip
ip = '192.168.3.177'  #Direccion ip del arduino de momento se usa para update de estado<s

ip_disp1 = 0
ip_disp2 = 0
ip_disp3 = 0

disp1_move = 0
disp2_move = 0
disp3_move = 0

port = 5000											#Puerto de comunicacion con Arduino


#------- Configuracion IP Servidor Python ---------- #
server_host= '127.0.0.1'
server_port=int(2000)								#Puerto del servidor
server_on = True									#Variable de control de flujo

#------- Configuracion SQL ---------#

DB_HOST = 'localhost' 
DB_USER = 'root' 
DB_PASS = 'eudora' 
DB_NAME = 'temperatura'
DB_TABLE = 'parciales'
DB_TABLE_SAVE = 'historico'

datos_sql = [DB_HOST, DB_USER, DB_PASS, DB_NAME]	#Array de tados SQL

#------- Configuracion de tiempo -------#

time_triger = 10									#Tiempo en minutos
sleep_time = 0.5 									#Tiempo en segundos de espera por cada bucle de programa (control de CPU)
time_alive = 5										#Tiempo para latido de vida de Arduino

#------- Adecuar el valor de minutos a segundos

time_triger = time_triger * 60

status = "offline"									#variable para saber estado de dispositivo actual (se evita la reescritura en SQL)


#------- Declaracion de Classes ----#
#-----------------------------------#
class timer():
    #cada vez que se manda el start, se reinicia el contador.
    
    status=False                        #Variable para saber si hay que iniciar o reiniciar el temorizador
    wtime=False                         # Cuanto tiempo debe estar esperando el temporizador 
    name=False                          #Nombre asignado al temporizador
    step=0.25                           #cada cuanto tienpo se comprueba no se reinicia el bucle
    i=0                                 #Variable usada para el bucle de tiempo de espera
    disp= False                         #Variable que indica que ip tiene el dispositivo a controlar
    global python_args
    
    def __init__(empty):
        if python_args.verbose:
            print "Clase timer creada" 
        
    def start(self):
        
        if (self.status == False):
            t = threading.Thread(target=self.timer_start)
            t.start()
        else:
            if python_args.verbose:
                print self.name + " reinicio"
            self.i=0
            
    def stop(self):
        if (self.status == False):
            self.i=0
        else:
            self.i=5000
            
    def timer_start(self):
        self.status = "working"
        
        self.wtime = self.wtime -1 #Resto 1 para compensar bug de 1 segundo mas.    
        
        while (self.i <= (self.wtime * 4)):
            time.sleep(self.step)
            self.i=self.i+1
            
        if python_args.verbose:    
            print self.name + " Finalizado tiempo esperando: " + str(self.wtime)
            
        ardu_output("z",self.disp)     #Envio paro a arduino
       
        #Reinicio las variables de control de tiempo
        self.i=0
        self.status = False
        
    def continous(self,funtion):
        
        while server_on:
            print "siempre"
            funtion
            time.sleep(self.wtime)
            
        
        
def ip_dispositivos():
    
    global ip_disp1
    global ip_disp2
    global ip_disp3
    
    sql = "SELECT `ip` FROM `dispositivos` order by `id` asc"
    result = db_conexion(sql)
                                                                                #    El dispositivo 0 es el servidor, por lo que no lo tengo en cuenta.
    ip_disp1 = result[1]
    ip_disp2 = result[2]
    ip_disp3 = result[3]

def time_move():
    
    global disp1_move
    global disp2_move
    global disp3_move
    
    sql = "SELECT `move` FROM `dispositivos` order by `id` asc"
    result = db_conexion(sql)
                                                                                #    El dispositivo 0 es el servidor, por lo que no lo tengo en cuenta.
    disp1_move = result[1]
    disp2_move = result[2]
    disp3_move = result[3]
    
    
        
class ServerHandler (SocketServer.BaseRequestHandler):
	global python_args
        
	def selec(self, data):
            
            if (data == "1"):
                if python_args.verbose:
                    print "Timer1 iniciado"
                timer1.start()
                
                
            if (data == "2"):
                if python_args.verbose:
                    print "Timer2 iniciado"
                timer2.start()
                
                
            if (data == "3"):
                if python_args.verbose:
                    print "Timer3 iniciado"
                timer3.start()
                
                
        def handle (self):
                global ip_disp1
                global ip_disp2
                global ip_disp3
                
		self.data = self.request.recv(1024).strip()
	
		if self.data == "beat":						#Control para no imprimir en pantalla latidos
			self.request.send(str("ack"))
                        
		else:	
                        datos = self.data.split(",")                            #separo la cadena y formo una lista datos[1] = persiana datos[0] = accion
                        
                        if python_args.verbose:
                            print "Persiana: " , datos[1]
                            print "Accion: " , datos[0]
                        
                        if datos[1] == "1":
                            disp = ip_disp1
                        if datos[1] == "2":
                            disp = ip_disp2
                        if datos[1] == "3":
                            disp = ip_disp3
                        
			if datos[0] == "abajo":
				self.request.send(str("Bajando"))
                                ardu_output("b",disp)
                                self.selec(datos[1])
                                
                        if datos[0] == "Subir":
				self.request.send(str("Subiendo"))
                                ardu_output("s",disp)
                                self.selec(datos[1])
                                
                        if datos[0] == "stop":
				self.request.send(str("Paro"))
                                ardu_output("z",disp)
                                self.selec(datos[1])
                                
                        if datos[0] == "update":
                                self.request.send(str("Actualizando"))
				update();
		
class miserver():
	global server_on
	global python_args
        global python_args
        
        
	def __init__(empty):
            if python_args.verbose:
		print "La clase" , "esta iniciada"
		
	def start(self):
	
		t = threading.Thread(target=self.server_start)					#Creo el hilo para la escucha de puerto
		t.start()														#Inicio el hilo de la escucha de puerto
		
                if python_args.verbose:
                    print "Servidor iniciado y a la espera"
		
	def server_start(self):																#Funcion de inicio y control de puerto de escucha
		server = SocketServer.TCPServer((server_host,server_port),ServerHandler)		#Creo el objeto servidor
		server.socket.settimeout(5.0)													#Selecciono un timeout del servidor de 5 segundos (evito fallo de cierre de server)
		
		while server_on:																#Control de variable de cierre de hilo principal
			server.handle_request()
		else:
                        server.socket.shutdown()
                        server.socket.close()
        
        def server_start2(self):
                server = SocketServer.TCPServer((server_host,server_port),ServerHandler)
                server_thread = threading.Thread(target=server.serve_forever)
                server_thread.daemon = True
                server_thread.start()    
                
	 
#------- Declaracion de funciones --#
#-----------------------------------#



def db_conexion(sql):						#Funcion para insertar/ rescatar datos en SQL


	if python_args.verbose:	#Mostrar la consulta si esta en modo vervose
            print sql
		
	db = MySQLdb.connect(*datos_sql)		#Ejecutar la consulta de SQL
	cursor = db.cursor()
	cursor.execute(sql)
	db.commit()
	db.close()
    
	fields=cursor.fetchall()				#Particionar el resultado de la consulta
	result = []								#Crear el array que devuelve la funcion
	
	for field in fields:
		result.append(field[0])				#Guardar todos los datos particionados en el array

	return result							#Fin de la funcion y retorno de los datos

def average_temp():							# funcion para sacar la media diaria antes del borrado
	
	sql = "SELECT valor FROM " + DB_TABLE 	#Sacar todos los valores de la tabla parciales
	result = db_conexion(sql)
	
	total_line = len(result) 				#saber la longitud del array retornado
	
	id = 0
	total = 0
	
	while id < total_line :					#sumar todos los resultados en una unica variable
		total = result[id] + total
		id = id +1
	
	average = total / total_line			#Sacar la media dividiendo la suma total / el numero de resultados
	
	return average							#Retornar el array con el valor resultado
						
def date_time(method='0'):					#funcion que devuelve fechas, por defecto la actual . 1 = la fecha de ayer
	
	if method == '1':
	
		yesterday = datetime.date.today () - datetime.timedelta (days=1)
		return yesterday.strftime ("%Y-%m-%d %H:%M:%S")
		
	else :
	
		return time.strftime('%Y-%m-%d %H:%M:%S')
	
def get_insert_temp(time_starting_point):

	#----- Introducir los valores de temperatura en la tabla parciales ---------------------#
	#---------------------------------------------------------------------------------------#
	
	elapsed_time = time.time () - time_starting_point			#saber cuanto tiempo ha trascurrido desde el ultimo dato introducido
	elapsed_time_int = int(elapsed_time)
	
	if elapsed_time_int > time_triger:							#Comparar el tiempo trascurrido desde que se introdujo el ultimo dato y el tiempo definido
		
		time_starting_point = time.time()						#Guardo el tiempo de el ultimo dato insertado en DB
		
		#rec_data, addr = ardu_output("t",ip_disp1)                                            #Le pido a arduino la temperatura actual esp8266
                if (rec_data != 0 ):
    
                        temp = float(rec_data) 								#Convert string rec_data to float temp
			sql = query = "INSERT INTO " + DB_TABLE + " (valor) VALUES ('%s')" % temp
			db_conexion(sql) 									#Insertar valor de temperatura en SQL 
			return 1			
		else:
			pass	
	
        
def ardu_output(data,disp):
	
        addr = disp;
        client=socket.socket(socket.AF_INET,socket.SOCK_STREAM);
        
        try:
            client.connect((disp,5000)); 
            time.sleep(0.11);
            try:
                client.send(data);
            except:
                if python_args.verbose:
                     print "el cliente " + disp + " esta OFFLLINE"
                return 0	
                pass
        except:
            if python_args.verbose:
                print "el cliente " + disp + " esta OFFLLINE"
            return 0	
            pass
        
        try:	
		#rec_data, addr = client.recv(2048) 		#Read response from arduino , wait for ok
                rec_data = client.recv(2048)                                #Read response from arduino , wait for ok
                return rec_data, addr	
	except:												#No me ha respondido arduino						
		return 0	
		pass
		
def update():
    
    ip_dispositivos()
    time_move()
    
    timer1.disp = ip_disp1
    timer2.disp = ip_disp2
    timer3.disp = ip_disp3
    
    timer1.wtime = disp1_move
    timer2.wtime = disp2_move
    timer3.wtime = disp3_move
    if python_args.verbose:
        print "Se han actualizado los valores de los dispositivos."
    
def latido(ip_disp, status):
      
     if (ardu_output("p",ip_disp) != 0 ):                                    #Mandar p a Arduino y nos responde con su id
                        	
	if status == "offline":                                              #escribo la entrada en SQL solo si estaba offline
				
            sql = "UPDATE `dispositivos` SET `status`= 'ONLINE' , `ip` = '"+ ip +"' , `Time` = '" + date_time() + "' WHERE `id` = '1'"
            db_conexion(sql)
            status = "online"
            return  status
#            if get_insert_temp(time_starting_point):			#Llamo a funcion de introducir DB.parciales y actualizo hora de entrada para siguiente control
#            time_starting_point = time.time()
				
	else:													#Si no hay respuesta salta el error y sabemos que esta en offline
				
            if status == "online":								#Escribo la entrada en SQL solo si estaba online
																	
		sql = "UPDATE `dispositivos` SET `status`= 'OFFLINE' , `ip` = '"+ ip +"' , `Time` = '" + date_time() + "' WHERE `id` = '1'"
		db_conexion(sql)
		status = "offline" 
                return  status
        
#------- Declarar variables de control tiempo -------#
#----------------------------------------------------#

time_starting_point = time.time()                                                       #Tomar el tiempo de inicio de ejecucion de programa para triger
time_starting_point_alive = time.time()							#Tomar el tiempo de inicio de ejecucion de programa para triger de alive

		
#------- Inicio del programa -------#
#-----------------------------------#


ip_dispositivos()                                                       #Rescatar de la base de datos las ip de los dispositivos 
time_move()

#client_socket = socket(AF_INET, SOCK_DGRAM)                             #Set up the Socket
client_socket=socket.socket(socket.AF_INET,socket.SOCK_STREAM);
#client_socket = socket(socket.AF_INET,socket.SOCK_STREAM)
client_socket.settimeout(1) 						#Only wait 1 second for a response
address = (ip_disp1 , port)						#define arduino IP and port


os.system('clear') 													#Borrado de pantalla
servidor1 = miserver()                                                                                                  #Declaro que quiero un servidor
servidor1.start()													#Inicio el servidor

timer1=timer()
timer1.name="Timer1"
timer1.wtime = disp1_move
timer1.disp = ip_disp1

timer2=timer()
timer2.name="Timer2"
timer2.wtime = disp2_move
timer2.disp = ip_disp2

timer3=timer()
timer3.name="Timer3"
timer3.wtime = disp3_move
timer3.disp = ip_disp3


 
try:										
	while(1):
			
		
	#----- Preparar valor maximo, medio y minimo en la tabla historico a las 23:59:58 ---------#
	#------------------------------------------------------------------------------------------#
		#get_insert_temp()									#Recoger e insertar la temperatura en la tabla de parciales
		
#		if time.strftime("%S") >= "58" and time.strftime("%S") <= "59" :
#			if time.strftime("%M") == "59": 
#				if time.strftime("%H") == "23":
#					
#					sql = "INSERT INTO " + DB_TABLE_SAVE + " (Valor, fecha) select valor , fecha from " + DB_TABLE + "  where valor in (select max(valor) from " + DB_TABLE + " ) limit 1"
#					db_conexion(sql) 								#Insertar valor MAX de temperatura en SQL
#					
#					sql = "UPDATE " + DB_TABLE_SAVE + " SET `type`='max' ORDER BY id DESC LIMIT 1"	
#					db_conexion(sql)								#Insertar el valor en la columna type de SQL
#
#					sql = "INSERT INTO " + DB_TABLE_SAVE + " (Valor, fecha) select valor , fecha from " + DB_TABLE + "  where valor in (select min(valor) from " + DB_TABLE + " ) limit 1"
#					db_conexion(sql) 								#Insertar valor MIN de temperatura en SQL
#					
#					sql = "UPDATE " + DB_TABLE_SAVE + " SET `type`='min' ORDER BY id DESC LIMIT 1"	
#					db_conexion(sql)								#Insertar el valor en la columna type de SQL
#					
#					sql = "insert into " + DB_TABLE_SAVE + " (valor, type, fecha) values (" + str(average_temp()) + " , 'med', '" + date_time('1') + "' )"
#					db_conexion(sql)								#Insertar valor MED de temperatura en SQL
#					
#					sql = "truncate " + DB_TABLE
#					db_conexion(sql)								#Borrar la tabla de parciales de SQL
#					
#					time.sleep(1)
					
		
	#------------- Comprobar tiempo para saber si arduino esta vivo ------------------------#
	#------------- Insertar temperatura en tabla parciales ---------------------------------#


		elapsed_time_alive = time.time () - time_starting_point_alive			#saber cuanto tiempo ha trascurrido desde el ultimo dato introducido
		elapsed_time_int_alive = int(elapsed_time_alive)
		
		if elapsed_time_int_alive > time_alive:							#Comparar el tiempo trascurrido desde que se introdujo el ultimo dato y el tiempo definido
			
			
			
			time_starting_point_alive = time.time()						#Guardo el tiempo de la ultima pregunta de alive
			#status = latido(ip_disp3,status)
 						
#			if (ardu_output("p",ip_disp3) != 0 ):                                    #Mandar p a Arduino y nos responde con su id
#                        	
#				if status == "offline":								#escribo la entrada en SQL solo si estaba offline
#				
#					sql = "UPDATE `dispositivos` SET `status`= 'ONLINE' , `ip` = '"+ ip +"' , `Time` = '" + date_time() + "' WHERE `id` = '1'"
#					db_conexion(sql)
#					status = "online"
#					
#				if get_insert_temp(time_starting_point):			#Llamo a funcion de introducir DB.parciales y actualizo hora de entrada para siguiente control
#					time_starting_point = time.time()
#				
#			else:													#Si no hay respuesta salta el error y sabemos que esta en offline
#				
#				if status == "online":								#Escribo la entrada en SQL solo si estaba online
#																	
#					sql = "UPDATE `dispositivos` SET `status`= 'OFFLINE' , `ip` = '"+ ip +"' , `Time` = '" + date_time() + "' WHERE `id` = '1'"
#					db_conexion(sql)
#					status = "offline"
                                            
		time.sleep(sleep_time)										#Esperar para controlar el uso de CPU de RPI
		
except KeyboardInterrupt:
    if python_args.verbose:
	print "Detectado el cierre del hilo principal"
	print "Matando hilos secundarios"
    server_on = False
    os.system('sudo killall python') 
