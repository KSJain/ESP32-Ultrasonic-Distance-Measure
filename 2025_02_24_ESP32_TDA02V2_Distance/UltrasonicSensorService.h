#pragma once

#include <Arduino.h>

typedef void (*ThresholdCallback)();
typedef void (*UltrasonicSensorCallback)(float);

class UltrasonicSensorService {
public:
    UltrasonicSensorService(uint8_t triggerPin, uint8_t echoPin);

    void begin();
    void startMeasurement();
    void update(); // Call in loop

    void setTriggerThreshold(float threshold);
    void onThresholdCrossed(ThresholdCallback callback);
    void onResolutionUpdate(UltrasonicSensorCallback callback);

private:
    enum State {
        IDLE,
        WAITING_FOR_ECHO_START,
        WAITING_FOR_ECHO_END
    };

    uint8_t _triggerPin;
    uint8_t _echoPin;
    State _state;

    unsigned long _lastTriggerTime;
    unsigned long _echoStartTime;
    unsigned long _echoEndTime;
    unsigned long _lastReadingTime;

    bool _newReadingAvailable;
    float _lastDistanceCm;

    float _threshold;
    ThresholdCallback _thresholdCallback;
    UltrasonicSensorCallback _resolutionCallback;

    static constexpr float MIN_DISTANCE_CM = 20.0;
    static constexpr float MAX_DISTANCE_CM = 600.0;
    static constexpr unsigned long TIMEOUT_US = 30000; // 30ms timeout
};
