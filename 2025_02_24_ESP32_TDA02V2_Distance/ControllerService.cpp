#include "ControllerService.h"

ControllerService::ControllerService(int triggerPin, int echoPin) {
  _ultrasonicSensor = new UltrasonicSensor(triggerPin, echoPin);
}

void ControllerService::begin() {
  _ultrasonicSensor->begin();
  
  _ultrasonicSensor->onResolutionUpdate([this](float distanceInCm) {
    Serial.print(distanceInCm);
    Serial.println(" cm");
  });
  
  _ultrasonicSensor->onThresholdCrossed([]() {
    Serial.println("Threshold crossed! Take action.");
  });
}

void ControllerService::tick() {
  _ultrasonicSensor->tick();
}
