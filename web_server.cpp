#include "web_server.h"
#include "temperature_sensor.h"
#include "thermostat.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define DOUBLE_MESSAGES 9

namespace WebServer {
  ESP8266WebServer server(80);

  void setup() {
    server.on("/", index);
    server.on("/submit", submit);
    server.on("/autotune", autotune);
    server.on("/json", json);
    server.begin();
  }

  void handle_client() {
    server.handleClient();
  }

  String bool_to_string(float bool_value) {
    return String(bool_value ? "true" : "false");
  }

  String bool_to_checked(float bool_value) {
    return String(bool_value ? " checked" : "");
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
                <td>Sensor temperature:</td>
                <td>)" + String(TemperatureSensor::temperature) + R"(</td>
              </tr>
              <tr>
                <td>PID output:</td>
                <td>)" + Thermostat::pid_output_average + R"(</td>
              </tr>
              <tr>
                <td>Heat on:</td>
                <td>
                  <input type="checkbox" disabled)" +
                      bool_to_checked(Thermostat::heat_on) + R"(>
                </td>
              </tr>
              <tr>
                <td>Autotune running:</td>
                <td>
                  <input type="checkbox" disabled)" +
                      bool_to_checked(Thermostat::autotune_running) + R"(>
                </td>
              </tr>

              <tr><td colspan="2">&nbsp;</td></tr>

              <tr>
                <td>Thermostat enabled:</td>
                <td>
                  <input id="enabled" name="enabled" type="checkbox")" +
                      bool_to_checked(Thermostat::settings.enabled) + R"(>
                </td>
              </tr>
              <tr>
                <td>Temperature:</td>
                <td>
                  <input id="temperature" name="temperature" type="number"
                       step="0.1" value=")" + Thermostat::settings.temperature +
                       R"(">
                </td>
              </tr>
              <tr>
                <td>Heat on threshold:</td>
                <td>
                  <input id="heat_on_threshold" name="heat_on_threshold"
                       type="number" step="0.1" value=")" +
                       Thermostat::settings.heat_on.threshold + R"(">
                </td>
              </tr>
              <tr>
                <td>Heat on delta:</td>
                <td>
                  <input id="heat_on_delta" name="heat_on_delta" type="number"
                       step="0.1" value=")" +
                       Thermostat::settings.heat_on.delta + R"(">
                </td>
              </tr>

              <tr><td colspan="2">&nbsp;</td></tr>

              <tr>
                <td>PID Kp:</td>
                <td>
                  <input id="pid_kp" name="pid_kp" type="number" step="0.1"
                       value=")" + Thermostat::settings.pid.kp + R"(">
                </td>
              </tr>
              <tr>
                <td>PID Ki:</td>
                <td>
                  <input id="pid_ki" name="pid_ki" type="number" step="0.1"
                       value=")" + Thermostat::settings.pid.ki + R"(">
                </td>
              </tr>
              <tr>
                <td>PID Kd:</td>
                <td>
                  <input id="pid_kd" name="pid_kd" type="number" step="0.1"
                       value=")" + Thermostat::settings.pid.kd + R"(">
                </td>
              </tr>
              <tr>
                <td>PID window seconds:</td>
                <td>
                  <input id="pid_window_seconds" name="pid_window_seconds"
                       type="number" step="0.1" value=")" +
                       Thermostat::settings.pid.window_seconds + R"(">
                </td>
              </tr>

              <tr><td colspan="2">&nbsp;</td></tr>

              <tr>
                <td>PID autotune lookback minutes:</td>
                <td>
                  <input id="pid_autotune_lookback_minutes"
                       name="pid_autotune_lookback_minutes" type="number"
                       step="0.1" value=")" +
                       Thermostat::settings.pid_autotune.lookback_minutes +
                       R"(">
                </td>
              </tr>
              <tr>
                <td>PID autotune noise band:</td>
                <td>
                  <input id="pid_autotune_noise_band"
                       name="pid_autotune_noise_band" type="number" step="0.1"
                       value=")" +
                       Thermostat::settings.pid_autotune.noise_band + R"(">
                </td>
              </tr>

              <tr><td colspan="2">&nbsp;</td></tr>

              <tr>
                <td colspan="2">
                  <button id="submit">Submit</button>
                </td>
              </tr>
              <tr>
                <td colspan="2">
                  <button id="start_autotune">Start autotune</button>
                </td>
              </tr>
              <tr>
                <td colspan="2">
                  <button id="stop_autotune">Stop autotune</button>
                </td>
              </tr>
            </table>

            <script>
              document.getElementById("submit").onclick = function() {
                var xhttp = new XMLHttpRequest();
                xhttp.open("POST", "/submit", true);
                xhttp.send(
                    "enabled=" + document.getElementById("enabled").checked +
                    "&temperature=" + document.getElementById(
                        "temperature").value +
                    "&pid_kp=" + document.getElementById("pid_kp").value +
                    "&pid_ki=" + document.getElementById("pid_ki").value +
                    "&pid_kd=" + document.getElementById("pid_kd").value +
                    "&pid_window_seconds=" + document.getElementById(
                        "pid_window_seconds").value +
                    "&heat_on_threshold=" + document.getElementById(
                        "heat_on_threshold").value +
                    "&heat_on_delta=" + document.getElementById(
                        "heat_on_delta").value +
                    "&pid_autotune_lookback_minutes=" +
                    document.getElementById(
                        "pid_autotune_lookback_minutes").value +
                    "&pid_autotune_noise_band=" + document.getElementById(
                        "pid_autotune_noise_band").value)
              }

              document.getElementById("start_autotune").onclick = function() {
                var xhttp = new XMLHttpRequest();
                xhttp.open("POST", "/autotune", true);
                xhttp.send("enabled=true");
              }

              document.getElementById("stop_autotune").onclick = function() {
                var xhttp = new XMLHttpRequest();
                xhttp.open("POST", "/autotune", true);
                xhttp.send("enabled=false");
              }
            </script>
          </body>
        </html>)"));
  }

  void json() {
    server.send(200, "application/json", String(R"(
        {"temperature": )" + String(Thermostat::settings.temperature) + R"(,
         "sensor_temperature": )" + String(
             TemperatureSensor::temperature) + R"(,
         "enabled": )" + bool_to_string(Thermostat::settings.enabled) + R"(,
         "heat_on": )" + bool_to_string(Thermostat::heat_on) + R"(,

         "pid_kp": )" + Thermostat::settings.pid.kp + R"(,
         "pid_ki": )" + Thermostat::settings.pid.ki + R"(,
         "pid_kd": )" + Thermostat::settings.pid.kd + R"(,
         "pid_output": )" + Thermostat::pid_output_average + "}"));
  }

  void autotune() {
    if (server.arg("enabled").equals("true")) {
      Thermostat::start_autotune();
      server.send(200, "text/plain", "Autotune started.");
    } else if (server.arg("enabled").equals("false")) {
      Thermostat::cancel_autotune();
      server.send(200, "text/plain", "Autotune stopped.");
    } else {
      server.send(
          400, "text/plain", "\"enabled\" must be \"true\" or \"false\".");
    }
  }

  void submit() {
    String response_ok;
    String response_error;
    Thermostat::settings_struct new_settings = Thermostat::settings;

    for (int args_i = 0; args_i < server.args(); args_i++) {
      String line_ok;
      String line_error;
      String arg_name = server.argName(args_i);
      String arg_value = server.arg(args_i);

      if (arg_name.equals("enabled")) {
        if (arg_value.equals("true") or arg_value.equals("false")) {
          new_settings.enabled = server.arg("enabled").equals("true");

          line_ok = String(
              "Thermostat " +
              String(new_settings.enabled ? "enabled" : "disabled"));
        } else {
          line_error = String(
              "must be either \"true\" or \"false\", not " + arg_value);
        }
      } else {
        double arg_value_double = (double)arg_value.toFloat();

        if (arg_value_double < 0) {
          line_error = String(
              "must be a positive number (given " + arg_value + ").");
        } else if (arg_name.equals("temperature")) {
          new_settings.temperature = arg_value_double;
          line_ok += "Temperature";
        } else if (arg_name.equals("pid_kp")) {
          new_settings.pid.kp = arg_value_double;
          line_ok += "PID Kp";
        } else if (arg_name.equals("pid_ki")) {
          new_settings.pid.ki = arg_value_double;
          line_ok += "PID Ki";
        } else if (arg_name.equals("pid_kd")) {
          new_settings.pid.kd = arg_value_double;
          line_ok += "PID Kd";
        } else if (arg_name.equals("pid_window_seconds")) {
          new_settings.pid.window_seconds = arg_value_double;
          line_ok += "PID window seconds";
        } else if (arg_name.equals("heat_on_threshold")) {
          new_settings.heat_on.threshold = arg_value_double;
          line_ok += "Heat on threshold";
        } else if (arg_name.equals("heat_on_delta")) {
          new_settings.heat_on.delta = arg_value_double;
          line_ok += "Heat on delta";
        } else if (arg_name.equals("heat_on_delta")) {
          new_settings.heat_on.delta = arg_value_double;
          line_ok += "Heat on delta";
        } else if (arg_name.equals("pid_autotune_lookback_minutes")) {
          new_settings.pid_autotune.lookback_minutes = arg_value_double;
          line_ok += "PID autotune lookback minutes";
        } else if (arg_name.equals("pid_autotune_noise_band")) {
          new_settings.pid_autotune.noise_band = arg_value_double;
          line_ok += "PID autotune noise band";
        }

        if (line_ok.length()) {
          line_ok += " set to " + String(arg_value_double);
        }
      }

      if (! line_ok.length() and ! line_error.length()) {
        line_error = String("invalid argument");
      }

      if (line_error.length()) {
        if (response_error.length()) {
          response_error += "\n";
        }
        response_error += String(arg_name + ": " + line_error);
      }

      if (line_ok.length()) {
        if (response_ok.length()) {
          response_ok += "\n";
        }
        response_ok += String(line_ok + ".");
      }
    }

    if (response_error.length()) {
      server.send(400, "text/plain", response_error);
      return;
    } else if (response_ok.length()) {
      Thermostat::apply_settings(new_settings);
      server.send(200, "text/plain", response_ok);
    } else {
      server.send(400, "text/plain", "No arguments submitted.");
    }
  }
}
