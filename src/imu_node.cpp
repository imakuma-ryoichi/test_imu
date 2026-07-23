#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>
#include <chrono>
#include <string>
#include <array>

#include "imu_bno055/bno055.hpp"
#include "imu_bno055/bno055_reg.hpp"



class BNO055Node: public rclcpp::Node
{
public:
  BNO055Node()
  : Node("bno055_node")
  {
    this->declare_parameter<std::string>(
        "bno055.i2c.device",
        "/dev/i2c-1"
        );
    
    this->declare_parameter<int>(
        "bno055.i2c.address",
        0x28
        );

    this->declare_parameter<int>("pub_interval", 20);

    dev_ = this->get_parameter("bno055.i2c.device") .as_string();
    addr_ = static_cast<uint8_t>(this->get_parameter("bno055.i2c.address").as_int());
    pub_interval_ = this->get_parameter("pub_interval").as_int();

    imu_ = std::make_unique<BNO055>(dev_, addr_);

    if (!imu_->init()) {
      RCLCPP_ERROR(
        this->get_logger(),
        "Failed to initialize BNO055"
        );
    } else {
      RCLCPP_INFO(
        this->get_logger(),
        "BNO055 initialized successfully"
      );
      imu_ready_ = true;
    }



    imu_pub_ = this->create_publisher<std_msgs::msg::Float32>("imu_yaw", 10);

    timer_ = this->create_wall_timer(
        std::chrono::milliseconds(pub_interval_),
        std::bind(&BNO055Node::timer_callback, this));
  }

private:
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr imu_pub_;
  rclcpp::TimerBase::SharedPtr timer_;

  std::unique_ptr<BNO055> imu_;
  std::array<float, 3> eul_;
  std::string dev_; 
  uint8_t addr_;
  int pub_interval_;
  bool imu_ready_ = false;

  void timer_callback() 
  {
    if (!imu_ready_) return;

    if (!imu_->readEuler(eul_)) return;
    
    std_msgs::msg::Float32 yaw_msg;

    yaw_msg.data = eul_[0];

    imu_pub_ -> publish(yaw_msg);

  }
};


int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<BNO055Node>());
  rclcpp::shutdown();
  return 0;
}
