#include "imu_bno055/bno055_data.hpp"
#include "imu_bno055/bno055_config.hpp"
#include "imu_bno055/bno055_scale.hpp"

#include <string>
#include <yaml-cpp/yaml.h>
#include <stdexcept>
#include <fstream>


BNO055Config loadConfig(const std::string& path)
{
    BNO055Config config;

    YAML::Node root = YAML::LoadFile(path);

    auto bno055 = root["bno055"];

    config.device = bno055["i2c"]["device"].as<std::string>();

    config.address = bno055["i2c"]["address"].as<uint8_t>();

    config.mode = parseMode(bno055["mode"].as<std::string>());


    // Acceleration
    auto acc_unit = parseAccUnit(
        bno055["unit"]["acceleration"].as<std::string>()
    );

    config.unit.acceleration.unit = acc_unit;

    switch (acc_unit)
    {
        case BNO055Unit::ACC_UNIT_METER_PER_SECOND_PER_SECOND:
            config.unit.acceleration.scale = BNO055Scale::ACC_LSB_TO_MPS2;
            break;

        case BNO055Unit::ACC_UNIT_MG:
            config.unit.acceleration.scale = BNO055Scale::ACC_LSB_TO_MG;
            break;

        default:
            throw std::runtime_error("Unknown acceleration unit");
    }


    // Gyroscope
    auto gyro_unit = parseGyrUnit(
        bno055["unit"]["gyro"].as<std::string>()
    );

    config.unit.gyro.unit = gyro_unit;

    switch (gyro_unit)
    {
        case BNO055Unit::GYR_UNIT_DPS:
            config.unit.gyro.scale = BNO055Scale::GYR_LSB_TO_DPS;
            break;

        case BNO055Unit::GYR_UNIT_RPS:
            config.unit.gyro.scale = BNO055Scale::GYR_LSB_TO_RPS;
            break;

        default:
            throw std::runtime_error("Unknown gyro unit");
    }


    // Euler
    auto euler_unit = parseEulUnit(
        bno055["unit"]["euler"].as<std::string>()
    );

    config.unit.euler.unit = euler_unit;

    switch (euler_unit)
    {
        case BNO055Unit::EUL_UNIT_DEGREES:
            config.unit.euler.scale = BNO055Scale::EUL_LSB_TO_DEG;
            break;

        case BNO055Unit::EUL_UNIT_RADIANS:
            config.unit.euler.scale = BNO055Scale::EUL_LSB_TO_RAD;
            break;

        default:
            throw std::runtime_error("Unknown euler unit");
    }


    // Calibration
    config.calibration.acc_offset[0] =
        bno055["calibration"]["acc_offset"][0].as<int16_t>();

    config.calibration.acc_offset[1] =
        bno055["calibration"]["acc_offset"][1].as<int16_t>();

    config.calibration.acc_offset[2] =
        bno055["calibration"]["acc_offset"][2].as<int16_t>();


    config.calibration.gyr_offset[0] =
        bno055["calibration"]["gyr_offset"][0].as<int16_t>();

    config.calibration.gyr_offset[1] =
        bno055["calibration"]["gyr_offset"][1].as<int16_t>();

    config.calibration.gyr_offset[2] =
        bno055["calibration"]["gyr_offset"][2].as<int16_t>();


    config.calibration.mag_offset[0] =
        bno055["calibration"]["mag_offset"][0].as<int16_t>();

    config.calibration.mag_offset[1] =
        bno055["calibration"]["mag_offset"][1].as<int16_t>();

    config.calibration.mag_offset[2] =
        bno055["calibration"]["mag_offset"][2].as<int16_t>();


    config.calibration.acc_radius =
        bno055["calibration"]["acc_radius"].as<int16_t>();

    config.calibration.mag_radius =
        bno055["calibration"]["mag_radius"].as<int16_t>();


    return config;
}

BNO055Mode parseMode(const std::string& mode)
{
    if (mode == "NDOF") {
        return BNO055Mode::NDOF;
    }

    if (mode == "CONFIG") {
        return BNO055Mode::CONFIG;
    }

    throw std::runtime_error("Unknown BNO055 mode");
}


BNO055Unit parseAccUnit(const std::string& unit)
{
    if (unit == "MPS2") {
        return BNO055Unit::ACC_UNIT_METER_PER_SECOND_PER_SECOND;
    }

    if (unit == "MG") {
        return BNO055Unit::ACC_UNIT_MG;
    }

    throw std::runtime_error("Unknown acceleration unit");
}


BNO055Unit parseGyrUnit(const std::string& unit)
{
    if (unit == "DPS") {
        return BNO055Unit::GYR_UNIT_DPS;
    }

    if (unit == "RPS") {
        return BNO055Unit::GYR_UNIT_RPS;
    }

    throw std::runtime_error("Unknown gyro unit");
}


BNO055Unit parseEulUnit(const std::string& unit)
{
    if (unit == "DEG") {
        return BNO055Unit::EUL_UNIT_DEGREES;
    }

    if (unit == "RAD") {
        return BNO055Unit::EUL_UNIT_RADIANS;
    }

    throw std::runtime_error("Unknown euler unit");
}
