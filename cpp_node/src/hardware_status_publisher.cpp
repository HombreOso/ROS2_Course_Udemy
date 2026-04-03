#include "rclcpp/rclcpp.hpp"
#include "my_robot_interfaces/msg/hardware_status.hpp"
using namespace std::chrono_literals;


    
class HardwareStatusPublisherNode : public rclcpp::Node // MODIFY NAME
{
public:
    HardwareStatusPublisherNode() : Node("hardware_status_publisher") // MODIFY NAME
    {
        publisher_ = this->create_publisher<my_robot_interfaces::msg::HardwareStatus>("hardware_status", 10);
        timer_ = this->create_wall_timer(0.5s, std::bind(&HardwareStatusPublisherNode::publishHardwareStatus, this));
        RCLCPP_INFO(this->get_logger(), "Hardware status publisher has been started");
    }
    
private:
    void publishHardwareStatus()
    {
        auto msg = my_robot_interfaces::msg::HardwareStatus();
        msg.temperature = 45.0;
        msg.are_motors_ready = true;
        msg.debug_message = "Everything is working fine";
        publisher_->publish(msg);
    }

    rclcpp::Publisher<my_robot_interfaces::msg::HardwareStatus>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};
    
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<HardwareStatusPublisherNode>(); // MODIFY NAME
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}