

String str(Temp_type);

if((str == "DH") or (str == "dh")){

        Temperature = dht.readTemperature(); // Gets the values of the temperature
        Humidity = dht.readHumidity(); // Gets the values of the humidity
        delay(100);
        Temperature = dht.readTemperature(); // Gets the values of the temperature
        Humidity = dht.readHumidity(); // Gets the values of the humidity
        Serial.print("Valor Leido=   ");
        Serial.println(Temperature);
        client.println(Temperature);
        

} else {

    tempOff =0;
    
    while (tempOff == 0){
      total = total - readings[readIndex];
      readings[readIndex] = analogRead(tempPin);
      total = total + readings[readIndex];
      readIndex = readIndex + 1;
      
       if (readIndex >= numReadings) {
              readIndex = 0;
              tempOff=1;
          }
    }
    
       /* int analogValue = analogRead(tempPin);
        Serial.print("Valor Leido=   ");
        Serial.println(analogValue);
        client.println(analogValue);
        */
        average = total / numReadings;
        Serial.print("Valor Leido LM35=   ");
        Serial.println(average);
        client.println(average);      
}

         
