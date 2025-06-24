#include "UltrasonicSensor.h"

  UltrasonicSensor::UltrasonicSensor(
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
    _lastThresholdUpdateValue(0),
    _delta(0),
    _thresholdCallback(nullptr),
    _resolutionCallback(nullptr)
  {}

  void UltrasonicSensor::begin() {
    pinMode(_triggerPin, OUTPUT);
    pinMode(_echoPin, INPUT);
    digitalWrite(_triggerPin, LOW);
  }

  void UltrasonicSensor::startMeasurement() {
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

  void UltrasonicSensor::tick() {
    // Serial.println("TICK");
    unsigned long now = micros();

    switch (_state) {
      case IDLE: {
        unsigned long nowMillis = millis();
        if (nowMillis - _lastReadingTime >= READING_DELAY) { // every 500ms
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
          float distanceCm = (duration / 2.0) * SPEED_OF_SOUND_CM_PER_SEC; // D = ST

          // Validate distance
          if (distanceCm >= MIN_DISTANCE_CM && distanceCm <= MAX_DISTANCE_CM) {
            // Exponential Moving Average (EMA) smoothing
            _latestDistanceReadingInCM = (_latestDistanceReadingInCM < 0) ? distanceCm : (MOVING_AVG_ALPHA * distanceCm + (1.0f - MOVING_AVG_ALPHA) * _latestDistanceReadingInCM);
            _newReadingAvailable = true;

            // Callbacks
            float delta = getDelta(distanceCm, _latestDistanceReadingInCM);            
            if ( _resolutionCallback && updateForResolutionThreshold(delta)) {
              _resolutionCallback(_latestDistanceReadingInCM, delta);
            }
          
            if (_thresholdCallback && _latestDistanceReadingInCM <= _threshold && _lastThresholdUpdateValue != _latestDistanceReadingInCM) {
              _thresholdCallback(_latestDistanceReadingInCM);
            }

          } else {
            // _latestDistanceReadingInCM = -1;
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

  float UltrasonicSensor::getDelta(float newReading, float latestReading) {
    return newReading - latestReading;
  }

  bool UltrasonicSensor::updateForResolutionThreshold(float delta) {
    float deltaThreshold = 1;
    switch (_sensorSenstivity) {
      case LOW_SENSITIVITY:
        deltaThreshold = 3;
        break;
      
      case MED_SENSITIVITY:
        deltaThreshold = 1;
        break;
      
      case HIGH_SENSITIVITY:
        deltaThreshold = .1;
        break;

      default:
        return false;
    }
    return ((delta >= deltaThreshold) || (delta <= -deltaThreshold));
  }