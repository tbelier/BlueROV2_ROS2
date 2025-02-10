import sys
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
from std_msgs.msg import Float64
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Pose
from scipy.spatial.transform import Rotation
from pymavlink import mavutil
from std_msgs.msg import Float64MultiArray
import numpy as np
from scipy.spatial.transform import Rotation as R


#initializing mavlink master
master = mavutil.mavlink_connection('tcp:192.168.2.2:14550')
master.wait_heartbeat()

# channels =  [ pitch ,  roll , vertical, yaw , forward, lateral, cam pan speed, cam tilt speed, lums1, lums2]
channels = []
class Control(Node):
    def __init__(self):
        super().__init__('Control')# Init Node

        self.publisherPressure = self.create_publisher(Float64, '/sensor/pressure', 10)
        self.twist_publisher   = self.create_publisher(Twist, '/sensor/attitude_twist', 10)
        self.press_msg = Float64()
        self.twist_msg = Twist()

        # Subscribersself.press_msg = Float64()
        self.twist_msg = Twist()
        self.subscriptionJoy = self.create_subscription(Joy, '/joy', self.callbackJoy, 10)
        self.subscriptionU = self.create_subscription(Twist, 'real/u', self.callbackU, 10)

        # Callback timers
        timer_period = 0.1  # seconds
        self.timer = self.create_timer(timer_period, self.callbackMavlink)
        #self.Tcolor("coucou", "orange")
        # Buttons
        self.LT, self.LB, self.RT, self.RB = 0., 0., 0., 0.  # Elev -,Elev +, Gain -, Gain +
        self.crossVert, self.crossHoriz = 0., 0.          # Camera orientation top+/bottom-, Spotlights +/-
        self.joyLvert = 0.    # Unused
        self.joyLhori = 0.  # Rotate Left-/+Right
        self.joyRvert = 0.   # XY plane backward-/+forward
        self.joyRhoriz = 0. # XY plane left-/+right
        self.butX = 0  # Switch Manual, Stabilize, Auto, Unused control 
        self.butStart, self.butBack = 0,0

        # Pose
        self.forward, self.lateral, self.ascend = 0., 0., 0.
        self.yaw, self.pitch, self.roll = 0., 0., 0.

        self.forwardd, self.laterald, self.ascendd = 0., 0., 0.
        self.yawd, self.pitchd, self.rolld = 0., 0., 0.
        
        # Booleans
        self.isArmed = False
        self.isAutonomous = False

        # Init with normal flight mode 
        self.set_mode('STABILIZE') # also exist 'MANUAL', 'STABILIZE'

        # Gains
        self.kJoy, self.kAscend = 2, 1
        self.lightValue = 1100

        # Get messages
        self.U = Twist()


        self.k = 0

    def callbackU(self,msg):
        self.U = msg
    
    def send_mavlink_msg(self, channels):
        rc_channel_values = [1500 for _ in range(18)]
        for i in range(0,len(channels)) :
            channel_id = i
            if channel_id < 0 or channel_id > 17:
                print("Channel " + str(channel_id) + " does not exist.")
                return

            rc_channel_values[channel_id] = channels[i]
        master.mav.rc_channels_override_send(
            master.target_system,                # target_system
            master.target_component,             # target_component
            *rc_channel_values)                  # RC channel list, in microseconds.

    def set_mode(self, mode):
        # Check if mode is available
        if mode not in master.mode_mapping():
            print('Unknown mode : {}'.format(mode))
            print('Try:', list(master.mode_mapping().keys()))
            sys.exit(1)

        # Get mode ID
        mode_id = master.mode_mapping()[mode]

        master.mav.set_mode_send(
                master.target_system,
                mavutil.mavlink.MAV_MODE_FLAG_CUSTOM_MODE_ENABLED,
                mode_id)
                
    def callbackJoy(self,msg):
        button = msg.buttons
        Jaxes = msg.axes
        #self.get_logger().info(f"coucouJoy")
        self.LB, self.RB = button[4], button[5]  
        self.LT, self.RT = Jaxes[2],  Jaxes[5] # attention de base les trigers démarrent à 1 et descendent à -1 lorsqu'elles sont enclenchées
    
        self.joyYaw, self.joyPitch, self.joyRoll = -Jaxes[3], 0, 0
        self.joyForward, self.joyLateral, self.joyAscend = Jaxes[1], -Jaxes[0], Jaxes[4]

        self.butA = button[0]     # Switch to Autonomous
        self.butB = button[1]     # Unused
        self.butX = button[2]     # Switch Manual control 
        self.butY = button[3]     # Set Stabilize mode 
        
        self.butBack  = button[6] # DisArming
        self.butStart = button[7] # Arming
        
        self.butLight    = -Jaxes[6] # Lights
        self.butTiltUp   = button[5] # Camera Tilt Up Speed
        self.butTiltDown = button[4] # Camera Tilt Down Speed

        self.butPan = Jaxes[7] # Camera Pan Speed

        if self.butStart == 1: 
            self.isArmed = True
            master.mav.command_long_send(master.target_system, master.target_component, mavutil.mavlink.MAV_CMD_COMPONENT_ARM_DISARM, 0, 1, 0, 0, 0, 0, 0, 0)
            master.motors_armed_wait()
            self.Tcolor("ROV armed","b")
            #self.Tcolor("coucou", "orange")

        if self.butBack  == 1: 
            self.isArmed = False
            master.mav.command_long_send( master.target_system, master.target_component, mavutil.mavlink.MAV_CMD_COMPONENT_ARM_DISARM, 0, 0, 0, 0, 0, 0, 0, 0)
            master.motors_disarmed_wait()
            self.Tcolor("ROV disarmed","b")

        if self.butA     == 1: 
            self.isAutonomous = True
            self.Tcolor("ROV in Autonomous","b")
        if self.butB     == 1: 
            self.isAutonomous = False  
            self.Tcolor("ROV in Manual","b")
        if self.butY     == 1: 
            isStabilizeForced = True  
            self.set_mode('STABILIZE')

        if self.butX     == 1: 
            isStabilizeForced = False 
            self.set_mode('MANUAL')

    def callbackMavlink(self):
        static_channels = [1500,1500,1500,1500,1500,1500,1500,1500,1100,1500,1500,1500,1100,1500,1500,1500]
        if self.isArmed:
            if self.isAutonomous:
                forward = int(1500+400*np.tanh(self.kJoy*self.joyForward))
                ascend  = int(1500+400*np.tanh(-self.kAscend*self.U.linear.z))
                lateral = int(1500+400*np.tanh(self.kJoy*self.joyLateral))
                #self.Tcolor(f"self.U.linear.z : {self.U.linear.z}", "blue")
                #self.Tcolor(f"self.kAscend*self.U.linear.z) : {self.kAscend*self.U.linear.z}", "blue")
                #self.Tcolor(f"int(1500+400*np.tanh(self.kAscend*self.U.linear.z)) : {int(1500+400*np.tanh(self.kAscend*self.U.linear.z))}", "blue")

                self.lightValue  = int(max(1100,min(1900,self.lightValue + 50*self.butLight)))
                cameraPan  = int(1500+400*np.tanh(self.kJoy*(self.butTiltUp-self.butTiltDown)))
                cameraTilt  = int(1500+400*np.tanh(self.kJoy*(self.butPan)))

                channels = [1500,1500,ascend,1500,forward,lateral,cameraPan,cameraTilt,self.lightValue,1500,1500,1500,1100,1500,1500,1500]
                #self.Tcolor(channels, "red")
                self.send_mavlink_msg(channels)

            else:
                yaw   = int(1500+400*np.tanh(self.kJoy*self.joyYaw))
                pitch = int(1500+400*np.tanh(self.kJoy*self.joyPitch)) # = 0 when in ROV config 
                roll  = int(1500+400*np.tanh(self.kJoy*self.joyRoll))  # = 0 when in ROV config

                forward = int(1500+400*np.tanh(self.kJoy*self.joyForward))
                lateral = int(1500+400*np.tanh(self.kJoy*self.joyLateral))
                ascend  = int(1500+400*np.tanh(self.kJoy*self.joyAscend))

                self.lightValue  = int(max(1100,min(1900,self.lightValue + 50*self.butLight)))
                cameraPan  = int(1500+400*np.tanh(self.kJoy*(self.butTiltUp-self.butTiltDown)))
                cameraTilt  = int(1500+400*np.tanh(self.kJoy*(self.butPan)))

                channels = [pitch,roll,ascend,yaw,forward,lateral,cameraPan,cameraTilt,self.lightValue,1500,1500,1500,1100,1500,1500,1500]
                #self.Tcolor(channels, "green")
                self.send_mavlink_msg(channels)
        else:
            self.send_mavlink_msg(static_channels)
               
    def Tcolor(self, text, color):
        if color == "b" or color == "blue": self.get_logger().info(f"\033[94m {text} \033[0m")
        if color == "r" or color == "red": self.get_logger().info(f"\033[91m {text} \033[0m")
        if color == "g" or color == "green": self.get_logger().info(f"\033[92m {text} \033[0m")
        if color == "o" or color == "orange": self.get_logger().info(f"\033[93m {text} \033[0m")
        
def main(args=None):
    rclpy.init(args=args)
    control = Control()
    rclpy.spin(control)
    rclpy.shutdown()


if __name__ == "__main__":
    main()







