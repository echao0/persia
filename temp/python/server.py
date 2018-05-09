
import ctypes
libc = ctypes.cdll.LoadLibrary('libc.so.6')
libc.prctl(15, 'server', 0, 0, 0)

                
from socket import *
import SocketServer
import socket
import os 
import threading
import time

import argparse                                                                 #biblioteca para argumentos
 
parser = argparse.ArgumentParser()
parser.add_argument("-v", "--verbose", action="store_true")
python_args = parser.parse_args()

#------- Configuracion IP Servidor Python ---------- #
server_host= '127.0.0.1'                                                        #direccion del server.
server_port=int(2002)								#Puerto del servidor
server_on = True									#Variable de control de flujo
working = False 

ip_disp1 = "192.168.3.171"
ip_disp2 = "192.168.3.172"
ip_disp3 = "192.168.3.173"

time_triger = 10									#Tiempo en minutos
sleep_time = 0.7 									#Tiempo en segundos de espera por cada bucle de programa (control de CPU)
time_alive = 30	
lanza = 0

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
                                print "Bajando"
                                #ardu_output("b",disp)
                                self.selec(datos[1])
                                
                        if datos[0] == "Subir":
				self.request.send(str("Subiendo"))
                                print "Subiendo"
                                #ardu_output("s",disp)
                                self.selec(datos[1])
                                
                        if datos[0] == "stop":
				self.request.send(str("Paro"))
                                print "Paro"
                                #ardu_output("z",disp)
                                self.selec(datos[1])
                                
                        if datos[0] == "update":
                                self.request.send(str("Actualizando"))
				update();
                        
                        if datos[0] == "reset":
                                print "Reseteando el programa"
                                self.request.send(str("Reiniciando"))
				#restart_program();
                                
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
		try:
                    server = SocketServer.TCPServer((server_host,server_port),ServerHandler)		#Creo el objeto servidor
                    server.socket.settimeout(20.0)													#Selecciono un timeout del servidor de 5 segundos (evito fallo de cierre de server)
		
                    while server_on:																#Control de variable de cierre de hilo principal
                    	server.handle_request()
                    else:
                        server.socket.shutdown()
                        server.socket.close()
                        
                        
                except:
                   # print "##### NO ES POSIBLE INICIAR EL SERVER #####"
                   # print "      ##### Espero 30 segundos #####"
                    time.sleep(30)
                    self.server_start()
                    
        def server_start2(self):
                server = SocketServer.TCPServer((server_host,server_port),ServerHandler)
                server_thread = threading.Thread(target=server.serve_forever)
                server_thread.daemon = True
                server_thread.start()    

servidor1 = miserver()                                                                                                  #Declaro que quiero un servidor
servidor1.start()

try:										
	while(1):
                #[[3, 1, 1, 0, 0, 1, 1, 1, 's', '07:50']]
                
                if (lanza ==65):                        
                        print "----------------"
                        print "Lanzo latidos"
                        print "----------------"
                        lanza = 0
                lanza = lanza + 1
            
                time.sleep(sleep_time)
		
except KeyboardInterrupt:
    if python_args.verbose:
	print "Detectado el cierre del hilo principal"
	print "Matando hilos secundarios"
    server_on = False
    os.system('sudo killall server') 