
#include <SoftwareSerial.h>
SoftwareSerial esp8266(7, 6); // RX | TX

String ardu_ip = "192.168.3.172";
int    ardu_id = 50;                //Hay que introducir en ASCII 49 = 1; 50 = 2; 51 = 3; 
String wifi_name = "Your_ssid";
String wifi_pass = "your_password";
String wifi_test = "192.168.3.1";

//-------------Para transistor
//---
//  alto = LOW;
//  bajo = HIGH;

char alto = LOW;
char bajo = HIGH;

bool DEBUG = false;

String MSJ;
int accion = 0;
// int testMode = 12;
String readstring;

unsigned long previousMillis = 0;          // Guardar el tiempo del ultimo lanzamiento
const long interval = 31000;               // intervalo de comprobacion de desconexion 39000
int reload = 0;                          //Variable de deteccion de desconexion de esp
String test = "";                         //Variable con respuesta de test de estado de esp

const int timeout = 100;


int readStop = false;


void setup()
{
  Serial.begin(9600);     
  esp8266.begin(9600);
  
  delay(1000);
  
  // pinMode(ConfigButton, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  
  
  //----------PARTE DE CONTROL DE GPIO----------------//

  int outS = 9;
  int outB = 11;
  
  digitalWrite(outS, alto);
  pinMode(outS, OUTPUT);        // Salida de rele

  digitalWrite(outB, alto);
  pinMode(outB, OUTPUT);        // Salida de rele

  // pinMode(testMode, INPUT);    // Control de entrada para salto de config esp8266
  

    //--------------------------------------------------------------------------//
    //Configurar esp8266
    //--------------------------------------------------------------------------//
    Serial.println("Inic conf  ESP8266");
    delay(500);
    load_esp();
  //}

  //--------------------------------------------------------------------------//
  //--------------------------------------------------------------------------//
  Serial.println("");
  Serial.println("ESP Ready");
  Serial.println("");
}

void loop()
{
  

    //-----------------Deteccion de desconexion de ESP8266 ----------------------------------//
    //---------------------------------------------------------------------------------------//

    unsigned long currentMillis = millis();                     //Guardar el tiempo actual para poder comprobar

    if (currentMillis - previousMillis >= interval) {
      Serial.println(currentMillis - previousMillis);
      previousMillis = currentMillis;                   // ultima comprobación de esp

//Saber el estado de la conexion

      //String test = sendData("AT+CWJAP?", 1000, DEBUG);
      //Serial.println(test);
      
      //sacar el nombre de la wifi
              
      //test=test.substring(test.indexOf(":")+2,test.indexOf(",")-1);
      //Serial.println(test);

//Saber si estas conectado a la WIFI
    String test_wifi = sendData("AT+CIPSTATUS", 1000, DEBUG);
    Serial.println(test_wifi);

    //sacar el estado de la wifi 2 es correcto
    
      test_wifi = test_wifi.substring(22, 23 );

      if (test_wifi == "5" ) {
      
       Serial.println("No hay respuesta de red");
       Serial.println("RECONFIGURANDO ESP");
        reload = 1;
        load_esp();
        delay(200);
       }


// realizar ping a router
//    Serial.println("");
//    Serial.println("Compruebo");
//    
//    String test = sendData("AT+PING=\""+wifi_test+"\"", 1000, DEBUG);
//
//    test=test.substring(25, 26);
//
//    int test_server = test.toInt();
//    
//    for (int x = 1; x < 8; x++){
//
//      if (x == test_server){
//        test_server="1";
//        break;
//      }
//      
//    }
//    
//    if (test_server != "1") {
//      
//      Serial.println("No hay respuesta de red");
//      Serial.println("RECONFIGURANDO ESP");
//      reload = 1;
//      test_server = 0;
//      load_esp();
//      delay(200);
//      }

    
//Saber si esta activado el modo servidor.
      
      test = sendData("AT+CIPMUX?", 500, false);
      String test_cut = test.substring(21, 22);            //Quitar todos los caracteres y quedarse con el estado

      if (test_cut == "0") {
        Serial.println("ESP FALLA");
        Serial.println("RECONFIGURANDO ESP");
        reload = 1;
        test_cut = 1;
        load_esp();
      }
      
      
    }

    //---------------------------------------------------------------------------------------//
    //---------------------------------------------------------------------------------------//

    if ( esp8266.available() )   {
      //+IPD,0,1:u
      long int time = millis();

      while ( (time + 100) > millis())
      {
        if (esp8266.available()) {
          char c = esp8266.read(); // read the next character.
          
        // if ((c!='\r') && (c!='\n')){  //evito guardar el retorno y el salto y todo va en una linea
                MSJ += c;
        //  }
        }
      }
      
      if (MSJ.length() > 13 and MSJ.length() < 25) {
         
        if (DEBUG == true) {
          Serial.println("Mensaje 1: " + MSJ.substring(11, 12));
        }
        if (DEBUG == true) {
          Serial.println("Mensaje 2: " + MSJ.substring(23, 24));
          Serial.println("");
        }

        control_relee(MSJ.substring(11, 12));
        delay(10);
        control_relee(MSJ.substring(23, 24));

      }

      if (MSJ.length() < 13 and MSJ.length() > 11 ) {
         
        String datReq = MSJ.substring(11, 12);
        
        if (DEBUG == true) {
          Serial.println("Mensaje: " + datReq);
          Serial.println("");
          
        }
        if (control_relee(datReq)){
            previousMillis = currentMillis;
             if (DEBUG == true) {Serial.println("Reset tempo de control");}
          };
       // control_relee(datReq);
      }

      MSJ = "";
    }


    // listen for user input and send it to the ESP8266

    if (DEBUG == true) {
      if ( Serial.available() ) {
        esp8266.write( Serial.read() );
      }
    }
  }



