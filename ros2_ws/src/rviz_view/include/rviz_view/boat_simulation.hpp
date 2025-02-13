#ifndef BOAT_SIMULATION_HPP
#define BOAT_SIMULATION_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <geometry_msgs/msg/pose.hpp>
#include <visualization_msgs/msg/marker.hpp>
#include <tf2_geometry_msgs/tf2_geometry_msgs.hpp>
#include <Eigen/Dense>

class BoatSimulation : public rclcpp::Node
{
public:
    BoatSimulation();

private:
    void init_parameters();
    void init_interfaces();
    // void simulate_once();
    void publish_marker();
    void pose_callback(const geometry_msgs::msg::PoseStamped::SharedPtr msg);

    Eigen::Matrix<double, 6, 1> state_;
    double velocity_;
    double dt_;
    double roll_;
    double pitch_;
    double yaw_;

    double q1_;
    double q2_;
    double q3_;
    double w_;

    double x_;
    double y_;
    double z_;

    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr pose_publisher_;
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_publisher_;
    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr pose_subscription_;
    rclcpp::TimerBase::SharedPtr timer_;
};

#endif // BOAT_SIMULATION_HPP
