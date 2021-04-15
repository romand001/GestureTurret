#include <Arduino.h>

#define SERVO1_PIN 1
#define SERVO2_PIN 2
#define LASER_PIN 3

void setup() {
    pinMode(SERVO1_PIN, OUTPUT);
    pinMode(SERVO2_PIN, OUTPUT);
    pinMode(LASER_PIN, OUTPUT);

    Serial.begin(9600);
    
    digitalWrite(LASER_PIN, HIGH);
    // delay(500);
    // digitalWrite(LASER_PIN, LOW);
    // delay(500);
    // digitalWrite(LASER_PIN, HIGH);
    // delay(500);
    // digitalWrite(LASER_PIN, LOW);

}

void loop() {

    if (Serial.available()) {

        int device = Serial.readStringUntil('-').toInt();
        int value = Serial.readString().toInt();
        Serial.print(device, value);

        switch (device) {
            case 1:
                if (value >= 0 && value <= 255) {
                    analogWrite(SERVO1_PIN, value);
                }
                break;
            case 2:
                if (value >= 0 && value <= 255) {
                    analogWrite(SERVO2_PIN, value);
                }
                break;
            case 3:
                if (value == 0 || value == 1) {
                    digitalWrite(LASER_PIN, value);
                }
                break;
        }
    }

}