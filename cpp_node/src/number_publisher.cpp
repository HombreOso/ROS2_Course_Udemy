#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/msg/int64.hpp"
#include "example_interfaces/srv/set_bool.hpp"

using namespace std::chrono_literals;
using namespace std::placeholders;

class NumberPublisher : public rclcpp::Node
{
public:
    NumberPublisher() : Node("number_publisher")
    {
        publisher_ = this->create_publisher<example_interfaces::msg::Int64>("number", 10);
        timer_ = this->create_wall_timer(0.5s, std::bind(&NumberPublisher::publishNumber, this));
        reset_server_ = this->create_service<example_interfaces::srv::SetBool>(
            "reset_counter",
            std::bind(&NumberPublisher::callbackResetCounter, this, _1, _2));
        RCLCPP_INFO(this->get_logger(), "NumberPublisher has been started");
    }

private:
    void publishNumber()
    {
        auto msg = example_interfaces::msg::Int64();
        msg.data = number_++;
        publisher_->publish(msg);
    }

    void callbackResetCounter(
        const example_interfaces::srv::SetBool::Request::SharedPtr request,
        example_interfaces::srv::SetBool::Response::SharedPtr response)
    {
        if (request->data) {
            number_ = 0;
            response->success = true;
            response->message = "Publisher counter reset to 0";
        } else {
            response->success = false;
            response->message = "Reset request ignored (data=false)";
        }
    }

    int64_t number_ = 0;
    rclcpp::Publisher<example_interfaces::msg::Int64>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Service<example_interfaces::srv::SetBool>::SharedPtr reset_server_;

};
    
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<NumberPublisher>(); // MODIFY NAME
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}