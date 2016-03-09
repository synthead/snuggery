#ifndef THERMOSTAT_H
#define THERMOSTAT_H

namespace Thermostat {
  struct settings_heat_on_struct {
    double threshold;
    double delta;
  };

  struct settings_pid_struct {
    double kp;
    double ki;
    double kd;

    double window_seconds;
  };

  struct settings_pid_autotune_struct {
    unsigned int lookback_minutes;
    double noise_band;
  };

  struct settings_struct {
    double temperature;
    bool enabled;

    settings_pid_struct pid;
    settings_pid_autotune_struct pid_autotune;
    settings_heat_on_struct heat_on;
  };

  extern settings_struct settings;
  extern bool autotune_running;
  extern bool heat_on;
  extern double pid_output_average;

  void setup();
  void update();
  void handle_enabled();
  void update_pid_tunings();
  void set_heat(bool);
  void reset_window();
  void start_autotune();
  void cancel_autotune();
  void apply_settings(settings_struct);
  void write_settings();
}

#endif
