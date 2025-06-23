#include "UltrasonicSensorService.h"

  UltrasonicSensorService::UltrasonicSensorService(
    uint8_t triggerPin, 
    uint8_t echoPin,
    SensorSenstivity sensorSenstivity
  ): 
    _triggerPin(triggerPin),
    _echoPin(echoPin),
    _sensorSenstivity(sensorSenstivity),
    _state(IDLE),
    _lastTriggerTime(0), _echoStartTime(0), _echoEndTime(0),
    _lastReadingTime(0),
    _latestDistanceReadingInCM(0),
    _newReadingAvailable(false),
    _threshold(10),
    _delta(0),
    _thresholdCallback(nullptr),
    _resolutionCallback(nullptr)
  {}

  void UltrasonicSensorService::begin() {
    pinMode(_triggerPin, OUTPUT);
    pinMode(_echoPin, INPUT);
    digitalWrite(_triggerPin, LOW);
  }

  void UltrasonicSensorService::startMeasurement() {
    // Only allow new measurement if idle
    if (_state == IDLE) {
        digitalWrite(_triggerPin, LOW);
        delayMicroseconds(2);
        digitalWrite(_triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(_triggerPin, LOW);

        _state = WAITING_FOR_ECHO_START;
        _lastTriggerTime = micros();
    }
  }

  void UltrasonicSensorService::tick() {
    unsigned long now = micros();

    switch (_state) {
      case IDLE: {
        unsigned long nowMillis = millis();
        if (nowMillis - _lastReadingTime >= 500) { // every 500ms
          startMeasurement();
        }
        break;
      }

      case WAITING_FOR_ECHO_START: {
        if (digitalRead(_echoPin) == HIGH) {
          _echoStartTime = now;
          _state = WAITING_FOR_ECHO_END;
        } else if (now - _lastTriggerTime > TIMEOUT_US) {
          _state = IDLE;
        }
        break;
      }
      
      case WAITING_FOR_ECHO_END: {
        if (digitalRead(_echoPin) == LOW) {
          _echoEndTime = now;
          unsigned long duration = _echoEndTime - _echoStartTime;
          float distanceCm = (duration / 2.0) * 0.0343;

          // Validate distance
          if (distanceCm >= MIN_DISTANCE_CM && distanceCm <= MAX_DISTANCE_CM) {
            // Exponential Moving Average (EMA) smoothing
            const float alpha = 0.6;
            _latestDistanceReadingInCM = (_latestDistanceReadingInCM < 0) ? distanceCm : (alpha * distanceCm + (1.0f - alpha) * _latestDistanceReadingInCM);
            _newReadingAvailable = true;

            // Callbacks
            float delta = 0;
            if (_resolutionCallback) {
              _resolutionCallback(_latestDistanceReadingInCM, delta);
            }
          
            if (_thresholdCallback && _latestDistanceReadingInCM <= _threshold) {
              _thresholdCallback(_latestDistanceReadingInCM);
            }
          } else {
            _latestDistanceReadingInCM = -1;
            _newReadingAvailable = false;
          }

          _lastReadingTime = millis();
          _state = IDLE;
        } else if (now - _echoStartTime > TIMEOUT_US) {
          _state = IDLE;
        }
        break;
      }
    }
  }