#include <Arduino.h>
#include <Servo.h>
#include "servo_limits.h"

#define SERVO1_PIN 1
#define SERVO2_PIN 2
#define LASER_PIN 3

Servo servo1;
Servo servo2;

void setup() {

    pinMode(LASER_PIN, OUTPUT);

    servo1.attach(SERVO1_PIN);
    servo2.attach(SERVO2_PIN);
    
    digitalWrite(LASER_PIN, HIGH);

    Serial.begin(9600);

}

void loop() {

    if (Serial.available() >= 5) {

        int device = Serial.readStringUntil('-').toInt();
        int value = Serial.readStringUntil('\n').toInt();
        Serial.print(device); Serial.print("-"); Serial.println(value);

        switch (device) {
            case 1:
                value = max(value, S1_MIN);
                value = min(value, S1_MAX);
                servo1.write(value);
                break;
            case 2:
                value = max(value, S2_MIN);
                value = min(value, S2_MAX);
                servo2.write(value);
                break;
            case 3:
                if (value == 0 || value == 1) {
                    digitalWrite(LASER_PIN, value);
                }
                break;
        }
    }

    delay(10);

}