#include "wifi_settings.h"
#include "web_server.h"
#include "temperature_sensor.h"
#include "thermostat.h"
#include <ESP8266WiFi.h>
#include <DallasTemperature.h>
 
void setup() {
  WiFi.begin(WIFI_SETTINGS_ESSID, WIFI_SETTINGS_PASSWORD);
  WebServer::setup();
  TemperatureSensor::setup();
}
 
void loop() {
  WebServer::handle_client();
  TemperatureSensor::update_occasionally();
}
