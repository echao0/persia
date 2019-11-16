#include <ESP8266WiFi.h>
#include "DHT.h"

String ver = "2.0";

/*
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

//-----------ETH22------------
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
uint8_t DHTPin = D3;
DHT dht(DHTPin, DHTTYPE);                
float Temperature;
float Humidity;
//--------------------------


//-----------OTA------------
#include "OTA.h"
//--------------------------

//----------EEPROM----------
#include "eeprom.h"
//--------------------------

IPAddress ip(0,0,0,0); // where xx is the desired IP Address
IPAddress gateway(0,0,0,0); // set gateway to match your network
IPAddress subnet(0,0,0,0); // set subnet mask to match your


WiFiServer server(5000);

int tempPin= A0;
char outS = D1;
char outB = D2;
char alto = LOW;
char bajo = HIGH;
int configure = 0; //Variable para entrar en config remotamente

int accion = 0;

//-------------Web_Conf-------
  const long interval = 1000;
  int entry = 0;
  unsigned long currentMillis = 0;
  
//----------------------------

//-------------Smoothing-------
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int tempOff = 0;
//----------------------------



void setup() {

Serial.begin(9600);
delay(10);

strcat(wifi_ssid_private, "Me-House");
strcat(wifi_password_private, "Et-micasa");


//-------------Smoothing-------

for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
//----------------------------
  
// ---- Leer los datos de configuracion desde EEPROM

readEEPROM(0,32,wifi_ssid_private);
readEEPROM(32,32,wifi_password_private);
readEEPROM(64,16,esubnet);
readEEPROM(80,16,ipAddr);
readEEPROM(96,16,egateway);
readEEPROM(112,3,Temp_type);

//---- Convertir los resultados de string a IPADDRES----------

ip.fromString(ipAddr);
subnet.fromString(esubnet);
gateway.fromString(egateway);



//-------------------------------------------------------------

delay(500);

Serial.println("");
Serial.println("Temp_type");
Serial.println("");
Serial.println("---------------------------------------------------------");
Serial.print("Versión de Software V");
Serial.println(ver);
Serial.println("---------------------------------------------------------");
Serial.println("");

Serial.println("");
Serial.println("---------------------------------------------------------");
Serial.println("");
Serial.print("Nombre de la wifi: ");
Serial.println(wifi_ssid_private);
Serial.print("Password de la wifi: ");
Serial.println(wifi_password_private);
Serial.print("Ip del terminal: ");
Serial.println(ip);
Serial.print("mascara del terminal: ");
Serial.println(subnet);
Serial.print("Gateway del terminal: ");
Serial.println(gateway);
Serial.print("Tipo de sensor: ");
Serial.println(Temp_type);
Serial.println("");

Serial.println("---------------------------------------------------------");
Serial.println("");


 //-----------IN/OUT------------
  
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
 
  pinMode(outS, OUTPUT);
  digitalWrite(outS, bajo);
  
  pinMode(outB, OUTPUT);
  digitalWrite(outB, bajo);

  pinMode(0, INPUT); // Boton de flash como entrada para web config
  
  pinMode(DHTPin, INPUT); // Lectura Eth22
  dht.begin();
  
//-----------------------------

  Serial.println();
  Serial.println();
  Serial.print("Conectandose a red : ");
  Serial.println(wifi_ssid_private);

  WiFi.mode(WIFI_STA); //especifico que no quiero crear wifi

   WiFi.config(ip, gateway, subnet);
   WiFi.begin(wifi_ssid_private, wifi_password_private); //Conexión a la red

  int pass = 0;         //Control de tiempo maximo para la conexión wifi
  
  while ((WiFi.status() != WL_CONNECTED) and (pass != 15))
  {
    delay(500);
    Serial.print(".");
    pass++;
    
  }

  if (WiFi.status() == WL_CONNECTED){
          Serial.println("");
          Serial.println("---------------------------------------------------------");
          Serial.println("");
          Serial.println("WiFi conectado");
            server.begin(); //Iniciamos el servidor
          Serial.println("Servidor Iniciado");
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
                      
                    }
          ESP.reset();
  
    }



}

void loop() {

  WiFiClient client = server.available();
  client.setTimeout(50); //especificar el tiempo maximo de espera del cliente
  
  //----------------------------------------------------------------------------------
  // Activo la pag de configuración cuando el botón = (Flash) es pulsado durante Variable "interval"
  //----------------------------------------------------------------------------------
  if (configure == 1) {
            Serial.println("");
            Serial.println("---------------------------------------------------------");
            Serial.println("Entrando en modo de configuracion");
            Serial.println("---------------------------------------------------------");
            Serial.println("");
            entry = 1;
            #include "Web_Setting.h"
    
  }
  
  configure = 0;
  
  while (digitalRead(0) == 0){
       
         if (entry == 0){currentMillis = millis(); entry = 1;}
        
          if ((millis() - currentMillis >= interval)) {
            Serial.println("");
            Serial.println("---------------------------------------------------------");
            Serial.println("Entrando en modo de configuracion");
            Serial.println("---------------------------------------------------------");
            Serial.println("");

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
            Serial.print("Nuevo Paquete: ");
            digitalWrite(2, LOW); // Si se activa el encendemos el led cuando tengamos un paquete
            
            //esperamos hasta que hayan datos disponibles
            while (!client.available() && client.connected())
            {
              delay(1);
            }
        
            // Leemos la primera el mensaje del servidor.
            String linea1 = client.readStringUntil('\r');
            Serial.println(linea1);
        
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
            
            
        
            client.flush();
            // client.println("Connection: close");
            digitalWrite(2, HIGH);

  }
}
