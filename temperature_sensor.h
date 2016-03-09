#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

namespace TemperatureSensor {
  extern double temperature;

  void setup();
  void update();
  void update_occasionally();
}

#endif
