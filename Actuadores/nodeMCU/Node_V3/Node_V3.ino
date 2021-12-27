String ver = "3.5";

/*
 * Version 3.5:
 *    Añado la out D4 para la alimentación del sensor de temperatura
 *    De esta manera se puede resetear a distancia con el comando h.
 *    Falta configurar las señales de mqtt.
 *    ALIMENTACION DE DHT en pin D5
 *    
 * Version 3.3:
 *      Añado en callback estado de lock para no actue.
 * Version 3.3:
 *      Añado reset si salta el trigger de tiempo para OTA.
 *      
 * Version 3.2:
 *      Elimino todas las conexiones serial
 *            Es necesario para la union con el FW de pulsadores y el uso de GPIO 3 en ESP01
 *      ADD Mensajes de getInfo: Alive . connecting . Reconecting
 *      ADD archivo de configuracion:
 *          espName
 *          mqttUser
 *          mqttPassword
 *          etc
 *      MOD mensajes de temperatura en JSON
 *      DHT22 forzado en Temp_read.h
 *      add topic -> persia/temp
 *            
 * Version 3.1:
 *      Mqtt GPIO Control -> Es posible gobernar GPIO con Mqtt (up,down,stop)
 *      JSON: Espacios erroneos rectificados
 *      Cambio de nombre libreria callback -> mqttCallback
 *      Mqtt Topic: Anado topicGeneral = persia/general
 *      
 * Version 3.0:
 *      Add Basic MQTT
 *      MODO auto -> Entrega por tiempo en millis la informacion de temperatura
 *      Mqtt topics -> Nombre dinamico teniendo en cuenta el nombre de espName 
 *      
 * Version 2.6:
 *     Error -> Corrección de Excepcion:3 al iniciar WIFI
 *     
 * Version 2.5:
 *     m -> Retorna mac + tipo de sensor
 *     
 * Version 2.0:
 *    var Temp_type -> DHT or LM3 -> Definir el tipo de sensor por web
 *    Reinicio automatico tras no conexión con wifi
 *    Reparar Web para nuevos modulos NODEMCU
 *    v -> Saber versión actual de disponsitivo
 *    r -> Reset ESP
 *    ap -> poner en modo configuración
 *    
 * Version 1.0:
 *    z -> Desactivar D1 & D2
 *    s -> Activar D1
 *    b -> Activar D2
 *    p -> Alive
 *    t -> Lectura de temperatura DHT
 *    OTA -> Actualización "over the air"
 *    Botón Flash ->Web de configuración IP - RED
 *    
 */
 
#include <ESP8266WiFi.h>
#include "DHT.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"

//-----------MQTT----------------------------
const char* topicConnect = "persia/connect";
const char* topicLog = "persia/log";
String topicMode = "persia/"+espName+"/mode";
String topicOrders = "persia/"+espName+"/order";
String topicGeneral = "persia/general";
const char* topicTemp = "persia/temp";
boolean lock = false;

String WorkMode = "";
int waitTime;
unsigned long initialWaitTime = 0;

const unsigned long eventInterval = 10000; // Reconnect time to mqtt
unsigned long previousTime = 0;

WiFiClient espClient;
PubSubClient client(espClient);


//-----------ETH22------------
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
uint8_t DHTPin = D3;
char aliDHT = D5;       //Pin de alimentacion de sensor VCC
DHT dht(DHTPin, DHTTYPE);                
float Temperature;
float Humidity;

//-----------------Variables Generales----------------------

int tempPin= A0;
char outS = D1;
char outB = D2;
char alto = LOW;
char bajo = HIGH;
int configure = 0; //Variable para entrar en config remotamente

int accion = 0;

//-----------------Include Functions----------------------
#include "OTA.h"
#include "eeprom.h"
#include "toCharFunction.h"
#include "mqttCallback.h"
#include "mqttReconnect.h"

//-------------Web_Conf-------
  const long interval = 1000;
  int entry = 0;
  unsigned long currentMillis = 0;

