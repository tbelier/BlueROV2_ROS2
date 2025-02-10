#ifndef BOAT_SIMULATION_HPP
#define BOAT_SIMULATION_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <Eigen/Dense>

class BoatSimulation : public rclcpp::Node {
public:
    BoatSimulation();

private:
    void init_parameters();
    void init_interfaces();
    void integration_euler(double u1, double dt);
    void simulate_once();
    void cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr msg);
    void publish_marker();

    Eigen::Matrix<double, 3, 1> state_;
    double velocity_;
    double dt_;
    double u1_;

    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr pose_publisher_;
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_publisher_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_subscription_;
    rclcpp::TimerBase::SharedPtr timer_;
};

#endif // BOAT_SIMULATION_HPP
