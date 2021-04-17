#include <Arduino.h>
#include <Servo.h>
#include "servo_limits.h"

#define SERVO1_PIN 1
#define SERVO2_PIN 2
#define LASER_PIN 3

Servo servo1;
Servo servo2;

void setup() {

    // set laser pin to output
    pinMode(LASER_PIN, OUTPUT);

    // initialize servo library to PWM pins (takes care of making them outputs)
    servo1.attach(SERVO1_PIN);
    servo2.attach(SERVO2_PIN);
    
    // set laser on
    digitalWrite(LASER_PIN, HIGH);

    // initialize serial communication with PC
    Serial.begin(9600);

}

void loop() {

    if (Serial.available() >= 10) { // check if we have 10 bytes or more (length of command message)

        int angle1 = Serial.readStringUntil(',').toInt(); // angle 1 is first number
        int angle2 = Serial.readStringUntil(',').toInt(); // angle 2 is second number
        int laser = Serial.readStringUntil('\n').toInt(); // laser is third number

        // NOTE: if the above conversions to int fail, they evaluate to 0 (this is fine)

        // enforce angles to be between within limits defined in header file
        angle1 = max(angle1, S1_MIN);
        angle1 = min(angle1, S1_MAX);

        angle2 = max(angle2, S1_MIN);
        angle2 = min(angle2, S1_MAX);

        // use library to set angle of servos
        servo1.write(angle1);
        servo2.write(angle2);

        // check that laser is either 0 (off) or 1 (on)
        if (laser == 0 || laser == 1) {
            digitalWrite(LASER_PIN, laser);
        }

    }

    delay(10);

}