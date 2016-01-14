#include "thermostat.h"
#include "temperature_sensor.h"
#include <Arduino.h>

#define MOSFET_PIN 0
#define INITIAL_TEMPERATURE 19.0
#define INITIAL_ENABLED false
#define TEMPERATURE_DEVIATION 0.15

namespace Thermostat {
  float temperature = INITIAL_TEMPERATURE;
  bool enabled = INITIAL_ENABLED;
  bool heat_on;

  void setup() {
    pinMode(MOSFET_PIN, OUTPUT);
    set_heat(false);
  }

  void set_heat(bool is_on) {
    digitalWrite(MOSFET_PIN, ! is_on);
    heat_on = is_on;
  }

  void update() {
    if (! enabled) {
      set_heat(false);
    } else {
      if (heat_on && temperature + TEMPERATURE_DEVIATION <=
          TemperatureSensor::temperature) {
        set_heat(false);
      } else if (! heat_on && temperature - TEMPERATURE_DEVIATION >=
                 TemperatureSensor::temperature) {
        set_heat(true);
      }
    }
  }
}
