#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

using namespace std::chrono_literals;

using namespace std::placeholders;

class AddTwoIntsClientNode : public rclcpp::Node
{
private:
    rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr client_;
public:
    AddTwoIntsClientNode () : Node("add_two_ints_client")
    {
        client_ = this->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints_client");
    };

    void callAddTwoInts(int a, int b)
    {
        // wait fo the service to be available
        while(! client_->wait_for_service(1s))
        {
            RCLCPP_WARN(this->get_logger(), "Waiting for service to be available...");
        }

        // build a request
        auto request = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
        request->a = a;
        request->b = b;

        // send request
        auto response_future = client_->async_send_request(request, 
            std::bind(&AddTwoIntsClientNode::callbackCallAddTwoInts, this, _1));


    }

    void callbackCallAddTwoInts(
        rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedFutureWithRequest future_with_request)
    {
        const auto & request_response_pair = future_with_request.get();
        const auto & request = request_response_pair.first;
        const auto & response = request_response_pair.second;
        RCLCPP_INFO(
            this->get_logger(),
            "Sum of %d and %d is %d",
            (int)(request->a),
            (int)(request->b),
            (int)(response->sum));
    }
    
};



int main(int argc, char **argv)
{
    // initialize ROS2 communication
    rclcpp::init(argc, argv);
    // create a SHared Pointer for the Node
    auto node = std::make_shared<rclcpp::Node>("add_two_ints_client_no_oop"); // MODIFY NAME
    // register a service client in the node
    // interface type (example_interfaces::srv::AddTwoInts) 
    // and service name ("add_two_ints") 
    // are to be specified
    auto client = node->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints");

    // wait until the service is available
    // the loop is quit as soon as the service is available (server has started)
    while (!client->wait_for_service(1s))
    {
        RCLCPP_WARN(node->get_logger(), "Waiting for the Service to be available...");
    }

    // build a request
    auto request = std::make_shared<example_interfaces::srv::AddTwoInts::Request>();
    request->a = 9;
    request->b = 7;

    // send the request
    auto response_future = client->async_send_request(request);

    // spin until the response is received
    rclcpp::spin_until_future_complete(node, response_future);

    // process the response
    auto response = response_future.get();
    RCLCPP_INFO(node->get_logger(), "%d + %d = %d", (int)request->a, (int)request->b, (int)response->sum);

    // shutdown the Node
    rclcpp::shutdown();

    // standard return of the main() function in C++
    return 0;
}