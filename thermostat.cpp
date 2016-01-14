#include "thermostat.h"
#include "temperature_sensor.h"

#define INITIAL_TEMPERATURE 19.0
#define INITIAL_ENABLED false
#define TEMPERATURE_DEVIATION 0.15

namespace Thermostat {
  float temperature = INITIAL_TEMPERATURE;
  bool enabled = INITIAL_ENABLED;
  bool heat_on = false;

  void update() {
    if (! enabled) {
      heat_on = false;
    } else {
      if (heat_on && temperature + TEMPERATURE_DEVIATION <=
          TemperatureSensor::temperature) {
        heat_on = false;
      } else if (! heat_on && temperature - TEMPERATURE_DEVIATION >=
                 TemperatureSensor::temperature) {
        heat_on = true;
      }
    }
  }
}
