#pragma once

namespace BNO055Scale
{
    // Acceleration
    constexpr float ACC_LSB_TO_MPS2 = 1.0f / 100.0f; // 1 m/s² = 100 LSB
    constexpr float ACC_LSB_TO_MG   = 1.0f;          // 1 mg = 1 LSB

    // Gyroscope
    constexpr float GYR_LSB_TO_DPS  = 1.0f / 16.0f;  // 1 dps = 16 LSB
    constexpr float GYR_LSB_TO_RPS  = 1.0f / 900.0f; // 1 rad/s = 900 LSB

    // Euler angle
    constexpr float EUL_LSB_TO_DEG  = 1.0f / 16.0f;  // 1 deg = 16 LSB
    constexpr float EUL_LSB_TO_RAD  = 1.0f / 900.0f; // 1 rad = 900 LSB

    // Magnetometer
    constexpr float MAG_LSB_TO_UT   = 1.0f / 16.0f;  // 1 uT = 16 LSB
}
