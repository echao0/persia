#!/usr/bin/env python

# application: [Persia_core]
# version:
# runtime: python
# api_version: 2.7
# autor: echao0
#
# ---Thanks to all the people how share all his knowledge, because they made this things possible!!!
# -------------be curious, be open source, be maker, be open mind and be free!!!
#
# 
#
# Use -v to modo vervose

# ------- SQL confuguration ---------#
DB_HOST = 'localhost'
DB_USER = 'persia'
DB_PASS = '/Persia/'
DB_NAME = 'persia'

datos_sql = [DB_HOST, DB_USER, DB_PASS, DB_NAME]  # Array de tados SQL

# -----------------------Change process name to "persia"-----------------
import ctypes

libc = ctypes.cdll.LoadLibrary('libc.so.6')
libc.prctl(15, 'persia', 0, 0, 0)
# -----------------------------------------------------------------------

from socket import *
import SocketServer
import socket

from decimal import Decimal
import time  # Se importa para poder leer la hora
import os  # Se importa para poder borrar pantalla de linux
import sys  # Se importa para poder pasar paremetros en la llamada
import MySQLdb  # Server Connection to MySQL
import datetime  # Para poder consrguir la fecha de ayer
import threading  # Para poder realizar varios Hilos
import argparse  # biblioteca para argumentos

disp = {}  # Dic to storage objects to access using ID
timers = {}  # Dic to storage Timers to access using ID

parser = argparse.ArgumentParser()
parser.add_argument("-v", "--verbose", action="store_true")
python_args = parser.parse_args()

port = 5000  # Devices default comunication port

# ------- Configuration Python Server ---------- #
server_host = ''
server_port = int(2000)  # Server socket to comunicate to webpage
server_on = True  # Variable de control de flujo
working = False  # variable para control pila de salida ardu_out

# ------- Time configuration -------#

sleep_time = 0.7  # Time to wait in infinite loop (CPU control)
time_alive = 35  # Time to check if devices are online


# -------------- Classes ------------#
# -----------------------------------#
class Timer():
    # cada vez que se manda el start, se reinicia el contador.

    status = False  # Variable para saber si hay que iniciar o reiniciar el temorizador
    wtime = False  # Cuanto tiempo debe estar esperando el temporizador
    name = False  # Nombre asignado al temporizador
    step = 0.25  # cada cuanto tienpo se comprueba no se reinicia el bucle
    i = 0  # Variable usada para el bucle de tiempo de espera
    disp = False  # Variable que indica que ip tiene el dispositivo a controlar
    beat = False  # Saber si es timer para latido de vida de dispositivo
    beatWtime = False  # variable de recarga de latido
    trigger = False  # variable trigger para timer.

    global python_args

    def __init__(empty):
        if python_args.verbose:
            print "Clase timer creada"

    def __del__(self):
        if python_args.verbose:
            print "El Temporizador: " + self.name + " ha sido destruido"

    def start_short(self):

        if (self.status == False):
            t = threading.Thread(target=self.timer_start_short)
            t.start()
        else:
            if python_args.verbose:
                print self.name + " reinicio"
            self.i = 0

    def start(self):

        if (self.status == False):
            t = threading.Thread(target=self.timer_start)
            t.start()
        else:
            if python_args.verbose:
                print self.name + " reinicio"
            self.i = 0

    def stop(self):
        if (self.status == False):
            self.i = 0
        else:
            self.i = 5000

    def timer_start(self):

        self.status = "working"

        if self.beat == True: self.beatWtime = self.wtime  # Si el contador es de latido / bucle guardo el tiempo definido para el reinicio

        self.wtime = self.wtime - 1  # Resto 1 para compensar bug de 1 segundo mas.

        while (self.i <= (self.wtime * 4)):
            time.sleep(self.step)
            self.i = self.i + 1

        self.wtime = self.wtime + 1  # Sumo 1 para colocar el tiempo inicial.

        if python_args.verbose:
            if self.beat == False: print self.name + " Finalizado tiempo esperando: " + str(self.wtime)

        if self.beat == False: disp[str(self.name)].comm("z")  # Envio paro a arduino
        # Reinicio las variables de control de tiempo
        self.i = 0
        self.status = False

        if self.beat == True:
            self.trigger = True  # colocar variable global a 1 para ejecutar programa de latido / beat / bucle
            self.wtime = self.beatWtime  # Vuelvo a colocar el tiempo de espera para Latido / bucle
            self.start()  # Reinicio el contador en caso que sea de Latido

    def get_trigger(self):  # Getter de variable de latido
        return self.trigger

    def set_trigger(self, x):  # setter de variable de latido
        self.trigger = x

    def timer_start_short(self):

        global trigger  # variable global para saber cuando el temporizador de latido esta listo
        self.status = "working"

        if self.beat == True: self.beatWtime = self.wtime  # Si el contador es de latido / bucle guardo el tiempo definido para el reinicio

        self.wtime = self.wtime - 1  # Resto 1 para compensar bug de 1 segundo mas.

        while (self.i <= self.wtime):  # No multiplico por 4, por lo que tengo 1/4 del tiempo programado.
            time.sleep(self.step)
            self.i = self.i + 1

        self.wtime = self.wtime + 1  # Sumo 1 para colocar el tiempo inicial.

        if python_args.verbose:
            if self.beat == False: print self.name + " Finalizado tiempo esperando: " + str(self.wtime)

        if self.beat == False: disp[str(self.name)].comm("z")  # Envio paro a arduino
        # Reinicio las variables de control de tiempo
        self.i = 0
        self.status = False

        if self.beat == True:
            trigger = True  # colocar variable global a 1 para ejecutar programa de latido / beat / bucle
            self.wtime = self.beatWtime  # Vuelvo a colocar el tiempo de espera para Latido / bucle
            self.start()  # Reinicio el contador en caso que sea de Latido

    def continous(self, funtion):

        while server_on:
            print "siempre"
            funtion
            time.sleep(self.wtime)

    def get_name(self):
        return self.name

    def get_wtime(self):
        return self.wtime

    def get_disp(self):
        return self.disp

