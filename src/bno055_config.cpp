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

    config.unit.acceleration = parseAccUnit(bno055["unit"]["acceleration"].as<std::string>());

    config.unit.gyro = parseGyrUnit(bno055["unit"]["gyro"].as<std::string>());

    config.unit.euler = parseEulUnit(bno055["unit"]["euler"].as<std::string>());
    
    return config;
}

BNO055Mode parseMode(const std::string& value)
{
    if (value == "CONFIG") return BNO055Mode::CONFIG;

    if (value == "NDOF") return BNO055Mode::NDOF;

    throw std::runtime_error("Unknown mode");
}

BNO055Unit parseAccUnit(const std::string& value)
{
    if (value == "m/s2") return BNO055Unit::ACC_UNIT_METER_PER_SECOND_PER_SECOND;
    if (value == "mg") return BNO055Unit::ACC_UNIT_MG;

    throw std::runtime_error("Unknown acceleration unit");
}


BNO055Unit parseGyrUnit(const std::string& value)
{
    if (value == "dps") return BNO055Unit::GYR_UNIT_DPS;
    if (value == "rps") return BNO055Unit::GYR_UNIT_RPS;

    throw std::runtime_error("Unknown gyro unit");
}


BNO055Unit parseEulUnit(const std::string& value)
{
    if (value == "degrees") return BNO055Unit::EUL_UNIT_DEGREES;
    if (value == "radians") return BNO055Unit::EUL_UNIT_RADIANS;

    throw std::runtime_error("Unknown euler unit");
}
