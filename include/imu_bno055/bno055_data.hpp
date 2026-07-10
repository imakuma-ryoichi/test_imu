#pragma once

#include <cstdint>

struct CalibrationData {
    int16_t acc_offset[3];
    int16_t gyr_offset[3];
    int16_t mag_offset[3];

    int16_t acc_radius;
    int16_t mag_radius;
};
