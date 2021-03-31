#include <Arduino.h>
#include <Arduino_LSM9DS1.h>
#include <SensorFusion.h>

SF fusion;

float ax, ay, az, gx, gy, gz, mx, my, mz;
float pitch, roll, yaw;
float deltat;
bool ready = true;

void setup() {
    Serial.begin(9600);
    while (!Serial);

    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (1);
    }
    Serial.print("Accelerometer sample rate = ");
    Serial.print(IMU.accelerationSampleRate());
    Serial.println(" Hz");
    Serial.print("Gyroscope sample rate = ");
    Serial.print(IMU.gyroscopeSampleRate());
    Serial.println(" Hz");
    Serial.print("Magnetic field sample rate = ");
    Serial.print(IMU.magneticFieldSampleRate());
    Serial.println(" Hz");

}

void loop() {

    ready = true;

    if (IMU.accelerationAvailable()) IMU.readAcceleration(ax, ay, az);
    else ready = false;
    if (IMU.gyroscopeAvailable()) IMU.readGyroscope(gx, gy, gz);
    else ready = false;
    if (IMU.magneticFieldAvailable()) IMU.readMagneticField(mx, my, mz);
    else ready = false;

    if (ready) {

        deltat = fusion.deltatUpdate();

        fusion.MadgwickUpdate(gx, gy, gz, ax, ay, az, mx, my, mz, deltat);
        // fusion.MahonyUpdate(gx, gy, gz, ax, ay, az, mx, my, mz, deltat);

        pitch = fusion.getPitch();
        roll = fusion.getRoll();
        yaw = fusion.getYaw();

        Serial.print(pitch); Serial.print(", ");
        Serial.print(roll); Serial.print(", ");
        Serial.println(yaw);

    }
    

    delay(10);



}