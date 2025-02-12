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

        self.rollConsigne  = -45 *np.pi/180 #rad
        self.pitchConsigne = 0 *np.pi/180 #rad
        self.yawConsigne   = 0 *np.pi/180 #rad
        self.Kp,self.Kd = 2,2
        self.errIx = 0.
        self.errIy = 0.
        self.errIz = 0.
        
    def callbackIMU(self,msg):
        self.oldIMU = self.IMU
        self.IMU = msg

        
        #self.Tcolor(self.depth, "red")
        #self.Tcolor(self.depthConsigne, "blue")
         # ça ça marche
        errPx = self.rollConsigne-self.IMU.angular.x
        errPy = self.pitchConsigne-self.IMU.angular.y
        errPz = self.yawConsigne-self.IMU.angular.z
        self.errIx += errPx
        self.errIy += errPy
        self.errIz += errPz
        #self.Tcolor(f"errPx : {errPx}", "red")
        #self.Tcolor(f"errIx : {self.errIx}", "orange")
        self.Ureal.angular.x = self.Kp*errPx + self.errIx
        self.Ureal.angular.y = self.Kp*errPy + self.errIy
        self.Ureal.angular.z = self.Kp*errPz + self.errIz
        
        #ça faut tester
        #errP = self.depthConsigne-self.depth
        #errD = self.pressure-self.oldPressure


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
