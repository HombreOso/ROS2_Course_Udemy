#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/string.hpp"

using namespace std::placeholders;

// namespace std::placeholders

    
class CppSubscriber : public rclcpp::Node // MODIFY NAME
{
public:
    CppSubscriber() : Node("node_name") // MODIFY NAME
    {
        subscriber_ = this->create_subscription<example_interfaces::msg::String>(
            "robot_news", 10,
            std::bind(&CppSubscriber::callbackRobotNews, this, _1)
        );
        RCLCPP_INFO(this->get_logger(), "CppSubscriber has been started.");
    }
    
private:
    void callbackRobotNews(const example_interfaces::msg::String::SharedPtr msg)
    {
        RCLCPP_INFO(this-> get_logger(), "%s", msg->data.c_str());
    }

    rclcpp::Subscription<example_interfaces::msg::String>::SharedPtr subscriber_;
};
    
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<CppSubscriber>(); // MODIFY NAME
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}