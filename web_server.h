#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>

namespace WebServer {
  void setup();
  void handle_client();
  void index();
  void json();
  void submit();
  void autotune();
}

#endif
