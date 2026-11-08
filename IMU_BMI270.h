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

// Function used in the initialization of the BMI270, converts and passes the 12 bit register address to set the internal memory pointer for the passing of the config array.
bool setConfigAddress(uint8_t deviceadd, uint16_t address);

bool configureAccelerometer(uint8_t deviceadd, AcelODR odr, AcelRANGE range, AcelBANDWITH bandwith, performanceFilter perfFilter);

enum class AcelODR
{
    ODR_25,
    ODR_50,
    ODR_100,
    ODR_200,
    ODR_400,
    ODR_800,
    ODR_1600
};

enum class AcelRANGE
{
  RANGE_2G,
  RANGE_4G,
  RANGE_8g,
  RANGE_16G
};

enum class AcelBANDWITH
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

enum class performanceFilter
{
  POWER_OPT = 0,
  PERF_OPT = 1
};

bool getRangeBits(AcelRANGE range, uint8_t &bits);

bool getODRBits(AcelODR odr, uint8_t &bits);

bool getBandwithBits(AcelBANDWITH bandwith, uint8_t &bits);
#endif