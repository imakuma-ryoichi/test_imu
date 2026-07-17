#include "imu_bno055/bno055_data.hpp"
#include "imu_bno055/bno055_config.hpp"

#include <string>
#include <yaml-cpp/yaml.h>
#include <stdexcept>


BNO055Config loadConfig(const std::string& path)
{
    BNO055Config config;

    YAML::Node root = YAML::LoadFile(path);

    auto bno055 = root["bno055"];

    config.device = bno055["i2c"]["device"].as<std::string>();

    config.address = bno055["i2c"]["address"].as<uint8_t>();

    config.mode = parseMode(bno055["mode"].as<std::string>());    

    return config;
}

BNO055Mode parseMode(const std::string& value)
{
    if (value == "CONFIG") return BNO055Mode::CONFIG;

    if (value == "NDOF") return BNO055Mode::NDOF;

    throw std::runtime_error("Unknown mode");
}
