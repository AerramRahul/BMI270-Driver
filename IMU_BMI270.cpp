

#include "IMU_BMI270.h"
#include "BMI270_config.h"


bool readRegister(uint8_t deviceAddr, uint8_t regAddr, uint8_t &data)
{
  Wire.beginTransmission(deviceAddr);

  // Setting the internal memory pointer.
  Wire.write(regAddr);

  // Repeated Start.
  uint8_t error = Wire.endTransmission(false);
  if(error != 0)
  {
    return false; // Communication failed.
  }
  uint8_t bytesReceived = Wire.requestFrom(deviceAddr, 1); // Returns number of bytes in the memory buffer.

  if(Wire.available() >= 1 && bytesReceived == 1)
  {
    data = Wire.read();
    return true;
  }
  else
  return false;
  
}



bool writeRegister(uint8_t deviceAddr, uint8_t regAddr, uint8_t data)
{
  Wire.beginTransmission(deviceAddr);

  Wire.write(regAddr);

  Wire.write(data);

  uint8_t error = Wire.endTransmission();

  return (error == 0);
}



bool readRegisters(uint8_t deviceAddr, uint8_t regAddr, uint8_t *data, uint8_t length)
{

  Wire.beginTransmission(deviceAddr);

  Wire.write(regAddr); // Setting internal memory pointer.

  uint8_t error = Wire.endTransmission(false); // Repeated Start

  if(error != 0)
  {
    return false;
  }

  uint8_t bytesReceived = Wire.requestFrom(deviceAddr, length);

  if(bytesReceived == length && Wire.available() >= length)
  {
    for(int i = 0; i<length; i++)
    {
      data[i] = Wire.read();
    }
    return true;
  }
  else
  return false;

}



bool writeRegisters(uint8_t deviceAddr, uint8_t regAddr, const uint8_t *data, uint8_t length)
{

  Wire.beginTransmission(deviceAddr);

  Wire.write(regAddr);

  size_t bytesWritten = Wire.write(data, length);

  uint8_t error = Wire.endTransmission();
  
  return (error == 0 && bytesWritten == length);
}



bool setConfigAddress(uint8_t deviceAddr, uint16_t address)
{
  uint8_t lowNibble = address & 0x0F; // Extract the right most 4 bits.

  uint8_t highByte = (address >> 4) & 0xFF; // Extracts the right most 8 bits.

  if(!writeRegister(deviceAddr, 0x5B, lowNibble))
  {
    return false;
  }

  if(!writeRegister(deviceAddr, 0x5C, highByte))
  {
    return false;
  }

  return true;

}


bool initializeBMI270(uint8_t deviceAddr)
{
  uint8_t chipID;

  if(!readRegister(deviceAddr, 0x00, chipID))
    return false;

  if(chipID != 0x24)
    return false;

  // Writing to disable PWR_CONF.adv_power_save
  if(!writeRegister(deviceAddr, 0x7C, 0x00))
    return false;

  // Waiting for 450 microseconds;
  delayMicroseconds(450);

  // Preparing config load INIT_CTRL = 0x00
  if(!writeRegister(deviceAddr, 0x59, 0x00))
    return false;

  const uint16_t size = sizeof(bmi270_config_file);

  // Burst Writing the config data.
  for(int i = 0; i < size; i+=32)
  { 
    uint16_t configAddress = i / 2; // This is because the BMI 270 needs the data written in a 16 bit format.

    uint16_t chunkRemaining = size - i;
    uint8_t length = (chunkRemaining > 32) ? 32: chunkRemaining; 
    
    if(!setConfigAddress(deviceAddr, configAddress))
      return false;

    if(!writeRegisters(deviceAddr, 0x5E, &bmi270_config_file[i], length))
      return false;

  }

  // Final write
  if(!writeRegister(deviceAddr, 0x59, 0x01))
    return false;

  // Waiting for 20 ms
  delay(20);

  uint8_t status;
  if(!readRegister(deviceAddr, 0x21, status))
    return false;

  return ((status & 0x0F) == 0x01); /* We did status & 0x0F because the BMI270's INTERNAL_STATUS register contains a
                                      message field in its lower four bits. We're interested in whether that field
                                      reports successful initialization (0x01). */
}

