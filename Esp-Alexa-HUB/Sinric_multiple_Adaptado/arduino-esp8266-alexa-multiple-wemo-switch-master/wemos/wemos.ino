//https://github.com/kakopappa/arduino-esp8266-alexa-multiple-wemo-switch
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>
#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"

// prototypes
boolean connectWifi();

//on/off callbacks 
bool serverLightsOn();
bool serverLightsOff();
bool kitchenLightsOn();
bool kitchenLightsOff();

bool lamparaOn();
bool lamparaOff();

bool persiana1On();
bool persiana1Off();

bool persiana2On();
bool persiana2Off();

bool persiana3On();
bool persiana3Off();

//------------Local Outputs -----------------------

char out1 = D1;


char outON = LOW;
char outOFF = HIGH;

//-------------------------------------------------

//-------------------------------------------------
//-------Modificado para TCP-----------------------

    const uint16_t port = 2000;
    const char * host = "192.168.3.181";
    WiFiClient client;
//-------------------------------------------------



// Change this before you flash
const char* ssid = "Me-House";
const char* password = "Et-micasa";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *server = NULL;
Switch *kitchen = NULL;

Switch *lampara = NULL;
Switch *persiana1 = NULL;
Switch *persiana2 = NULL;
Switch *persiana3 = NULL;

bool isserverLightsOn = false;
bool isKitchenLightstsOn = false;

bool islamparaLightstsOn = false;
bool ispersiana1LightstsOn = false;
bool ispersiana2LightstsOn = false;
bool ispersiana3LightstsOn = false;


void setup()
{
  Serial.begin(115200);
  
  pinMode(out1, OUTPUT);
  digitalWrite(out1, outOFF);
   
  // Initialise wifi connection
  wifiConnected = connectWifi();
  
  if(wifiConnected){
    upnpBroadcastResponder.beginUdpMulticast();
    
    // Define your switches here. Max 10
    // Format: Alexa invocation name, local port no, on callback, off callback
    server = new Switch("Server", 80, serverLightsOn, serverLightsOff);
    kitchen = new Switch("Quieto", 81, kitchenLightsOn, kitchenLightsOff);
    
    lampara = new Switch("lampara", 82, lamparaOn, lamparaOff);
    persiana1 = new Switch("persiana 1", 83, persiana1On, persiana1Off);
    persiana2 = new Switch("persiana 2", 84, persiana2On, persiana2Off);
    persiana3 = new Switch("persiana 3", 85, persiana3On, persiana3Off);
    

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*server);
    upnpBroadcastResponder.addDevice(*kitchen);
    
    upnpBroadcastResponder.addDevice(*lampara);
    upnpBroadcastResponder.addDevice(*persiana1);
    upnpBroadcastResponder.addDevice(*persiana2);
    upnpBroadcastResponder.addDevice(*persiana3);
  }
}
 
void loop()
{
	 if(wifiConnected){
      upnpBroadcastResponder.serverLoop();
      
      kitchen->serverLoop();
      server->serverLoop();
      
      lampara->serverLoop();
      persiana1->serverLoop();
      persiana2->serverLoop();
      persiana3->serverLoop();
      
	 }
}
bool lamparaOn() {    
    islamparaLightstsOn = true;  
    
          //-------------------------------------------------
          //-------Modificado para TCP-----------------------
          
          if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
          
          client.print("Subir,4");
          client.stop();
          
          //-------------------------------------------------
            
    return islamparaLightstsOn;
}

bool lamparaOff() {
    islamparaLightstsOn = false;

          //-------------------------------------------------
          //-------Modificado para TCP-----------------------
          
          if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
    
          client.print("stop,4");
          client.stop();

          //-------------------------------------------------
    
    return islamparaLightstsOn;
}

bool persiana1On() {
    Serial.println("Switch persiana1 turn on ...");
    
    ispersiana1LightstsOn = true;  
    //-------------------------------------------------
          //-------Modificado para TCP-----------------------
          
          if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
          Serial.println("Conectado a persia");
          
          client.print("Subir,1");
          
          Serial.println("El paquete se ha enviado");
          
          Serial.println("closing connection");
          client.stop();
          
          //-------------------------------------------------  
    return ispersiana1LightstsOn;
}

