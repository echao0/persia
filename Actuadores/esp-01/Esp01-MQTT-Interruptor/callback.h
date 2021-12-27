void callback(char* topic, byte* payload, unsigned int length) {
  StaticJsonDocument<256> doc;
  deserializeJson(doc, payload, length);
  JsonObject obj = doc.as<JsonObject>();


if (String(topic) == String(topicGeneral)){
          String order = obj[String("order")];
              
              if (order == "getInfo" ) {
                      client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"version\": \""+String(ver)+"\",\"IP\": \""+WiFi.localIP().toString()+"\"}")); 
                    }

        }

if (String(topic) == String(topicMode)){
      String Mode = obj[String("mode")];
      
      if (Mode == "OTA" ) {
                        client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"mode\": \"OTA->ON\"}"));
                        OTA();
                    }    
  
}

if (String(topic) == String(topicOrders)){
            
              String order = obj[String("order")];
              
              if (order == "restart" ) {
                      client.publish(topicLog, toCharFunction(espName+"-Restarting"));
                      delay(200);
                      ESP.reset();
                    }
}

  
}
