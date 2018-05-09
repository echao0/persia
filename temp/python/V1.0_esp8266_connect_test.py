import socket
import time 
import argparse 
import sys

def mylog(texto):
    outfile = open('/var/www/html/temp/python/log_esp8266.txt', 'w') # Indicamos el valor 'w' para crear 'a' para anadir. GRABAMOS UN LOG
    outfile.write(texto)
    outfile.write('\n')
    outfile.close()
    
class timer():
    #cada vez que se manda el start, se reinicia el contador.
    
    status=False                        #Variable para saber si hay que iniciar o reiniciar el temorizador
    wtime=False                         # Cuanto tiempo debe estar esperando el temporizador 
    name=False                          #Nombre asignado al temporizador
    step=0.25                           #cada cuanto tienpo se comprueba no se reinicia el bucle
    i=0                                 #Variable usada para el bucle de tiempo de espera
    disp= False                         #Variable que indica que ip tiene el dispositivo a controlar
    beat=False                          #Saber si es timer para latido de vida de dispositivo
    secure=False                        #saber si es un timer de control de reenvio de seguridad / trato de errores de envio
    beatWtime = False                   #variable de recarga de latido
    command  = False                    #variable para saber que debo mandar (funcion de reenvio de seguridad de comando / trato de errores)       
    
    global python_args
    
    
    def __init__(empty):
        if python_args.verbose:
            print "Clase timer creada"
    
    def start_short(self):
        
        if (self.status == False):
            t = threading.Thread(target=self.timer_start_short)
            t.start()
        else:
            if python_args.verbose:
                print self.name + " reinicio"
            self.i=0
            
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
        global outTimer                                                          #variable global para saber cuando el temporizador de latido esta listo
        self.status = "working"
        
        if self.beat == True:self.beatWtime = self.wtime                        #Si el contador es de latido / bucle guardo el tiempo definido para el reinicio
        
        self.wtime = self.wtime -1 #Resto 1 para compensar bug de 1 segundo mas.    
        
        while (self.i <= (self.wtime * 4)):
            time.sleep(self.step)
            self.i=self.i+1
            
        self.wtime = self.wtime +1 #Sumo 1 para colocar el tiempo inicial.
        
        if python_args.verbose:    
            if self.beat == False: print self.name + " Finalizado tiempo esperando: " + str(self.wtime)
                
        if self.beat == False: ardu_output("z",self.disp)                               #Envio paro a arduino
                                                                                        #Reinicio las variables de control de tiempo
        self.i=0
        self.status = False
        
        if self.beat == True:
                    outTimer=True                                         #colocar variable global a 1 para ejecutar programa de latido / beat / bucle
                    self.wtime = self.beatWtime                                 #Vuelvo a colocar el tiempo de espera para Latido / bucle
                    self.start()                                              #Reinicio el contador en caso que sea de Latido
    
    def timer_start_short(self):
        
        global outTimer                                                          #variable global para saber cuando el temporizador de latido esta listo
        self.status = "working"
        
        if self.beat == True:self.beatWtime = self.wtime                        #Si el contador es de latido / bucle guardo el tiempo definido para el reinicio
        
        self.wtime = self.wtime -1                                              #Resto 1 para compensar bug de 1 segundo mas.    
        
        while (self.i <= self.wtime):                                           #No multiplico por 4, por lo que tengo 1/4 del tiempo programado.
            time.sleep(self.step)
            self.i=self.i+1
            
        self.wtime = self.wtime +1 #Sumo 1 para colocar el tiempo inicial.
        
        if python_args.verbose:    
            if self.beat == False: print self.name + " Finalizado tiempo esperando: " + str(self.wtime)
            
            
        if self.beat == False: ardu_output("z",self.disp)                               #Envio paro a arduino
                                                                                        #Reinicio las variables de control de tiempo
        self.i=0
        self.status = False
        
        if self.beat == True:
                    outTimer=True                                         #colocar variable global a 1 para ejecutar programa de latido / beat / bucle
                    self.wtime = self.beatWtime                                 #Vuelvo a colocar el tiempo de espera para Latido / bucle
                    self.start()                                              #Reinicio el contador en caso que sea de Latido
                    
    def continous(self,funtion):
        
        while server_on:
            print "siempre"
            funtion
            time.sleep(self.wtime)
            
                
parser = argparse.ArgumentParser()

parser.add_argument("-ip", "--ip", help="python esp8266_connect_test -ip 192.168.3.171 -c COMANDO")
parser.add_argument("-c", "--command", help="p -> Latido ; s -> Subir ; b -> Bajar ; z -> Stop")

python_args = parser.parse_args()

if not python_args.ip:
    print "Modo de uso:"
    print ""
    print "python esp8266_connect_test -ip 192.168.3.172 -c p"
    print ""
    print "escribe esp8266_connect_test -h para mas informacion "
    print ""
    sys.exit(0)
    
if not python_args.command:
    print "Modo de uso:"
    print ""
    print "python esp8266_connect_test -ip 192.168.3.172 -c p"
    print ""
    print "escribe esp8266_connect_test -h para mas informacion "
    print ""
    sys.exit(0)
    

try:
    client=socket.socket(socket.AF_INET,socket.SOCK_STREAM);
    client.settimeout(1);
    client.connect((python_args.ip,5000));

    time.sleep(0.11);
    
    client.send(python_args.command);
    
    while 1:
        data = client.recv(1024)
        if not data: break
        print "Esto me ha devuelto: " + data;
        break

    client.close()

except socket.timeout:
        print "timed out when connecting to %s"
        client.close()
        client = None
        mylog('mi texto molon')

except socket.error:
        print "error when communicating with %s"
        client.close()
        client = None
        
        
