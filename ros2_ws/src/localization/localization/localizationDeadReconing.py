import sys
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist, PoseStamped
from pymavlink import mavutil
from std_msgs.msg import Float64
import time
from message2.msg import Usbl
import numpy as np
import tf_transformations

from std_msgs.msg import Header  
class Localization(Node):
    def __init__(self):
        super().__init__('Localization')# Init Node

        # Subscribers
        self.subscriptionPressure = self.create_subscription(Float64, 'sensor/pressure', self.callbackPressure, 10)
        self.subscriptionUsbl = self.create_subscription(Usbl, 'usbl_data', self.callbackUsbl, 10)
        self.subscriptionImu = self.create_subscription(Twist, 'attitude_twist', self.callbackImu, 10)

        # Publishers
        self.publisherPose = self.create_publisher(PoseStamped, 'pose', 10)
        
        self.pressure = 0.
        self.pose = PoseStamped()

    def callbackPressure(self,msg):
        self.oldPressure = self.pressure
        self.pressure = msg.data
        self.depth = (self.pressure-1000.)/100.
        self.pose.pose.position.z = self.depth


    def callbackImu(self,msg):
        self.IMU = msg

        quaternion = tf_transformations.quaternion_from_euler(self.IMU.angular.x, self.IMU.angular.y, self.IMU.angular.z)

        # Mise à jour de self.pose avec le quaternion
        self.pose.pose.angular.x = quaternion[0]  # Composante x
        self.pose.pose.angular.y = quaternion[1]  # Composante y
        self.pose.pose.angular.z = quaternion[2]  # Composante z
        self.pose.pose.angular.w = quaternion[3]  # Composante w

    def callbackUsbl(self, msg):
        self.usbl = msg
        self.pose.pose.position.x = msg.range * np.cos(msg.azimuth*np.pi/180)
        self.pose.pose.position.y = msg.range * np.sin(msg.azimuth*np.pi/180)
        
        self.pose.header = Header()  # Create a new Header object
        self.pose.header.stamp = self.get_clock().now().to_msg()  # Set the current time
        self.publisherPose.publish(self.pose)

    def Tcolor(self, text, color):
        if color == "b" or color == "blue": self.get_logger().info(f"\033[94m {text} \033[0m")
        if color == "r" or color == "red": self.get_logger().info(f"\033[91m {text} \033[0m")
        if color == "g" or color == "green": self.get_logger().info(f"\033[92m {text} \033[0m")
        if color == "o" or color == "orange": self.get_logger().info(f"\033[93m {text} \033[0m")
        
def main(args=None):
    rclpy.init(args=args)
    localization = Localization()
    rclpy.spin(localization)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
