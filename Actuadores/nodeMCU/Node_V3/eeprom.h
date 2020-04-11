#include <EEPROM.h>

char wifi_ssid_private[32];
char wifi_password_private[32];
char ipAddr[16] = "192.168.3.172"; // Varible para modificar desde WEB
char esubnet[16] = "255.255.255.0"; // Varible para modificar desde WEB
char egateway[16] = "192.168.3.1"; // Varible para modificar desde WEB
char Temp_type[3];

void writeEEPROM(int startAdr, int laenge, char* writeString) {
      EEPROM.begin(512); //Max bytes of eeprom to use
      yield();

      for (int i = 0; i < laenge; i++)
        {
          EEPROM.write(startAdr + i, writeString[i]);
          //Serial.print(writeString[i]);
        }
       EEPROM.commit();
       EEPROM.end();           
}

void readEEPROM(int startAdr, int maxLength, char* dest) {
  
  EEPROM.begin(512);
  delay(10);
  
  for (int i = 0; i < maxLength; i++)
    {
      dest[i] = char(EEPROM.read(startAdr + i));
    }
  
  EEPROM.end();
  //Serial.println(dest);
}

void clearEEPROM()
{
  EEPROM.begin(512);
  // write a 0 to all 512 bytes of the EEPROM
  for (int i = 0; i < 512; i++)
    EEPROM.write(i, 0);

  // turn the LED on when we're done
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  EEPROM.end();
}