def update():
    pass

    if python_args.verbose:
        print "Se han actualizado los valores de los dispositivos."

class Heating():

#Require class timer()

    heatingReachTemp = 0    #Temperatura que debe de alcanzar antes de apagar
    heatingActTemp = 0      #Temperatura interior actual
    heatingStayTemp = False #Mantener encendido al alcanzar la temperatura
    heatingOn = False       #Esta heating encendido o apagado?
    heatingStatus = False  # Variable para saber estado del heating (trabajando o no)
    hetingFullOff = False   #apagar el controlador manual?

    name = False  # Nombre asignado al heating
    heatingDisp = "5"  # Variable que indica que ip tiene el dispositivo a controlar salida
    tempDisp = "4"    #Variable con la id del dispositivo que tiene el termometro

    disp = False

    global python_args

    def set_heatingReachTemp(self, data):
        self.heatingReachTemp = data

    def heatingActTemp(self, data):
        self.heatingActTemp = data

    def heatingStayTemp(self, data):
        self.heatingStayTemp = data

    def heatingOn(self, data):
        self.heatingOn = data

    def hetingFullOff(self, data):
        self.hetingFullOff = data

    def __init__(empty):
        if python_args.verbose:
            print "Clase heating creada"

    def __del__(self):
        if python_args.verbose:
            print "El heating: " + self.name + " ha sido destruido"

    def get_temp(self):
        millivolts = (float(disp[self.tempDisp].comm("t")) / 1024.0) * 3300;
        celsius = millivolts / 10
        celsius = celsius
        celsius = celsius - 2
        return celsius

    def start(self):
        self.heatingOn = True
        self.heatingActTemp = self.get_temp() #Saber la temperatura actual.

        if self.heatingActTemp < self.heatingReachTemp:
            disp[self.heatingDisp].comm("s")        #Activo la output de encendido
            self.heatingStatus = True

    def heating_off(self,temp):
        if python_args.verbose:
            print "Apagado de caldera recibido heating_off"
        self.heatingOn = False
        disp[self.heatingDisp].comm("z")

    def raise_Temp(self, temp):
        if python_args.verbose:
            print "Alcanzar la temperatura encendido raise_Temp"
        self.heatingActTemp = self.get_temp()
        self.heatingReachTemp = self.heatingActTemp + temp
        self.heatingStayTemp = False
        if python_args.verbose:
            print "temperatura a alcanzar "
            print self.heatingReachTemp
        self.start()

    def heating_stay(self, temp):
        if python_args.verbose:
            print "Mantener la temperatura encendido raise_Temp"
        self.heatingReachTemp = temp
        self.heatingStayTemp = True
        self.start()

    def heating_full_off(self, temp):
        if python_args.verbose:
            print "Apagado total (manual) recibido heating_full_off"
        self.heatingReachTemp = -10
        self.heatingOn = False
        self.hetingFullOff = False
        disp[self.heatingDisp].comm("b")

    def heating_timer_control(self):
        if python_args.verbose:
            print "dentro de control de heating"
        if self.heatingOn == True:
            self.heatingActTemp = self.get_temp()  # Saber la temperatura actual.
            if python_args.verbose:
                print "La temperatura actual es: "
                print self.heatingActTemp
                print "La temperatura alcanza: "
                print self.heatingReachTemp

            if self.heatingActTemp >= self.heatingReachTemp:
                if python_args.verbose:
                    print "Se ha alcanzado la temperatura : "
                    print self.heatingActTemp

                disp[self.heatingDisp].comm("z")
                self.heatingStatus = False

                if not self.heatingStayTemp:
                    self.heatingOn = False

            if self.heatingStayTemp == True and not self.heatingStatus and self.heatingActTemp + 0.5 < self.heatingReachTemp:
                disp[self.heatingDisp].comm("s")
                self.heatingStatus = True

