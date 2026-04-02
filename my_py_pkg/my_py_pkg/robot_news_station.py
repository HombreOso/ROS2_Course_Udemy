#!/usr/bin/env python3
import rclpy
from rclpy.node import Node

from example_interfaces.msg import String

class RobotNewsStation(Node):
    def __init__(self):
        super().__init__("robot_news_station")  # MODIFY node_name
        

def main(args=None):
    rclpy.init(args=args)
    node = RobotNewsStation()  # MODIFY node_name
    rclpy.spin(node)
    rclpy.shutdon()

if __name__ == "__main__":
    main()