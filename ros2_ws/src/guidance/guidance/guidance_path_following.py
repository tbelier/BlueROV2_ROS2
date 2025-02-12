import sys
import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist, Pose
from pymavlink import mavutil
from std_msgs.msg import Float64

from message2.msg import Usbl
import numpy as np
import time
from roblib import *
import os

class Guidance(Node):
    def __init__(self):
        super().__init__('Guidance')# Init Node

        # Subscribers
        self.subscriptionPose = self.create_subscription(Pose, 'Pose', self.callbackPose, 10)

        # Publishers
        self.publisherU = self.create_publisher(Twist, 'real/u', 10)
        
        self.pose = Pose()
        self.U = Twist()
        self.U.linear.x = 0.5

        self.t0 = time.time()

        self.Lpath = getPath("AllPointsXYCSTheta.npy")

        # Params de la simulation
        self.K, self.K1, self.Kdy1, self.K10 = 10, 10, 1, 10
        self.s = 0


    def callbackPose(self,msg):
        X = np.array([[msg.linear.x],
                      [msg.linear.y],
                      [msg.angular.z]])
        u1, u2 = self.U.linear.x,self.U.angular.z
        U = np.array([[u1],
                      [u2]]) # U is not self.U directly, one is ROS twist the other one a numpy array
                
        psi = msg.angular.z
        self.pose = msg

        B = np.array([[cos(psi), 0], 
                      [sin(psi), 0], 
                      [0, 1]])
        
        X0 = B @ U
        x0,y0,psi0 = X0.flatten()
        V_0 = np.array([[x0],
                        [y0]])
        W_0 = np.array([[psi0]])

        lievre, lievreCc = interrogation_chemin(self.Lpath, s)
        lievreX,lievreY,lievrePsi = lievre.flatten()
        
        E = lievre-X
        errX, errY, theta = E.flatten()
        theta = sawtooth1D(theta, "pi")
        errXY = np.array([[errX],
                          [errY]])
        R = np.array([[np.cos(lievrePsi), -np.sin(lievrePsi)],
                      [np.sin(lievrePsi),  np.cos(lievrePsi)]])
        
        errX_Fresnet = inv(R) @ errXY
        
        s1, y1 = errX_Fresnet.flatten() 
        sp = u1 * cos(theta) - self.K1 * s1
        lievreXp_Fresnet = np.array([[sp], 
                                     [0]])
        dot_X_e_F = lievreXp_Fresnet - inv(R) @ V_0

        s1p,y1p = dot_X_e_F.flatten()
        psip_lievre = lievreCc * sp

        delta = -np.arctan(self.Kdy1 * y1)
        deltaPrime = -self.Kdy1 / (1 + (self.Kdy1 * y1) ** 2)
        deltaP = deltaPrime * y1p

        Err_Ang = sawtooth1D(delta - theta, "pi")
        if Err_Ang > np.pi: Err_Ang -= 2 * np.pi
        elif Err_Ang < -np.pi: Err_Ang += 2 * np.pi
            
        dot_Theta_Control = deltaP + self.K * Err_Ang
        dot_psi = psip_lievre - dot_Theta_Control
        u2 = dot_psi

        if sp < 0: sp = 0
        s = s + sp*(self.t0-time.time())
        self.t0 = time.time()

        self.U.linear.x = u1
        self.U.angular.z = u2
        self.publisherU.publish(self.U)

    def Tcolor(self, text, color):
        if color == "b" or color == "blue": self.get_logger().info(f"\033[94m {text} \033[0m")
        if color == "r" or color == "red": self.get_logger().info(f"\033[91m {text} \033[0m")
        if color == "g" or color == "green": self.get_logger().info(f"\033[92m {text} \033[0m")
        if color == "o" or color == "orange": self.get_logger().info(f"\033[93m {text} \033[0m")


def find_index_in_interval_np(s, pathCSY): #on donne une valeur s et une liste 1D, le code renvoie le premier indice entre lesquels se trouve notre valeur s
    pathCSY = np.array(pathCSY)  # Convertir la liste en tableau NumPy
    X,Y,C,S,Psi = pathCSY.T
    Smax = max(S)
    indices = np.where((pathCSY[:-1] <= s) & (s < pathCSY[1:]))[0]
    return indices[0] if len(indices) > 0 else None

def interrogation_chemin(L, s):
    index = find_index_in_interval_np(s, L)
    lievreX, lievreY, lievreC, lievreS, lievrePsi = L[index]
    return np.array([[lievreX],
                     [lievreY],
                     [lievrePsi]]), lievreC



def getPath(str):
    script_path = os.path.abspath(__file__)
    script_dir = os.path.dirname(script_path)
    loaded_array = np.load(f"{script_dir}/{str}")
    X,Y,C,S,Psi = loaded_array.T
    Smax = max(S)
    return np.column_stack((X,Y,C,S,Psi))



def sawtoothvector(V, modulo = "2pi"):
    if modulo == "2pi":
        x,y,psi = V.flatten()
        psi = psi%(2*pi)
        return array([[x],[y],[psi]])
    
    if modulo == "pi":
        x,y,psi = V.flatten()
        psi = (psi + np.pi) % (2 * np.pi) - np.pi
        return array([[x],[y],[psi]])
    
def sawtooth1D(psi, modulo = "2pi"):
    if modulo == "2pi":
        return psi%(2*pi)
    
    if modulo == "pi":
        return (psi + np.pi) % (2 * np.pi) - np.pi
    


def main(args=None):
    rclpy.init(args=args)
    guidance = Guidance()
    rclpy.spin(guidance)
    rclpy.shutdown()


if __name__ == "__main__":
    main()
