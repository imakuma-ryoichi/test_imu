#pragma once

#include <cstdint>
#include <iostream>

constexpr uint8_t EXPECT_CHIP_ID = 0xA0;



enum class BNO055Mode : uint8_t {
  //ここに必要なレジスタ番号を記入
  CONFIG = 0X00,
  NDOF = 0x0C,
};

enum class BNO055Unit : uint8_t {

  //EUL_Unit
  EUL_UNIT_DEGREES = 0,
  EUL_UNIT_RADIANS = 1,

  //GYR_Unit
  GYR_UNIT_DPS = 0,
  GYR_UNIT_RPS = 1,

  //ACC_Unit
  ACC_UNIT_METER_PER_SECOND_PER_SECOND = 0,
  ACC_UNIT_MG = 1,
  
};



//acc, gyr, qua, eul
enum class BNO055Reg : uint8_t {

  CHIP_ID = 0x00,
  UNIT_SEL = 0X3B,
  OPR_MODE = 0x3D,

  //Acceleration data
  ACC_DATA_X_LSB = 0x08,
  ACC_DATA_X_MSB = 0x09,
  ACC_DATA_Y_LSB = 0x0A,
  ACC_DATA_Y_MSB = 0x0B,
  //ACC_DATA_Z_LSB = 0x0C,
  //ACC_DATA_Z_MSB = 0x0D,

  //Gyroscope data
  GYR_DATA_X_LSB = 0x14,
  GYR_DATA_X_MSB = 0x15,
  GYR_DATA_Y_LSB = 0x16,
  GYR_DATA_Y_MSB = 0x17,
  GYR_DATA_Z_LSB = 0x18,
  GYR_DATA_Z_MSB = 0x19,

  //quaternion data
  QUA_DATA_W_LSB = 0x20,
  QUA_DATA_W_MSB = 0x21,
  QUA_DATA_X_LSB = 0x22,
  QUA_DATA_X_MSB = 0x23,
  QUA_DATA_Y_LSB = 0x24,
  QUA_DATA_Y_MSB = 0x25,
  QUA_DATA_Z_LSB = 0x26,
  QUA_DATA_Z_MSB = 0x27,

  //euler data
  EUL_DATA_X_LSB = 0x1A,
  EUL_DATA_X_MSB = 0x1B,
  EUL_DATA_Y_LSB = 0x1C,
  EUL_DATA_Y_MSB = 0x1D,
  EUL_DATA_Z_LSB = 0x1E,
  EUL_DATA_Z_MSB = 0x1F,

  //Acceleration offset
  ACC_OFFSET_X_LSB = 0x55,
  ACC_OFFSET_X_MSB = 0x56,
  ACC_OFFSET_Y_LSB = 0x57,
  ACC_OFFSET_Y_MSB = 0x58,

  //gyroscope offset
  GYR_OFFSET_X_LSB = 0x61,
  GYR_OFFSET_X_MSB = 0x62,
  GYR_OFFSET_Y_LSB = 0x63,
  GYR_OFFSET_Y_MSB = 0x64,
  GYR_OFFSET_Z_LSB = 0x65,
  GYR_OFFSET_Z_MSB = 0x66,

};

//角速度、加速度

namespace {

constexpr uint8_t ACC_UNIT_MASK = 0x01;
constexpr uint8_t GYR_UNIT_MASK = 0x02;
constexpr uint8_t EUL_UNIT_MASK = 0x04;

}
