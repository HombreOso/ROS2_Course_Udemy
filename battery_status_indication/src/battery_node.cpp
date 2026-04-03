#include "rclcpp/rclcpp.hpp"
#include "my_robot_interfaces/msg/led_panel_state.hpp"
#include "my_robot_interfaces/srv/set_led.hpp"
#include <algorithm>

using namespace std::chrono_literals;
using namespace std::placeholders;

class BatteryNode : public rclcpp::Node
{
public:
    BatteryNode() : Node("battery")
    {
        set_led_client_ = this->create_client<my_robot_interfaces::srv::SetLed>("set_led");
        led_panel_state_subscriber_ = this->create_subscription<my_robot_interfaces::msg::LedPanelState>(
            "number", 
            10,
            std::bind(
                &BatteryNode::callbackReadLedState, 
                this, 
                _1));
        timer_ = this->create_wall_timer(1s, std::bind(&BatteryNode::set_battery_status, this));
        RCLCPP_INFO(this->get_logger(), "NumberPublisher has been started");
    }

private:

    void callbackReadLedState(const my_robot_interfaces::msg::LedPanelState::SharedPtr msg)
    {
        std::copy(msg->led_panel_state.begin(), msg->led_panel_state.end(), led_panel_state);
        RCLCPP_INFO(this-> get_logger(), 
        "LED no. 1 is %s: LED no. 2 is %s: LED no. 3 is %s", 
        led_panel_state[0]?"ON":"OFF",
        led_panel_state[1]?"ON":"OFF",
        led_panel_state[2]?"ON":"OFF");
    }

    void callSetLed(bool led_states[3])
    {
        // wait fo the service to be available
        while (rclcpp::ok() && !set_led_client_->wait_for_service(1s))
        {
            RCLCPP_WARN(this->get_logger(), "Waiting for service to be available...");
        }
        if (!rclcpp::ok()) {
            return;
        }

        // build a request
        auto request = std::make_shared<my_robot_interfaces::srv::SetLed::Request>();

        // copy the led states to the request (built-in C-style arrays)
        std::copy(led_states, led_states + 3, request->led_states_set_points.begin());

        // send request
        auto response_future = set_led_client_->async_send_request(request, 
            std::bind(&BatteryNode::callbackCallSetLed, this, _1));
    }

    void set_battery_status()
    {
        if (!battery_is_discharged && counter_ == time_to_discharge_the_battery / 1s)
        {
            battery_is_discharged = true;
            led_panel_state[2] = true;
            callSetLed(led_panel_state);
            counter_ = 0;

        }
        else if (battery_is_discharged && counter_ == time_to_recharge_the_battery / 1s)
        {
            battery_is_discharged = false;
            led_panel_state[2] = false;
            callSetLed(led_panel_state);
            counter_ = 0;
        }
        counter_++;
        
    }



    void callbackCallSetLed(
        rclcpp::Client<my_robot_interfaces::srv::SetLed>::SharedFutureWithRequest future_with_request)
    {
        const auto & request_response_pair = future_with_request.get();
        const auto & request = request_response_pair.first;
        RCLCPP_INFO(
            this->get_logger(),
            "LED states set to: %s, %s, %s",
            request->led_states_set_points[0]?"ON":"OFF",
            request->led_states_set_points[1]?"ON":"OFF",
            request->led_states_set_points[2]?"ON":"OFF");
    }



    int64_t number_ = 0;
    int64_t counter_ = 0;
    bool led_panel_state[3] = {false, false, false};
    bool battery_is_discharged = false;
    const std::chrono::seconds time_to_discharge_the_battery = 6s;
    const std::chrono::seconds time_to_recharge_the_battery = 2s;
    rclcpp::Client<my_robot_interfaces::srv::SetLed>::SharedPtr set_led_client_;
    rclcpp::Subscription<my_robot_interfaces::msg::LedPanelState>::SharedPtr led_panel_state_subscriber_;
    rclcpp::TimerBase::SharedPtr timer_;
};
    
int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<BatteryNode>(); // MODIFY NAME
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}