void callback(char* topic, byte* payload, unsigned int length) {

  DynamicJsonDocument doc(200);
  deserializeJson(doc, payload);
  JsonObject obj = doc.as<JsonObject>();
   
      //Serial.println("-----------------------------------------");
      //Serial.print("topic: ");
      //Serial.println(topic);
        
//          if (String(topic) == String(topicOrders)){
//            
//              String order = obj[String("order")];
            
//              Serial.print("Order : ");
//              Serial.println(order);
//              Serial.println("-----------------------------------------");
//              Serial.println("");
              
//                if (order == "test" ) {
//                      client.publish(topicLog, toCharFunction("12345678901234567890123"));
//                      delay(200);
//                    }
//                    
//                if (order == "restart" ) {
//                      client.publish(topicLog, toCharFunction(espName+"-Restarting"));
//                      delay(200);
//                      ESP.reset();
//                    }
                    
//                if (order == "getInfo" ) {
//                      client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"version\": \""+String(ver)+"\",\"IP\": \""+WiFi.localIP().toString()+"\",\"MAC\": \""+WiFi.macAddress()+"\"}"));
//                    }
//          }

        if (String(topic) == String(topicGeneral)){
          String order = obj[String("order")];
            
//              Serial.print("Order : ");
//              Serial.println(order);
//              Serial.println("-----------------------------------------");
//              Serial.println("");

              if (order == "getInfo" ) {
                      client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"version\": \""+String(ver)+"\",\"IP\": \""+WiFi.localIP().toString()+"\",\"MAC\": \""+WiFi.macAddress()+"\"}")); 
                    }
          
        }

}
