#include "thermostat.h"

#define INITIAL_TEMPERATURE 19.0
#define INITIAL_ENABLED false

namespace Thermostat {
  float temperature = INITIAL_TEMPERATURE;
  bool enabled = INITIAL_ENABLED;
  bool heat_on = false;
}
