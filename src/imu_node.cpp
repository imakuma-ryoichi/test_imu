#include "imu_bno055/bno055.hpp"

#include <array>
#include <iostream>

int main()
{
    BNO055 imu("/dev/i2c-1", 0x28);
  //これもymlに追い出す

    std::cout << "init start" << '\n';

    if (!imu.init()) {
        std::cerr << "BNO055 init failed" << '\n';
        return 1;
    }

    std::cout << "init success" << '\n';

  std::array<float, 3> acc{};
  std::array<float, 3> gyr{};
  std::array<float, 4> quat{};

  if (imu.readAcceleration(acc)) {
    std::cout << "acc: "
              << acc[0] << ", "
              << acc[1] << ", "
              << acc[2] << '\n';
  }

  if (imu.readGyroscope(gyr)) {
    std::cout << "gyro: "
              << gyr[0] << ", "
              << gyr[1] << ", "
              << gyr[2] << '\n';
  }


  if (imu.readQuaternion(quat)) {
    std::cout << "quat: "
              << quat[0] << ", "
              << quat[1] << ", "
              << quat[2] << ", "
              << quat[3] << '\n';
  }
    return 0;
  }
