#include "web_server.h"
#include "ds18b20.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

namespace WebServer {
  ESP8266WebServer server(80);

  void setup() {
    server.on("/", temperature);
    server.begin();
  }

  void handle_client() {
    server.handleClient();
  }

  void temperature() {
    char temperature[6];
    sprintf(
        temperature, "Temperature: %d.%02d",
        DS18B20::temperature / 100, DS18B20::temperature % 100);

    server.send(200, "text/plain", temperature);
  }
}
