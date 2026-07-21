#include "imu_bno055/bno055.hpp"
#include "imu_bno055/bno055_config.hpp"
#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    BNO055 imu("/dev/i2c-1", 0x28);

    if (!imu.init()) {
        std::cerr << "IMU init failed\n";
        return 1;
    }

    std::cout << "IMU initialized\n";

    while (true) {
        IMUData imu_data;

        if (!imu.readData(imu_data)) {
            std::cerr << "Failed to read IMU data\n";
        } else {
            std::cout
                << "Acc : "
                << imu_data.acceleration[0] << ", "
                << imu_data.acceleration[1] << ", "
                << imu_data.acceleration[2] << '\n';

            std::cout
                << "Gyro: "
                << imu_data.gyroscope[0] << ", "
                << imu_data.gyroscope[1] << ", "
                << imu_data.gyroscope[2] << '\n';

            std::cout
                << "Quat: "
                << imu_data.quaternion[0] << ", "
                << imu_data.quaternion[1] << ", "
                << imu_data.quaternion[2] << ", "
                << imu_data.quaternion[3] << "\n\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(1)); // 1秒ごと
    }

    return 0;
}
