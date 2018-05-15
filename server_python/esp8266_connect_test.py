import socket
import time 
import argparse 
import sys

parser = argparse.ArgumentParser()

parser.add_argument("-ip", "--ip", help="python esp8266_connect_test -ip 192.168.3.171 -c COMANDO")
parser.add_argument("-c", "--command", help="p -> Alive ; s -> Up ; b -> down ; z -> Stop")

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
        

except socket.error:
        print "error when communicating with %s"
        client.close()
        client = None
        
        
