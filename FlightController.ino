#include "IMU_BMI270.h"

int16_t accX = 0;
int16_t accY = 0;
int16_t accZ= 0;

int16_t gyroX = 0;
int16_t gyroY = 0;
int16_t gyroZ= 0;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);

  Serial.println("Flight controller Starting...\n");
  Wire.begin(8,9);

  if(initializeBMI270(0x69))
  {
      Serial.println("Initialization was successful!");
  }
  else
  {
      Serial.println("Initialization failed :(");
      return;
  }

    Serial.println("Configuring the accelerometer and the gyroscope...");

    if (configureAccelerometer(0x69, AccelODR::ODR_100, AccelRANGE::RANGE_4G, AccelBandwidth::NORMAL, AccelPerformanceFilter::PERF_OPTIMIZED))
    {
        Serial.println("Accelerometer configured!");
    }
    else
    {
        Serial.println("Accelerometer configuration failed.");
        return;
    }

    if (configureGyroscope(0x69, GyroODR::ODR_100, GyroBandwidth::NORMAL, GyroFilterPerformance::PERF_OPTIMIZED, GyroNoisePerformance::PERF_OPTIMIZED, GyroOISRange::RANGE_250_DPS, GyroRange::RANGE_500_DPS))
    {
        Serial.println("Gyroscope configured!");
    }
    else
    {
        Serial.println("Gyroscope configuration failed.");
        return;
    }

    Serial.println("Starting the sensors...");

    if (setSensorPowerState(0x69, SensorPowerState::ENABLE, SensorType::ACCELEROMETER) &&
        setSensorPowerState(0x69, SensorPowerState::ENABLE, SensorType::GYROSCOPE))
    {
        Serial.println("Sensors powered on!");
    }

    else
    {
        Serial.println("Sensor activation failed..");
        return;
    }

}

void loop() {
  // put your main code here, to run repeatedly:

  if (!readAccelerometerData(0x69, accX, accY, accZ))
  {
      return;
  }
  if (!readGyroscopeData(0x69, gyroX, gyroY, gyroZ))
  {
      return;
  }

  Serial.println("Accelerometer data:");
    Serial.print(accX);
    Serial.print(" ");
    Serial.print(accY);
    Serial.print(" ");
    Serial.print(accZ);
    Serial.println();

    Serial.println("Gyroscope data:");
    Serial.print(gyroX);
    Serial.print(" ");
    Serial.print(gyroY);
    Serial.print(" ");
    Serial.print(gyroZ);
    Serial.println();

    delay(10000);
}