//----------------------------------------------------//
// FINAL DE LOOP DE EJECUCION
//----------------------------------------------------//

int control_relee(String datReq) {
  int outS = 9;
  int outB = 11;
  int rtn = 0;
  
  if (datReq == "p") { //Do the following if Pulse of life is requested
  
    sendData("AT+CIPSEND=0,1", 10, false);
    esp8266.write(ardu_id);       //numero del dispositivo
    rtn = 1;
  }

  if (datReq == "s" ) {

    digitalWrite(outS, alto);         // Detengo el rele de bajada
    if (accion != 0 ) {
      delay(500);
    }
    digitalWrite(outB, bajo);        //Activo rele de subida
    accion = 1;                    //Cambio la variable a movimiento

    sendData("AT+CIPSEND=0,1", 10, false);
    esp8266.write("1");
  }

  if (datReq == "b" ) {

    digitalWrite(outB, alto);        //detengo rele de subida
    if (accion != 0 ) {
      delay(500);
    }
    digitalWrite(outS, bajo);         // activo el rele de bajada
    accion = 1;                    //Cambio la variable a movimiento

    sendData("AT+CIPSEND=0,1", 10, false);
    esp8266.write("1");

  }

  if (datReq == "z" ) {

    digitalWrite(outB, alto);          //detengo rele de subida
    digitalWrite(outS, alto);         // detengo el rele de bajada
    accion = 0;                    //Cambio la variable a paro

    sendData("AT+CIPSEND=0,1", 10, false);
    esp8266.write("1");
  }

  if (datReq == "k" ) {

    digitalWrite(outB, alto);          //detengo rele de subida
    digitalWrite(outS, alto);         // detengo el rele de bajada
    
    delay(100);
    sendData("AT+CIPSEND=0,1", 10, false);
    esp8266.write("1");
    
    Serial.println("init conf   Esp8266");
    load_esp();      
    
    accion = 0;                            //Cambio la variable a paro

    
  }
  
  
  datReq = "";
  return rtn;
  

}

int load_esp() {

  if (sendData("AT", 100, DEBUG) == "") {
    Serial.println("NO ESP8266");
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(150);
    digitalWrite(LED_BUILTIN, LOW);
    delay(150);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(150);
    digitalWrite(LED_BUILTIN, LOW);
    delay(150);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(150);
    digitalWrite(LED_BUILTIN, LOW);
    delay(150);
    digitalWrite(LED_BUILTIN, HIGH);

    sendData("AT+RST", 1000, DEBUG);
    //sendData("AT+RST=1", 2000, DEBUG);
    
    sendData("ATE1", 2000, DEBUG);              //Activar el echo de comando
    sendData("AT+CWMODE_DEF=1", 2000, DEBUG);   //activar el modo de cliente
    sendData("AT+CIPSTA=\""+ardu_ip+"\"", 2000, DEBUG);   //Seleccionar la IP
    sendData("AT+CWJAP=\""+wifi_name+"\",\""+wifi_pass+"\"", 6000, DEBUG);    //Conectarse a la Wifi
    sendData("AT+CIPMUX=1", 2000, DEBUG);   //Creo el servidor para recibir conexiones
    sendData("AT+CIPSERVER=1,5000", 2000, DEBUG); //Selecciono el puerto de entrada de conexiones


    Serial.println("ESp8266 OK!!");
    reload = 0;
    digitalWrite(LED_BUILTIN, LOW);

  }
}


String sendData(String command, const int timeout, boolean debug)
{

  esp8266.print(command + "\r\n"); // send the read character to the esp8266

  String response = "";

  long int time = millis();
 
  while ( (time + timeout) > millis())
  {
    if (esp8266.available())
    {
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }

  if (debug)
  {
    Serial.print(response);
  }

  return response;
}



