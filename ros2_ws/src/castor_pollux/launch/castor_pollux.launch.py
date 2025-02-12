from launch import LaunchDescription
from launch import LaunchService
from launch_ros.actions import Node
from launch.substitutions import Command, LaunchConfiguration
import launch_ros.actions
import os
import launch
from ament_index_python import get_package_share_directory

def generate_launch_description():

	node_vision_treatment = Node(
        package='bluerov_vision',
        executable='bluerov_visionNode',
        name='bluerov_vision',
	)
	
	node_vision = Node(
        package='bluerov_ros_camera',
        executable='bluerov_ros_cameraNode',
        name='bluerov_ros_cameraNode',
    )

	node_sensors = Node(
		package='sensors', # nom du package
		executable='sensors_node', # nom de l'executable
		name='sensors_node', # nom du node lors du lancement
	)
	node_guidage = Node(
		package='guidance', # nom du package
		executable='guidance_depth', # nom de l'executable
		name='guidance_depth', # nom du node lors du lancement
	)

	node_control = Node(
            package='control',
            executable='control',
            name='control_node'
    )
	node_joy = Node(
            package='joy',
            executable='joy_node',
            name='joy_node',
    )

	node_usbl = Node(
		package='usbl_seatrac', # nom du package
		executable='usbl_pub_cpp', # nom de l'executable
		name='usbl_seatrac', # nom du node lors du lancement
	)

	# retour de la fonction avec la liste des nodes à lancer
	return LaunchDescription([

		node_sensors,
		node_guidage,
		node_joy,
		node_control,
		node_vision,
		node_vision_treatment,
		node_usbl,
		launch.actions.ExecuteProcess(cmd=['ros2', 'bag', 'record', '/sensor/attitude_twist', '/sensor/pressure', '/usbl_data', '/joy', "real/u" ],output='screen') 
	])
