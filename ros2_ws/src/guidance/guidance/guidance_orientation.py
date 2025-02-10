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
        self.subscriptionIMU = self.create_subscription(Twist, 'sensor/attitude_twist', self.callbackIMU, 10)

        # Publishers
        self.publisherU = self.create_publisher(Twist, 'real/u', 10)
        
        self.IMU = Twist()
        self.oldIMU = Twist()
        self.Ureal = Twist()

        self.rollConsigne  = 5 *np.pi/180 #rad
        self.pitchConsigne = 5 *np.pi/180 #rad
        self.yawConsigne   = 5 *np.pi/180 #rad
        self.Kp,self.Kd = 0.5,2
        self.errI = 0.
        
    def callbackIMU(self,msg):
        self.oldIMU = self.IMU
        self.IMU = msg

        
        #self.Tcolor(self.depth, "red")
        #self.Tcolor(self.depthConsigne, "blue")
         # ça ça marche
        errP = self.yawConsigne-self.IMU.angular.z
        self.errI += errP
        self.Ureal.angular.z = self.Kp*errP + self.errI
        
        #ça faut tester
        #errP = self.depthConsigne-self.depth
        #errD = self.pressure-self.oldPressure

        self.Ureal.angular.z = self.Kp*errP #+ self.Kd*errD

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