//-------------Smoothing-------
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int tempOff = 0;
//----------------------------

void setup() {

//---------- Solve Excepcion 3 when connect to wifi
WiFi.persistent(false);
WiFi.disconnect(true);
Serial.begin(9600);
Serial.println(F("DHTxx test!"));
//----------------------

delay(10);

strcat(wifi_ssid_private, "");
strcat(wifi_password_private, "");

  
// ---- Leer los datos de configuracion desde EEPROM

readEEPROM(0,32,wifi_ssid_private);
readEEPROM(32,32,wifi_password_private);
readEEPROM(64,16,esubnet);
readEEPROM(80,16,ipAddr);
readEEPROM(96,16,egateway);
readEEPROM(112,3,Temp_type);


//strcpy(wifi_ssid_private, "ChachiNet");
//strcpy(wifi_password_private, "Et-micasa");


//---- Convertir los resultados de string a IPADDRES----------

ip.fromString(ipAddr);
subnet.fromString(esubnet);
gateway.fromString(egateway);

Serial.println(wifi_ssid_private);
Serial.println(wifi_password_private);
Serial.println(ip);
Serial.println(subnet);

delay(500);

 //-----------IN/OUT------------
  
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
 
  pinMode(outS, OUTPUT);
  digitalWrite(outS, bajo);
  
  pinMode(outB, OUTPUT);
  digitalWrite(outB, bajo);

  pinMode(aliDHT, OUTPUT);    //Alimentación de sensor de temperatura
  digitalWrite(aliDHT, HIGH);

  pinMode(0, INPUT); // Boton de flash como entrada para web config
  
  pinMode(DHTPin, INPUT); // Lectura Eth22
  dht.begin();

  
//--------------WIFI START---------------

  WiFi.mode(WIFI_STA); //especifico que no quiero crear wifi

   WiFi.config(ip, gateway, subnet);
   WiFi.begin(wifi_ssid_private, wifi_password_private); //Conexión a la red

  int pass = 0;         //Control de tiempo maximo para la conexión wifi
  
  while ((WiFi.status() != WL_CONNECTED) and (pass != 15))
  {
    Serial.print(".");
    delay(500);
    pass++;
    
  }
  Serial.println("conectado");
  Serial.println(WiFi.localIP());

  if (WiFi.status() == WL_CONNECTED){
          
            server.begin(); //Iniciamos el servidor
          
  } else {
          pass = 0;
          while ((pass != 10))
                    {
                      delay(1000);
                      pass++;
                      
                      if (digitalRead(0) == 0){
                              #include "Web_Setting.h"
                              }
                    }
          ESP.reset();
    }

//-----------MQTT START----------------------------

  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  
  while (!client.connected()) {

    if (client.connect(toCharFunction(espName), mqttUser, mqttPassword )) {
      
       client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"version\": \""+String(ver)+"\",\"IP\": \""+WiFi.localIP().toString()+"\",\"MAC\": \""+WiFi.macAddress()+"\",\"staus\": \"connecting\"}"));
       client.subscribe(toCharFunction(topicMode));
       client.subscribe(toCharFunction(topicOrders));
       client.subscribe(toCharFunction(topicGeneral));
       client.subscribe(toCharFunction(topicTemp));
    } else {
      delay(2000);
      Serial.println("NO");
    }
  }
  

}