class Device():

    def __init__(self, name, ip, wtime, id, infi=False, heatingReachTemp=0, heatingActTemp=0, heatingStayTemp=False, heatingOn=False, hetingFullOff=False):
        self.name = name
        self.ip = ip
        self.wtime = wtime
        self.infi = infi  # never stop automatically
        self.id = id
        self.status = "offline"  # It's online or offline
        self.heatingReachTemp = heatingReachTemp
        self.heatingActTemp = heatingActTemp
        self.heatingStayTemp = heatingStayTemp
        self.heatingOn = heatingOn
        self.hetingFullOff = hetingFullOff

        if python_args.verbose:
            print "id------------ : " + self.id
            print "Decive created : " + self.name
            print "ip asociada--- : " + self.ip
            print "tiempo-------- : " + self.wtime
            if self.infi:
                print "-es infinito-"
            print ""

    def __del__(self):
        if python_args.verbose:
            print "El objeto: " + self.name + " ha sido destruido"

    def get_name(self):
        return self.name

    def get_id(self):
        return self.id

    def get_ip(self):
        return self.ip

    def get_status(self):
        return self.status

    def get_wtime(self):
        return self.wtime

    def get_infi(self):
        return self.infi

    def set_infi(self, new_infi):
        self.infi = new_infi

    def set_ip(self, new_ip):
        self.ip = new_ip

    def set_wtime(self, new_wtime):
        self.wtime = new_wtime

    def set_status(self, new_status):
        self.status = new_status

    def set_id(self, new_id):
        self.id = new_id

    def set_heatingReachTemp(self, data):
        self.heatingReachTemp = data

    def heatingActTemp(self, data):
        self.heatingActTemp = data

    def heatingStayTemp(self, data):
        self.heatingStayTemp = data

    def heatingOn(self, data):
        self.heatingOn = data

    def hetingFullOff(self, data):
        self.hetingFullOff = data

