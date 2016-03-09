#include "wifi_settings.h"
#include "web_server.h"
#include "temperature_sensor.h"
#include "thermostat.h"
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
 
void setup() {
  WiFi.begin(WIFI_SETTINGS_ESSID, WIFI_SETTINGS_PASSWORD);
  WiFi.waitForConnectResult();
  ArduinoOTA.begin();

  Thermostat::setup();
  TemperatureSensor::setup();
  WebServer::setup();
}
 
void loop() {
  ArduinoOTA.handle();

  WebServer::handle_client();
  TemperatureSensor::update_occasionally();
  Thermostat::update();
}
