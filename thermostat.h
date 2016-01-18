#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#define THERMOSTAT_MIN 0
#define THERMOSTAT_MAX 50

namespace Thermostat {
  extern float temperature;
  extern bool enabled;
  extern bool heat_on;

  void setup();
  void set_enabled(bool);
  void set_temperature(float);
  void set_heat(bool);
  void update();

  template <class T> int eeprom_read(int, T&);
  template <class T> int eeprom_write(int, const T&);
}

#endif