# ------General functions --------------------------------------

    def comm(self, data):

        try:
            working = True
            client = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
            client.settimeout(15);
            client.connect((self.ip, 5000));
            time.sleep(0.11);
            client.send(data);

            while 1:
                resp = client.recv(1024)
                if not resp:
                    break
                if python_args.verbose:
                    print "Le envio: " + data + " a dispositivo " + self.ip + " y me ha devuelto: " + resp;
                break
            client.close()
            return resp

        except socket.timeout:
            # if python_args.verbose:
            #  print "timed out when connecting to " + disp
            client.close()
            client = None
            print "no ha sido posible madar el paquete."
            return 0


        except socket.error:
            # if python_args.verbose:
            #   print "error when communicating with " + disp
            client.close()
            client = None
            return 0

    def latido(self):
        if python_args.verbose:
            print "Latido hacia:" + self.ip

        if (self.comm("p") != 0):  # Mandar p a Arduino y nos responde con su id

            if self.status == "offline":  # escribo la entrada en SQL solo si estaba offline

                sql = "UPDATE `dispositivos` SET `status`= 'ONLINE' , `ip` = '" + self.ip + "' , `Time` = '" + date_time() + "' WHERE `id` = '" + self.id + "'"
                db_conexion(sql)
                self.status = "online"
                return self.status

        else:  # Si no hay respuesta salta el error y sabemos que esta en offline

            if self.status == "online":  # Escribo la entrada en SQL solo si estaba online

                sql = "UPDATE `dispositivos` SET `status`= 'OFFLINE' , `ip` = '" + self.ip + "' , `Time` = '" + date_time() + "' WHERE `id` = '" + self.id + "'"
                db_conexion(sql)
                self.status = "offline"
                return self.status

        return self.status

def devices_timers():
    """What for?:
       Recibe:
       Return:"""
    global disp
    global timers

    disp = {}  # destroy all created devices
    timers = {}  # destroy all created timers

    sql = "SELECT `id`, `ip`, `move` FROM `dispositivos` order by `id` asc"

    result = db_conexion_total(sql)

    x = 0
    for i in range(len(result)):
        try:
            if x == 3:
                disp[str(result[i])] = Device(str(result[i]), str(result[i + 1]), str(result[i + 2]), str(result[i]))
                x = 0
            x = x + 1

        except:
            print "It's not possible to create devices"

    # -----Create the timers for devices

    for key, value in disp.iteritems():  # Get all dispositives ID

        if int(disp[str(key)].get_wtime()) == 0: disp[key].set_infi(True) # if move time like 0 its and infinite device and not timer needed

        if not disp[key].get_infi():  # the dispositive it's not infinite need a timer

            timers[str(key)] = Timer()
            timers[str(key)].name = str(disp[str(key)].get_name())
            timers[str(key)].wtime = int(disp[str(key)].get_wtime())
            timers[str(key)].disp = disp[str(key)].get_ip()

class ServerHandler(SocketServer.BaseRequestHandler):
    def handle(self):
        self.jump = False
        self.data = self.request.recv(1024).strip()

        if self.data == "beat":  # Control para no imprimir en pantalla latidos
            self.request.send(str("ack"))

        else:
            datos = self.data.split(",")  # separo la cadena y formo una lista datos[1] = persiana datos[0] = accion

            if python_args.verbose:
                print "Persiana: " + datos[1]
                print "Accion: " + datos[0]

            if datos[0] == "abajo":
                resp = disp[str(datos[1])].comm("b")
                self.request.send(str(resp))

            if datos[0] == "Subir":
                resp = disp[str(datos[1])].comm("s")
                self.request.send(str(resp))

            if datos[0] == "stop":
                resp = disp[str(datos[1])].comm("z")
                self.request.send(str(resp))

            if datos[0] == "temp":
                resp = disp[str(datos[1])].comm("t")
                self.request.send(str(resp))

            if datos[0] == "update":
                self.request.send(str("Actualizando"))
                devices_timers();

            if datos[0] == "hRaise":
                heating.raise_Temp(int(datos[1]))
                self.jump = True
                self.request.send(str("1"))

            if datos[0] == "hStay":
                heating.heating_stay(int(datos[1]))
                self.jump = True
                self.request.send(str("1"))

            if datos[0] == "hOff":
                heating.heating_off(int(datos[1]))
                self.jump = True
                self.request.send(str("1"))

            if datos[0] == "hFOff":
                heating.heating_full_off(int(datos[1]))
                self.jump = True
                self.request.send(str("1"))

            if datos[0] == "server":
                self.request.send(str("Apagando el servidor"))
                os.system("ssh echao@192.168.3.151 'sudo shutdown now'")
                #mandatory to creater sudo su RSA-KEY and send it to server

            if not self.jump:
                if not disp[str(datos[1])].get_infi():          #If not a infinite device, start timer to automatically stop
                    timers[str(datos[1])].start()

