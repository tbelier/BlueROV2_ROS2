from pymavlink import mavutil
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from std_msgs.msg import Float64
import numpy as np
from geometry_msgs.msg import Twist

# Sensor values (pressure, yawspeed)
#master = mavutil.mavlink_connection('tcp:192.168.2.2:14550')
#master = mavutil.mavlink_connection('udpin:0.0.0.0:14550')
master = mavutil.mavlink_connection('tcp:192.168.2.2:14550')
master.wait_heartbeat()

class PublishSensors(Node):
    def __init__(self):
        # Init Node
        super().__init__('sensors')
        self.publisherPressure = self.create_publisher(Float64, '/sensor/pressure', 1)
        self.twist_publisher = self.create_publisher(Twist, '/sensor/attitude_twist', 1)

        self.timerPressure = self.create_timer(0.001, self.callback_publisherPressure)
        #self.timerAttitude = self.create_timer(1.2, self.callback_publisherAttitude)

        # Initialize messages
        self.press_msg = Float64()
        self.twist_msg = Twist()

        self.k=0

    def callback_publisherPressure(self):
        #self.Tcolor(self.k,"orange")
        if self.k == 0 :
            m2 = master.recv_match(type='ATTITUDE', blocking =False)
            if m2 is not None :
                self.Tcolor(m2,"blue")
                # Conversion en degrés
                yaw_deg = (m2.yaw / np.pi) * 180.
                pitch_deg = (m2.pitch / np.pi) * 180.
                roll_deg = (m2.roll / np.pi) * 180.
                
                self.twist_msg.angular.x = m2.roll
                self.twist_msg.angular.y = m2.pitch
                self.twist_msg.angular.z = m2.yaw
                
                self.twist_publisher.publish(self.twist_msg)
                self.k = 1

        if self.k == 1: 
            m = master.recv_match(type='SCALED_PRESSURE2', blocking =False)
            if m is not None:
                self.Tcolor(m,"red")
                p = m.press_abs  # hPa = mB
                self.press_msg.data = p
                self.publisherPressure.publish(self.press_msg)
                self.k = 0

    #def callback_publisherAttitude(self):

    def Tcolor(self, text, color):
        if color == "b" or color == "blue": self.get_logger().info(f"\033[94m {text} \033[0m")
        if color == "r" or color == "red": self.get_logger().info(f"\033[91m {text} \033[0m")
        if color == "g" or color == "green": self.get_logger().info(f"\033[92m {text} \033[0m")
        if color == "o" or color == "orange": self.get_logger().info(f"\033[93m {text} \033[0m")

def main():
    rclpy.init()
    sensorsNode = PublishSensors()
    rclpy.spin(sensorsNode)
    rclpy.shutdown()

if __name__ == "__main__":
    main()


"""
ros2 pkg create my_python_pkg --build-type ament_python
"""

"""
Commander chaque moteur :
message du type : ACTUATOR_OUTPUT_FUNCTION_MOTOR%n%  avec %n% le moteur en quesstion  
"""

"""
Messages de control 
MAV_CMD_NAV_SET_YAW_SPEED
MANUAL_CONTROL
RC_CHANNELS_OVERRIDE
MANUAL_SETPOINT 
SET_ATTITUDE_TARGET
ACTUATOR_CONTROL_TARGET


"""
"""
self.get_logger().info('\033[91mArmed !\033[0m')
            # Bleu : 94
            # Rouge: 91
            # Vert: 92
            # Orange: 93
"""


            