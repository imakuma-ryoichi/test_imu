
#include "imu_bno055/bno055.hpp"

#include <iostream>

int main()
{
    BNO055 imu("/dev/i2c-1", 0x28);

    std::cout << "init start" << std::endl;

    if (!imu.init()) {
        std::cerr << "BNO055 init failed" << std::endl;
        return 1;
    }

    std::cout << "init success" << std::endl;

    auto acc = imu.readAcceleration();

    std::cout << "acc x = "
              << acc[0]
              << " m/s^2"
              << std::endl;

    std::cout << "acc y = "
              << acc[1]
              << " m/s^2"
              << std::endl;

    return 0;
}
