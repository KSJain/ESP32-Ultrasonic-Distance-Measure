#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

#include <Arduino.h>
#include <functional>

class UltrasonicSensor {
public:
    using UltrasonicSensorCallback = std::function<void(float distance)>;

    UltrasonicSensor(int triggerPin, int echoPin);

    void begin();
    void tick();

    void setTriggerThreshold(int threshold);
    void onThresholdCrossed(void (*callback)());
    void onResolutionUpdate(UltrasonicSensorCallback callback);

private:
    int _triggerPin;
    int _echoPin;
    unsigned long _lastReadingTime;
    
    int _threshold;
    void (*_thresholdCallback)();

    // void (*_resolutionCallback)(long);
    UltrasonicSensorCallback _resolutionCallback;

    long _lastDistance;
    long measureDistance();
};

#endif