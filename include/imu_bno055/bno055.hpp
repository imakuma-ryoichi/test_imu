#pragma once

#include <cstdint>
#include <array>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#include "imu_bno055/bno055_reg.hpp"

// 後で実装はcppに書く

class BNO055 
{
public:
  BNO055(std::string dev, uint8_t addr);


bool init(); 

std::array<float,3> readEuler(); 
//未実装

template <typename T>
constexpr uint8_t toUint8(T value)
{
  return static_cast<uint8_t>(value);
}
  
  std::array<float, 2> readAcceleration();
  std::array<float, 3> readGyroscope();
  std::array<float, 4> readQuaternion();

private:

  bool writeReg(uint8_t reg, uint8_t value);

  uint8_t readReg(uint8_t reg);
 
  std::string dev_;

  uint8_t addr_;

  int fd_;

  bool setOprMode(BNO055Mode mode);
  
  int16_t readInt16(BNO055Reg lsb_leg);

  bool expectSetUnit();//そのうちinitに追加

  bool setUnit();
  
  uint8_t toUnit();
  
  bool expectChipID();
};








