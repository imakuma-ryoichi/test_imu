#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include "imu_bno055/bno055.hpp"
#include "imu_bno055/bno055_reg.hpp"
#include <thread>
#include <chrono>
#include <array>
#include <cstdint>
#include <iostream>


  bool BNO055::setUnit() 
  {
    uint8_t unit = toUnit();

    return writeReg(toUint8(BNO055Reg::UNIT_SEL), unit);

  }

  bool BNO055::expectSetUnit()
  {
    uint8_t current = readReg(toUint8(BNO055Reg::UNIT_SEL));
    uint8_t expected = toUnit();

    uint8_t mask =
        ACC_UNIT_MASK |
        GYR_UNIT_MASK |
        EUL_UNIT_MASK;

    return ((current & mask) == (expected & mask));

  }

  uint8_t BNO055::toUnit()
  {
    return 
      toUint8(BNO055Unit::EUL_UNIT_RADIANS) |
      toUint8(BNO055Unit::GYR_UNIT_DPS) |
      toUint8(BNO055Unit::ACC_UNIT_METER_PER_SECOND_PER_SECOND);
  }


int16_t BNO055::readInt16(BNO055Reg lsb_reg)
{
  uint16_t rawValue;

  uint8_t lsb = toUint8(lsb_reg);
  uint8_t msb = lsb + 1;

  lsb = readReg(lsb);
  msb = readReg(msb);
  
//  value << msb | lsb;

  rawValue = (static_cast<uint16_t>(msb) << 8) | lsb; //ここがどうなるかやね
  return static_cast<int16_t>(rawValue);
  
}

std::array<float, 2> BNO055::readAcceleration() {
//平面走行でZ軸は不要のため いったん動くか見るのでこの設定
  std::array<float, 2> accValue;

  accValue[0] = readInt16(BNO055Reg::ACC_DATA_X_LSB);
  accValue[1] = readInt16(BNO055Reg::ACC_DATA_Y_LSB);

  return accValue;
}


