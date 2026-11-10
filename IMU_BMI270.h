#ifndef IMU_BMI270_H
#define IMU_BMI270_H

#include <Arduino.h>
#include <Wire.h>


/**
 * @brief Specifies the ODR (Output Data Rate) value of the accelerometer.
 */
enum class AccelODR
{
  ODR_25, ///< Sets the ODR value to 25 Hz.
  ODR_50, ///< Sets the ODR value to 50 Hz.
  ODR_100, ///< Sets the ODR value to 100 Hz.
  ODR_200, ///< Sets the ODR value to 200 Hz.
  ODR_400, ///< Sets the ODR value to 400 Hz.
  ODR_800, ///< Sets the ODR value to 800 Hz.
  ODR_1600 ///< Sets the ODR value to 1600 Hz.
};


/**
 * @brief Speicifies the Range of the accelerometer.
 */
enum class AccelRANGE
{
  RANGE_2G, ///< Sets the range to +/-2g.
  RANGE_4G, ///< Sets the range to +/-4g.
  RANGE_8G, ///< Sets the range to +/-8g.
  RANGE_16G ///< Sets the range to +/-16g.
};


/**
 * @brief Specifies the bandwidth paramter of the accelerometer.
 *
 * @details Bandwidth parameter determines the filter configuration and averaging for undersampling mode.
 */
enum class AccelBandwidth
{
  OSR2, ///< OSR4 mode, no averaging.
  OSR4, ///< OSR2 mode, average 2 samples.
  NORMAL, ///< Normal mode, average 4 samples.
  CIC_MODE, ///< CIC mode, average 8 samples.
  RES_AVG16, ///< Resoultion enchancement with 16 sample averaging.
  RES_AVG32, ///< Resoultion enchancement with 32 sample averaging.
  RES_AVG64, ///< Resoultion enchancement with 64 sample averaging.
  RES_AVG128 ///< Resoultion enchancement with 128 sample averaging.
};


enum class AccelPerformanceFilter
{
  POWER_OPTIMIZED = 0,
  PERF_OPTIMIZED = 1
};

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

enum class SensorType
{
  ACCELEROMETER,
  GYROSCOPE
};

enum class SensorPowerState
{
  ENABLE,
  DISABLE
};


/**
 * @brief Reads a single register via the I2C Communication protocol.
 *
 * @param[in] deviceAddr The I2C device address of the BMI270.
 * @param[in] regAddr Address of the register to be read.
 * @param[out] data Variable in which the byte read is stored.
 *
 * @return true if the register reading succeeds, otherwise returns false.
 */
bool readRegister(uint8_t deviceAddr, uint8_t regAddr, uint8_t &data);


/**
 * @brief Burst reads multiple registers using the I2C Communication protocol.
 *
 * @param[in] deviceAddr The I2C device address of the BMI270.
 * @param[in] regAddr The address of the starting register of the burst read.
 * @param[in] length The number of bytes to be read in the burst transaction.
 * @param[out] data Variable in which the bytes read will be stored.
 *
 * @return true if the burst read succeeds, otherwise returns false.
 */
bool readRegisters(uint8_t deviceAddr, uint8_t regAddr, uint8_t *data, uint8_t length);


/**
 * @brief Writes a byte to a specified register via the I2C communication protocol.
 *
 * @param[in] deviceAddr The I2C address of the BMI270.
 * @param[in] regAddr The address of the register to which the byte is to be written.
 * @param[in] data The byte to be written.
 *
 * @return true if the write succeeds, otherwise returns false.
 */
bool writeRegister(uint8_t deviceAddr, uint8_t regAddr, uint8_t data);


/**
 * @brief Burst writes to multiple registers via the I2C communication protocol.
 *
 * @param[in] deviceAddr The I2C address of the BMI270.
 * @param[in] regAddr The address of the starting register of the burst write transaction.
 * @param[in] data The bytes to be written.
 * @param[in] length The number of bytes to be written in the burst write transaction.
 *
 * @return true if the burst write succeeds, otherwise returns false.
 */
bool writeRegisters(uint8_t deviceAddr, uint8_t regAddr, const uint8_t *data, uint8_t length);



/**
 * @brief Initializes the BMI270 IMU.
 *
 * @details Performs the BMI270 initialization sequence, including disabling the advanced power state,
 * uploading the configuration file, and checking the initialization status.

 * @param[in] deviceAddr The I2C address of the BMI270.
 *
 * @return true if initialization succeeds, otherwise returns false.
 *
 * @pre The I2C communication must be initialized before calling this function.
 *
 * @post The configuration file has been loaded and the BMI270 is ready for further configuration.
 */
bool initializeBMI270(uint8_t deviceAddr);



/* Function used in the initialization of the BMI270, converts and passes the 12 bit register address to set the
   internal memory pointer for the passing of the config array. */
bool setConfigAddress(uint8_t deviceAddr, uint16_t address);

bool getAccelRangeBits(AccelRANGE range, uint8_t &bits);

bool getAccelODRBits(AccelODR odr, uint8_t &bits);

bool getAccelBandwidthBits(AccelBandwidth bandwidth, uint8_t &bits);

bool configureAccelerometer(uint8_t deviceAddr, AccelODR odr, AccelRANGE range, AccelBandwidth bandwidth,
                            AccelPerformanceFilter perfFilter);

bool getGyroRangeBits(GyroRange range, uint8_t &bits);

bool getGyroBandwidthBits(GyroBandwidth bandwidth, uint8_t &bits);

bool getGyroOISRangeBits(GyroOISRange oisRange, uint8_t &bits);

bool getGyroODRBits(GyroODR odr, uint8_t &bits);

bool configureGyroscope(uint8_t deviceAddr, GyroODR odr, GyroBandwidth bandwidth, GyroFilterPerformance filterPerf, GyroNoisePerformance noisePerf, GyroOISRange oisRange, GyroRange range);

bool setSensorPowerState(uint8_t deviceAddr, SensorPowerState powerState, SensorType type);

bool readAccelerometerData(uint8_t deviceAddr, int16_t &xData, int16_t &yData, int16_t &zData);

bool readGyroscopeData(uint8_t deviceAddr, int16_t &xData, int16_t &yData, int16_t &zData);

float getAccSensitivity(AccelRANGE range);

float getGyroSensitivity(GyroRange range);

bool scaleAccelerometerData(
  int16_t &scaledX, int16_t &scaledY, int16_t &scaledZ,
  int16_t rawX, int16_t rawY, int16_t rawZ, AccelRANGE range
  );

bool scaleGyroscopeData(
  int16_t &scaledX, int16_t &scaledY, int16_t &scaledZ,
  int16_t rawX, int16_t rawY, int16_t rawZ, GyroRange range
  );

#endif