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
            get_package_share_directory('rviz_view'),
            'config',
            'rviz_view.yaml'
        )
    )

    return LaunchDescription([
        declare_params,
        Node(
            package='rviz_view',
            executable='target_publisher_node',
            name='target_publisher_node',
            output='screen'
        ),
        Node(
            package='rviz_view',
            executable='boat_simulation_node',
            name='simulation',
            output='screen',
            parameters=[LaunchConfiguration('params_file')]
        ),
        Node(
            package='rviz_view',
            executable='boat_controller_node',
            name='boat_controller_node',
            output='screen'
        )
    ])
