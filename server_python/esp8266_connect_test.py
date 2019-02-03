import socket
import time 
import argparse 
import sys

parser = argparse.ArgumentParser()

parser.add_argument("-ip", "--ip", help="python esp8266_connect_test -ip 192.168.3.171 -c COMANDO")
parser.add_argument("-c", "--command", help="p -> Alive ; s -> Up ; b -> down ; z -> Stop")
parser.add_argument("-pre", "--prefabrica", help="paquete prefabricado para pedir temperatura a ip 180")

python_args = parser.parse_args()

if python_args.prefabrica == "dht22":
	python_args.ip = "192.168.3.180"
	python_args.command = "t"

if not python_args.ip:
    print "Modo de uso:"
    print ""
    print "python esp8266_connect_test -ip 192.168.3.172 -c p"
    print ""
    print "python esp8266_connect_test -pre dht22"
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
    client.settimeout(5);
    client.connect((python_args.ip,5000));

    time.sleep(0.11);
    
    client.send(python_args.command);
    #time.sleep(0.5);


    data = ""

    while 1:
        data += client.recv(1024)
        data = data.decode("utf-8")

        if data.find("\r\n") > 0:
            break

	#if not data: break
     #   print "Esto me ha devuelto: " + data;
     #   break
    print "Esto me ha devuelto: " + data;
    client.close()

except socket.timeout:
        print "timed out when connecting to %s"
        client.close()
        client = None
        

except socket.error:
        print "error when communicating with %s"
        client.close()
        client = None
        
        
