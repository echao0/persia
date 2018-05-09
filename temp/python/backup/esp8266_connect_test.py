import socket
import time 

comando="s";

try:
    client=socket.socket(socket.AF_INET,socket.SOCK_STREAM);
    client.settimeout(1);
    client.connect(('192.168.3.171',5000));

    time.sleep(0.11);
    client.send("a")
    #client.send("s")
    client.send(comando);
    
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
        
        
