#include "imu_bno055/bno055.hpp"

int main()
{
    BNO055 imu;

    if (!imu.init()) {
        return 1;
    }

    return 0;
}
