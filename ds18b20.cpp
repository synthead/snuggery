#include "ds18b20.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define DS18B20_BYTES 9
#define DS18B20_PIN 0

namespace DS18B20 {
  int temperature;

  OneWire one_wire(DS18B20_PIN);
  DallasTemperature sensors(&one_wire);

  void setup() {
    sensors.begin();
  }

  void read() {
    sensors.requestTemperatures();
    temperature = sensors.getTempCByIndex(0) * 100;
  }
}
