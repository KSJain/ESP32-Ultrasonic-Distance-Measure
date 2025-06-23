#include "ControllerService.h"

ControllerService::ControllerService(int triggerPin, int echoPin) {
  _sensor = new UltrasonicSensorService(triggerPin, echoPin, UltrasonicSensorService::SensorSenstivity::MED_SENSITIVITY);
}

void ControllerService::begin() {
  _sensor->begin();
  
  _sensor->setTriggerThreshold(30.0); // Example threshold at 30 cm

  _sensor->onThresholdCrossed([](float distance) {
    Serial.print("Threshold crossed!");
    Serial.print(distance);
    Serial.println(" cm");
  });

  _sensor->onResolutionUpdate([](float distance, float delta) {
    Serial.print("Updated Distance: ");
    Serial.print(distance);
    Serial.print(" cm delta ");
    Serial.print(delta);
    Serial.println(" cm");
  });
}

void ControllerService::tick() {
  _sensor->tick();
}