class miserver():
    global server_on
    global python_args
    global python_args

    def __init__(empty):
        if python_args.verbose:
            print "La clase", "esta iniciada"

    def start(self):

        t = threading.Thread(target=self.server_start)  # Creo el hilo para la escucha de puerto
        t.start()  # Inicio el hilo de la escucha de puerto
        if python_args.verbose:
            print "Servidor iniciado y a la espera"

    def server_start(self):  # Funcion de inicio y control de puerto de escucha
        try:
            server = SocketServer.TCPServer((server_host, server_port), ServerHandler)  # Creo el objeto servidor
            server.socket.settimeout(
                3.0)  # Selecciono un timeout del servidor de 5 segundos (evito fallo de cierre de server)

            while server_on:  # Control de variable de cierre de hilo principal
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
        server = SocketServer.TCPServer((server_host, server_port), ServerHandler)
        server_thread = threading.Thread(target=server.serve_forever)
        server_thread.daemon = True
        server_thread.start()

    # ------- Declaracion de funciones --#

def db_conexion(sql):  # Funcion para insertar/ rescatar datos en SQL

    if python_args.verbose:  # Mostrar la consulta si esta en modo vervose
        print sql

    db = MySQLdb.connect(*datos_sql)  # Ejecutar la consulta de SQL
    cursor = db.cursor()
    cursor.execute(sql)
    db.commit()
    db.close()

    fields = cursor.fetchall()  # Particionar el resultado de la consulta
    result = []  # Crear el array que devuelve la funcion

    for field in fields:
        result.append(field[0])  # Guardar todos los datos particionados en el array

    return result  # Fin de la funcion y retorno de los datos

def db_conexion_total(sql):  # Funcion para insertar/ rescatar datos en SQL

    if python_args.verbose:  # Mostrar la consulta si esta en modo vervose
        print sql

    db = MySQLdb.connect(*datos_sql)  # Ejecutar la consulta de SQL
    cursor = db.cursor()
    cursor.execute(sql)
    db.commit()
    db.close()

    fields = cursor.fetchall()  # Particionar el resultado de la consulta
    result = []  # Crear el array que devuelve la funcion

    for field in fields:
        result.append(field[0])  # Guardar todos los datos particionados en el array
        result.append(field[1])
        result.append(field[2])

    return result  # Fin de la funcion y retorno de los datos

def date_time(method='0'):  # funcion que devuelve fechas, por defecto la actual . 1 = la fecha de ayer

    if method == '1':
        yesterday = datetime.date.today() - datetime.timedelta(days=1)
        return yesterday.strftime("%Y-%m-%d %H:%M:%S")

    if method == '0':
        return time.strftime('%Y-%m-%d %H:%M:%S')

    if method == '2':
        return time.strftime('%H:%M')

    if method == '3':
        return time.strftime('%a')

def timer_auto():
    # Sirve par recoger de la base de datos las acciones automaticas programadas
    matriz = []  # La matriz se usara para tener todos los datos de la DB activos

    sql = "SELECT  `disp` FROM `temporiza` WHERE `active` = '1'"
    result = db_conexion(sql)

    for i in range(len(result)):  # crear la matriz con la longitud necesaria
        matriz.append([0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0])

    for x in range(0, 2):

        for y in range(1, 8):
            sql = "SELECT  `" + str(y) + "` FROM `temporiza` WHERE `active` = '1'"
            result = db_conexion(sql)
            for i in range(len(result)):
                matriz[i][y] = int(result[i])

        sql = "SELECT  `disp` FROM `temporiza` WHERE `active` = '1'"
        result = db_conexion(sql)

        for i in range(len(result)):
            matriz[i][0] = int(result[i])

        sql = "SELECT  `type` FROM `temporiza` WHERE `active` = '1'"
        result = db_conexion(sql)
        for i in range(len(result)):
            matriz[i][8] = result[i]

        sql = "SELECT  `hour` FROM `temporiza` WHERE `active` = '1'"
        result = db_conexion(sql)
        for i in range(len(result)):
            matriz[i][9] = result[i]

        sql = "SELECT  `short` FROM `temporiza` WHERE `active` = '1'"
        result = db_conexion(sql)
        for i in range(len(result)):
            matriz[i][10] = result[i]

    return matriz