bool getAccelRangeBits(AccelRANGE range, uint8_t &bits)
{
  switch(range)
  {
    case AccelRANGE::RANGE_2G:
      bits = 0x00;
      break;

    case AccelRANGE::RANGE_4G: 
      bits = 0x01;
    break;

    case AccelRANGE::RANGE_8G: 
      bits = 0x02;
    break;

    case AccelRANGE::RANGE_16G: 
      bits = 0x03;
    break;

      default: return false;
  }
  return true;
}



bool getAccelODRBits(AccelODR odr, uint8_t &bits)
{
  switch(odr)
  {
    case AccelODR::ODR_25:
      bits = 0x06;
    break;

    case AccelODR::ODR_50:
      bits = 0x07;
    break;

    case AccelODR::ODR_100:
      bits = 0x08;
    break;

    case AccelODR::ODR_200:
      bits = 0x09;
    break;
    case AccelODR::ODR_400:
      bits = 0x0a;
    break;

    case AccelODR::ODR_800:
      bits = 0x0b;
    break;

    case AccelODR::ODR_1600:
      bits = 0x0c;
    break;

    default: return false;
  }
  return true;
}



bool getAccelBandwidthBits(AccelBandwidth bandwidth, uint8_t &bits)
{
  switch(bandwidth)
  {
    case AccelBandwidth::OSR4:
      bits = 0x00;
    break;

    case AccelBandwidth::OSR2:
      bits = 0x01;
    break;
    
    case AccelBandwidth::NORMAL:
      bits = 0x02;
    break;
    
    case AccelBandwidth::CIC_MODE:
      bits = 0x03;
    break;
    
    case AccelBandwidth::RES_AVG16:
      bits = 0x04;
    break;
    
    case AccelBandwidth::RES_AVG32:
      bits = 0x05;
    break;
    
    case AccelBandwidth::RES_AVG64:
      bits = 0x06;
    break;
    
    case AccelBandwidth::RES_AVG128:
      bits = 0x07;
    break;
    
    default: return false;
  }
    return true;
}



bool configureAccelerometer(uint8_t deviceAddr, AccelODR odr, AccelRANGE range, AccelBandwidth bandwidth,
                            AccelPerformanceFilter perfFilter)
{
  constexpr uint8_t REG_ACC_CONF = 0x40;
  constexpr uint8_t REG_ACC_RANGE = 0x41;

  uint8_t range_bits = 0; 
  uint8_t odr_bits = 0;
  uint8_t bandwidth_bits = 0;
  uint8_t perfFilter_bits = 0;

  if(!getAccelRangeBits(range, range_bits))
  {
    return false;
  }

  if(!getAccelODRBits(odr, odr_bits))
  {
    return false;
  }

  if(!getAccelBandwidthBits(bandwidth, bandwidth_bits))
  {
    return false;
  }

  perfFilter_bits = static_cast <uint8_t> (perfFilter);

  uint8_t accConfigValue =  (odr_bits) | (bandwidth_bits << 4) | (perfFilter_bits << 7);

  if(!writeRegister(deviceAddr, REG_ACC_CONF, accConfigValue))
  {
    return false;
  }

  if(!writeRegister(deviceAddr, REG_ACC_RANGE, range_bits))
  {
    return false;
  }

  return true;
}


bool getGyroRangeBits(GyroRange range, uint8_t &bits)
{
    switch(range)
    {
      case GyroRange::RANGE_125_DPS:
        bits = 0x04;
        break;

      case GyroRange::RANGE_250_DPS:
        bits = 0x03;
        break;

      case GyroRange::RANGE_500_DPS:
        bits = 0x02;
        break;

      case GyroRange::RANGE_1000_DPS:
        bits = 0x01;
        break;

      case GyroRange::RANGE_2000_DPS:
        bits = 0x00;
        break;

      default: return false;
    }
  return true;
}

