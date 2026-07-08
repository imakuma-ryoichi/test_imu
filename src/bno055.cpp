#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include "imu_bno055/bno055.hpp"
#include "imu_bno055/bno055_reg.hpp"
#include <thread>
#include <chrono>
#include <array>
#include <cstdint>
#include <iostream>

// コンパイル確認用 後で削除
int main()
{
    BNO055 imu("/dev/i2c-1", 0x28);

    return 0;
}
