#include <ESP8266WiFi.h>

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

int accion = 0;

//-------------Web_Conf-------
  const long interval = 1000;
  int entry = 0;
  unsigned long currentMillis = 0;
//----------------------------


void setup() {

Serial.begin(115200);
delay(10);

//strcat(wifi_ssid_private, "ssid");
//strcat(wifi_password_private, "password");


// ---- Leer los datos de configuracion desde EEPROM

readEEPROM(0,32,wifi_ssid_private);
readEEPROM(32,32,wifi_password_private);
readEEPROM(64,16,esubnet);
readEEPROM(80,16,ipAddr);
readEEPROM(96,16,egateway);

//---- Convertir los resultados de string a IPADDRES----------

ip.fromString(ipAddr);
subnet.fromString(esubnet);
gateway.fromString(egateway);

//-------------------------------------------------------------

delay(500);

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

 //-----------IN/OUT------------
  
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
 
  pinMode(outS, OUTPUT);
  digitalWrite(outS, bajo);
  
  pinMode(outB, OUTPUT);
  digitalWrite(outB, bajo);

  pinMode(0, INPUT); // Boton de flash como entrada para web config
  
//-----------------------------

  Serial.println();
  Serial.println();
  Serial.print("Conectandose a red : ");
  Serial.println(wifi_ssid_private);

  WiFi.mode(WIFI_STA); //especifico que no quiero crear wifi
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(wifi_ssid_private, wifi_password_private); //Conexión a la red

  int pass = 0;         //Control de tiempo maximo para la conexión wifi
  
  while ((WiFi.status() != WL_CONNECTED) and (pass != 30))
  {
    delay(500);
    Serial.print(".");
    pass++;
    
  }
  Serial.println("");
  Serial.println("WiFi conectado");


  server.begin(); //Iniciamos el servidor
  Serial.println("Servidor Iniciado");


  Serial.print("IP Configurada: ");
  Serial.println(WiFi.localIP()); //Obtenemos la IP

}