bool getGyroOISRangeBits(GyroOISRange oisRange, uint8_t &bits)
{
  switch (oisRange)
  {
    case GyroOISRange::RANGE_250_DPS:
      bits = 0x00;
      break;

    case GyroOISRange::RANGE_2000_DPS:
      bits = 0x01;
      break;

    default: return false;
  }
  return true;
}


bool getGyroODRBits(GyroODR odr, uint8_t &bits)
{
  switch (odr)
  {
    case GyroODR::ODR_25:
      bits = 0x06;
      break;

    case GyroODR::ODR_50:
      bits = 0x07;
      break;

    case GyroODR::ODR_100:
      bits = 0x08;
      break;

    case GyroODR::ODR_200:
      bits = 0x09;
      break;

    case GyroODR::ODR_400:
      bits = 0x0a;
      break;

    case GyroODR::ODR_800:
      bits = 0x0b;
      break;

    case GyroODR::ODR_1600:
      bits = 0x0c;
      break;

    case GyroODR::ODR_3200:
      bits = 0x0d;
      break;

    default: return false;
  }

  return true;
}


bool getGyroBandwidthBits(GyroBandwidth bandwidth, uint8_t &bits)
{
  switch (bandwidth)
  {
    case GyroBandwidth::OSR2:
      bits = 0x01;
      break;

    case GyroBandwidth::OSR4:
      bits = 0x00;
      break;

    case GyroBandwidth::NORMAL:
      bits = 0x02;
      break;

    default: return false;
  }

  return true;
}

bool configureGyroscope(uint8_t deviceAddr, GyroODR odr, GyroBandwidth bandwidth, GyroFilterPerformance filterPerf, GyroNoisePerformance noisePerf, GyroOISRange oisRange, GyroRange range)
{
  constexpr uint8_t REG_GYR_CONF = 0x42;
  constexpr uint8_t REG_GYR_RANGE = 0x43;

  uint8_t odr_bits = 0;
  uint8_t bandwidth_bits = 0;
  uint8_t oisRange_bits = 0;
  uint8_t range_bits = 0;
  uint8_t noisePerf_bits = 0;
  uint8_t filterPerf_bits = 0;

  if (!getGyroODRBits(odr, odr_bits))
  {
    return false;
  }

  if (!getGyroBandwidthBits(bandwidth, bandwidth_bits))
  {
    return false;
  }

  if (!getGyroRangeBits(range, range_bits))
  {
    return false;
  }

  if (!getGyroOISRangeBits(oisRange, oisRange_bits))
  {
    return false;
  }

  noisePerf_bits = static_cast<uint8_t>(noisePerf);
  filterPerf_bits = static_cast<uint8_t>(filterPerf);

  uint8_t gyroConfigValue = (odr_bits) | (bandwidth_bits << 4) | (noisePerf_bits << 6) | (filterPerf_bits << 7);

  uint8_t gyroRangeConfigValue = (range_bits) | (oisRange_bits << 3);

  if (!writeRegister(deviceAddr, REG_GYR_CONF, gyroConfigValue))
  {
    return false;
  }

  if (!writeRegister(deviceAddr, REG_GYR_RANGE, gyroRangeConfigValue))
  {
    return false;
  }

  return true;
}

