#include "OTA.h"

void setup() {
    ESP.eraseConfig();
    ESP.reset();
}
void loop() {
  OTA();
  
  
  }
