#include <fcntl.h>
#include <sys/ioctl.h>
#include "imu_bno055/bno055_data.hpp"
#include "imu_bno055/bno055.hpp"
#include "imu_bno055/bno055_reg.hpp"
#include <thread>
#include <chrono>
#include <array>
#include <cstdint>
#include <iostream>

BNO055::BNO055(const BNO055Config& config) 
  : config_(config),
    fd_(-1)
  {}

bool BNO055::init() 
  {

    //二度読み込み防止
  if (fd_ >= 0) return false;

  fd_ = open(config_.device.c_str(), O_RDWR);
    
  if (fd_ == -1) {
    perror("open failed");
    return false; 
  }
    
  int ret = ioctl(fd_, I2C_SLAVE, config_.address);

  if (ret == -1) {
    perror("ioctl failed");
    close(fd_);
    fd_ = -1;
    return false;
  }


  if (!expectChipID()) {
    std::cerr << "CHIP_ID mismatch" << '\n';
    close(fd_);
    fd_ = -1;
    return false;
  }

  if(!setUnit(config_.unit)) {
    std::cerr << "SET UNIT failed" << '\n';
    close(fd_);
    fd_ = -1;
    return false;
  }

  if (!expectSetUnit(config_.unit)) {
    std::cerr << "UNIT_SEL verification failed" << '\n';
    close(fd_);
    fd_ = -1;
    return false;
  }

  if (!setOprMode(BNO055Mode::CONFIG)) {//センサーをNDOFmodeにする
    close(fd_);
    fd_ = -1;
    return false;
  }


  if (!writeCalibration(config_.calibration)) {
    std::cerr << "Failed to write calibration" << '\n';
  }

  if (!setOprMode(config_.mode)) {//センサーをNDOFmodeにする
    close(fd_);
    fd_ = -1;
    return false;
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
  
  return true;

  }
 
BNO055::~BNO055()
{
    if (fd_ >= 0) {
      close(fd_);
      fd_ = -1;
    }
}

 
bool BNO055::readReg(BNO055Reg reg, uint8_t& out_value)
  {
    return readRegs(reg, &out_value, 1);
  }
  

 bool BNO055::writeReg(uint8_t reg, uint8_t value) 
  {
    ssize_t ret;

    std::array<uint8_t, 2> buf;

    buf[0] = reg;
    buf[1] = value;

    for (int i = 0; i < MAX_RETRY; i++) {
      ret = write (fd_, buf.data(), 2);//2byte 書く
     
      if (ret != 2) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); 
        continue;
      }
      return true;
    }
  
    perror("writeReg failed");
    return false; 
  }


  bool BNO055::setUnit(const UnitConfig& unit) 
  {
    uint8_t val = toUnit(unit);

    return writeReg(toUint8(BNO055Reg::UNIT_SEL), val);

  }

  bool BNO055::expectSetUnit(const UnitConfig& unit)
  {
    uint8_t current;

    if (!readReg(BNO055Reg::UNIT_SEL, current)) return false; 

    uint8_t expected = toUnit(unit);

    uint8_t mask =
        ACC_UNIT_MASK |
        GYR_UNIT_MASK |
        EUL_UNIT_MASK;

    return ((current & mask) == (expected & mask));

  }

  uint8_t BNO055::toUnit(const UnitConfig& unit)
  {
    return 
      toUint8(unit.euler) |
      toUint8(unit.gyro) |
      toUint8(unit.acceleration);
  }

bool BNO055::writeInt16(uint8_t lsb_reg, int16_t raw_value)
{
  uint16_t value = static_cast<uint16_t>(raw_value);

  uint8_t lsb_value, msb_value;
  
  lsb_value = value & 0xFF;
  msb_value = (value >> 8) & 0xFF;

  uint8_t addr = lsb_reg;
  uint8_t msb_addr = addr + 1;

  if (!writeReg(addr, lsb_value)) return false;

  if (!writeReg(msb_addr, msb_value)) return false;

  return true;

}

int16_t BNO055::combineInt16(uint8_t lsb, uint8_t msb)
{
    return 
      static_cast<int16_t>(
      (static_cast<uint16_t>(msb) << 8) |
       static_cast<uint16_t>(lsb)
    );
}
// m/s^2 == /100.0f , mg == /1.0f  
bool BNO055::readIMUData(IMUData& data) 
{

  if (!readAcceleration(data.acceleration)) {
    return false;
  }

  if (!readGyroscope(data.gyroscope)) {
    return false;
  }

  if (!readQuaternion(data.quaternion)) {
    return false;
  }
  
  if (!readEuler(data.euler)) {
    return false;
  }

  return true;
}

