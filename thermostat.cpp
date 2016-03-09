#include "thermostat.h"
#include "temperature_sensor.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>

#define MOSFET_PIN 16
#define HEAT_ON_LED_PIN 0

#define EEPROM_TOTAL_BYTES 4096
#define EEPROM_SETTINGS_LOCATION 0

#define PID_OUTPUT_RANGE 100

namespace Thermostat {
  settings_struct settings;
  bool heat_on = false;

  unsigned long window_end_millis;
  double pid_output;
  double pid_output_sum;
  int pid_output_count;
  double pid_output_average = 0.0;

  bool autotune_running = false;

  PID pid(
      &TemperatureSensor::temperature, &pid_output, &settings.temperature,
      settings.pid.kp, settings.pid.ki, settings.pid.kd, DIRECT);

  PID_ATune pid_autotune(&TemperatureSensor::temperature, &pid_output);

  void setup() {
    pinMode(MOSFET_PIN, OUTPUT);
    pinMode(HEAT_ON_LED_PIN, OUTPUT);

    pid.SetOutputLimits(-PID_OUTPUT_RANGE, PID_OUTPUT_RANGE);

    pid_autotune.SetOutputStep(PID_OUTPUT_RANGE);
    pid_autotune.SetControlType(1);  // PID control type.

    EEPROM.begin(EEPROM_TOTAL_BYTES);
    byte* p = (byte*)(void*)&settings;
    for (unsigned int i = 0; i < sizeof(settings); i++) {
      *p++ = EEPROM.read(EEPROM_SETTINGS_LOCATION + i);
    }

    update_pid_tunings();
    handle_enabled();
  }

  void update() {
    if (settings.enabled) {
      if (autotune_running) {
        if (pid_autotune.Runtime()) {
          autotune_running = false;

          settings.pid.kp = pid_autotune.GetKp();
          settings.pid.ki = pid_autotune.GetKi();
          settings.pid.kd = pid_autotune.GetKd();

          update_pid_tunings();
          write_settings();
        }
      }

      if (! autotune_running) {
        pid.Compute();
      }

      pid_output_sum += pid_output;
      pid_output_count++;

      unsigned long now = millis();
      if (now >= window_end_millis) {
        pid_output_average = pid_output_sum / pid_output_count;

        if (pid_output_average >= settings.heat_on.threshold) {
          set_heat(true);
        } else if (pid_output_average + settings.heat_on.delta <=
                   settings.heat_on.threshold) {
          set_heat(false);
        }

        reset_window();
      }
    }
  }

  void handle_enabled() {
    if (settings.enabled) {
      reset_window();
      pid.SetMode(AUTOMATIC);
    } else {
      cancel_autotune();
      pid.SetMode(MANUAL);
      set_heat(false);
    }
  }

  void update_pid_tunings() {
    pid.SetTunings(settings.pid.kp, settings.pid.ki, settings.pid.kd);
  }

  void set_heat(bool is_on) {
    digitalWrite(MOSFET_PIN, is_on);
    digitalWrite(HEAT_ON_LED_PIN, ! is_on);

    heat_on = is_on;
  }

  void reset_window() {
    window_end_millis = millis() + settings.pid.window_seconds * 1000;
    pid_output_sum = 0.0;
    pid_output_count = 0;
  }

  void start_autotune() {
    pid_autotune.SetLookbackSec(settings.pid_autotune.lookback_minutes * 60);
    pid_autotune.SetNoiseBand(settings.pid_autotune.noise_band);

    autotune_running = true;
  }

  void cancel_autotune() {
    if (autotune_running) {
      pid_autotune.Cancel();
      autotune_running = false;
    }
  }

  void apply_settings(settings_struct new_settings) {
    settings_struct old_settings = settings;
    settings = new_settings;

    if (settings.enabled != new_settings.enabled) {
      handle_enabled();
    }

    if (settings.temperature != new_settings.temperature) {
      cancel_autotune();
    }

    if (settings.pid.kp != new_settings.pid.kp or
        settings.pid.ki != new_settings.pid.ki or
        settings.pid.kd != new_settings.pid.kd) {
      update_pid_tunings();
    }

    write_settings();
  }

  void write_settings() {
    const byte* p = (const byte*)(const void*)&settings;
    for (unsigned int i = 0; i < sizeof(settings); i++) {
      EEPROM.write(EEPROM_SETTINGS_LOCATION + i, *p++);
    }

    EEPROM.commit();
  }
}
