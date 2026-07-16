#pragma once

#include <cstdint>
#include <array>

struct CalibrationData {

  std::array<int16_t, 3> accOffset;
  std::array<int16_t, 3> gyrOffset;
  std::array<int16_t, 3> magOffset;

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

struct IMUData {
  std::array<float, 3> acceleration;
  std::array<float, 3> gyroscope;
  std::array<float, 4> quaternion;
  std::array<float, 3> euler;
  bool imu_available;//IMUが正常に動作しているかどうかのフラグ
};

