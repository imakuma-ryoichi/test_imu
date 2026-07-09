#include "imu_bno055/bno055.hpp"

int main()
{
    BNO055 imu;

    if (!imu.init()) {
        return 1;
    }

    auto acc = imu.readAccelerationX();

std::cout << "acc x = "
          << acc
          << " m/s^2"
          << std::endl;

    return 0;
}
