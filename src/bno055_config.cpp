#include "imu_bno055/bno055_data.hpp"
#include "imu_bno055/bno055_config.hpp"

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

    config.unit.acceleration = parseAccUnit(bno055["unit"]["acceleration"].as<std::string>());

    config.unit.gyro = parseGyrUnit(bno055["unit"]["gyro"].as<std::string>());

    config.unit.euler = parseEulUnit(bno055["unit"]["euler"].as<std::string>());

    config.calibration.acc_offset[0] = bno055["calibration"]["acc_offset"][0].as<int16_t>();
    config.calibration.acc_offset[1] = bno055["calibration"]["acc_offset"][1].as<int16_t>();
    config.calibration.acc_offset[2] = bno055["calibration"]["acc_offset"][2].as<int16_t>();

    config.calibration.gyr_offset[0] = bno055["calibration"]["gyr_offset"][0].as<int16_t>();
    config.calibration.gyr_offset[1] = bno055["calibration"]["gyr_offset"][1].as<int16_t>();
    config.calibration.gyr_offset[2] = bno055["calibration"]["gyr_offset"][2].as<int16_t>();

    config.calibration.mag_offset[0] = bno055["calibration"]["mag_offset"][0].as<int16_t>();
    config.calibration.mag_offset[1] = bno055["calibration"]["mag_offset"][1].as<int16_t>();
    config.calibration.mag_offset[2] = bno055["calibration"]["mag_offset"][2].as<int16_t>();

    config.calibration.acc_radius = bno055["calibration"]["acc_radius"].as<int16_t>();

    config.calibration.mag_radius = bno055["calibration"]["mag_radius"].as<int16_t>();   
    return config;
}

bool saveConfig(const std::string& path, const CalibrationData& calib_data)
{
    YAML::Node root = YAML::LoadFile(path);

    auto calibration = root["bno055"]["calibration"];

    calibration["acc_offset"][0] = calib_data.acc_offset[0];
    calibration["acc_offset"][1] = calib_data.acc_offset[1];
    calibration["acc_offset"][2] = calib_data.acc_offset[2];

    calibration["gyr_offset"][0] = calib_data.gyr_offset[0];
    calibration["gyr_offset"][1] = calib_data.gyr_offset[1];
    calibration["gyr_offset"][2] = calib_data.gyr_offset[2];

    calibration["mag_offset"][0] = calib_data.mag_offset[0];
    calibration["mag_offset"][1] = calib_data.mag_offset[1];
    calibration["mag_offset"][2] = calib_data.mag_offset[2];

    calibration["acc_radius"] = calib_data.acc_radius;
    calibration["mag_radius"] = calib_data.mag_radius;


    std::ofstream fout(path);

    if (!fout) return false;

    fout << root;

    return true;
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