def timer_auto_lanza(matriz):  # saber con la matriz de timer_auto() si es la hora de lanzar accion automatica
    """What for?: This def send activation to the devices when the system time its specified in DB.temporiza
       Recibe Matrix whit all lines from DB.temporiza where active = 1.
       Return: Nothing, but activate the device output."""

    if python_args.verbose:
        print "hora actual: " + date_time('2');

    weekdays_dic = {'none': 0, 'Mon': 1, 'Tue': 2, 'Wed': 3, 'Thu': 4, 'Fri': 5, 'Sat': 6, 'Sun': 7}

    for i in range(len(matriz)):
        if python_args.verbose:
            print ""
            print "---------------------------"
            print "id: " + str(matriz[i][0]) + " Hora: " + str(matriz[i][9]) + " short: " + str(matriz[i][10])

        if (matriz[i][weekdays_dic[date_time('3')]] == 1):

            if (matriz[i][9] == date_time('2')):

                if matriz[i][10] == 0:
                    timers[str(matriz[i][0])].start();  # Start timer of the device
                else:
                    timers[str(matriz[i][0])].start_short();  # Short movement?

                disp[str(matriz[i][0])].comm(str(matriz[i][8]))  # Send command to device

def timer_heating():
    print "dentro de timer heating"
    heatingT = Timer()
    heatingT.name = "latido"  # Timer for online pulse
    heatingT.wtime = 10
    heatingT.beat = True  # Activate infinite counter
    heatingT.start()

# ----------- Program Start ---------#
# -----------------------------------#
# outfile = open('/var/www/html/temp/python/texto.txt', 'w') # Indicamos el valor 'w'.
# outfile.write('Fusce vitae leo purus, a tempor nisi.\n')
# outfile.close()

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
client_socket.settimeout(1)                      # Only wait 1 second for a response

os.system('clear')
if python_args.verbose:
    titulo = "-"
    print titulo.center(50, "-")
    titulo = '\033[1;34m Servidor Control Persianas \033[1;m'
    print titulo.center(50, " ")
    titulo = "-"
    print titulo.center(50, "-")

servidor1 = miserver()
servidor1.start()  # Server start to recive packets

# ----------Create diccionary with all devices from DB-------------------

devices_timers()  # Create all devices and timers

# -----------------Infinite timers for alarms and online pulse-------------

timerLatido = Timer()
timerLatido.name = "latido"         #Timer for online pulse
timerLatido.wtime = time_alive
timerLatido.beat = True  # Activate infinite counter
timerLatido.start()

timerAutomove = Timer()
timerAutomove.name = "automove"     #Timer for automovement
timerAutomove.wtime = 60
timerAutomove.beat = True  # Activate infinite counter
timerAutomove.start()

heatingT = Timer()
heatingT.name = "heating"  # Timer for online pulse
heatingT.wtime = 10
heatingT.beat = True  # Activate infinite counter
heatingT.start()

heating = Heating()     #Creo el heating


# ---------------------------------------------------------------------------------------

try:
    while (1):

        if timerAutomove.get_trigger() == True:
            var1 = timer_auto()                        # control de timers programados para movimiento automatico. Esto devuelve la matriz con lo necesario.
            timer_auto_lanza(var1)
            timerAutomove.set_trigger(False)

        if timerLatido.get_trigger() == True:

            if python_args.verbose:
                print "----------------"
                print "Lanzo latidos"
                print "----------------"

            for key, value in disp.iteritems():
                disp[str(key)].latido()

            if python_args.verbose:
                print "----------------"

            timerLatido.set_trigger(False)

        if heatingT.get_trigger() == True:
            heating.heating_timer_control()
            heatingT.set_trigger(False)

        time.sleep(sleep_time)

except KeyboardInterrupt:
    if python_args.verbose:
        print "Detectado el cierre del hilo principal"
        print "Matando hilos secundarios"
    disp = ""
    timers = ""
    server_on = False
    os.system('sudo killall persia')
