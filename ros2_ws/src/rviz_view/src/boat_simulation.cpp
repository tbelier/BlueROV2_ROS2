#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "visualization_msgs/msg/marker.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"
#include <Eigen/Dense>
#include "rviz_view/boat_simulation.hpp"
#define PI 3.141592653589793
BoatSimulation::BoatSimulation() : Node("boat_simulation_node"),
                                   state_(Eigen::Matrix<double, 6, 1>::Zero()),
                                   velocity_(1.0), // Vitesse constante
                                   dt_(1 / 60.0)   // Période de simulation (40 ms, soit 25 Hz)
{
    init_parameters();
    init_interfaces();
    // timer_ = this->create_wall_timer(
    //     std::chrono::milliseconds(static_cast<int>(dt_ * 1000)),
    //     std::bind(&BoatSimulation::simulate_once, this));

    roll_ = 0.0;
    pitch_ = 0.0;
    yaw_ = 0.0;
}

void BoatSimulation::init_parameters()
{
    this->declare_parameter<double>("initial_x", 0.0);
    this->declare_parameter<double>("initial_y", 0.0);
    this->declare_parameter<double>("initial_theta", 0.0);
    this->declare_parameter<double>("initial_roll", 0.0);
    this->declare_parameter<double>("initial_pitch", 0.0);
    this->declare_parameter<double>("initial_yaw", 0.0);

    state_(0) = this->get_parameter("initial_x").as_double();
    state_(1) = this->get_parameter("initial_y").as_double();
    state_(2) = this->get_parameter("initial_theta").as_double();
    state_(3) = this->get_parameter("initial_roll").as_double();
    state_(4) = this->get_parameter("initial_pitch").as_double();
    state_(5) = this->get_parameter("initial_yaw").as_double();

    RCLCPP_INFO(this->get_logger(), "Initial parameters position: x=%.2f, y=%.2f, theta=%.2f", state_(0), state_(1), state_(2));
    RCLCPP_INFO(this->get_logger(), "Initial parameters orientation: x=%.2f, y=%.2f, theta=%.2f", state_(3), state_(4), state_(5));
}

void BoatSimulation::init_interfaces()
{
    pose_publisher_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("boat_pose", 10);
    marker_publisher_ = this->create_publisher<visualization_msgs::msg::Marker>("boat_marker", 10);
    imu_subscription_ = this->create_subscription<geometry_msgs::msg::Twist>(
        "/sensor/attitude_twist", 10, std::bind(&BoatSimulation::imu_callback, this, std::placeholders::_1));
}

void BoatSimulation::imu_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
{
    roll_ = msg->angular.x;
    pitch_ = msg->angular.y;
    yaw_ = msg->angular.z;
    RCLCPP_INFO(this->get_logger(), "Received angular velocity command: %.2f,%.2f,%.2f", roll_, pitch_, yaw_);

    state_(3) = roll_ / 180 * PI;
    state_(4) = pitch_ / 180 * PI; // ATTENTION A CHECKER SI LES ANGLES SORTIS DE /sensor/attitude_twist sont en deg ou rad
    state_(5) = yaw_ / 180 * PI;

    simulate_once();
}

void BoatSimulation::simulate_once()
{
    auto pose_msg = geometry_msgs::msg::PoseStamped();
    pose_msg.header.stamp = this->now();
    pose_msg.header.frame_id = "map";

    pose_msg.pose.position.x = state_(0);
    pose_msg.pose.position.y = state_(1);
    pose_msg.pose.position.z = state_(2);
    pose_msg.pose.orientation.x = state_(3);
    pose_msg.pose.orientation.y = state_(4);
    pose_msg.pose.orientation.z = state_(5);

    tf2::Quaternion q;
    q.setRPY(state_(3), state_(4), state_(5));
    pose_msg.pose.orientation = tf2::toMsg(q);

    pose_publisher_->publish(pose_msg);
    RCLCPP_INFO(this->get_logger(), "Published Pose: [x=%.2f, y=%.2f, theta=%.2f]", state_(0), state_(1), state_(2));
    RCLCPP_INFO(this->get_logger(), "Published Orientation: [x=%.2f, y=%.2f, theta=%.2f]", state_(3), state_(4), state_(5));

    publish_marker();
}

void BoatSimulation::publish_marker()
{
    visualization_msgs::msg::Marker marker;
    marker.header.stamp = this->now();
    marker.header.frame_id = "map";
    marker.ns = "boat_marker";
    marker.id = 0;
    marker.type = visualization_msgs::msg::Marker::MESH_RESOURCE;
    marker.action = visualization_msgs::msg::Marker::ADD;

    marker.pose.position.x = state_(0);
    marker.pose.position.y = state_(1);
    marker.pose.position.z = state_(2);

    tf2::Quaternion q;
    q.setRPY(state_(3), state_(4), state_(5));
    marker.pose.orientation = tf2::toMsg(q);

    marker.scale.x = 2.0;
    marker.scale.y = 2.0;
    marker.scale.z = 2.0;

    marker.color.r = 0.0f;
    marker.color.g = 0.0f;
    marker.color.b = 1.0f;
    marker.color.a = 1.0;

    marker.mesh_resource = "package://rviz_view/meshes/bluerovLight.dae";

    marker_publisher_->publish(marker);
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BoatSimulation>());
    rclcpp::shutdown();
    return 0;
}
