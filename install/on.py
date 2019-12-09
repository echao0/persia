import time
import os
from socket import *
import SocketServer
import socket
import MySQLdb  # Server Connection to MySQL
blink_interval = .5 #Time interval in Seconds

DB_HOST = 'localhost'
DB_USER = 'persia'
DB_PASS = '/Persia/'
DB_NAME = 'persia'
datos_sql = [DB_HOST, DB_USER, DB_PASS, DB_NAME]  # Array de tados SQL

def db_conexion(sql): # Funcion para insertar/ rescatar datos en SQL
    db = MySQLdb.connect(*datos_sql) # Ejecutar la consulta de SQL
    cursor = db.cursor()
    cursor.execute(sql)
    db.commit()
    db.close()
    fields = cursor.fetchall() # Particionar el resultado de la consulta
    result = [] # Crear el array que devuelve la funcion
    for field in fields:
     result.append(field[0]) # Guardar todos los datos particionados en el array
    return result  # Fin de la funcion y retorno de los datos

os.system('/etc/init.d/apache2 restart')
os.system('/etc/init.d/mysql restart')

try:
  sql = "SELECT `disp` FROM `temporiza` WHERE `active` = '1'"
  result = db_conexion(sql)
  while True:
    time.sleep(blink_interval)
except:
  os.system('/opt/persia/install/db.sh')
  while True:
    time.sleep(blink_interval)


