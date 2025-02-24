#define TRIGGER_PIN  32
#define ECHO_PIN     33

void setup() {
    Serial.begin(9600);
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}

void loop() {
    // Ensure a clean signal
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    
    // Send a 10-microsecond pulse to trigger the measurement
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
  
    // Read the echo pulse duration
    long duration = pulseIn(ECHO_PIN, HIGH);
  
    // Convert duration to distance (Speed of sound = 343m/s)
    float distance_cm = duration * 0.0343 / 2; 

    Serial.print("Distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");

    delay(500);  // Wait before the next measurement
}