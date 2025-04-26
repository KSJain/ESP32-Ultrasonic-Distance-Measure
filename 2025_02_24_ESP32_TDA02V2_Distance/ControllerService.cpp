#include "ControllerService.h"

ControllerService::ControllerService(int triggerPin, int echoPin) {
  _sensor = new UltrasonicSensorService(triggerPin, echoPin); // TRIGGER_PIN, ECHO_PIN

}

void ControllerService::begin() {
  _sensor->begin();
  
  _sensor->setTriggerThreshold(30.0); // Example threshold at 30 cm

  _sensor->onThresholdCrossed([]() {
    Serial.println("Threshold crossed! Object is very close!");
  });

  _sensor->onResolutionUpdate([](float distance) {
    Serial.print("Updated Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
  });
}

void ControllerService::tick() {
  _sensor->update();
}
