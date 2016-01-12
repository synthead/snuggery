#include "wifi_settings.h"
#include <ESP8266WiFi.h>
 
void setup() {
  WiFi.begin(WIFI_SETTINGS_ESSID, WIFI_SETTINGS_PASSWORD);
}
 
void loop() {
}
