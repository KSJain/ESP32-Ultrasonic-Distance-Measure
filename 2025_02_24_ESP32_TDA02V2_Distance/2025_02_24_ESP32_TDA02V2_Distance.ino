#include "ControllerService.h"

#define TRIGGER_PIN  32
#define ECHO_PIN     33

ControllerService controller(TRIGGER_PIN,ECHO_PIN);

void setup() {
  Serial.begin(9600);
  controller.begin();
}

void loop() {
  controller.tick();
}