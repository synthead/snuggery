#include "wifi_settings.h"
#include "web_server.h"
#include "temperature_sensor.h"
#include "thermostat.h"
#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <DallasTemperature.h>
#include <ArduinoOTA.h>
 
void setup() {
  WiFi.begin(WIFI_SETTINGS_ESSID, WIFI_SETTINGS_PASSWORD);
  ArduinoOTA.begin();

  Thermostat::setup();
  TemperatureSensor::setup();
  WebServer::setup();
}
 
void loop() {
  ArduinoOTA.handle();

  WebServer::handle_client();
  TemperatureSensor::update_occasionally();
}
