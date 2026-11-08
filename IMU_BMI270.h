#ifndef IMU_BMI270_H
#define IMU_BMI270_H

#include <Arduino.h>
#include <Wire.h>

// To read a single byte from a register.
bool readRegister(uint8_t deviceadd, uint8_t regadd, uint8_t &data);

// To read multiple bytes from a register.
bool readRegisters(uint8_t deviceadd, uint8_t regadd, uint8_t *data, uint8_t length);

// To write a byte into a register.
bool writeRegister(uint8_t deviceadd, uint8_t regadd, uint8_t data);

// To execute the initialization sequence of the BMI270
bool initializeBMI270(uint8_t deviceadd);

// To write to multiple registers.
bool writeRegisters(uint8_t deviceadd, uint8_t regadd, const uint8_t *data, uint8_t length);

/* Function used in the initialization of the BMI270, converts and passes the 12 bit register address to set the
   internal memory pointer for the passing of the config array. */
bool setConfigAddress(uint8_t deviceadd, uint16_t address);

enum class AccelODR
{
    ODR_25,
    ODR_50,
    ODR_100,
    ODR_200,
    ODR_400,
    ODR_800,
    ODR_1600
};

enum class AccelRANGE
{
  RANGE_2G,
  RANGE_4G,
  RANGE_8G,
  RANGE_16G
};

enum class AccelBandwidth
{
  OSR2,
  OSR4,
  NORMAL,
  CIC_MODE,
  RES_AVG16,
  RES_AVG32,
  RES_AVG64,
  RES_AVG128
};

enum class AccelPerformanceFilter
{
  POWER_OPTIMIZED = 0,
  PERF_OPTIMIZED = 1
};

bool getAccelRangeBits(AccelRANGE range, uint8_t &bits);

bool getAccelODRBits(AccelODR odr, uint8_t &bits);

bool getAccelBandwidthBits(AccelBandwidth bandwidth, uint8_t &bits);

bool configureAccelerometer(uint8_t deviceadd, AccelODR odr, AccelRANGE range, AccelBandwidth bandwidth,
                            AccelPerformanceFilter perfFilter);

enum class GyroODR
{
  ODR_25,
  ODR_50,
  ODR_100,
  ODR_200,
  ODR_400,
  ODR_800,
  ODR_1600,
  ODR_3200
};

enum class GyroBandwidth
{
  OSR4,
  OSR2,
  NORMAL
};

enum class GyroNoisePerformance
{
  POWER_OPTIMIZED = 0,
  PERF_OPTIMIZED = 1
};

enum class GyroFilterPerformance
{
  POWER_OPTIMIZED = 0,
  PERF_OPTIMIZED = 1
};

enum class GyroRange
{
  RANGE_2000_DPS,
  RANGE_1000_DPS,
  RANGE_500_DPS,
  RANGE_250_DPS,
  RANGE_125_DPS
};

enum class GyroOISRange
{
  RANGE_250_DPS,
  RANGE_2000_DPS
};

bool getGyroRangeBits(GyroRange range, uint8_t &bits);

bool getGyroBandwidthBits(GyroBandwidth bandwidth, uint8_t &bits);

bool getGyroOISRangeBits(GyroOISRange oisRange, uint8_t &bits);

bool getGyroODRBits(GyroODR odr, uint8_t &bits);

bool configureGyroscope(uint8_t deviceadd, GyroODR odr, GyroBandwidth bandwidth, GyroFilterPerformance filterPerf, GyroNoisePerformance noisePerf, GyroOISRange oisRange, GyroRange range);
#endif