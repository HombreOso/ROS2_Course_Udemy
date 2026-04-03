#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from example_interfaces.srv import SetBool

def main(args=None):
    rclpy.init()
    node = Node("reset_counter_client_no_oop")

    client = node.create_client(SetBool, "reset_counter")
    while not client.wait_for_service(2.0):
        node.get_logger().warn("Waiting for Reset Counter server...")

    request = SetBool.Request()
    request.data = True

    future = client.call_async(request)
    rclpy.spin_until_future_complete(node, future)

    response = future.result()

    node.get_logger().info("The result is: " + str(response.success) + ". Why?: " + response.message)

    rclpy.shutdown()

if __name__ == "__main__":
    main()