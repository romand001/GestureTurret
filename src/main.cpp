#include <Arduino.h>
#include <Arduino_LSM9DS1.h>
#include <SensorFusion.h>

SF fusion;

float ax, ay, az, gx, gy, gz, mx, my, mz;
float pitch, roll, yaw;
float deltat;

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


    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() && IMU.magneticFieldAvailable()) {

        IMU.readAcceleration(ax, ay, az);
        IMU.readGyroscope(gx, gy, gz);
        IMU.readMagneticField(mx, my, mz);

        gx *= 0.0174533f;
        gy *= 0.0174533f;
        gz *= 0.0174533f;
        
        
        deltat = fusion.deltatUpdate();

        fusion.MadgwickUpdate(gx, gy, gz, ax, ay, az, mx, my, mz, deltat);

        pitch = fusion.getPitch();
        roll = fusion.getRoll();
        yaw = fusion.getYaw();

        Serial.print(pitch); Serial.print(", ");
        Serial.print(roll); Serial.print(", ");
        Serial.println(yaw);

    }


}