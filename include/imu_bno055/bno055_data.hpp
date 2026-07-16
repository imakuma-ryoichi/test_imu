#pragma once

#include <cstdint>
#include <array>

struct CalibrationData {

  std::array<int16_t, 3> accOffset;
  std::array<int16_t, 3> gyrOffset;
  std::array<int16_t, 3> magOffset;

//int16_t accRadius;
  int16_t accRadius;
  int16_t magRadius;

  bool operator==(const CalibrationData& other) const
{
    return accOffset == other.accOffset &&
           gyrOffset == other.gyrOffset &&
           magOffset == other.magOffset &&
           accRadius == other.accRadius &&
           magRadius == other.magRadius;
}
};

struct CalibrationStatus {
  uint8_t sys;
  uint8_t gyr;
  uint8_t acc;
  uint8_t mag;
};