void loop() {

//----------------------------------------------------------------------------------
// MQTT bucle and reconned if needed
//----------------------------------------------------------------------------------

   if (!client.connected()) {   //if no connection to mqtt brocker
  unsigned long currentTime = millis();

  if (currentTime - previousTime >= eventInterval) {
     mqttReconnect();
     previousTime = currentTime;
  }
 
 } else {
    client.loop();
  }

 //----------------------------------------------------------------------------------
// Modo automativo

if ((WorkMode == "auto") and (millis() > initialWaitTime + waitTime)){
     
      /*StaticJsonDocument<200> doc;
      doc["sensor"] = "gps";
      doc["time"] = 1351824120;
      String myOutput; serializeJson(doc, myOutput);  
      char myOutput_char[200];
      myOutput.toCharArray(myOutput_char, 200);
      client.publish(topicLog,myOutput_char);*/

      //client.publish(topicLog, toCharFunction(espName+"-AUTOMATICO!"));
      Temperature = dht.readTemperature();
      Humidity = dht.readHumidity();
      client.publish(topicTemp, toCharFunction("{\"disp\": \""+espName+"\",\"Temperatura\": \""+String(Temperature)+"\",\"Humedad\": \""+String(Humidity)+"\"}"));
      initialWaitTime = millis();
  }


  WiFiClient client = server.available();
  client.setTimeout(50); //especificar el tiempo maximo de espera del cliente


  
  //----------------------------------------------------------------------------------
  // Activo la pag de configuración cuando el botón = (Flash) es pulsado durante Variable "interval"
  //----------------------------------------------------------------------------------
  if (configure == 1) {
            #include "Web_Setting.h"
            }
  
  configure = 0;
  
  while (digitalRead(0) == 0){
       
         if (entry == 0){currentMillis = millis(); entry = 1;}
        
          if ((millis() - currentMillis >= interval)) {
                          #include "Web_Setting.h"
                          }
            
            delay(10);
        }
           
      if(entry != 0){currentMillis = 0;entry = 0;ESP.reset();}
              
  //----------------------------------------------------------------------------------
  // Programa estandar de control
  //----------------------------------------------------------------------------------

  
  if (client) //Si hay un cliente presente
  {
            //digitalWrite(2, LOW); // Si se activa el encendemos el led cuando tengamos un paquete
            
            //esperamos datos disponibles
            while (!client.available() && client.connected())
            {
              delay(1);
            }
        
            String linea1 = client.readStringUntil('\r');
        
            if (linea1 == "OTA" ) {
              client.println("OTA ONLINE");
              OTA();
            }
        
            if (linea1 == "p" ) {
              client.println("1");
            }
            
            if (linea1 == "s" ) {
              digitalWrite(outB, bajo);         // Detengo el rele de bajada
              if (accion != 0 ) {
                delay(500);
              }
              digitalWrite(outS, alto);        //Activo rele de subida
              accion = 1;                    //Cambio la variable a movimiento
              client.println("1");
            }
        
            if (linea1 == "b" ) {
              digitalWrite(outS, bajo);        //detengo rele de subida
              if (accion != 0 ) {
                delay(500);
              }
              digitalWrite(outB, alto);         // activo el rele de bajada
              accion = 1;                    //Cambio la variable a movimiento
              client.println("1");
            }
        
            if (linea1 == "h" ) {

              client.println("1");
              
              digitalWrite(aliDHT, LOW);        //detengo la alimentación del sensor de temperatura  
                
                delay(2500);
                
              digitalWrite(aliDHT, HIGH);         // activo la alimentación del sensor de temperatura                  
              
              pinMode(DHTPin, INPUT); // Lectura Eth22
              dht.begin();
              
            }
            
            if (linea1 == "z" ) {
              digitalWrite(outB, bajo);          //detengo rele de subida
              digitalWrite(outS, bajo);         // detengo el rele de bajada
              accion = 0;                    //Cambio la variable a paro
              client.println("1");
            }
        
           if (linea1 == "t" ) {
                #include "Temp_read.h"
            }

            if (linea1 == "v" ) {
              client.println(ver);
            }
            
            if (linea1 == "r" ) {
              client.println("reset");
              ESP.reset(); 
            }

            if (linea1 == "ap" ) {
              client.println("config");
              configure = 1;
            }

            if (linea1 == "m" ) {
              
              client.print(WiFi.macAddress());
              client.print("&");
              client.println(Temp_type);

            }
            
            client.flush();
            digitalWrite(2, HIGH);

  }

}
