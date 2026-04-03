#include "rclcpp/rclcpp.hpp"
#include "my_robot_interfaces/msg/led_panel_state.hpp"
#include "my_robot_interfaces/srv/set_led.hpp"
#include <algorithm>

using namespace std::placeholders;
    
class SetLedServerNode : public rclcpp::Node // MODIFY NAME
{
public:
    SetLedServerNode() : Node("set_led_server") // MODIFY NAME
    {
        server_ = this->create_service<my_robot_interfaces::srv::SetLed>("set_led",
        std::bind(&SetLedServerNode::callbackSetLed, this, _1, _2));
        led_panel_state_publisher_ = this->create_publisher<my_robot_interfaces::msg::LedPanelState>("led_panel_state", 10);
        RCLCPP_INFO(this->get_logger(), "Set Led Server has been started");

    }
    
private:
    
    void callbackSetLed(const my_robot_interfaces::srv::SetLed::Request::SharedPtr request,
                        const my_robot_interfaces::srv::SetLed::Response::SharedPtr response)

    {
        std::copy(request->led_states_set_points.begin(), request->led_states_set_points.end(), led_panel_state);
        RCLCPP_INFO(this->get_logger(), "LED states set to: %s, %s, %s",
            led_panel_state[0]?"ON":"OFF",
            led_panel_state[1]?"ON":"OFF",
            led_panel_state[2]?"ON":"OFF");
        response->success = true;
        response->message = "LED states set successfully";

    }

    void publishLedPanelState()
    {
        auto msg = my_robot_interfaces::msg::LedPanelState();
        std::copy(led_panel_state, led_panel_state + 3, msg.led_panel_state.begin());
        led_panel_state_publisher_->publish(msg);
    }   


    rclcpp::Service<my_robot_interfaces::srv::SetLed>::SharedPtr server_;
    rclcpp::Publisher<my_robot_interfaces::msg::LedPanelState>::SharedPtr led_panel_state_publisher_;
    bool led_panel_state[3] = {false, false, false};

};
    
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<SetLedServerNode>(); // MODIFY NAME
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}