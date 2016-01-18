#include "thermostat.h"
#include "temperature_sensor.h"
#include <Arduino.h>
#include <EEPROM.h>

#define MOSFET_PIN 16
#define HEAT_ON_LED_PIN 0

#define TEMPERATURE_DEVIATION 0.15

#define EEPROM_LOCATION_ENABLED 0
#define EEPROM_LOCATION_TEMPERATURE 1

namespace Thermostat {
  float temperature;
  bool enabled;
  bool heat_on;

  void setup() {
    pinMode(MOSFET_PIN, OUTPUT);
    pinMode(HEAT_ON_LED_PIN, OUTPUT);

    EEPROM.begin(5);
    eeprom_read(EEPROM_LOCATION_ENABLED, enabled);
    eeprom_read(EEPROM_LOCATION_TEMPERATURE, temperature);

    set_heat(false);
  }

  void set_enabled(bool new_enabled) {
    if (new_enabled != enabled) {
      eeprom_write(EEPROM_LOCATION_ENABLED, new_enabled);
      enabled = new_enabled;
    }
  }

  void set_temperature(float new_temperature) {
    if (new_temperature != temperature) {
      eeprom_write(EEPROM_LOCATION_TEMPERATURE, new_temperature);
      temperature = new_temperature;
    }
  }

  void set_heat(bool is_on) {
    digitalWrite(MOSFET_PIN, is_on);
    digitalWrite(HEAT_ON_LED_PIN, ! is_on);

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

  template <class T> int eeprom_read(int ee, T& value) {
    byte* p = (byte*)(void*)&value;
    unsigned int i;

    for (i = 0; i < sizeof(value); i++) {
      *p++ = EEPROM.read(ee++);
    }

    EEPROM.commit();
    return i;
  }

  template <class T> int eeprom_write(int ee, const T& value) {
    const byte* p = (const byte*)(const void*)&value;
    unsigned int i;

    for (i = 0; i < sizeof(value); i++) {
      EEPROM.write(ee++, *p++);
    }

    EEPROM.commit();
    return i;
  }
}
