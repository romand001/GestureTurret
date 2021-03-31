#include <Arduino.h>
#include <Arduino_LSM9DS1.h>
#include <SensorFusion.h>
#include <ArduinoBLE.h>

SF fusion; // object for sensor fusion

float ax, ay, az, gx, gy, gz, mx, my, mz; // sensor measurements
float pitch, roll, yaw; // orientation 
float deltat; // time elapsed between measurements (for sensor fusion)

void setup() {
    
    // start serial and wait for it to initialize
    Serial.begin(9600);
    while (!Serial);

    // initialize IMU and check if successful
    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (1);
    }

    // print out sample rates of sensors
    Serial.print("Accelerometer sample rate = ");
    Serial.print(IMU.accelerationSampleRate()); // 119Hz
    Serial.println(" Hz");
    Serial.print("Gyroscope sample rate = ");
    Serial.print(IMU.gyroscopeSampleRate()); // 119Hz
    Serial.println(" Hz");
    Serial.print("Magnetic field sample rate = ");
    Serial.print(IMU.magneticFieldSampleRate()); // 20Hz
    Serial.println(" Hz");

}

void loop() {

    // if data from all 3 sensors is available
    // magnetometer is bottleneck, results in 20Hz sample rate (should be enough)
    if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable() && IMU.magneticFieldAvailable()) {

        // read sensor measurements and update the variables
        IMU.readAcceleration(ax, ay, az);
        IMU.readGyroscope(gx, gy, gz);
        IMU.readMagneticField(mx, my, mz);

        // scale from deg/s to rad/s
        gx *= 0.0174533f;
        gy *= 0.0174533f;
        gz *= 0.0174533f;
        
        // calculate time elapsed since last occurance
        deltat = fusion.deltatUpdate();

        // run sensor fusion algorithm
        fusion.MadgwickUpdate(gx, gy, gz, ax, ay, az, mx, my, mz, deltat);

        // get the orientation
        pitch = fusion.getPitch();
        roll = fusion.getRoll();
        yaw = fusion.getYaw();

        Serial.print(pitch); Serial.print(", ");
        Serial.print(roll); Serial.print(", ");
        Serial.println(yaw);

    }


}