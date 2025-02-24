#include <Arduino.h>
#include "UltrasonicSensorService.h"
#include "ControllerService.h"

UltrasonicSensorService ultrasonicSensor(32, 33);  // Trigger pin 9, Echo pin 10
ControllerService controller(&ultrasonicSensor);

void setup() {
  Serial.begin(9600);
  controller.begin();
}

void loop() {
  controller.tick();
}
