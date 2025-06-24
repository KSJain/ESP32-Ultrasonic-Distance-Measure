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
    static constexpr unsigned long TIMEOUT_US = 30000;
    static constexpr unsigned long READING_DELAY = 3000;
    static constexpr float MIN_DISTANCE_CM = 4.0;
    static constexpr float MAX_DISTANCE_CM = 300.0;
    static constexpr float SPEED_OF_SOUND_CM_PER_SEC = 0.0343;
    static constexpr float MOVING_AVG_ALPHA = 0.6; // 0.0 () 1.0

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

    bool updateForResolutionThreshold(float delta);
    float getDelta(float newReading, float latestReading);
    void startMeasurement();

};

#endif