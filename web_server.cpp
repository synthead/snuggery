#include "web_server.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

namespace WebServer {
  ESP8266WebServer server(80);

  void setup() {
    server.on("/", hello_world);
    server.begin();
  }

  void handle_client() {
    server.handleClient();
  }

  void hello_world() {
    server.send(200, "text/plain", "Hello world!");
  }
}
