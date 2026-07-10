#pragma once

#include <cstdint>
#include <array>

struct CalibrationData {

  std::array<int16_t, 3> accOffset;
  std::array<int16_t, 3> gyrOffset;

//int16_t accRadius;
};
