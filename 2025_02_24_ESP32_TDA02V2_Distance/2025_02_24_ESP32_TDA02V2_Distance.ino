#define TRIGGER_PIN  32
#define ECHO_PIN     33

#include "UltrasonicSensor.h"

UltrasonicSensor sensor(TRIGGER_PIN, ECHO_PIN);

void setup() {
    Serial.begin(9600);
    sensor.begin();
}

void loop() {
    sensor.triggerMeasurement();
    delay(200);

    float distance = sensor.getDistance();

    Serial.print("Distance: ");
    if (distance > 0) {
        Serial.print(distance);
        Serial.println(" cm");
    } else {
        Serial.println("Measurement error.");
    }

    delay(500);
}