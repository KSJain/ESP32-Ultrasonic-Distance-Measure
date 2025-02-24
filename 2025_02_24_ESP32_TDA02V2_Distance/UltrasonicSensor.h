#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

#include <Arduino.h>

class UltrasonicSensor {
public:
    UltrasonicSensor(uint8_t trigPin, uint8_t echoPin);
    void begin();
    void triggerMeasurement();
    float getDistance(); // Returns latest measured distance

private:
    static void IRAM_ATTR echoISR(void* arg); // Interrupt service routine
    static volatile long startTime;
    static volatile long endTime;
    uint8_t trigPin, echoPin;
    float lastValidDistance; // Stores last good reading
};

#endif