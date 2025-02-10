from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    # Déclarer les arguments de lancement pour les paramètres
    declare_params = DeclareLaunchArgument(
        'params_file',
        default_value=os.path.join(
            get_package_share_directory('td2'),
            'config',
            'td2.yaml'
        )
    )

    return LaunchDescription([
        declare_params,
        Node(
            package='td2',
            executable='target_publisher_node',
            name='target_publisher_node',
            output='screen'
        ),
        Node(
            package='td2',
            executable='boat_simulation_node',
            name='boat_simulation_node_1',
            namespace='boat1',
            output='screen',
            parameters=[LaunchConfiguration('params_file')]
        ),
        Node(
            package='td2',
            executable='boat_controller_node',
            name='boat_controller_node_1',
            namespace='boat1',
            output='screen'
        ),
        Node(
            package='td2',
            executable='boat_simulation_node',
            name='boat_simulation_node_2',
            namespace='boat2',
            output='screen',
            parameters=[LaunchConfiguration('params_file')]
        ),
        Node(
            package='td2',
            executable='boat_controller_node',
            name='boat_controller_node_2',
            namespace='boat2',
            output='screen'
        ),
        Node(
            package='td2',
            executable='boat_simulation_node',
            name='boat_simulation_node_3',
            namespace='boat3',
            output='screen',
            parameters=[LaunchConfiguration('params_file')]
        ),
        Node(
            package='td2',
            executable='boat_controller_node',
            name='boat_controller_node_3',
            namespace='boat3',
            output='screen'
        )
    ])
