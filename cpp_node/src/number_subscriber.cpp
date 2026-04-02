#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/int64.hpp"
#include "example_interfaces/msg/string.hpp"
#include <cinttypes>

using namespace std::placeholders;
using namespace std::chrono_literals;

// namespace std::placeholders

    
class NumberSubscriber : public rclcpp::Node // MODIFY NAME
{
public:
    NumberSubscriber() : Node("number_subscriber") // MODIFY NAME
    {
        subscriber_ = this->create_subscription<example_interfaces::msg::Int64>(
            "number", 10,
            std::bind(&NumberSubscriber::callbackNumberNews, this, _1)
        );
        publisher_ = this->create_publisher<example_interfaces::msg::String>("robot_news", 10);
        timer_ = this->create_wall_timer(0.5s, std::bind(&NumberSubscriber::publishDataPlusNumber, this));
        RCLCPP_INFO(this->get_logger(), "NumberSubscriber has been started.");
    }
    
private:
    void callbackNumberNews(const example_interfaces::msg::Int64::SharedPtr msg)
    {
        last_number_ = msg->data;
        RCLCPP_INFO(this->get_logger(), "%" PRId64, msg->data);
    }

    void publishDataPlusNumber()
    {
        auto msg = example_interfaces::msg::String();
        msg.data = std::string("Data is ") + std::to_string(last_number_);
        publisher_->publish(msg);
    }

    int64_t last_number_ = 0;
    rclcpp::Subscription<example_interfaces::msg::Int64>::SharedPtr subscriber_;
    rclcpp::Publisher<example_interfaces::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};
    
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<NumberSubscriber>(); // MODIFY NAME
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}