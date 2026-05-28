from launch import LaunchDescription
from launch_ros.actions import Node
from launch.substitutions import Command
from launch_ros.parameter_descriptions import ParameterValue
from ament_index_python.packages import get_package_share_directory
import os


def generate_launch_description():
    #获取功能包路径 ultra_description_pkg
    ultra_description_pkg = get_package_share_directory("max_description")

    #使用xacro读取urdf文件中内容
    urdf_path = os.path.join(ultra_description_pkg,"urdf","max.urdf")
    robot_description = ParameterValue(Command(["xacro",' ', urdf_path]),value_type=str)
    

    # 发布机器人描述参数  robot_state_publisher：加载机器人urdf文件 
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_description}]
    )

    #joint_state_publisher:发布关节状态
    joint_state_publisher = Node(
        package="joint_state_publisher",
        executable="joint_state_publisher"
    )

    return LaunchDescription([
        robot_state_publisher,
        joint_state_publisher
    ])


