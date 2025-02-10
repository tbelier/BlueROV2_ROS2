import sys
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from pymavlink import mavutil
from std_msgs.msg import Float64

from message2.msg import Usbl
import numpy as np

class Guidance(Node):
    def __init__(self):
        super().__init__('Guidance')# Init Node

        # Subscribers
        self.subscriptionPressure = self.create_subscription(Float64, 'sensor/pressure', self.callbackPressure, 10)

        # Publishers
        self.publisherU = self.create_publisher(Twist, 'real/u', 10)
        
        self.pressure, self.oldPressure = 0.,0.
        self.Ureal = Twist()

        self.depthConsigne = 5
        self.Kp,self.Kd = 0.5,2
        self.errI = 0.
        
    def callbackPressure(self,msg):
        self.oldPressure = self.pressure
        self.pressure = msg.data

        self.depth = (self.pressure-1000.)/100.
        
        #self.Tcolor(self.depth, "red")
        #self.Tcolor(self.depthConsigne, "blue")
         # ça ça marche
        errP = self.depthConsigne-self.depth
        self.errI += errP
        self.Ureal.linear.z = self.Kp*errP + self.errI
        
        #ça faut tester
        #errP = self.depthConsigne-self.depth
        #errD = self.pressure-self.oldPressure

        self.Ureal.linear.z = self.Kp*errP #+ self.Kd*errD

        self.publisherU.publish(self.Ureal)

    def Tcolor(self, text, color):
        if color == "b" or color == "blue": self.get_logger().info(f"\033[94m {text} \033[0m")
        if color == "r" or color == "red": self.get_logger().info(f"\033[91m {text} \033[0m")
        if color == "g" or color == "green": self.get_logger().info(f"\033[92m {text} \033[0m")
        if color == "o" or color == "orange": self.get_logger().info(f"\033[93m {text} \033[0m")
        
def main(args=None):
    rclpy.init(args=args)
    guidance = Guidance()
    rclpy.spin(guidance)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
