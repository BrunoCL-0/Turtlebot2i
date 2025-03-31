import launch
import launch.actions
import launch_ros.actions
from launch_ros.substitutions import FindPackageShare
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():
    # Incluir o launch do kobuki_node
    kobuki_node_launch = launch.actions.IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            FindPackageShare("kobuki_node"), "/launch/kobuki_node-launch.py"
        ])
    )

    # Incluir o launch do teleop_twist_joy
    teleop_launch = launch.actions.IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            FindPackageShare("teleop_twist_joy"), "/launch/teleop-launch.py"
        ]),
        # Passando parâmetros específicos para o launch do teleop
        launch_arguments={
            "config_filepath": "/home/turtlebot/ros2_ws/src/Turtlebot2i/kobuki_test/kobuki_controller/pdp.config.yaml",
            "joy_vel": "/joy/velocity"
        }.items()
    )

    # Node do teste da bateria (se necessário)
    battery_test_node = launch_ros.actions.Node(
        package="kobuki_battery_test",
        executable="kobuki_battery_test",
        name="battery_test"
    )

    # Adicionar o node_manager
    node_manager = launch_ros.actions.Node(
        package="kobuki_controller",
        executable="node_manager",
        name="node_manager"
    )

    return launch.LaunchDescription([
        kobuki_node_launch,
        teleop_launch,  # Incluir o launch do teleop
        battery_test_node,
        node_manager  # Incluir o node_manager
    ])
