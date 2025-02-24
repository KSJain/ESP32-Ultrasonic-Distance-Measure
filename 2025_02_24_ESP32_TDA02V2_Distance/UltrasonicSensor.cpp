#include "UltrasonicSensor.h"

volatile long UltrasonicSensor::startTime = 0;
volatile long UltrasonicSensor::endTime = 0;

UltrasonicSensor::UltrasonicSensor(uint8_t trigPin, uint8_t echoPin)
    : trigPin(trigPin), echoPin(echoPin), lastValidDistance(-1) {}

void UltrasonicSensor::begin() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    attachInterruptArg(digitalPinToInterrupt(echoPin), echoISR, this, CHANGE);
}

void UltrasonicSensor::triggerMeasurement() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
}

// Interrupt Service Routine (ISR)
void IRAM_ATTR UltrasonicSensor::echoISR(void* arg) {
    if (digitalRead(static_cast<UltrasonicSensor*>(arg)->echoPin) == HIGH) {
        startTime = micros();
    } else {
        endTime = micros();
    }
}

float UltrasonicSensor::getDistance() {
    long duration = endTime - startTime;

    // Fail-safe: Invalid duration check
    if (duration <= 0 || duration > 40000) { 
        return lastValidDistance; // Return last good reading instead of error
    }

    // Convert to distance in cm
    float distance = (duration * 0.0343) / 2;

    // Fail-safe: Ignore very short readings (possible noise)
    if (distance < 2) {
        return lastValidDistance;
    }

    // Exponential Moving Average (EMA) smoothing
    const float alpha = 0.6;  // Adjust for smoother vs. responsive data
    lastValidDistance = (lastValidDistance < 0) ? distance : (alpha * distance + (1 - alpha) * lastValidDistance);
    
    return lastValidDistance;
}