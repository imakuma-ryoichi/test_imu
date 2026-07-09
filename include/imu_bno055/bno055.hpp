#pragma once

#include <cstdint>
#include <array>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

#include "imu_bno055/bno055_reg.hpp"

// 後で実装はcppに書く

class BNO055 
{
public:
  BNO055(std::string dev, uint8_t addr) 
  : dev_(dev), addr_(addr), fd_(-1)
  {}

  bool init() 
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
  

  std::array<float,3> readEuler() {//未実装
    std::array<float, 3> euler{0.0f, 0.0f, 0.0f};//コンパイル通すために一時的においている

    return euler;
  }

  
//build通ったらテンプレートにする
  constexpr uint8_t toUint8(BNO055Mode mode)
  {
    return static_cast<uint8_t>(mode);
  }


  constexpr uint8_t toUint8(BNO055Reg reg)
  {
    return static_cast<uint8_t>(reg);
  }

  constexpr uint8_t toUint8(BNO055Unit unit) 
  {
    return static_cast<uint8_t>(unit);
  }

  bool expectChipID()
  {
    return readReg(toUint8(BNO055Reg::CHIP_ID)) == EXPECT_CHIP_ID;
  }// 後でデータシート見る
 /* 
  void fetchData(int parameter) {
    
    int ret;

    ret = writeReg()
  }
*/

  std::array<float, 2> readAcceleration();
  std::array<float, 3> readGyroscope();
  std::array<float, 4> readQuaternion();

private:

  bool writeReg(uint8_t reg, uint8_t value) 
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
 
  uint8_t readReg(uint8_t reg)
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
  

  std::string dev_;

  uint8_t addr_;

  int fd_;

  bool setOprMode(BNO055Mode mode)
  {
    return writeReg(
        toUint8(BNO055Reg::OPR_MODE),
        static_cast<uint8_t>(mode)
    );
  }

  int16_t readInt16(BNO055Reg lsb_leg);

  bool expectSetUnit();//そのうちinitに追加

  bool setUnit();
  
  uint8_t toUnit();
};








