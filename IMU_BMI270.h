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
 * @brief Specifies the range of the accelerometer.
 */
enum class AccelRANGE
{
  RANGE_2G, ///< Sets the range to +/-2g.
  RANGE_4G, ///< Sets the range to +/-4g.
  RANGE_8G, ///< Sets the range to +/-8g.
  RANGE_16G ///< Sets the range to +/-16g.
};


/**
 * @brief Specifies the bandwidth parameter of the accelerometer.
 *
 * @details Bandwidth parameter determines the filter configuration and averaging for undersampling mode.
 */
enum class AccelBandwidth
{
  OSR2, ///< OSR4 mode, no averaging.
  OSR4, ///< OSR2 mode, average 2 samples.
  NORMAL, ///< Normal mode, average 4 samples.
  CIC_MODE, ///< CIC mode, average 8 samples.
  RES_AVG16, ///< Resoulution enchancement with 16 sample averaging.
  RES_AVG32, ///< Resoulution enchancement with 32 sample averaging.
  RES_AVG64, ///< Resoulution enchancement with 64 sample averaging.
  RES_AVG128 ///< Resoulution enchancement with 128 sample averaging.
};


/**
 * @brief Specifies the filter performance mode of the accelerometer.
 */
enum class AccelPerformanceFilter
{
  POWER_OPTIMIZED = 0, ///< Power optimized.
  PERF_OPTIMIZED = 1 ///< Performace optimized.
};


/**
 * @brief Specifies ODR (Output data rate) value of the gyroscope.
 */
enum class GyroODR
{
  ODR_25, ///< Sets the ODR value to 25 Hz.
  ODR_50, ///< Sets the ODR value to 50 Hz.
  ODR_100, ///< Sets the ODR value to 100 Hz.
  ODR_200, ///< Sets the ODR value to 200 Hz.
  ODR_400, ///< Sets the ODR value to 400 Hz.
  ODR_800, ///< Sets the ODR value to 800 Hz.
  ODR_1600, ///< Sets the ODR value to 1600 Hz.
  ODR_3200 ///< Sets the ODR value to 3200 Hz.
};


/**
 * @brief Specifies the bandwidth configuration of the gyroscope.
 */
enum class GyroBandwidth
{
  OSR4, ///< OSR4 mode.
  OSR2, ///< OSR2 mode.
  NORMAL ///< Normal mode.
};


/**
 * @brief Specifies the noise performance configuration of the gyroscope.
 */
enum class GyroNoisePerformance
{
  POWER_OPTIMIZED = 0, ///< Power optimized.
  PERF_OPTIMIZED = 1 ///< Performance optimized.
};


/**
 * @brief Specifies the filter performance mode of the gyroscope.
 */
enum class GyroFilterPerformance
{
  POWER_OPTIMIZED = 0, ///< Power optimized.
  PERF_OPTIMIZED = 1 ///< Performance optimized.
};


/**
 * @brief Specifies the angular measurement range of the gyroscope.
 */
enum class GyroRange
{
  RANGE_2000_DPS, ///< Sets the range to +/- 2000 dps, 16.4 LSB/dps.
  RANGE_1000_DPS, ///< Sets the range to +/- 1000 dps, 32.8 LSB/dps.
  RANGE_500_DPS, ///< Sets the range to +/- 500 dps, 65.6 LSB/dps.
  RANGE_250_DPS, ///< Sets the range to +/- 250 dps, 131.2 LSB/dps.
  RANGE_125_DPS ///< Sets the range to +/- 125 dps, 262.4 LSB/dps.
};


/**
 * @brief Specifies the Optical Image Stabilization measurement range of the gyroscope.
 */
enum class GyroOISRange
{
  RANGE_250_DPS, ///< Sets the range to +/- 250 dps, 131.2 LSB/dps.
  RANGE_2000_DPS ///< Sets the range to +/- 2000 dps, 16.4 LSB/dps.
};


/**
 * @brief Specifies the type of sensor.
 */
enum class SensorType
{
  ACCELEROMETER,
  GYROSCOPE
};


/**
 * @brief Specifies the power state of the sensor.
 */
