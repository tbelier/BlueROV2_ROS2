from launch import LaunchDescription
from launch import LaunchService
from launch_ros.actions import Node
from launch.substitutions import Command, LaunchConfiguration
import launch_ros.actions
import os
import launch
from ament_index_python import get_package_share_directory

def generate_launch_description():
	#pkg_share = get_package_share_directory("guidage")
	#rviz_config_file = os.path.join(pkg_share, "config", "config.rviz")
	
	# node_RawData = Node(
	# 	package='subsonus_pkg', # nom du package
	# 	executable='nodeRawdata_surface', # nom de l'executable
	# 	name='nodeRawdata_surface', # nom du node lors du lancement
	# )
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
	
	node_vision = Node(
        package='bluerov_vision',
        executable='bluerov_visionNode',
        name='bluerov_vision',
	)
	
	node_image = Node(
        package='bluerov_ros_camera',
        executable='bluerov_ros_cameraNode',
        name='bluerov_ros_cameraNode',
    )
	
	node_sensors = Node(
		package='sensors', # nom du package
		executable='sensors_node', # nom de l'executable
		name='sensors_node', # nom du node lors du lancement
	)
	"""
	node_Guidage = Node(
		package='guidage', # nom du package
		executable='nodeGuidage', # nom de l'executable
		name='nodeGuidage', # nom du node lors du lancement
	)
    
	node_usbl = Node(
		package='usbl_seatrac', # nom du package
		executable='usbl_pub_cpp', # nom de l'executable
		name='usbl_seatrac', # nom du node lors du lancement
	)
    """
	# retour de la fonction avec la liste des nodes à lancer
	return LaunchDescription([
		# Node capteur de pression
		
		# node_usbl,
		# Nodes USBL
		# node_RawData, 
		# Nodes Contrôle
		node_control,
		node_sensors,
		node_joy,
		# Nodes vision
		node_image,
		node_vision,
		# Node Guidage
		#node_Guidage,
		#launch.actions.ExecuteProcess(cmd=['ros2', 'bag', 'record', '/vision/pose', '/real/cmd_vel', 'subsonus/track_packet_filtered', 'subsonus/raw_sensors_packet_ROV','sensor/diff_press'],output='screen') 
	])
