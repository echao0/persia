 /* Verison  ESP01-V5.0
 *  Version para los interruptores MQTT (SOLO)
 *  Modificado para Home assistant
 *  En la versión anterior tardaba mucho en enviar la orden MQTT

  RECORDAR LA RESISTENCIA - NO ES NECESARIA
   * Verison  ESP01-V4.1
   *  Añado reset en si salta el trigger en OTA // Esp01 30s max
   *  


 * Verison  ESP01-V4
 *  Version para los interruptores
 *  Funciona el callback y se le pueden dar ordenes 
 *  Funciona  OTA
 * 
 * Version ESP-MQTT-V3:
 *     Mqtt Topic: Anado topicGeneral = persia/general
 *     Mqtt topics -> Nombre dinamico teniendo en cuenta el nombre de espName 
 * 
 */
 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#ifndef STASSID
#define STASSID "Me-House"
#define STAPSK  "Et-micasa"
#endif

//-----------NAME----------------------------
const char* ver = "ESP01-V5.0";
String espName = "pul3";

//-----------MQTT----------------------------
const char* topicConnect = "persia/connect";
const char* topicLog = "persia/log";
const char* topicStatus = "persia/switch/status";

String topicMode = "persia/"+espName+"/mode";
String topicOrders = "persia/"+espName;
String topicGeneral = "persia/general";
//String topicStatus = "persia/switch"+espName+"/status";


const char* mqttServer = "192.168.3.181";
const int mqttPort = 1883;
const char* mqttUser = "pul3"; //test:test ; pul3:hola
const char* mqttPassword = "hola";

String WorkMode = "";
int waitTime;

const unsigned long eventInterval = 10000; // Reconnect time to mqtt
unsigned long previousTime = 0;

WiFiClient espClient;
PubSubClient client(espClient);

//-----------Network----------------------------
const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.3.181";
const uint16_t port = 2000;

//-----------------Control----------------------

const int  buttonPin = 3;    // / Descomentar para extra GPIO 1 TX GPIO 3 RX - GP2 no inicia ESP en pin alto.
const int ledPin = 13;       // the pin that the LED is attached to
// Variables will change:

int buttonState = 0;         // current state of the button
int lastButtonState = 2;     // previous state of the button

const int timeReadStop = 250; // Time to doble change 3 state to stop
long startTime = 0; // Variable to control millis to timeReadStop
int jump = 0; //State 3 detected

//-----------------Include Functions----------------------
#include "OTA.h"
#include "toCharFunction.h"
#include "sendFunction.h"
#include "callback.h"
#include "mqttReconnect.h"

//-----------------Program----------------------

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // GPIO 3 RX - Dispone de pullup interno
  pinMode(ledPin, OUTPUT);
    
  //Serial.begin(115200);
  
  //Serial.println();
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int pass = 0;         //Control de tiempo maximo para la conexión wifi
  
  while ((WiFi.status() != WL_CONNECTED) and (pass != 15)) {
      delay(500);
    //  Serial.print(".");
      pass++;
     }
     
if (WiFi.status() == WL_CONNECTED){
//          Serial.println("");
//          Serial.println("---------------------------------------------------------");
//          Serial.println("");
//          Serial.println("WiFi conectado");
//          Serial.print("IP Configurada: ");
//          Serial.println(WiFi.localIP()); //Obtenemos la IP
//          Serial.println("");
//          Serial.println("---------------------------------------------------------");
//          Serial.println("");
  } else {
//          Serial.println("");
//          Serial.println("---------------------------------------------------------");
//          Serial.println("");
//          Serial.println("WiFi NO conectado, revise la configuración");
//          Serial.println("Para configurar pulsar el botón de flash durante 4 sec");
//          Serial.println("");
//          Serial.println("---------------------------------------------------------");
//          Serial.println("");
//          Serial.println("Se reiniciará en 10 sec");
          pass = 0;
          while ((pass != 10))
                    {
                      delay(1000);
//                      Serial.print(".");
                      pass++;
                      
                     // if (digitalRead(0) == 0){
                     //         #include "Web_Setting.h"
                     //         }
                    }
          ESP.reset();
    }

//-----------MQTT----------------------------

  client.setServer(mqttServer, mqttPort);
  delay(3000);
  client.setCallback(callback); //SI descomento me da un error de memoria

  while (!client.connected()) {
  //  Serial.println("Connecting to MQTT...");

    if (client.connect(toCharFunction(espName), mqttUser, mqttPassword )) {
   //    Serial.println("connected");
       client.publish(topicLog, toCharFunction(espName+" - Conectado!"));
       client.subscribe(toCharFunction(topicMode));
       client.subscribe(toCharFunction(topicOrders));
       client.subscribe(toCharFunction(topicGeneral));
       client.subscribe(toCharFunction(topicStatus));
    } else {
    //  Serial.print("failed with state ");
    //  Serial.print(client.state());
      delay(2000);
    }
  }
  
}


void loop() {

 if (!client.connected()) {   //if no connection to mqtt brocker
  unsigned long currentTime = millis();

  if (currentTime - previousTime >= eventInterval) {
     mqttReconnect();
     previousTime = currentTime;
//     Serial.println("RE_connected");
  }
 
 } else {
    client.loop();
  }
 
 buttonState = digitalRead(buttonPin);   // read the pushbutton input pin:

 if (lastButtonState == 2){   //Do nothing if esp8266 wake up
     lastButtonState = buttonState;
  }
 
 if (buttonState != lastButtonState) {
         lastButtonState = buttonState;
         startTime = millis();

         while (millis() < startTime + timeReadStop) {
            
            buttonState = digitalRead(buttonPin);
            
            //Serial.println("Dentro del bucle 3");
            
            if (buttonState != lastButtonState) {
              //Serial.println("Estado 3");
              client.publish(topicStatus, toCharFunction(espName+"-Stop"));
             // sendFunction("stop,3");
              jump = 1;
              break;
            }
          
         }
                   
  if (jump == 0){        
              if (buttonState == HIGH) {
                // if the current state is HIGH then the button went from off to on:
//                Serial.println("on");
                client.publish(topicStatus, toCharFunction(espName+"-On"));
//                sendFunction("Subir,3");
          
              } else {
                // if the current state is LOW then the button went from on to off:
//                Serial.println("off");
                client.publish(topicStatus, toCharFunction(espName+"-Off"));
//                sendFunction("abajo,3");
              }
      }
    delay(50);
  }
  lastButtonState = buttonState;
  jump = 0;
  delay(50);
  
}
