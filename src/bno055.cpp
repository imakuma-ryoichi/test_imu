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

// コンパイル確認用 後で削除
int main()
{
    BNO055 imu("/dev/i2c-1", 0x28);

    return 0;
}


  bool BNO055::setUnit() 
  {
    uint8_t unit = toUnit();

    return writeReg(toUint8(BNO055Reg::UNIT_SEL), unit);

  }

  bool BNO055::expectSetUnit() 
  {
        toUint8(EUL_UNIT_MASK);

    return ((current & mask) == expected);

  }

  uint8_t BNO055::toUnit()
  {
    return (
      toUint8(BNO055Unit::EUL_UNIT_RADIANS) |
      toUint8(BNO055Unit::GYR_UNIT_DPS) |
      toUint8(BNO055Unit::ACC_UNIT_METER_PER_SECOND_PER_SECOND);
    )
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
//平面走行でZ軸は不要のため 
  std::array<float, 2> accValue;

  int16_t x = readInt16(BNO055Reg::ACC_DATA_X_LSB);
  int16_t y = readInt16(BNO055Reg::ACC_DATA_Y_LSB);


}

void BNO055::setUint() {
  
}

namespace {

constexpr uint8_t ACC_UNIT_MASK = 0x01;
constexpr uint8_t GYR_UNIT_MASK = 0x02;
constexpr uint8_t EUL_UNIT_MASK = 0x04;

}
