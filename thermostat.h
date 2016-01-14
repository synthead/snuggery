#ifndef THERMOSTAT_H
#define THERMOSTAT_H

namespace Thermostat {
  extern float temperature;
  extern bool enabled;
  extern bool heat_on;

  void update();
}

#endif
