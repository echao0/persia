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
                        //client.publish(topicLog, "Modo automatico");
                      }
//                if (Mode == "OTA" ) {
//                        Serial.println("Modo OTA");
//                        client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"mode\": \"OTA->ON\"}"));
//                        OTA();
//                    }
            }
        
          if (String(topic) == String(topicOrders)){
            
              String order = obj[String("order")];
            
              Serial.print("Order : ");
              Serial.println(order);
              Serial.println("-----------------------------------------");
              Serial.println("");
              
                if (order == "test" ) {
                      client.publish(topicLog, toCharFunction("12345678901234567890123"));
                      delay(200);
                    }
                    
                if (order == "restart" ) {
                      client.publish(topicLog, toCharFunction(espName+"-Restarting"));
                      delay(200);
                      ESP.reset();
                    }
                    
                if (order == "getInfo" ) {
                      client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"version\": \""+String(ver)+"\",\"IP\": \""+WiFi.localIP().toString()+"\",\"MAC\": \""+WiFi.macAddress()+"\"}"));
                    }
          }

        if (String(topic) == String(topicGeneral)){
          String order = obj[String("order")];
            
              Serial.print("Order : ");
              Serial.println(order);
              Serial.println("-----------------------------------------");
              Serial.println("");

              if (order == "getInfo" ) {
                      client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"version\": \""+String(ver)+"\",\"IP\": \""+WiFi.localIP().toString()+"\",\"MAC\": \""+WiFi.macAddress()+"\"}")); 
                    }
          
        }

}
