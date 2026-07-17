#include "imu_bno055/bno055_data.hpp"
#include <string>
#include <yaml-cpp/yaml.h>
#include <stdexcept>


BNO055Config loadConfig(const std::string& path)
{
    BNO055Config config;

    YAML::Node root = YAML::LoadFile(path);

    auto bno055 = root["bno055"];

    config.device =
        bno055["i2c"]["device"].as<std::string>();

    config.address =
        bno055["i2c"]["address"].as<uint8_t>();

    return config;
}
