#pragma once

#include <cstdint>
#include <iostream>

enum class BNO055Mode : uint8_t {
  //ここに必要なレジスタ番号を記入
  CHIP_ID = 0xA0,
  OPR_MODE = 0x3D,
  NDOF = 0x0C,
};

constexpr uint8_t EXPECT_CHIP_ID = 0xA0;
constexpr uint8_t OPR_MODE = 0x00;
