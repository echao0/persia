
Serial.println("");
Serial.println("---------------------------------------------------------");
Serial.println("Entrando en modo de configuracion");
Serial.println("---------------------------------------------------------");
Serial.println("");
entry = 1;

ESP.eraseConfig(); //Borramos toda la configuracion actual
             
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
                                  
                                  String linea1 = client.readStringUntil('#');
                                  Serial.println(linea1);

                              //------Reviso si hay algo en la linea LINEAL (recibida de explorador, para saber que se ha completado el form)--------
                                  
                                   if (linea1.indexOf("wifi=")>0){  
                                    
                                               Serial.println("Wifi: " + linea1.substring(linea1.indexOf("wifi=") +5, linea1.indexOf("&"))); 
                                               Serial.println("Password: " + linea1.substring(linea1.indexOf("pass=") +5, linea1.indexOf("&",linea1.indexOf("pass="))));
                                               Serial.println("Ip dispositivo: " + linea1.substring(linea1.indexOf("disp_ip=") +8, linea1.indexOf("&" ,linea1.indexOf("disp_ip="))));
                                               Serial.println("Mascara dispositivo: " + linea1.substring(linea1.indexOf("disp_masc=") +10, linea1.indexOf("&",linea1.indexOf("disp_masc="))));
                                               Serial.println("Gateway dispositivo: " + linea1.substring(linea1.indexOf("disp_gate=") +10, linea1.indexOf("&",linea1.indexOf("disp_gate="))));
                                               Serial.println("Tipode sensor temperatura: " + linea1.substring(linea1.indexOf("disp_sensor=") +12, linea1.indexOf(" ",linea1.indexOf("disp_sensor="))));
                                                
                                                String leido = "";
            
                                                
                                              leido = linea1.substring(linea1.indexOf("wifi=") +5, linea1.indexOf("&"));
                                              leido.toCharArray(wifi_ssid_private,32);            //Convierto el String a Char
            
                                              leido = linea1.substring(linea1.indexOf("pass=") +5, linea1.indexOf("&",linea1.indexOf("pass=")));
                                              leido.toCharArray(wifi_password_private,32);        //Convierto el String a Char
                                              
                                              leido = linea1.substring(linea1.indexOf("disp_ip=") +8, linea1.indexOf("&" ,linea1.indexOf("disp_ip=")));
                                              leido.toCharArray(ipAddr,16);                       //Convierto el String a Char
                                              
                                              leido = linea1.substring(linea1.indexOf("disp_masc=") +10, linea1.indexOf("&",linea1.indexOf("disp_masc=")));
                                              leido.toCharArray(esubnet,16);                      //Convierto el String a Char
            
                                              leido = linea1.substring(linea1.indexOf("disp_gate=") +10, linea1.indexOf("&",linea1.indexOf("disp_gate=")));
                                              leido.toCharArray(egateway,16);                     //Convierto el String a Char

                                              leido = linea1.substring(linea1.indexOf("disp_sensor=") +12, linea1.indexOf(" ",linea1.indexOf("disp_gate=")));
                                              leido.toCharArray(Temp_type,3); 
                                    
                                        //------------------GRABAR EN LA EEPROM LOS VALORES--------------------------------------
                                              
                                                writeEEPROM(0,32,wifi_ssid_private);                //32 byte max length
                                                writeEEPROM(32,32, wifi_password_private);          //32 byte max length
                                                writeEEPROM(64,16, esubnet);                         //16 byte max length
                                                writeEEPROM(80,16, ipAddr);                         //16 byte max length
                                                writeEEPROM(96,16, egateway);                         //16 byte max length
                                                writeEEPROM(112,3, Temp_type);                         //3 byte max length
                                              
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

                                    client.println("Tipo de sensor de temperatura:<br>");
                                    client.println("Puede ser: DHT o LM3<br>");
                                    client.println("<input type=\"text\" name=\"disp_sensor\" value=\"\">");
                                    client.println("<br><br>");
                                    
                                    client.println("<input type=submit></form>");
                                    
                                    client.println("</body></html>");
                                    
                                    delay(1);
                                    
                               }
                        //-------------------------------------------------------------------------------------------------

                        
                    }
