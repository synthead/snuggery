#include "temperature_sensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define DS18B20_PIN 2
#define UPDATE_TEMPERATURE_SECONDS 5

namespace TemperatureSensor {
  double temperature;
  int next_update_millis = 0;

  OneWire one_wire(DS18B20_PIN);
  DallasTemperature sensors(&one_wire);

  void setup() {
    sensors.begin();
  }

  void update() {
    sensors.requestTemperatures();
    temperature = sensors.getTempCByIndex(0);
  }

  void update_occasionally() {
    unsigned long now = millis();

    if (now >= next_update_millis) {
      update();
      next_update_millis = now + UPDATE_TEMPERATURE_SECONDS * 1000;
    }
  }
}
