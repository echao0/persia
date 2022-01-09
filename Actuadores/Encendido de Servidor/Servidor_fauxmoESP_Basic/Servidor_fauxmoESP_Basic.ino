#include <Arduino.h>
#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif
#include "fauxmoESP.h"

// Rename the credentials.sample.h file to credentials.h and 
// edit it according to your router configuration
#include "credentials.h"

fauxmoESP fauxmo;

// -----------------------------------------------------------------------------

#define SERIAL_BAUDRATE     115200

#define OFF_TIME            70000
#define SERVER              D2
#define LED_GREEN           5
#define LED_BLUE            0
#define LED_PINK            2
#define LED_WHITE           15

#define ID_SERVER           "servidor"
#define ID_GREEN            "green lamp"
#define ID_BLUE             "blue lamp"
#define ID_PINK             "pink lamp"
#define ID_WHITE            "white lamp"

//------------Local Outputs -----------------------

#define out1  D1
char outON = LOW;
char outOFF = HIGH;

// Conexion con el servidor-----------------------

const uint16_t port = 2000;
const char * host = "192.168.3.181";
WiFiClient client;

//-----Variables para temporizador de apagado server --------//
unsigned long timer = 0;
boolean timerConnect = false;
boolean seguro = false;

//-------------------------------------------------------------




// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

}

void setup() {

    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();

    // LEDs
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(out1, OUTPUT);
    pinMode(SERVER, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_PINK, OUTPUT);
    pinMode(LED_WHITE, OUTPUT);
    digitalWrite(out1, LOW);
    digitalWrite(SERVER, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_PINK, LOW);
    digitalWrite(LED_WHITE, LOW);

    // Wifi
    wifiSetup();

    // By default, fauxmoESP creates it's own webserver on the defined port
    // The TCP port must be 80 for gen3 devices (default is 1901)
    // This has to be done before the call to enable()
    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is required for gen3 devices

    // You have to call enable(true) once you have a WiFi connection
    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched
    fauxmo.enable(true);

    // You can use different ways to invoke alexa to modify the devices state:
    // "Alexa, turn yellow lamp on"
    // "Alexa, turn on yellow lamp
    // "Alexa, set yellow lamp to fifty" (50 means 50% of brightness, note, this example does not use this functionality)

    // Add virtual devices
    fauxmo.addDevice(ID_SERVER);
    //fauxmo.addDevice(ID_GREEN);
    //fauxmo.addDevice(ID_BLUE);
    //fauxmo.addDevice(ID_PINK);
    //fauxmo.addDevice(ID_WHITE);

    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        
        // Callback when a command from Alexa is received. 
        // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
        // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
        // Just remember not to delay too much here, this is a callback, exit as soon as possible.
        // If you have to do something more involved here set a flag and process it in your main loop.
        
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
        
         // Detectada la peticion de encendido y activo el relee - Cambio estado de timerConnect para evitar el apagado del relee
          if (state == 1){ 
              Serial.println("RELEE ON");
              digitalWrite(out1, LOW); 
              digitalWrite(LED_BUILTIN, LOW);
              timerConnect = false;
              seguro = false;
          }

          // Detectado el comando de apagado - Activo flag de timer ON y activo el seguro.
          if (state == 0){
            Serial.println("TIMER STARTED");
            timerConnect = true;
            seguro = true;
            timer = millis();
        }
        
        // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
        // Otherwise comparing the device_name is safer.

        if (strcmp(device_name, ID_SERVER)==0) {
            digitalWrite(SERVER, state ? HIGH : LOW);
        } else if (strcmp(device_name, ID_GREEN)==0) {
            digitalWrite(LED_GREEN, state ? HIGH : LOW);
        } else if (strcmp(device_name, ID_BLUE)==0) {
            digitalWrite(LED_BLUE, state ? HIGH : LOW);
        } else if (strcmp(device_name, ID_PINK)==0) {
            digitalWrite(LED_PINK, state ? HIGH : LOW);
        } else if (strcmp(device_name, ID_WHITE)==0) {
            digitalWrite(LED_WHITE, state ? HIGH : LOW);
        }

    });

}

void loop() {

//-------Temporizador apagar server ------- //
//------------------------------------------// 
   if ((timerConnect) and (seguro)){
    
     if (!client.connect(host, port)) {
            delay(1000); 
            }           
          Serial.println("OFF SERVER ORDER OK"); 
          client.print("server,4"); // mando paquete apagado SSH
          client.stop();
          seguro = false;
   }
   
   if ((timerConnect) and (millis() > timer + OFF_TIME)  ){
       Serial.println("RELEE OFF");
       digitalWrite(out1, HIGH);
       digitalWrite(LED_BUILTIN, HIGH);
       timerConnect = false;
    }

//----------------------------------------

    // fauxmoESP uses an async TCP server but a sync UDP server
    // Therefore, we have to manually poll for UDP packets
    fauxmo.handle();

    // This is a sample code to output free heap every 5 seconds
    // This is a cheap way to detect memory leaks
    static unsigned long last = millis();
    if (millis() - last > 5000) {
        last = millis();
        Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
    }

    // If your device state is changed by any other means (MQTT, physical button,...)
    // you can instruct the library to report the new state to Alexa on next request:
    // fauxmo.setState(ID_SERVER, true, 255);

}
