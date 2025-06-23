#ifndef ULTRASONICSENSORSERVICE_H
#define ULTRASONICSENSORSERVICE_H


#pragma once
#include <Arduino.h>
#include <functional>

class UltrasonicSensorService {
public:
    enum SensorSenstivity {
      LOW_SENSITIVITY, 
      MED_SENSITIVITY, 
      HIG_SENSITIVITY
    };

    UltrasonicSensorService(
      uint8_t triggerPin, 
      uint8_t echoPin,
      SensorSenstivity sensorSenstivity = LOW_SENSITIVITY
    );

    void begin();
    void tick();

    float getDistanceInCM() {
        return _latestDistanceReadingInCM;
    };

    float getTriggerThreshold(){
        return _threshold;
    };

    void setTriggerThreshold(float threshold) {
        _threshold = threshold;
    };

    void updateReadingResolution(SensorSenstivity sensorSenstivity) {
        _sensorSenstivity = sensorSenstivity;
    };

    using ThresholdCallback = std::function<void(float distance)>;
    using ResolutionUpdateCallback = std::function<void(float distance, float delta)>;

    void onThresholdCrossed(ThresholdCallback callback) {
        _thresholdCallback = callback;
    };

    void onResolutionUpdate(ResolutionUpdateCallback callback) {
        _resolutionCallback = callback;
    };

private:
    static constexpr float MIN_DISTANCE_CM = 20.0;
    static constexpr float MAX_DISTANCE_CM = 600.0;
    static constexpr unsigned long TIMEOUT_US = 30000; // 30ms 

    enum State {
        IDLE,
        WAITING_FOR_ECHO_START,
        WAITING_FOR_ECHO_END
    };

    uint8_t _triggerPin;
    uint8_t _echoPin;

    SensorSenstivity _sensorSenstivity;// = LOW_SENSITIVITY;
    State _state;

    unsigned long _lastTriggerTime;
    unsigned long _echoStartTime;
    unsigned long _echoEndTime;
    unsigned long _lastReadingTime;

    bool _newReadingAvailable;
    float _latestDistanceReadingInCM;

    float _threshold;
    float _delta = 0;

    ThresholdCallback _thresholdCallback;
    ResolutionUpdateCallback _resolutionCallback;

    float getDelta(float newReading, float latestReading);
    void startMeasurement();

};

#endif