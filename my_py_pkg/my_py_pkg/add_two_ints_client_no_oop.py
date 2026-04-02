#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from example_interfaces.srv import AddTwoInts

def main(args=None):
    rclpy.init()
    node = Node("add_two_ints_client_no_oop")

    client = node.create_client(AddTwoInts, "add_two_ints")
    while not client.wait_for_service(2.0):
        node.get_logger().warn("Waiting for Add Two Ints server...")

    request = AddTwoInts.Request()
    request.a = 4
    request.b = 5

    future = client.call_async(request)
    rclpy.spin_until_future_complete(node, future)

    response = future.result()

    node.get_logger().info("The sum is: " + str(response.sum))

    rclpy.shutdown()

if __name__ == "__main__":
    main()