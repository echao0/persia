void callback(char* topic, byte* payload, unsigned int length) {

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload);
  JsonObject obj = doc.as<JsonObject>();
   
      Serial.println("-----------------------------------------");
      Serial.print("topic: ");
      Serial.println(topic);
      
          if (String(topic) == String(topicMode)){
            
              String Mode = obj[String("mode")];
              String Time = obj[String("time")];
              
                   Serial.print("mode : ");
                   Serial.println(Mode);
        
                   Serial.print("Time : ");
                    Serial.println(Time);
        
                if (Mode == "auto") {
                        WorkMode = "auto";
                        waitTime = Time.toInt(); 
                        client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"mode\": \"Automatico\",\"Time\": \""+Time+"\"}")); 
                        
                      }
                      
                if (Mode == "standard") {
                        WorkMode = "standard";
                        waitTime = 0;
                        client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"mode\": \"Standard\"}"));
                      }
                      
                if (Mode == "OTA" ) {
                        Serial.println("Modo OTA");
                        client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"mode\": \"OTA->ON\"}"));
                        OTA();
                    }
            }
        
          if (String(topic) == String(topicOrders)){
            
              String order = obj[String("order")];
            
              Serial.print("Order : ");
              Serial.println(order);
              Serial.println("-----------------------------------------");
              Serial.println("");
                
                if (order == "test" ) {
                      client.publish(topicLog, toCharFunction("12345678901234567890123456789012345678901234567890"));
                      delay(200);
                    }
                    
                if (order == "restart" ) {
                      client.publish(topicLog, toCharFunction(espName+"-Restarting"));
                      delay(200);
                      ESP.reset();
                    }
                    
                if (order == "getInfo" ) {
                      client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"version\": \""+String(ver)+"\",\"IP\": \""+WiFi.localIP().toString()+"\",\"MAC\": \""+WiFi.macAddress()+"\",\"staus\": \"alive\"}")); 
                    }

                if (order == "help" ) {
                      client.publish(topicLog, toCharFunction("O:test,O:restart,O:getInfo,O:getTemp,M:auto,M:manual"));
                    }
                    
                if (order == "getTemp" ) {
                      Temperature = dht.readTemperature();
                      Humidity = dht.readHumidity();
                      client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"Temperatura\": \""+String(Temperature)+"\",\"Humedad\": \""+String(Humidity)+"\"}"));
                    }
                 if (order == "up" ) {
                      client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"Accion\": \"Subiendo\"}"));
                      digitalWrite(outB, bajo);         // Detengo el rele de bajada
                       if (accion != 0 ) {
                          delay(500);
                        }
                      digitalWrite(outS, alto);        //Activo rele de subida
                      accion = 1;
                    }

                if (order == "down" ) {
                   
                    client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"Accion\": \"Bajando\"}"));
                    
                    digitalWrite(outS, bajo);        //detengo rele de subida
                      if (accion != 0 ) {
                        delay(500);
                      }
                      digitalWrite(outB, alto);         // activo el rele de bajada
                      accion = 1;                    //Cambio la variable a movimiento
                        }


              if (order == "stop" ) {
                 
                    client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"Accion\": \"Stop\"}"));
                    digitalWrite(outB, bajo);          //detengo rele de subida
                    digitalWrite(outS, bajo);         // detengo el rele de bajada
                    accion = 0;                    //Cambio la variable a paro        
              }

              
              
          }

        if (String(topic) == String(topicGeneral)){
          String order = obj[String("order")];
            
              Serial.print("Order : ");
              Serial.println(order);
              Serial.println("-----------------------------------------");
              Serial.println("");

              if (order == "getInfo" ) {
                      client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"version\": \""+String(ver)+"\",\"IP\": \""+WiFi.localIP().toString()+"\",\"MAC\": \""+WiFi.macAddress()+"\",\"staus\": \"alive\"}")); 
                    }
          
        }
          /* Serial.print("Message:");
            for (int i = 0; i < length; i++) {
             Serial.print((char)payload[i]);
            }*/
}