bool setSensorPowerState(uint8_t deviceAddr, SensorPowerState powerState, SensorType type)
{
  constexpr uint8_t REG_PWR_CTRL = 0x7D;

  uint8_t currentPowerState = 0;

  if (!readRegister(deviceAddr, REG_PWR_CTRL, currentPowerState))
  {
    return false;
  }

  uint8_t modifiedPowerState = 0;
  uint8_t mask = 0;
  uint8_t bitPosition = 0;

  switch (type)
  {
    case SensorType::ACCELEROMETER:
      bitPosition = 2;
      break;
    case SensorType::GYROSCOPE:
      bitPosition = 1;
      break;

    default: return false;
  }

  switch (powerState)
  {
    case SensorPowerState::DISABLE:
      mask = static_cast<uint8_t>(~(1U << bitPosition));
      modifiedPowerState = (currentPowerState) & mask;
      break;

    case SensorPowerState::ENABLE:
      mask = static_cast<uint8_t>((1U << bitPosition));
      modifiedPowerState = (currentPowerState) | mask;
      break;

    default: return false;
  }

  if (!writeRegister(deviceAddr, REG_PWR_CTRL, modifiedPowerState))
  {
    return false;
  }

  return true;
}


bool readAccelerometerData(uint8_t deviceAddr, int16_t &xData, int16_t &yData, int16_t &zData)
{
  constexpr uint8_t REG_ACC_DATA_START = 0x0C;
  uint8_t buffer[6];

  if (!readRegisters(deviceAddr, REG_ACC_DATA_START, buffer, 6))
  {
    return false;
  }

  xData = static_cast<int16_t>((buffer[1] << 8) | buffer[0]);
  yData = static_cast<int16_t>((buffer[3] << 8) | buffer[2]);
  zData = static_cast<int16_t>((buffer[5] << 8) | buffer[4]);

  return true;

}

bool readGyroscopeData(uint8_t deviceAddr, int16_t &xData, int16_t &yData, int16_t &zData)
{
  constexpr uint8_t REG_GYR_DATA_START = 0x12;
  uint8_t buffer[6];

  if (!readRegisters(deviceAddr, REG_GYR_DATA_START, buffer, 6))
  {
    return false;
  }

  xData = static_cast<int16_t>((buffer[1] << 8) | buffer[0]);
  yData = static_cast<int16_t>((buffer[3] << 8) | buffer[2]);
  zData = static_cast<int16_t>((buffer[5] << 8) | buffer[4]);

  return true;

}

float getAccSensitivity(AccelRANGE range)
{
  switch (range)
  {
    case AccelRANGE::RANGE_2G: return 16384.0f;
    case AccelRANGE::RANGE_4G: return 8192.0f;
    case AccelRANGE::RANGE_8G: return 4096.0f;
    case AccelRANGE::RANGE_16G: return 2048.0f;
    default: return 0.0f;
  }
}


float getGyroSensitivity(GyroRange range)
{
  switch (range)
  {
    case GyroRange::RANGE_125_DPS: return 262.144f;
    case GyroRange::RANGE_250_DPS: return 131.072f;
    case GyroRange::RANGE_500_DPS: return 65.536f;
    case GyroRange::RANGE_1000_DPS: return 32.768f;
    case GyroRange::RANGE_2000_DPS: return 16.384f;
    default: return 0.0f;
  }
}

bool scaleAccelerometerData(
  float &scaledX, float &scaledY, float &scaledZ,
  int16_t rawX, int16_t rawY, int16_t rawZ, AccelRANGE range
  )
{
  float sensitivity = getAccSensitivity(range);

  if (sensitivity == 0.0f)
  {
    return false;
  }

  scaledX = rawX/sensitivity;
  scaledY = rawY/sensitivity;
  scaledZ = rawZ/sensitivity;

  return true;
}


bool scaleGyroscopeData(
  float &scaledX, float &scaledY, float &scaledZ,
  int16_t rawX, int16_t rawY, int16_t rawZ, GyroRange range
  )
{
  float sensitivity = getGyroSensitivity(range);

  if (sensitivity == 0.0f)
  {
    return false;
  }

  scaledX = rawX/sensitivity;
  scaledY = rawY/sensitivity;
  scaledZ = rawZ/sensitivity;

  return true;
}