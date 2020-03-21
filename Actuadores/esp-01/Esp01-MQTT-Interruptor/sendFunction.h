void sendFunction(String packet){

      Serial.print("connecting to ");
      Serial.print(host);
      Serial.print(':');
      Serial.println(port);
    
      // Use WiFiClient class to create TCP connections
      WiFiClient client;
      if (!client.connect(host, port)) {
        Serial.println("connection failed");
        delay(5000);
        return;
      }
    
      // This will send a string to the server
      Serial.println("sending data to server");
      if (client.connected()) {
        client.println(packet);
      }
    
      // wait for data to be available
      unsigned long timeout = millis();
      while (client.available() == 0) {
              if (millis() - timeout > 5000) {
                Serial.println(">>> Client Timeout !");
                client.stop();
                delay(6000);
                return;
                      }
            }

      Serial.println("receiving from remote server");
      while (client.available()) {
        char ch = static_cast<char>(client.read());
        Serial.print(ch);
      }
      Serial.println();
      Serial.println("closing connection");
      client.stop();
}
