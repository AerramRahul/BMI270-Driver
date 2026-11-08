

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
  uint8_t bytesRecieved = Wire.requestFrom(deviceadd, 1); // Returns number of bytes that have been put in the memory buffer.

  if(Wire.available() >= 1 && bytesRecieved == 1)
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

  uint8_t bytesRecieved = Wire.requestFrom(deviceadd, length);

  if(bytesRecieved == length && Wire.available() >= length)
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
  uint8_t lowbyte = address & 0x0F; // Using a bit mask, 0x0F is 0000 0000 1111, 1's retain the bit and 0's erase the bits, low byte is 4 bits
  uint8_t highbyte = (address >> 4) & 0xFF; // Using a bit mask, 0x0F is 0000 1111 1111, we first push off the four bits we read by shifting
                                            // 1's retain the bit and 0's erase the bits, high byte is 8 bits.

  if(!writeRegister(deviceadd, 0x5B, lowbyte))
  {
    return false;
  }

  if(!writeRegister(deviceadd, 0x5C, highbyte))
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
    uint16_t configAddress = i / 2; // This is beacuse the BMI 270 needs the data written in a 16 bit format.

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

  return ((status & 0x0F) == 0x01); // We did status & 0x0F because the BMI270's INTERNAL_STATUS register contains a message field in its lower four bits.
                                    // We're interested in whether that field reports successful initialization (0x01).
}

bool getRangeBits(AcelRANGE range, uint8_t &bits)
{
  switch(range)
  {
    case AcelRANGE::RANGE_2G: 
      bits = 0x00;
      break;

    case AcelRANGE::RANGE_4G: 
      bits = 0x01;
    break;

    case AcelRANGE::RANGE_8G: 
      bits = 0x02;
    break;

    case AcelRANGE::RANGE_16G: 
      bits = 0x03;
    break;

      default: return false;
  }
  return true;
}



bool getODRBits(AcelODR odr, uint8_t &bits)
{
  switch(odr)
  {
    case AcelODR::ODR_25:
      bits = 0x06;
    break;

    case AcelODR::ODR_50:
      bits = 0x07;
    break;

    case AcelODR::ODR_100:
      bits = 0x08;
    break;

    case AcelODR::ODR_200:
      bits = 0x09;
    break;
    case AcelODR::ODR_400:
      bits = 0x0a;
    break;

    case AcelODR::ODR_800:
      bits = 0x0b;
    break;

    case AcelODR::ODR_1600:
      bits = 0x0c;
    break;

    default: return false;
  }
  return true;
}



bool getBandwithBits(AcelBANDWITH bandwith, uint8_t &bits)
{
  switch(bandwith)
  {
    case AcelBANDWITH::OSR4:
      bits = 0x00;
    break;

    case AcelBANDWITH::OSR2:
      bits = 0x01;
    break;
    
    case AcelBANDWITH::NORMAL:
      bits = 0x02;
    break;
    
    case AcelBANDWITH::CIC_MODE:
      bits = 0x03;
    break;
    
    case AcelBANDWITH::RES_AVG16:
      bits = 0x04;
    break;
    
    case AcelBANDWITH::RES_AVG32:
      bits = 0x05;
    break;
    
    case AcelBANDWITH::RES_AVG64:
      bits = 0x06;
    break;
    
    case AcelBANDWITH::RES_AVG128:
      bits = 0x07;
    break;
    
    default: return false;
  }
    return true;
}



bool configureAccelerometer(uint8_t deviceadd, AcelODR odr, AcelRANGE range, AcelBANDWITH bandwith, performanceFilter perfFilter)
{
  
  

}