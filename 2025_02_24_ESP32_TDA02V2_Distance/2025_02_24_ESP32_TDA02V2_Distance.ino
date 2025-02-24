#include "UltrasonicSensor.h"
#include "esp_sleep.h"

UltrasonicSensor sensor(32,33); // TRIG = 32, ECHO = 33

void setup() {
    Serial.begin(9600);
    sensor.begin();
}

void loop() {
    sensor.triggerMeasurement(); // Start measurement
    delay(200); // Allow time for measurement
    float distance = sensor.getDistance();

    Serial.print("Distance: ");
    if (distance > 0) {
        Serial.print(distance);
        Serial.println(" cm");
    } else {
        Serial.println("Measurement error.");
    }

    // Power optimization - Light Sleep Mode
    Serial.println("Entering light sleep...");
    esp_sleep_enable_timer_wakeup(500 * 1000); // Sleep for 500ms (500,000µs)
    esp_light_sleep_start(); // Enter light sleep, will wake up automatically

    Serial.println("Waking up...");
}