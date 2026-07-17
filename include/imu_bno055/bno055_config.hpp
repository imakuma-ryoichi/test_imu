#pragma once

#include "imu_bno055/bno055_data.hpp"

BNO055Config loadConfig(const std::string& path);

BNO055Mode parseMode(const std::string& value);
