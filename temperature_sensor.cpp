#include "temperature_sensor.h"
#include "thermostat.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define DS18B20_PIN 2
#define UPDATE_TEMPERATURE_CYCLES 5000000

namespace TemperatureSensor {
  float temperature;
  int tick = UPDATE_TEMPERATURE_CYCLES;

  OneWire one_wire(DS18B20_PIN);
  DallasTemperature sensors(&one_wire);

  void setup() {
    sensors.begin();
  }

  void update() {
    sensors.requestTemperatures();
    temperature = sensors.getTempCByIndex(0);
    Thermostat::update();
  }

  void update_occasionally() {
    if (tick++ == UPDATE_TEMPERATURE_CYCLES) {
      tick = 0;
      update();
    }
  }
}