enum class SensorPowerState
{
  ENABLE, // Sets the power state to ON.
  DISABLE // Sets the power state to OFF.
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


/**
 * @brief Sets the destination address in the BMI270's internal configuration memory for subsequent configuration data writes.
 *
 * @details Extracts the lower 4 bits and the upper 8 bits of the 12 bit destination address in the BMI270's internal
 * configuration memory and writes these portions to the initialization address registers.
 *
 * @param[in] deviceAddr The I2C address of the BMI270.
 * @param[in] address The 12-bit destination address in the BMI270's internal configuration memory.
 *
 * @return true if the destination address is set successfully, otherwise returns false.
 */
bool setConfigAddress(uint8_t deviceAddr, uint16_t address);


/**
 * @brief Retrieves the encoded field value for a specific accelerometer measurement range.
 *
 * @param[in] range Accelerometer measurement range.
 * @param[out] bits Variable in which the encoded field value is stored.
 *
 * @return true if the encoded field value is retrieved successfully, false if the specified range is invalid.
 */
bool getAccelRangeBits(AccelRANGE range, uint8_t &bits);

/**
 * @brief Retrieves the encoded field value for a specific accelerometer output data rate configuration.
 *
 * @param[in] odr Accelerometer output data rate configuration.
 * @param[out] bits Variable in which the encoded field value is stored.
 *
 * @return true if the encoded field value is retrieved successfully, false if the specified output data rate configuration
 * is invalid.
 */
bool getAccelODRBits(AccelODR odr, uint8_t &bits);


/**
 * @brief Retrieves the encoded field value for a specific accelerometer bandwidth parameter configuration.
 *
 * @param[in] bandwidth Accelerometer bandwidth parameter.
 * @param[out] bits Variable in which the encoded field value is stored.
 *
 * @return true if the encoded field value is retrieved successfully, false if the bandwidth parameter specified is invalid.
 */
bool getAccelBandwidthBits(AccelBandwidth bandwidth, uint8_t &bits);


/**
 * @brief Configures the accelerometer by writing the encoded field values of the specified settings.
 *
 * @details Retrieves the encoded field values for each specified setting and forms the ACC_CONF (0x40) and the ACC_RANGE (0x41)
 * register values. Writes these values to their respective registers.
 *
 * @param[in] deviceAddr The I2C address of the BMI270.
 * @param[in] odr Accelerometer output data rate configuration.
 * @param[in] range Accelerometer measurement range.
 * @param[in] bandwidth Acccelerometer bandwidth parameter.
 * @param[in] perfFilter Accelerometer performance filter configuration.
 *
 * @return true if the accelerometer is configured successfully, false if invalid settings are specified or if the write failed.
 */
bool configureAccelerometer(uint8_t deviceAddr, AccelODR odr, AccelRANGE range, AccelBandwidth bandwidth,
                            AccelPerformanceFilter perfFilter);


/**
 * @brief Retrieves the encoded field value for the specified gyroscope measurement range.
 *
 * @param[in] range Gyroscope measurement range configuration.
 * @param[out] bits Variable to store the encoded field value.
 *
 * @return true if the encoded field value is retrieved successfully, false if the specified measurement range is invalid.
 */
bool getGyroRangeBits(GyroRange range, uint8_t &bits);


/**
 * @brief Retrieves the encoded field value for the specified gyroscope bandwidth parameter.
 *
 * @param[in] bandwidth Gyroscope bandwidth parameter.
 * @param[out] bits Variable to store the encoded field value.
 *
 * @return true if the encoded field value is retrieved successfully, false if the specified bandwidth parameter is invalid.
 */
bool getGyroBandwidthBits(GyroBandwidth bandwidth, uint8_t &bits);


/**
 * @brief Retrieves the encoded field value for the specified gyroscope Optical Image Stabilization measurement range.
 *
 * @param[in] oisRange Gyroscope Optical Image Stabilization measurement range.
 * @param[out] bits Variable to store the encoded field value.
 *
 * @return true if the encoded field value is retrieved successfully, false if the specified Optical Image Stabilization
 * measurement range is invalid.
 */
bool getGyroOISRangeBits(GyroOISRange oisRange, uint8_t &bits);


/**
 * @brief Retrieves the encoded field value for a specific gyroscope output data rate configuration.
 *
 * @param[in] odr Gyroscope output data rate configuration.
 * @param[out] bits Variable in which the encoded field value is stored.
 *
 * @return true if the encoded field value is retrieved successfully, false if the specified output data rate configuration
 * is invalid.
 */
bool getGyroODRBits(GyroODR odr, uint8_t &bits);


/**
 * @brief Configures the gyroscope by writing the encoded field values of the specified settings.
 *
 * @details Retrieves the encoded field values for the specified settings and forms the GYR_CONF (0x42) and the GYR_RANGE (0x43)
 * register values. Writes them to their respective register addresses.
 *
 * @param[in] deviceAddr The I2C address of the BMI270.
 * @param[in] odr Gyroscope output data rate configuration.
 * @param[in] bandwidth Gyroscope bandwidth parameter.
 * @param[in] filterPerf Gyroscope performance filter.
 * @param[in] noisePerf Gyroscope noise filter.
 * @param[in] oisRange Gyroscope Optical Image Stabilization measurement range.
 * @param[in] range Gyroscope measurement range.
 *
 * @return true if the configuration of the gyroscope is successful, false if the specified settings are invalid or if
 * the write fails.
 */
bool configureGyroscope(uint8_t deviceAddr, GyroODR odr, GyroBandwidth bandwidth, GyroFilterPerformance filterPerf, GyroNoisePerformance noisePerf, GyroOISRange oisRange, GyroRange range);


/**
 * @brief Sets the power state of the specified sensor to the selected power state.
 *
 * @param[in] deviceAddr The I2C address of the BMI270.
 * @param[in] powerState Power state to apply to the sensor.
 * @param[in] type Type of sensor whose power state is being modified.
 *
 * @return true if the power state is set successfully, else returns false if the specified sensor type or power state is invalid
 * or if the write fails.
 */
bool setSensorPowerState(uint8_t deviceAddr, SensorPowerState powerState, SensorType type);


/**
 * @brief Reads the raw measurements of the accelerometer along all three axes (X,Y,Z).
 *
 * @details Reads the raw measurements along each axis in two bytes and combines to form 16-bit signed integer values,
 * with the least significant byte first.
 * @param[in] deviceAddr The I2C address of the BMI270.
 * @param[out] xData Raw accelerometer measurement along X-axis.
 * @param[out] yData Raw accelerometer measurement along Y-axis.
 * @param[out] zData Raw accelerometer measurement along Z-axis.
 *
 * @return true if the raw accelerometer measurements are read successfully, otherwise returns false.
 */
bool readAccelerometerData(uint8_t deviceAddr, int16_t &xData, int16_t &yData, int16_t &zData);


/**
 * @brief Reads the raw measurements of the gyroscope along all three axes (X,Y,Z).
 *
 * @details Reads the raw measurements along each axis in two bytes and combines to form 16-bit signed integer values,
 * with the least significant byte first.
 * @param[in] deviceAddr The I2C address of the BMI270.
 * @param[out] xData Raw gyroscope measurement along X-axis.
 * @param[out] yData Raw gyroscope measurement along Y-axis.
 * @param[out] zData Raw gyroscope measurement along Z-axis.
 *
 * @return true if the raw gyroscope measurements are read successfully, otherwise returns false.
 */
bool readGyroscopeData(uint8_t deviceAddr, int16_t &xData, int16_t &yData, int16_t &zData);


/**
 * @brief Retrieves the accelerometer sensitivity for the specified measurement range.
 *
 * @param[in] range Accelerometer measurement range.
 *
 * @return sensitivity of the accelerometer in LSB/g for the specified range or 0.0f if an invalid range is specified.
 */
float getAccSensitivity(AccelRANGE range);


/**
 * @brief Retrieves the gyroscope sensitivity for the specified measurement range.
 *
 * @param[in] range Gyroscope measurement range.
 *
 * @return sensitivity of the gyroscope in degree/second for the specified range or 0.0f if an invalid range is specified.
 */
float getGyroSensitivity(GyroRange range);


/**
 * @brief Scales the raw accelerometer measurements along the three axes (X,Y,Z) according to the specified accelerometer
 * measurement range.
 *
 * @details Retrieves the sensitivity of the accelerometer corresponding to the specified measurement range and scales
 * the raw measurements along the three axes (X,Y,Z).
 *
 * @param[out] scaledX Scaled accelerometer measurement in g along the X-axis.
 * @param[out] scaledY Scaled accelerometer measurement in g along the Y-axis.
 * @param[out] scaledZ Scaled accelerometer measurement in g along the Z-axis.
 * @param[in] rawX Raw accelerometer measurement along the X-axis.
 * @param[in] rawY Raw accelerometer measurement along the Y-axis.
 * @param[in] rawZ Raw accelerometer measurement along the Z-axis.
 * @param[in] range Accelerometer measurement range.
 *
 * @return true if the raw measurements are scaled successfully, false if the specified measurement range is invalid.
 */
bool scaleAccelerometerData(
  float &scaledX, float &scaledY, float &scaledZ,
  int16_t rawX, int16_t rawY, int16_t rawZ, AccelRANGE range
  );


/**
 * @brief Scales the raw gyroscope measurements along the three axes (X,Y,Z) according to the specified gyroscope
 * measurement range.
 *
 * @param[out] scaledX Scaled gyroscope measurement in degrees/second along the X-axis.
 * @param[out] scaledY Scaled gyroscope measurement in degrees/second along the Y-axis.
 * @param[out] scaledZ Scaled gyroscope measurement in degrees/second along the Z-axis.
 * @param[in] rawX Raw gyroscope measurement along the X-axis.
 * @param[in] rawY Raw gyroscope measurement along the Y-axis.
 * @param[in] rawZ Raw gyroscope measurement along the Z-axis.
 * @param[in] range Gyroscope measurement range.
 *
 * @return true if the raw measurements are scaled successfully, false if the specified measurement range is invalid.
 */
bool scaleGyroscopeData(
  float &scaledX, float &scaledY, float &scaledZ,
  int16_t rawX, int16_t rawY, int16_t rawZ, GyroRange range
  );


#endif