bool persiana1Off() {
    Serial.println("Switch persiana 1 turn off ...");

    ispersiana1LightstsOn = false;
    //-------------------------------------------------
          //-------Modificado para TCP-----------------------
          
          if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
          Serial.println("Conectado a persia");
          
          client.print("abajo,1");
          
          Serial.println("El paquete se ha enviado");
          
          Serial.println("closing connection");
          client.stop();
          
          //-------------------------------------------------
    return ispersiana1LightstsOn;
}

bool persiana2On() {
    Serial.println("Switch persiana 2 turn on ...");
    
    ispersiana2LightstsOn = true; 
    //-------------------------------------------------
          //-------Modificado para TCP-----------------------
          
          if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
          Serial.println("Conectado a persia");
          
          client.print("Subir,2");
          
          Serial.println("El paquete se ha enviado");
          
          Serial.println("closing connection");
          client.stop();
          
          //-------------------------------------------------   
    return ispersiana2LightstsOn;
}

bool persiana2Off() {
    Serial.println("Switch persiana 2 turn off ...");

    ispersiana2LightstsOn = false;
    //-------------------------------------------------
          //-------Modificado para TCP-----------------------
          
          if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
          Serial.println("Conectado a persia");
          
          client.print("abajo,2");
          
          Serial.println("El paquete se ha enviado");
          
          Serial.println("closing connection");
          client.stop();
          
          //-------------------------------------------------
    return ispersiana2LightstsOn;
}

bool persiana3On() {
    Serial.println("Switch persiana 3 turn on ...");
    
    ispersiana3LightstsOn = true;   
    //-------------------------------------------------
          //-------Modificado para TCP-----------------------
          
          if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
          Serial.println("Conectado a persia");
          
          client.print("Subir,3");
          
          Serial.println("El paquete se ha enviado");
          
          Serial.println("closing connection");
          client.stop();
          
          //------------------------------------------------- 
    return ispersiana3LightstsOn;
}

bool persiana3Off() {
    Serial.println("Switch persiana 3 turn off ...");

    ispersiana3LightstsOn = false;
    //-------------------------------------------------
          //-------Modificado para TCP-----------------------
          
          if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
          Serial.println("Conectado a persia");
          
          client.print("abajo,3");
          
          Serial.println("El paquete se ha enviado");
          
          Serial.println("closing connection");
          client.stop();
          
          //-------------------------------------------------
    return ispersiana3LightstsOn;
}


bool serverLightsOn() {
    Serial.println("Switch 1 turn on ...");

    digitalWrite(out1, outON);
    
    isserverLightsOn = true;    
    return isserverLightsOn;
}

bool serverLightsOff() {
    Serial.println("Servidor 1 turn off ...");
    
    if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
              
    client.print("server,4"); 
    
    delay(60000);     //dar tiempo para que se apage el sistema
    digitalWrite(out1, outOFF);
    
    isserverLightsOn = false;
    return isserverLightsOn;
}

bool kitchenLightsOn() {
    Serial.println("Switch 2 turn on ...");
    //-------------------------------------------------
          //-------Modificado para TCP-----------------------
          
          if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
          Serial.println("Conectado a persia");
          
          client.print("stop,1");
          client.stop();

          if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
          
          client.print("stop,2");
          client.stop();

          if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
          
          client.print("stop,3");
          client.stop();
          
          
          Serial.println("El paquete se ha enviado");
          
          Serial.println("closing connection");
   
          
          //-------------------------------------------------

    isKitchenLightstsOn = true;
    return isKitchenLightstsOn;
}

bool kitchenLightsOff() {
  Serial.println("Switch 2 turn off ...");

  isKitchenLightstsOn = false;
  //-------------------------------------------------
          //-------Modificado para TCP-----------------------
          
          if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
          Serial.println("Conectado a persia");
          
          client.print("stop,1");
          client.stop();

          if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
          
          client.print("stop,2");
          client.stop();

          if (!client.connect(host, port)) {
                  Serial.println("connection failed");
                  Serial.println("wait 5 sec...");
                  delay(1000);
              }
          
          client.print("stop,3");
          client.stop();
          
          
          Serial.println("El paquete se ha enviado");
          
          Serial.println("closing connection");
   
          
          //-------------------------------------------------
  return isKitchenLightstsOn;
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi(){
  boolean state = true;
  int i = 0;
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10){
      state = false;
      break;
    }
    i++;
  }
  
  if (state){
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else {
    Serial.println("");
    Serial.println("Connection failed.");
  }
  
  return state;
}
