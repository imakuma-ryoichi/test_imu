#include "imu_bno055/bno055.hpp"
#include "imu_bno055/bno055_config.hpp"

#include <iostream>


int main()
{
    const std::string config_path = "config.yaml";

    BNO055 imu("/dev/i2c-1", 0x28);


    if (!imu.init()) {
        std::cerr << "IMU init failed\n";
        return 1;
    }


    std::cout << "IMU initialized\n";


    if (!imu.isCalib()) {
        std::cerr << "Calibration is not complete\n";
        return 1;
    }


    CalibrationData calib_data;


    // IMUからキャリブレーション値取得
    if (!imu.readCalibration(calib_data)) {
        std::cerr << "Failed to read calibration\n";
        return 1;
    }


    std::cout << "Calibration read successfully\n";


    // YAMLへ保存
    if (!saveConfig(config_path, calib_data)) {
        std::cerr << "Failed to save config\n";
        return 1;
    }


    std::cout << "Calibration saved\n";


    // YAMLから読み込み
    BNO055Config config = loadConfig(config_path);


    bool imu_ready;


    // IMUへ復元
    if (!imu.loadCalibration(config.calibration, imu_ready)) {
        std::cerr << "Failed to load calibration\n";
        return 1;
    }


    if (!imu_ready) {
        std::cerr << "IMU is not ready\n";
        return 1;
    }


    // 読み書き確認
    CalibrationData verify_data;

    if (!imu.readCalibration(verify_data)) {
        std::cerr << "Failed to verify calibration\n";
        return 1;
    }


    if (verify_data == config.calibration) {
        std::cout << "Calibration verify success\n";
    }
    else {
        std::cerr << "Calibration verify failed\n";
        return 1;
    }


    return 0;
}
