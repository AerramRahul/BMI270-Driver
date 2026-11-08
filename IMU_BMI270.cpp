

#include "IMU_BMI270.h"
#include "BMI270_config.h"


bool readRegister(uint8_t deviceadd, uint8_t regadd, uint8_t &data)
{
  Wire.beginTransmission(deviceadd);

  // Setting the internal memory pointer.
  Wire.write(regadd);

  // Repeated Start.
  uint8_t error = Wire.endTransmission(false);
  if(error != 0)
  {
    return false; // Communication failed.
  }
  uint8_t bytesReceived = Wire.requestFrom(deviceadd, 1); // Returns number of bytes in the memory buffer.

  if(Wire.available() >= 1 && bytesReceived == 1)
  {
    data = Wire.read();
    return true;
  }
  else
  return false;
  
}



bool writeRegister(uint8_t deviceadd, uint8_t regadd, uint8_t data)
{
  Wire.beginTransmission(deviceadd);

  Wire.write(regadd);

  Wire.write(data);

  uint8_t error = Wire.endTransmission();

  return (error == 0);
}



bool readRegisters(uint8_t deviceadd, uint8_t regadd, uint8_t *data, uint8_t length)
{

  Wire.beginTransmission(deviceadd);

  Wire.write(regadd); // Setting internal memory pointer.

  uint8_t error = Wire.endTransmission(false); // Repeated Start

  if(error != 0)
  {
    return false;
  }

  uint8_t bytesReceived = Wire.requestFrom(deviceadd, length);

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



bool writeRegisters(uint8_t deviceadd, uint8_t regadd, const uint8_t *data, uint8_t length)
{

  Wire.beginTransmission(deviceadd);

  Wire.write(regadd);

  size_t bytesWritten = Wire.write(data, length);

  uint8_t error = Wire.endTransmission();
  
  return (error == 0 && bytesWritten == length);
}



bool setConfigAddress(uint8_t deviceadd, uint16_t address)
{
  uint8_t lowNibble = address & 0x0F; /* Using a bit mask, 0x0F is 0000 0000 1111, 1's retain the bit and 0's erase
                                         the bits, low byte is 4 bits */

  uint8_t highByte = (address >> 4) & 0xFF; /* Using a bit mask, 0x0F is 0000 1111 1111, we first push off the
                                                 four bits we read by shifting 1's retain the bit and 0's erase the
                                                 bits, high byte is 8 bits. */

  if(!writeRegister(deviceadd, 0x5B, lowNibble))
  {
    return false;
  }

  if(!writeRegister(deviceadd, 0x5C, highByte))
  {
    return false;
  }

  return true;

}



bool initializeBMI270(uint8_t deviceadd)
{
  uint8_t chipID;

  if(!readRegister(deviceadd, 0x00, chipID))
    return false;

  if(chipID != 0x24)
    return false;

  // Writing to disable PWR_CONF.adv_power_save
  if(!writeRegister(deviceadd, 0x7C, 0x00))
    return false;

  // Waiting for 450 microseconds;
  delayMicroseconds(450);

  // Preparing config load INIT_CTRL = 0x00
  if(!writeRegister(deviceadd, 0x59, 0x00))
    return false;

  const uint16_t size = sizeof(bmi270_config_file);

  // Burst Writing the config data.
  for(int i = 0; i < size; i+=32)
  { 
    uint16_t configAddress = i / 2; // This is because the BMI 270 needs the data written in a 16 bit format.

    uint16_t chunkRemaining = size - i;
    uint8_t length = (chunkRemaining > 32) ? 32: chunkRemaining; 
    
    if(!setConfigAddress(deviceadd, configAddress))
      return false;

    if(!writeRegisters(deviceadd, 0x5E, &bmi270_config_file[i], length))
      return false;

  }

  // Final write
  if(!writeRegister(deviceadd, 0x59, 0x01))
    return false;

  // Waiting for 20 ms
  delay(20);

  uint8_t status;
  if(!readRegister(deviceadd, 0x21, status))
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



bool configureAccelerometer(uint8_t deviceadd, AccelODR odr, AccelRANGE range, AccelBandwidth bandwidth,
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

  if(!writeRegister(deviceadd, REG_ACC_CONF, accConfigValue))
  {
    return false;
  }

  if(!writeRegister(deviceadd, REG_ACC_RANGE, range_bits))
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

bool configureGyroscope(uint8_t deviceadd, GyroODR odr, GyroBandwidth bandwidth, GyroFilterPerformance filterPerf, GyroNoisePerformance noisePerf, GyroOISRange oisRange, GyroRange range)
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

  if (!writeRegister(deviceadd, REG_GYR_CONF, gyroConfigValue))
  {
    return false;
  }

  if (!writeRegister(deviceadd, REG_GYR_RANGE, gyroRangeConfigValue))
  {
    return false;
  }

  return true;
}