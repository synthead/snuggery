#include "web_server.h"
#include "ds18b20.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

namespace WebServer {
  ESP8266WebServer server(80);

  void setup() {
    server.on("/", index);
    server.begin();
  }

  void handle_client() {
    server.handleClient();
  }

  void index() {
    char html[1000];
    sprintf(
        html,
        R"(
            <!DOCTYPE html>
            <html>
              <head>
                <title>Snuggery</title>
              </head>
              <body>Temperature: %d.%02d</body>
            </html>
        )",
        DS18B20::temperature / 100,
        DS18B20::temperature % 100);

    server.send(200, "text/html", html);
  }
}