bool BNO055::readAcceleration(std::array<float, 3>& acc_value) 
{

  constexpr float ACC_SCALE = 1.0f / 100.0f;

  std::array<int16_t, 3> raw;

  if (!readInt16Array(BNO055Reg::ACC_DATA_X_LSB, raw)) return false;
  
  acc_value[0] = static_cast<float>(raw[0]) * ACC_SCALE;
  acc_value[1] = static_cast<float>(raw[1]) * ACC_SCALE;
  acc_value[2] = static_cast<float>(raw[2]) * ACC_SCALE;

  return true;
}

bool BNO055::readBytes(uint8_t* data, size_t length)
{
  ssize_t ret = read(fd_, data, length); 

  if (ret != static_cast<ssize_t>(length)) return false;

  return true;
}

bool BNO055::readRegs(BNO055Reg reg, uint8_t* data, size_t length)
{
  
  uint8_t raw_reg = toUint8(reg);
  
  ssize_t write_ret;

  for (int i = 0; i < MAX_RETRY; i++) {
    write_ret = write(fd_, &raw_reg, 1);

    if (write_ret != 1) continue;//後でエラー吐かせる関数を作る
    if (!readBytes(data, length)) continue;
  
    return true;
  }

  perror("Failed to readRegs");
  return false;
}


//Dps == /16.0f, Rps == /900.0f
bool BNO055::readGyroscope(std::array<float, 3>& gyr_value) 
{
  constexpr float GYR_SCALE = 1.0f / 900.0f;

  std::array<int16_t, 3> raw;

  if (!readInt16Array(BNO055Reg::GYR_DATA_X_LSB, raw)) return false;

  gyr_value[0] = static_cast<float>(raw[0]) * GYR_SCALE;
  gyr_value[1] = static_cast<float>(raw[1]) * GYR_SCALE;
  gyr_value[2] = static_cast<float>(raw[2]) * GYR_SCALE;

  return true;
}

// Qua /2^14
bool BNO055::readQuaternion(std::array<float, 4>& quat_value)
{
  constexpr float QUAT_SCALE = 1.0f / static_cast<float>(1 << 14);

  std::array<int16_t, 4> raw;

  if (!readInt16Array(BNO055Reg::QUAT_DATA_W_LSB, raw)) return false;

  quat_value[0] = static_cast<float>(raw[0]) * QUAT_SCALE;
  quat_value[1] = static_cast<float>(raw[1]) * QUAT_SCALE;
  quat_value[2] = static_cast<float>(raw[2]) * QUAT_SCALE;
  quat_value[3] = static_cast<float>(raw[3]) * QUAT_SCALE;

  return true;
}

// degree /16.0f   radians /900.0f
bool BNO055::readEuler(std::array<float, 3>& eul_value) 
{
  constexpr float EUL_SCALE = 1.0f / 900.0f;
  //ここの上ymlで追い出すなら使用知らんけど変数名の差異で変更させるだけで行ければ熱そう

  std::array<int16_t, 3> raw;

  if (!readInt16Array(BNO055Reg::EUL_DATA_X_LSB, raw)) return false;

  eul_value[0] = static_cast<float>(raw[0]) * EUL_SCALE;
  eul_value[1] = static_cast<float>(raw[1]) * EUL_SCALE;
  eul_value[2] = static_cast<float>(raw[2]) * EUL_SCALE;

  return true;
}



bool BNO055::expectChipID()
  {
    uint8_t chip_id;

    if (!readReg(BNO055Reg::CHIP_ID, chip_id)) return false;

    return chip_id == EXPECT_CHIP_ID;
  }

bool BNO055::setOprMode(BNO055Mode mode)
{
  if (!writeReg(toUint8(BNO055Reg::OPR_MODE), toUint8(mode))) return false;

  std::this_thread::sleep_for(std::chrono::milliseconds(20));

  uint8_t current_mode;

  if (!readReg(BNO055Reg::OPR_MODE, current_mode)) return false;
  if ((current_mode & 0x0F) != toUint8(mode)) return false;
//上のifはエラーコードを収納する関数に書かせる

  return true;
}

