#include "imu_bno055/bno055.hpp"
#include "imu_bno055/bno055_config.hpp"

#include <chrono>
#include <iostream>
#include <thread>

int main()
{
    BNO055 imu("/dev/i2c-5", 0x28); // 使用しているI2Cバス番号に合わせる

    if (!imu.init()) {
        std::cerr << "IMU init failed\n";
        return 1;
    }

    std::cout << "IMU initialized\n";

    IMUData imu_data;

    while (true) {

        if (!imu.readIMUData(imu_data)) {
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
                << imu_data.quaternion[3] << '\n';

            std::cout
                << "Euler: "
                << imu_data.euler[0] << ", "
                << imu_data.euler[1] << ", "
                << imu_data.euler[2] << "\n\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
