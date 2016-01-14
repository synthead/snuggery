#include "web_server.h"
#include "temperature_sensor.h"
#include "thermostat.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

namespace WebServer {
  ESP8266WebServer server(80);
  char html[1000];
  char thermostat_temperature[7];
  char sensor_temperature[7];
  char thermostat_enabled[6];
  char heat_on[6];

  void setup() {
    server.on("/", index);
    server.on("/submit", submit);
    server.begin();
  }

  void handle_client() {
    server.handleClient();
  }

  char* float_to_char(char* char_value, float float_value) {
    sprintf(
        char_value, "%d.%02d", (int)float_value,
        (int)(float_value * 100) % 100);
  }

  char* bool_to_char(char* char_value, float bool_value) {
    sprintf(char_value, "%s", bool_value ? "True" : "False");
  }

  void index() {
    float_to_char(thermostat_temperature, Thermostat::temperature);
    float_to_char(sensor_temperature, TemperatureSensor::temperature);
    bool_to_char(thermostat_enabled, Thermostat::enabled);
    bool_to_char(heat_on, Thermostat::heat_on);

    sprintf(
        html,
        R"(
            <!DOCTYPE html>
            <html>
              <head>
                <title>Snuggery</title>
              </head>
              <body>
                <table>
                  <tr>
                    <td>Thermostat temperature:</td>
                    <td>%s</td>
                  </tr>
                  <tr>
                    <td>Sensor temperature:</td>
                    <td>%s</td>
                  </tr>
                  <tr>
                    <td>Thermostat enabled:</td>
                    <td>%s</td>
                  </tr>
                  <tr>
                    <td>Heat on:</td>
                    <td>%s</td>
                  </tr>
                </table>
              </body>
            </html>
        )",
        &thermostat_temperature,
        &sensor_temperature,
        &thermostat_enabled,
        &heat_on);

    server.send(200, "text/html", html);
  }

  void submit() {
    if (server.hasArg("thermostat_temperature")) {
      Thermostat::temperature = server.arg("thermostat_temperature").toFloat();
    }

    if (server.hasArg("enabled")) {
      Thermostat::enabled = server.arg("enabled").toInt();
    }

    server.send(200, "text/plain", "Submitted.");
  }
}
