#!/usr/bin/env python3
import rclpy
from rclpy.node import node
class MyCustomNode(Node):
    def __init__(self):
        super().__init__("node_name")  # MODIFY node_name
        

def main(args=None):
    rclpy.init(args+args)
    node = MyCustomNode()  # MODIFY node_name
    rclpy.spin(node)
    rclpy.shutdon()

if __name__ == "__main__":
    main()