void loop() {

  WiFiClient client = server.available();
  client.setTimeout(50); //especificar el tiempo maximo de espera del cliente
  
  //----------------------------------------------------------------------------------
  // Activo la pag de configuración cuando el botón = (Flash) es pulsado durante Variable "interval"
  //----------------------------------------------------------------------------------
  
  while (digitalRead(0) == 0){
       
         if (entry == 0){currentMillis = millis(); entry = 1;}
        
          if (millis() - currentMillis >= interval) {
            Serial.println("---------------------------------");
            Serial.println("Entrando en modo de configuracion");
            Serial.println("---------------------------------");

             IPAddress local_IP(192,168,4,1);
             IPAddress gateway(192,168,4,1);
             IPAddress subnet(255,255,255,0);
             
            WiFiServer server(80);
            server.begin();
            
            Serial.print("Setting soft-AP configuration ... ");
            Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
          
            Serial.print("Setting soft-AP ... ");
            Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");
          
            Serial.print("Soft-AP IP address = ");
            Serial.println(WiFi.softAPIP());
            
 
            Serial.println("Ingrese desde un navegador web usando la siguiente IP:");
            Serial.println(WiFi.localIP()); //Obtenemos la IP
             
             for (int i=0; i <= 4; i++){
                        
                        digitalWrite(2, !digitalRead(2)); 
                        delay(500);
                        }
                        
              while (entry == 1){
         
                        //-------------------------------------------------------------------------------------------------
                        
                        WiFiClient client = server.available();
                         if (client) //Si hay un cliente presente
                               { 
                                  Serial.println("Nuevo Cliente");
                                  
                                  
                               //-------------------Esperar a que se conecte el cliente---------------
                                  
                                  while(!client.available()&&client.connected())
                                  {
                                  delay(1);
                                  }
                                  
                              // --------Leer la respuesta del cliente y guardarla en variable------------
                                  
                                  String linea1 = client.readStringUntil('r');
                                  Serial.println(linea1);

                              //------Reviso si hay algo en la linea LINEAL (recibida de explorador, para saber que se ha completado el form)--------
                                  
                                   if (linea1.indexOf("wifi=")>0){  
                                    
                                               Serial.println("Wifi: " + linea1.substring(linea1.indexOf("wifi=") +5, linea1.indexOf("&"))); 
                                               Serial.println("Password: " + linea1.substring(linea1.indexOf("pass=") +5, linea1.indexOf("&",linea1.indexOf("pass="))));
                                               Serial.println("Ip dispositivo: " + linea1.substring(linea1.indexOf("disp_ip=") +8, linea1.indexOf("&" ,linea1.indexOf("disp_ip="))));
                                               Serial.println("Mascara dispositivo: " + linea1.substring(linea1.indexOf("disp_masc=") +10, linea1.indexOf("&",linea1.indexOf("disp_masc="))));
                                               Serial.println("Gateway dispositivo: " + linea1.substring(linea1.indexOf("disp_gate=") +10, linea1.indexOf(" ",linea1.indexOf("disp_gate="))));
                                                
                                                String leido = "";
            
                                                
                                              leido = linea1.substring(linea1.indexOf("wifi=") +5, linea1.indexOf("&"));
                                              leido.toCharArray(wifi_ssid_private,32);            //Convierto el String a Char
            
                                              leido = linea1.substring(linea1.indexOf("pass=") +5, linea1.indexOf("&",linea1.indexOf("pass=")));
                                              leido.toCharArray(wifi_password_private,32);        //Convierto el String a Char
                                              
                                              leido = linea1.substring(linea1.indexOf("disp_ip=") +8, linea1.indexOf("&" ,linea1.indexOf("disp_ip=")));
                                              leido.toCharArray(ipAddr,16);                       //Convierto el String a Char
                                              
                                              leido = linea1.substring(linea1.indexOf("disp_masc=") +10, linea1.indexOf("&",linea1.indexOf("disp_masc=")));
                                              leido.toCharArray(esubnet,16);                      //Convierto el String a Char
            
                                              leido = linea1.substring(linea1.indexOf("disp_gate=") +10, linea1.indexOf(" ",linea1.indexOf("disp_gate=")));
                                              leido.toCharArray(egateway,16);                     //Convierto el String a Char
                                    
                                        //------------------GRABAR EN LA EEPROM LOS VALORES--------------------------------------
                                              
                                                writeEEPROM(0,32,wifi_ssid_private);                //32 byte max length
                                                writeEEPROM(32,32, wifi_password_private);          //32 byte max length
                                                writeEEPROM(64,16, esubnet);                         //16 byte max length
                                                writeEEPROM(80,16, ipAddr);                         //16 byte max length
                                                writeEEPROM(96,16, egateway);                         //16 byte max length
                                              
                                        //-----------------------------------------------------------------------------------------
                                              
                                                Serial.println("Wifi AP apagada");
                                                delay(1000);
                                                WiFi.mode(WIFI_STA);
                                                ESP.reset();                                      
                                 
                                             }
                                  
                                    client.flush(); 

                                    
                            //-------------------Enviando la web al explorador-----------------------------   
                                    
                                    client.println("HTTP/1.1 200 OK");
                                    client.println("Content-Type: text/html");
                                    client.println("Connection: close");// La conexión se cierra después de finalizar de la respuesta
                                    client.println() ;
                                    client.println("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">");
                                    client.println(" <html xmlns=\"http://www.w3.org/1999/xhtml\">");
                                    
                                    client.println("<!DOCTYPE HTML>");
                                    client.println("<html>");
                                    client.println("<head><title>Persia disp config :) </title>");
                                      
                                    client.println("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" /></head>");
                                    client.println("<body><h1> Configuración de Dispositivo</h1>");
                                    
                                    client.println("<form action=\"#\">");
                                    client.println("Wifi name:<br>");
                                    client.println("<input type=\"text\" name=\"wifi\" value=\"\">");
                                    client.println("<br><br>");
                                    
                                    client.println("Wifi Password:<br>");
                                    client.println("<input type=\"text\" name=\"pass\" value=\"\">");
                                    client.println("<br><br>");
                                    
                                    client.println("IP Dispositivo:<br>");
                                    client.println("<input type=\"text\" name=\"disp_ip\" value=\"\">");
                                    client.println("<br><br>");
                                    
                                    client.println("Mascara Dispositivo:<br>");
                                    client.println("<input type=\"text\" name=\"disp_masc\" value=\"\">");
                                    client.println("<br><br>");
                                    
                                    client.println("Gateway Dispositivo:<br>");
                                    client.println("<input type=\"text\" name=\"disp_gate\" value=\"\">");
                                    client.println("<br><br>");
                                    
                                    client.println("<input type=submit></form>");
                                    
                                    client.println("</body></html>");
                                    
                                    delay(1);
                                    
                               }
                        //-------------------------------------------------------------------------------------------------

                        
                    }
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
    digitalWrite(2, LOW);
    //esperamos hasta que hayan datos disponibles
    while (!client.available() && client.connected())
    {
      delay(1);
    }

    // Leemos la primera el mensaje del servidor.
    String linea1 = client.readStringUntil('r');
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
        int analogValue = analogRead(tempPin);
        //float millivolts = (analogValue/1024.0) * 3300; //3300 is the voltage provided by NodeMCU
        //float celsius = millivolts/10;
        Serial.print("Valor Leido=   ");
        Serial.println(analogValue);
        client.println(analogValue);
    }
    

    client.flush();
    // client.println("Connection: close");
    digitalWrite(2, HIGH);

  }
}
