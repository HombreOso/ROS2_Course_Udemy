#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from my_robot_interfaces.msg import HardwareStatus

class HardwareStatusPublisherNode(Node):
    def __init__(self):
        super().__init__("hardware_status_publisher")  # MODIFY node_name
        self.declare_parameter("publish_rate", 0.5)
        self.publisher_ = self.create_publisher(HardwareStatus, "hardware_status", 10)
        self.timer_ = self.create_timer(self.get_parameter("publish_rate").value, self.publish_hardware_status)
        self.get_logger().info("Hardware status publisher has been started")

    def publish_hardware_status(self):
        msg = HardwareStatus()
        msg.temperature = 45.0
        msg.are_motors_ready = True
        msg.debug_message = "Everything is working fine"
        self.publisher_.publish(msg)

def main(args=None):
    rclpy.init(args=args)
    node = HardwareStatusPublisherNode()  # MODIFY node_name
    rclpy.spin(node)
    rclpy.shutdon()

if __name__ == "__main__":
    main()