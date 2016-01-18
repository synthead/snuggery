#include "web_server.h"
#include "temperature_sensor.h"
#include "thermostat.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

namespace WebServer {
  ESP8266WebServer server(80);

  void setup() {
    server.on("/", index);
    server.on("/submit", submit);
    server.on("/json", json);
    server.begin();
  }

  void handle_client() {
    server.handleClient();
  }

  String bool_to_string(float bool_value) {
    return String(bool_value ? "true" : "false");
  }

  void index() {
    server.send(200, "text/html", String(R"(
        <!DOCTYPE html>
        <html>
          <head>
            <title>Snuggery</title>
          </head>
          <body>
            <table>
              <tr>
                <td>Thermostat temperature:</td>
                <td>)" + String(Thermostat::temperature) + R"(</td>
              </tr>
              <tr>
                <td>Sensor temperature:</td>
                <td>)" + String(TemperatureSensor::temperature) + R"(</td>
              </tr>
              <tr>
                <td>Thermostat enabled:</td>
                <td>)" + bool_to_string(Thermostat::enabled) + R"(</td>
              </tr>
              <tr>
                <td>Heat on:</td>
                <td>)" + bool_to_string(Thermostat::heat_on) + R"(</td>
              </tr>
            </table>
          </body>
        </html>)"));
  }

  void json() {
    server.send(200, "application/json", String(R"(
        {"thermostat_temperature": )" + String(Thermostat::temperature) + R"(,
         "sensor_temperature": )" + String(
            TemperatureSensor::temperature) + R"(,
         "thermostat_enabled": )" + bool_to_string(Thermostat::enabled) + R"(,
         "heat_on": )" + bool_to_string(Thermostat::heat_on) + "}"));
  }

  void submit() {
    String response;

    for (int check_set_i = 0; check_set_i < 2; check_set_i++) {
      for (int args_i = 0; args_i < server.args(); args_i++) {
        String line;
        String arg_name = server.argName(args_i);
        String arg_value = server.arg(args_i);

        if (check_set_i == 0) {
          if (arg_name.equals("thermostat_temperature")) {
            float arg_value_float = arg_value.toFloat();

            if (arg_value_float < THERMOSTAT_MIN or
                arg_value_float > THERMOSTAT_MAX) {
              line = String(
                  "must be between " + String(THERMOSTAT_MIN) + " and " +
                  String(THERMOSTAT_MAX) + " (given " + String(arg_value) +
                  ")");
            }
          } else if (arg_name.equals("enabled")) {
            if (! arg_value.equals("true") and ! arg_value.equals("false")) {
              line = String(
                  "must be either \"true\" or \"false\", not " + arg_value);
            }
          } else {
            line = String("invalid argument");
          }

          if (line.length()) {
            line = String(arg_name + ": " + line);
          }
        } else {
          if (arg_name.equals("thermostat_temperature")) {
            Thermostat::temperature = arg_value.toFloat();
            line = String(
                "Thermostat temperature set to " +
                String(Thermostat::temperature) + " C");
          } else if (arg_name.equals("enabled")) {
            Thermostat::enabled = server.arg("enabled").equals("true");

            line = String(
                "Thermostat " +
                String(Thermostat::enabled ? "enabled" : "disabled"));
          }
        }

        if (line.length()) {
          if (response.length()) {
            response += "\n";
          }
          response += String(line + ".");
        }
      }

      if (check_set_i == 0) {
        if (response.length()) {
          server.send(400, "text/plain", response);
          return;
        }
      } else {
        if (response.length()) {
          Thermostat::update();
          server.send(200, "text/plain", response);
        } else {
          server.send(400, "text/plain", "No arguments submitted.");
        }
      }
    }
  }
}