bool BNO055::writeCalibration(const CalibrationData &calib_data)
{ 

  if (!writeOffset(calib_data.acc_offset, BNO055Reg::ACC_OFFSET_X_LSB)) return false;
  if (!writeOffset(calib_data.gyr_offset, BNO055Reg::GYR_OFFSET_X_LSB)) return false;
  if (!writeOffset(calib_data.mag_offset, BNO055Reg::MAG_OFFSET_X_LSB)) return false;
  if (!writeInt16(toUint8(BNO055Reg::ACC_RADIUS_LSB), calib_data.acc_radius)) return false;
  if (!writeInt16(toUint8(BNO055Reg::MAG_RADIUS_LSB), calib_data.mag_radius)) return false;

  return true;
}

bool BNO055::applyCalibration(const CalibrationData &calib_data, bool& imu_ready)
{
  if (!setOprMode(BNO055Mode::CONFIG)) return false;

  bool success = writeCalibration(calib_data);

  if (!setOprMode(config_.mode)) {
    std::cerr << "Error: Failed to restore NDOF mode." << '\n';
    imu_ready = false;
    return false;
  }

  imu_ready = true;
  return success;
}

bool BNO055::readCalibration(CalibrationData &calib_data, bool& imu_ready)
{
  if (!setOprMode(BNO055Mode::CONFIG)) return false;

  bool success = readCalibrationData(calib_data);

  if (!setOprMode(config_.mode)) {
    std::cerr << "Error: Failed to restore NDOF mode." << '\n';
    imu_ready = false;
    return false;
  }

  imu_ready = true;
  return success;
}

bool BNO055::verifyCalibration(const CalibrationData &calib_data, bool& imu_ready)
{

  CalibrationData calib_data_read;

  if (!readCalibration(calib_data_read, imu_ready)) return false;

  return (calib_data == calib_data_read);
}

bool BNO055::writeOffset(const std::array<int16_t, 3>& offset_array, BNO055Reg base_reg) 
{
  
  uint8_t x_reg = toUint8(base_reg);
  uint8_t y_reg = x_reg + 2;
  uint8_t z_reg = y_reg + 2;

  if (!writeInt16(x_reg, offset_array[0])) return false;
  if (!writeInt16(y_reg, offset_array[1])) return false;
  if (!writeInt16(z_reg, offset_array[2])) return false;
  
  return true;
}
//値がちゃんと入ったか判定があった方がいいのでは？最初しか動かさないし多分
//判定というがそれは分けた方がいいのかどうか微妙なところではある


bool BNO055::readCalibrationData(CalibrationData &calib_data)
{

  if (!readInt16Array(BNO055Reg::ACC_OFFSET_X_LSB, calib_data.acc_offset)) return false;
  if (!readInt16Array(BNO055Reg::GYR_OFFSET_X_LSB, calib_data.gyr_offset)) return false;
  if (!readInt16Array(BNO055Reg::MAG_OFFSET_X_LSB, calib_data.mag_offset)) return false;

  std::array<int16_t, 1>raw;

  if (!readInt16Array(BNO055Reg::ACC_RADIUS_LSB, raw)) return false;

  calib_data.acc_radius = raw[0];

  if (!readInt16Array(BNO055Reg::MAG_RADIUS_LSB, raw)) return false;

  calib_data.mag_radius = raw[0];

  return true;
}


bool BNO055::isCalib()
  {
    
    CalibrationStatus status;

    if (!readCalibrationStatus(status)) return false;

    return (status.sys == CALIB_COMPLEATE && 
            status.acc == CALIB_COMPLEATE && 
            status.gyr == CALIB_COMPLEATE &&
            status.mag == CALIB_COMPLEATE ); 

    //ここは戻り値を受け取った側で何か出して
  }

//このなかにisCalibを入れたって良い
bool BNO055::readCalibrationStatus(CalibrationStatus& status) 
{

    uint8_t calib_data;

    if (!readReg(BNO055Reg::CALIB_STAT, calib_data)) return false;
    //ここには通信エラーかなんか入るか？いやreadRegであるからいらない
    status.sys = (calib_data >> SYS_CALIB_BIT_SHIFT) & CALIB_MASK;
    status.acc = (calib_data >> ACC_CALIB_BIT_SHIFT) & CALIB_MASK;//2bit抽出
    status.gyr = (calib_data >> GYR_CALIB_BIT_SHIFT) & CALIB_MASK;
    status.mag = calib_data & CALIB_MASK;

    return true; 
}








