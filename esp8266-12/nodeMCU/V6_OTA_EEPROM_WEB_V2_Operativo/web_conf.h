void web_conf(){

    const long interval = 1000;
    bool entry = 0;
    unsigned long currentMillis = 0;
  
         if (entry == 0){currentMillis = millis(); entry = 1;}
        
          if (millis() - currentMillis >= interval) {
            Serial.println("---------------------------------");
            Serial.println("Entrando en modo de configuracion");
            Serial.println("---------------------------------");

             IPAddress local_IP(192,168,4,1);
             IPAddress gateway(192,168,4,1);
             IPAddress subnet(255,255,255,0);
             
            
            
            Serial.print("Setting soft-AP configuration ... ");
            Serial.println(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
          
            Serial.print("Setting soft-AP ... ");
            Serial.println(WiFi.softAP("ESPsoftAP_01") ? "Ready" : "Failed!");
          
            Serial.print("Soft-AP IP address = ");
            Serial.println(WiFi.softAPIP());
            
 
            Serial.println("Ingrese desde un navegador web usando la siguiente IP:");
            Serial.println(WiFi.localIP()); //Obtenemos la IP
             
             for (int i=0; i <= 4; i++){
                        
                        digitalWrite(D0, LOW); 
                        delay(250);
                        digitalWrite(D0, HIGH); 
                        delay(250);
                        }
                        
              while (entry == 1){
                   
                        //Programa de WEB
                        //entry = 0;

                        //-------------------------------------------------------------------------------------------------
                        WiFiClient client = server.available();
                         if (client) //Si hay un cliente presente
                               { 
                                  Serial.println("Nuevo Cliente");
                                  
                                  
                                  //esperamos hasta que hayan datos disponibles
                                  while(!client.available()&&client.connected())
                                  {
                                  delay(1);
                                  }
                                  
                                  // Leemos la primera línea de la petición del cliente.
                                  String linea1 = client.readStringUntil('r');
                                  Serial.println(linea1);

                                    if (linea1.indexOf("LED=ON")>0) //Buscamos un LED=ON en la 1°Linea
                                    {
                                      digitalWrite(D4,LOW);
                                    }
                                    if (linea1.indexOf("LED=OFF")>0)//Buscamos un LED=OFF en la 1°Linea
                                    {
                                      digitalWrite(D4,HIGH);
                                    }
                                     if (linea1.indexOf("WIFI=STOP")>0)//Buscamos un WIFI=STOP en la 1°Linea
                                    {
                                      
                                      client.println("Wifi AP apagada");
                                      client.println("Reinicie el modulo");
                                      Serial.println("Wifi AP apagada");
                                      delay(5000);
                                      WiFi.mode(WIFI_STA);
                                    }
                                    
                                    client.flush(); 
                                                
                                    Serial.println("Enviando respuesta...");   
                                    //Encabesado http    
                                    client.println("HTTP/1.1 200 OK");
                                    client.println("Content-Type: text/html");
                                    client.println("Connection: close");// La conexión se cierra después de finalizar de la respuesta
                                    client.println();
                                    //Pagina html  para en el navegador
                                    client.println("<!DOCTYPE HTML>");
                                    client.println("<html>");
                                    client.println("<head><title>ESP CONFIG</title>");
                                    client.println("<body>");
                                    client.println("<h1 align='center'>Test ESP8266</h1>");
                                    client.println("<div style='text-align:center;'>");
                                    client.println("<br />");            
                                    client.println("<button onClick=location.href='./?LED=ON'>LED ON</button>");           
                                    client.println("<button onClick=location.href='./?LED=OFF'>LED OFF</button>");
                                    client.println("<button onClick=location.href='./?WIFI=STOP'>Kill Config</button>");
                                    client.println("<br />");
                                    client.println("</div>");
                                    client.println("</body>");
                                    client.println("</html>");
                                    
                                    delay(1);
                                    Serial.println("respuesta enviada");
                                    Serial.println();
                                    
                               }
                        //-------------------------------------------------------------------------------------------------

                        
                    }
            }
            
            delay(10);
  
  }
