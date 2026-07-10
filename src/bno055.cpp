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

BNO055::BNO055(std::string dev, uint8_t addr) 
  : dev_(dev), addr_(addr), fd_(-1)
  {}

bool BNO055::init() 
  {
   fd_ = open(dev_.c_str(), O_RDWR);
    
  if (fd_ == -1) {
    perror("open failed");
    return false; 
  }
    
  int ret = ioctl(fd_, I2C_SLAVE, addr_);

  if (ret == -1) {
    perror("ioctl failed");
    close(fd_);
    return false;
  }


  if (!expectChipID()) {
    std::cerr << "CHIP_ID mismatch" << std::endl;
    close(fd_);
    return false;
  }

  if(!setUnit()) {
    std::cerr << "SET UNIT failed" << std::endl;
    close(fd_);
    return false;
  }


  if (!setOprMode(BNO055Mode::NDOF)) {//センサーをNDOFmodeにする
    std::cerr << "OPR_MODE failed" << std::endl;
    close(fd_);
    return false;
  }
  
  std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
  
  return true;

  }
  

std::array<float,3> BNO055::readEuler() {//未実装
    std::array<float, 3> euler{0.0f, 0.0f, 0.0f};//コンパイル通すために一時的においている

    return euler;
  }

uint8_t BNO055::readReg(uint8_t reg)
  {
    uint8_t value = 0;
    ssize_t readRet, writeRet;


    writeRet = write(fd_, &reg, 1);//1byte送っている
  
    if (writeRet != 1) {
      perror("not reg write error");
      return 0;
    }

    readRet = read(fd_, &value, 1);

    if (readRet != 1) {
      perror("not reg read error");
      return 0;
    }

    return value;
    
  }
  

 bool BNO055::writeReg(uint8_t reg, uint8_t value) 
  {
    ssize_t ret;

    std::array<uint8_t, 2> buf;

    buf[0] = reg;
    buf[1] = value;

    ret = write (fd_, buf.data(), 2);

    if (ret != 2) {
      perror("writeReg failed");
      return false;
    }
  
  return true; 

  }


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
      toUint8(BNO055Unit::GYR_UNIT_RPS) |
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

  rawValue = (static_cast<uint16_t>(msb) << 8) | lsb; 
  return static_cast<int16_t>(rawValue);
  
}


// m/s^2 == /100.0f , mg == /1.0f  
std::array<float, 2> BNO055::readAcceleration() {

  constexpr float ACC_SCALE = 1.0f / 100.0f;

  std::array<float, 2> accValue;

  accValue[0] = static_cast<float>(readInt16(BNO055Reg::ACC_DATA_X_LSB)) * ACC_SCALE;
  accValue[1] = static_cast<float>(readInt16(BNO055Reg::ACC_DATA_Y_LSB)) * ACC_SCALE;

  return accValue;
}

//Dps == /16.0f, Rps == /900.0f
std::array<float, 3> BNO055::readGyroscope() 
{
  constexpr float GYR_SCALE = 1.0f / 900.0f;

  std::array<float, 3> gyrValue;

  gyrValue[0] = static_cast<float>(readInt16(BNO055Reg::GYR_DATA_X_LSB)) * GYR_SCALE;
  gyrValue[1] = static_cast<float>(readInt16(BNO055Reg::GYR_DATA_Y_LSB)) * GYR_SCALE;
  gyrValue[2] = static_cast<float>(readInt16(BNO055Reg::GYR_DATA_Z_LSB)) * GYR_SCALE;

  return gyrValue;
}

// Qua /2^14
std::array<float, 4> BNO055::readQuaternion()
{
  constexpr float QUAT_SCALE = 1.0f / static_cast<float>(1 << 14);

  std::array<float, 4> quatValue;

  quatValue[0] = static_cast<float>(readInt16(BNO055Reg::QUA_DATA_W_LSB)) * QUAT_SCALE;
  quatValue[1] = static_cast<float>(readInt16(BNO055Reg::QUA_DATA_X_LSB)) * QUAT_SCALE;
  quatValue[2] = static_cast<float>(readInt16(BNO055Reg::QUA_DATA_Y_LSB)) * QUAT_SCALE;
  quatValue[3] = static_cast<float>(readInt16(BNO055Reg::QUA_DATA_Z_LSB)) * QUAT_SCALE;

  return quatValue;
}


bool BNO055::expectChipID()
  {
    return readReg(toUint8(BNO055Reg::CHIP_ID)) == EXPECT_CHIP_ID;
  }


bool BNO055::setOprMode(BNO055Mode mode)
  {
    return writeReg(
        toUint8(BNO055Reg::OPR_MODE),
        static_cast<uint8_t>(mode)
    );
  }












