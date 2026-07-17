#pragma once

#include <cstdint>
#include <array>

struct CalibrationData {

  std::array<int16_t, 3> acc_offset;
  std::array<int16_t, 3> gyr_offset;
  std::array<int16_t, 3> mag_offset;

  int16_t acc_radius;
  int16_t mag_radius;

  bool operator==(const CalibrationData& other) const
{
    return acc_offset == other.acc_offset &&
           gyr_offset == other.gyr_offset &&
           mag_offset == other.mag_offset &&
           acc_radius == other.acc_radius &&
           mag_radius == other.mag_radius;
}
//そもそもしっかり書き込めているのかとか関数作る必要性があるのか
};

struct IMUData {
  std::array<float, 3> acceleration;
  std::array<float, 3> gyroscope;
  std::array<float, 4> quaternion;
  std::array<float, 3> euler;
  bool imu_available = true;//IMUが正常に動作しているかどうかのフラグ(e.g.modeが元に戻らなかった)
  bool is_valid = true;//今回の値が全てちゃんと取れているかのフラグ
};

struct CalibrationStatus {//詳細な情報がほしいなら使って
    uint8_t sys;
    uint8_t gyr;
    uint8_t acc;
    uint8_t mag;
};
