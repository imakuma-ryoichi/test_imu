#pragma once

#include <cstdint>
#include <iostream>

enum class BNO055Mode : uint8_t {
  //ここに必要なレジスタ番号を記入
  CONFIG = 0X00,
  NDOF = 0x0C,
};

enum class BNO055Reg : uint8_t {

  CHIP_ID = 0x00,
  OPR_MODE = 0x3D,
};

constexpr uint8_t EXPECT_CHIP_ID = 0xA0;
