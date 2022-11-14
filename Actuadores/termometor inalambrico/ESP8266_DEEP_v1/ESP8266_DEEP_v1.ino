String ver = "DEEP1.1";

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "toCharFunction.h"
#include <ArduinoJson.h>
#include "DHTesp.h"
#include "OTA.h"

String espName = "termo";
String topic_order = "persia/termo2/order";
String topic_dato = "persia/termo2";



/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Me-House"
#define WLAN_PASS       "Et-micasa"

/************************* Adafruit.io Setup *********************************/
#define SERVER      "192.168.3.181"
#define SERVERPORT  1883                   // use 8883 for SSL
#define MQTT_USERNAME    "test"
#define MQTT_PASS         "test"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, SERVER, SERVERPORT, MQTT_USERNAME, MQTT_PASS);

/****************************** Feeds ***************************************/
// Notice MQTT paths for AIO follow the form: feeds/<feedname>
Adafruit_MQTT_Publish topic_log = Adafruit_MQTT_Publish(&mqtt,"persia/temp");
Adafruit_MQTT_Publish topic_STATUS = Adafruit_MQTT_Publish(&mqtt,"persia/termo2/status");
Adafruit_MQTT_Publish topic_VOLTAGE = Adafruit_MQTT_Publish(&mqtt,"persia/termo2/voltage");
Adafruit_MQTT_Publish topic_TEMP = Adafruit_MQTT_Publish(&mqtt,"persia/termo2/temp");

// Setup a feed called 'calefaccion' for orders ans status.
Adafruit_MQTT_Subscribe topic_datos = Adafruit_MQTT_Subscribe(&mqtt,"persia/termo2");
Adafruit_MQTT_Subscribe topic_orders = Adafruit_MQTT_Subscribe(&mqtt,"persia/termo2/order");
Adafruit_MQTT_Subscribe topic_time = Adafruit_MQTT_Subscribe(&mqtt,"persia/termo2/time");

int x=1;
long timer;
void MQTT_connect();
DHTesp dht;

ADC_MODE(ADC_VCC);
int vcc;

void setup() {
  Serial.begin(9600);
  dht.setup(4,DHTesp::DHT22); // Connect DHT sensor to GPIO 17

  
// Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");

    // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&topic_datos);
  mqtt.subscribe(&topic_orders); 
  mqtt.subscribe(&topic_time); 
  
  
  }
  
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
  
}

void loop() {
   
  MQTT_connect();
  
  topic_STATUS.publish(toCharFunction("Online"));
 
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  //temperature = temperature-1; // Correccion de lectura contra TEMPE de Garmin

  topic_log.publish(toCharFunction("{\"disp\": \""+espName+"\",\"Temperatura\": \""+temperature+"\",\"Humedad\": \""+String(humidity)+"\"}"));
  topic_TEMP.publish(toCharFunction("{\"disp\": \""+espName+"\",\"Temperatura\": \""+temperature+"\",\"Humedad\": \""+String(humidity)+"\"}"));
  
  Serial.print("Temperatura :");
  Serial.println(temperature);

  vcc = ESP.getVcc();
  double dvcc = (float)vcc / 1024;
  Serial.println("Voltage: " + String(dvcc, 3) + "V");
  topic_VOLTAGE.publish(toCharFunction(String(dvcc, 3)));

  //delay(1000);
 
Adafruit_MQTT_Subscribe *subscription;

  
  while ((subscription = mqtt.readSubscription(500))) {
    if (subscription == &topic_datos) {
      Serial.print(F("Got: "));
      Serial.println((char *)topic_datos.lastread);
    }
    if (subscription == &topic_time) {
      Serial.print(F("Got: "));
      Serial.println((char *)topic_time.lastread);
      
      StaticJsonDocument<200> doc1;
      DeserializationError error = deserializeJson(doc1, (char *)topic_time.lastread);
      if (error) {
             Serial.print(F("deserializeJson() failed: "));
             Serial.println(error.f_str());
              return;
        }
  timer = doc1["time"];
      
      
    }
    if (subscription == &topic_orders) {
      Serial.print(F("Got Order: "));
      Serial.println((char *)topic_orders.lastread);

      StaticJsonDocument<200> doc1;
      DeserializationError error = deserializeJson(doc1, (char *)topic_orders.lastread);
      if (error) {
             Serial.print(F("deserializeJson() failed: "));
             Serial.println(error.f_str());
              return;
        }
        
        String order = doc1["order"];

        if (order == "test" ) {
                      topic_STATUS.publish(toCharFunction("12345678901234567890123456789012345678901234567890"));
                      delay(200);
                    }
        if (order == "vol" ) {
                      topic_STATUS.publish(toCharFunction(String(dvcc, 3)));
                      delay(200);
                    }

        if (order == "restart" ) {
                      topic_STATUS.publish(toCharFunction(espName+"-Restarting"));
                      delay(200);
                      //ESP.reset();
                      ESP.restart();
                    }
        if (order == "getInfo" ) {
                      //client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"version\": \""+String(ver)+"\",\"IP\": \""+WiFi.localIP().toString()+"\",\"MAC\": \""+WiFi.macAddress()+"\",\"staus\": \"alive\"}")); 
                      topic_STATUS.publish(toCharFunction("{\"disp\": \""+espName+"\",\"version\": \""+String(ver)+"\",\"IP\": \""+WiFi.localIP().toString()+"\",\"MAC\": \""+WiFi.macAddress()+"\"}"));
                    }
                    
        if (order == "OTA" ) {
                        Serial.println("Modo OTA");
                        //topic_STATUS.publish(toCharFunction("{\"disp\": \""+espName+"\",\"version\": \""+String(ver)+"\",\"IP\": \""+WiFi.localIP().toString()+"\"}"));
                        topic_STATUS.publish(toCharFunction("{\"disp\": \""+espName+"\",\"IP\": \""+WiFi.localIP().toString()+"\",\"mode\": \"OTA->ON\"}"));
                        OTA();
                    }

        if (order == "sleep" ) {
                        
                        Serial.println("Modo ESP8266 deep sleep durante 30 seg ");
                        topic_log.publish(toCharFunction("A DORMIR 30 seg"));
                        ESP.deepSleep(30e6); // 10e6 es 10.000.000 microsegundos´
                    }
                    
    }
  }
  
 //----------------------------------------------------------//
if (x>=1){
    //topic_STATUS.publish("a dormir");
    Serial.println("Sleep");
    topic_STATUS.publish(toCharFunction("Sleep"));
    delay(100);
    if (timer){
      Serial.println("timer");
      Serial.println(timer);
      ESP.deepSleep(timer);
    }else{
      ESP.deepSleep(210e6);
      }
  } // 10e6 es 10.000.000 microsegundos
 x = x+1;
 
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(200);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }

  Serial.println("MQTT Connected!");
  
}
