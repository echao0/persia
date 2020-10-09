void mqttReconnect(){
  
    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    Serial.println("Reconnecting to MQTT...");

    if (client.connect(toCharFunction(espName), mqttUser, mqttPassword )) {

      Serial.println("connected");
      client.publish(topicLog, toCharFunction("{\"disp\": \""+espName+"\",\"version\": \""+String(ver)+"\",\"IP\": \""+WiFi.localIP().toString()+"\",\"MAC\": \""+WiFi.macAddress()+"\",\"staus\": \"Reconnecting\"}"));
      client.subscribe(toCharFunction(topicMode));
      client.subscribe(toCharFunction(topicOrders));
      client.subscribe(toCharFunction(topicGeneral));
      client.subscribe(toCharFunction(topicTemp));
    } 

  
  }
