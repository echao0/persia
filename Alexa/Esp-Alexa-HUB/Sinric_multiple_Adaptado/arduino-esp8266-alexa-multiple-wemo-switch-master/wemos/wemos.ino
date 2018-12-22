//THANKS - https://github.com/kakopappa/arduino-esp8266-alexa-multiple-wemo-switch
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
bool servidorLightsOn();
bool servidorLightsOff();

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

Switch *servidor = NULL;
Switch *kitchen = NULL;
Switch *lampara = NULL;
Switch *persiana1 = NULL;
Switch *persiana2 = NULL;
Switch *persiana3 = NULL;

bool isservidorLightsOn = false;
bool isKitchenLightstsOn = false;
bool islamparaLightstsOn = false;
bool ispersiana1LightstsOn = false;
bool ispersiana2LightstsOn = false;
bool ispersiana3LightstsOn = false;

//-----Variables para temporizador de apagado server --------//
unsigned long timer = 0;
boolean timerConnect = false;
//-------------------------------------------------------------

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
    servidor = new Switch("servidor", 80, servidorLightsOn, servidorLightsOff);
    kitchen = new Switch("quieto", 81, kitchenLightsOn, kitchenLightsOff);
    
    lampara = new Switch("lampara", 82, lamparaOn, lamparaOff);
    persiana1 = new Switch("persiana 1", 83, persiana1On, persiana1Off);
    persiana2 = new Switch("persiana 2", 84, persiana2On, persiana2Off);
    persiana3 = new Switch("persiana 3", 85, persiana3On, persiana3Off);
    

    Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*servidor);
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
      servidor->serverLoop();
      lampara->serverLoop();
      persiana1->serverLoop();
      persiana2->serverLoop();
      persiana3->serverLoop();
      
	 }
//-------Temporizador apagar server ------- //
//------------------------------------------//   

   if ((timerConnect) and (millis() > timer + 25000 )  ){
       digitalWrite(out1, outOFF);
       timerConnect = false;
    }
    
 //------------------------------------------//
//------------------------------------------//

}

 //------------------------------------------//
 //FUNCIONES
//------------------------------------------//


bool lamparaOn() {    

          if (!client.connect(host, port)) {delay(1000);}
          client.print("Subir,4");
          client.stop();

           islamparaLightstsOn = true;        
    return islamparaLightstsOn;
}

bool lamparaOff() {

          if (!client.connect(host, port)) { delay(1000);}
    
          client.print("stop,4");
          client.stop();

           islamparaLightstsOn = false;
    return islamparaLightstsOn;
}

bool persiana1On() {

          if (!client.connect(host, port)) {delay(1000);}
          client.print("Subir,1");
          client.stop();
          
           ispersiana1LightstsOn = true;
    return ispersiana1LightstsOn;
}

bool persiana1Off() {
  
          if (!client.connect(host, port)) {delay(1000);}
          client.print("abajo,1");
          client.stop();
          
           ispersiana1LightstsOn = false;
    return ispersiana1LightstsOn;
}

bool persiana2On() {
          
          if (!client.connect(host, port)) {delay(1000);}
          client.print("Subir,2");
          client.stop();
          
           ispersiana2LightstsOn = true; 
    return ispersiana2LightstsOn;
}

bool persiana2Off() {

          if (!client.connect(host, port)) {delay(1000);}
          client.print("abajo,2");
          client.stop();
          
           ispersiana2LightstsOn = false;
    return ispersiana2LightstsOn;
}

bool persiana3On() {
  
          if (!client.connect(host, port)) {delay(1000);}
          client.print("Subir,3");
          client.stop();
          
           ispersiana3LightstsOn = true; 
    return ispersiana3LightstsOn;
}

bool persiana3Off() {
  
          if (!client.connect(host, port)) {delay(1000);}
          client.print("abajo,3");
          client.stop();
          
           ispersiana3LightstsOn = false;
    return ispersiana3LightstsOn;
}


bool servidorLightsOn() {

    digitalWrite(out1, outON);
    timerConnect = false;
    
           isservidorLightsOn = true;    
    return isservidorLightsOn;
}

bool servidorLightsOff() {
    Serial.println("dentro de OFF SERVER");
    
    if (!client.connect(host, port)) {delay(1000);}        
    client.print("server,4"); //mando paquete apagado SSH
    client.stop();
    
    timer = millis();
    timerConnect = true;
    
           isservidorLightsOn = false;
    return isservidorLightsOn;
}

bool kitchenLightsOn() {
      
          if (!client.connect(host, port)) {delay(1000);}
          client.print("stop,1");
          client.stop();

          if (!client.connect(host, port)) {delay(1000);}
          client.print("stop,2");
          client.stop();

          if (!client.connect(host, port)) {delay(1000);}
          client.print("stop,3");
          client.stop();

           isKitchenLightstsOn = true;
    return isKitchenLightstsOn;
}

bool kitchenLightsOff() {
  
          if (!client.connect(host, port)) {delay(500);}
          client.print("stop,1");
          client.stop();

          if (!client.connect(host, port)) {delay(500);}
          client.print("stop,2");
          client.stop();

          if (!client.connect(host, port)) {delay(500);}
          
          client.print("stop,3");
          client.stop();
          
         isKitchenLightstsOn = false;
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
