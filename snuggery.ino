#include "wifi_settings.h"
#include "web_server.h"
#include "ds18b20.h"
#include <ESP8266WiFi.h>
#include <DallasTemperature.h>
 
void setup() {
  WiFi.begin(WIFI_SETTINGS_ESSID, WIFI_SETTINGS_PASSWORD);
  WebServer::setup();
  DS18B20::setup();
}
 
void loop() {
  WebServer::handle_client();
  DS18B20::read();
}
