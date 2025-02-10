from pymavlink import mavutil
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from std_msgs.msg import Float64
import numpy as np

# Sensor values (pressure, yawspeed)
master = mavutil.mavlink_connection('udpin:0.0.0.0:14550')
master.wait_heartbeat()

class PublishSensors(Node):
    def __init__(self):
        # Init Node
        super().__init__('sensors')
        self.publisher1_ = self.create_publisher(Float64, '/sensor/pressure', 10)
        self.publisher2_ = self.create_publisher(Float64, '/sensor/yaw_speed', 10)

        self.timer = self.create_timer(1.0, self.callback_publisher1) #every 1 sec

        # Initialize messages
        self.press_msg = Float64()
        self.yaw_sp_msg = Float64()

    def callback_publisher1(self):
        
        m = master.recv_match(type='SCALED_PRESSURE2')
        if m is not None:
            p = m.press_abs  # hPa = mB
            self.press_msg.data = p
            self.publisher1_.publish(self.press_msg)
            self.get_logger().info(f'Published pressure: {p}')  # Log la pression
        else:
            self.get_logger().warn('No pressure data received')

        m = master.recv_match(type='ATTITUDE')
        if m is not None:
            w = (m.yawspeed / np.pi) * 180.  # deg/s
            self.yaw_sp_msg.data = w
            self.publisher2_.publish(self.yaw_sp_msg)
            self.get_logger().info(f'Published yaw speed: {w}')  # Log la vitesse de rotation
        else:
            self.get_logger().warn('No attitude data received')

def main():
    rclpy.init()
    master.wait_heartbeat() #TODO : sûr qu'il doit rester lui ?
    sensorsNode = PublishSensors()
    rclpy.spin(sensorsNode)
    rclpy.shutdown()

    master.wait_heartbeat() #à ajouter

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


            