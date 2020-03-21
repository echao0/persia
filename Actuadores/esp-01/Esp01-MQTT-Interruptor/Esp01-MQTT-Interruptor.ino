#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#ifndef STASSID
#define STASSID "Me-House"
#define STAPSK  "Et-micasa"
#endif

//-----------NAME----------------------------
const char* ver = "ESP-MQTT-V2";
String espName = "pul3";

//-----------MQTT----------------------------
const char* topicConnect = "persia/connect";
const char* topicLog = "persia/log";
const char* topicMode = "persia/pul3/mode";
const char* topicOrders = "persia/pul3/order";


const char* mqttServer = "echao.asuscomm.com";
const int mqttPort = 1883;
const char* mqttUser = "pul3";
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

const int  buttonPin = 2;    // the pin that the pushbutton is attached to
const int ledPin = 13;       // the pin that the LED is attached to
// Variables will change:

int buttonState = 0;         // current state of the button
int lastButtonState = 2;     // previous state of the button

//-----------------Include Functions----------------------
#include "OTA.h"
#include "toCharFunction.h"
#include "sendFunction.h"
#include "callback.h"
#include "mqttReconnect.h"

//-----------------Program----------------------

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
    
  Serial.begin(115200);
  
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  int pass = 0;         //Control de tiempo maximo para la conexión wifi
  
  while ((WiFi.status() != WL_CONNECTED) and (pass != 15)) {
      delay(500);
      Serial.print(".");
      pass++;
     }
     
if (WiFi.status() == WL_CONNECTED){
          Serial.println("");
          Serial.println("---------------------------------------------------------");
          Serial.println("");
          Serial.println("WiFi conectado");
          Serial.print("IP Configurada: ");
          Serial.println(WiFi.localIP()); //Obtenemos la IP
          Serial.println("");
          Serial.println("---------------------------------------------------------");
          Serial.println("");
  } else {
          Serial.println("");
          Serial.println("---------------------------------------------------------");
          Serial.println("");
          Serial.println("WiFi NO conectado, revise la configuración");
          Serial.println("Para configurar pulsar el botón de flash durante 4 sec");
          Serial.println("");
          Serial.println("---------------------------------------------------------");
          Serial.println("");
          Serial.println("Se reiniciará en 10 sec");
          pass = 0;
          while ((pass != 10))
                    {
                      delay(1000);
                      Serial.print(".");
                      pass++;
                      
                     // if (digitalRead(0) == 0){
                     //         #include "Web_Setting.h"
                     //         }
                    }
          ESP.reset();
    }

//-----------MQTT----------------------------

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect(toCharFunction(espName), mqttUser, mqttPassword )) {
       Serial.println("connected");
       client.publish(topicLog, toCharFunction(espName+"-Conectado!"));
       client.subscribe(topicMode);
       client.subscribe(topicOrders);
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
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
  }
 
 } else {
    client.loop();
  }
 
 buttonState = digitalRead(buttonPin);   // read the pushbutton input pin:

 if (lastButtonState == 2){   //Do nothing if esp8266 wake up
     lastButtonState = buttonState;
  }
 
 if (buttonState != lastButtonState) {
   
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button went from off to on:
      Serial.println("on");
      sendFunction("Subir,3");

    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
      sendFunction("abajo,3");
    }
    delay(50);
  }
  lastButtonState = buttonState;
  delay(50);
}
