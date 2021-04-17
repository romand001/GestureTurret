#include <Arduino.h>
#include <Arduino_LSM9DS1.h>
#include <SensorFusion.h>
#include <ArduinoBLE.h>

#define TRANSMIT_PERIOD 100 // ms period for checking sensor vals and sending

SF fusion; // object for sensor fusion

BLEService yprService("1234"); // BLE service for yaw, pitch, roll

// characteristics for yaw, pitch, and roll floats
BLEFloatCharacteristic yawChar("2713", BLERead); // yaw characteristic
BLEFloatCharacteristic pitchChar("2714", BLERead); // pitch characteristic
BLEFloatCharacteristic rollChar("2715", BLERead); // roll characteristic



float ax, ay, az, gx, gy, gz, mx, my, mz; // sensor measurements
float pitch, roll, yaw; // orientation 
float deltat; // time elapsed between measurements (for sensor fusion)

void setup() {

    digitalWrite(LED_PWR, LOW); // turn off LED to reduce consumption
    
    // start serial and wait for it to initialize
    Serial.begin(9600);
    while (!Serial);

    // initialize IMU and check if successful
    if (!IMU.begin()) {
        Serial.println("Failed to initialize IMU!");
        while (1);
    }
    // initialize BLE and check if successful
    if (!BLE.begin()) {
        Serial.println("starting BLE failed!");
        while (1);
    }

    BLE.setLocalName("MIE438 Device"); 
    // set service and add the characteristics
    BLE.setAdvertisedService(yprService);
    yprService.addCharacteristic(yawChar);
    yprService.addCharacteristic(pitchChar);
    yprService.addCharacteristic(rollChar);

    BLE.addService(yprService); // "publish" the service

    // write initial values for characteristics
    yawChar.writeValue(0.0);
    pitchChar.writeValue(0.0);
    rollChar.writeValue(0.0);

    BLE.advertise(); // start advertising

    Serial.print("BLE address = "); Serial.println(BLE.address());

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

// connection loop
void loop() {

    BLEDevice central = BLE.central(); // try to connect to central

    // if connection was successful
    if (central) {

        Serial.print("connected to central: "); Serial.println(central.address());

        // main program loop
        while(central.connected()) {


            unsigned long startTime = millis(); // get starting time to accurately control loop frequency

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
                yaw = fusion.getYaw();
                pitch = fusion.getPitch();
                roll = fusion.getRoll();

                yawChar.writeValue(yaw);
                pitchChar.writeValue(pitch);
                rollChar.writeValue(roll);


                // Serial.print(pitch); Serial.print(", ");
                // Serial.print(roll); Serial.print(", ");
                // Serial.println(yaw);

            }

            unsigned long execTime = millis() - startTime;

            delay(TRANSMIT_PERIOD - execTime); // keep loop period constant

        } // exit while(central.isconnected())

    }

    // if we are here, it means BLE got disconnected, will loop back and try to connect after a short delay
    // delay(1000);


}