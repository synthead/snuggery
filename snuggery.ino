#include "wifi_settings.h"
#include "web_server.h"
#include <ESP8266WiFi.h>
 
void setup() {
  WiFi.begin(WIFI_SETTINGS_ESSID, WIFI_SETTINGS_PASSWORD);
  WebServer::setup();
}
 
void loop() {
  WebServer::handle_client();
}
