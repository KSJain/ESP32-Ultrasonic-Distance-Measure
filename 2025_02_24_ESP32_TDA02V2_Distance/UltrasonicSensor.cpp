#include "UltrasonicSensor.h"

UltrasonicSensor::UltrasonicSensor(
    int triggerPin, 
    int echoPin
  ): 
    _triggerPin(triggerPin),
    _echoPin(echoPin),
    _lastDistance(-1),
    _threshold(10),
    _lastReadingTime(0),
    _thresholdCallback(NULL),
    _resolutionCallback(NULL)
  {}

  void UltrasonicSensor::begin() {
    pinMode(_triggerPin, OUTPUT);
    pinMode(_echoPin, INPUT);
  }

  void UltrasonicSensor::tick() {
    unsigned long currentTime = millis();
   if (currentTime - _lastReadingTime >= 2000) {  // Trigger reading every other second
      long distance = measureDistance();

      if (distance > 0 && _thresholdCallback) {
        _resolutionCallback(distance);
      }

      if (distance <= _threshold && _thresholdCallback) {
        _thresholdCallback();
      }

      _lastReadingTime = currentTime;
    }
  }

  void UltrasonicSensor::setTriggerThreshold(int threshold) {
    _threshold = threshold;
  }

  void UltrasonicSensor::onThresholdCrossed(void (*callback)()) {
    _thresholdCallback = callback;
  }

  void UltrasonicSensor::onResolutionUpdate(UltrasonicSensorCallback callback) {
    _resolutionCallback = callback;
  }

  long UltrasonicSensor::measureDistance() {
    digitalWrite(_triggerPin, LOW);
    delayMicroseconds(2);
    digitalWrite(_triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(_triggerPin, LOW);
  
    long duration = pulseIn(_echoPin, HIGH);

    // Convert to distance in cm
    float distance = (duration * 0.0343) / 2;

    // Fail-safe: Ignore very short readings (possible noise)
    if (distance < 2) {
        return _lastDistance;
    }

    // Exponential Moving Average (EMA) smoothing
    const float alpha = 0.6;  // Adjust for smoother vs. responsive data
    _lastDistance = (_lastDistance < 0) ? distance : (alpha * distance + (1 - alpha) * _lastDistance);
    
    return _lastDistance;
  }