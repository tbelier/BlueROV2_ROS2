#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "visualization_msgs/msg/marker.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.h"
#include <Eigen/Dense>

class BoatSimulation : public rclcpp::Node
{
public:
    BoatSimulation()
        : Node("boat_simulation_node"),
          state_(Eigen::Matrix<double, 3, 1>::Zero()),
          velocity_(1.0), // Vitesse constante
          dt_(0.04),      // Période de simulation (40 ms, soit 25 Hz)
          u1_(0.1)        // Commande de variation de cap initiale
    {
        init_parameters();
        init_interfaces();
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(static_cast<int>(dt_ * 1000)),
            std::bind(&BoatSimulation::simulate_once, this));
    }

private:
    void init_parameters()
    {
        this->declare_parameter<double>("initial_x", 0.0);
        this->declare_parameter<double>("initial_y", 0.0);
        this->declare_parameter<double>("initial_theta", 0.0);

        state_(0) = this->get_parameter("initial_x").as_double();
        state_(1) = this->get_parameter("initial_y").as_double();
        state_(2) = this->get_parameter("initial_theta").as_double();

        RCLCPP_INFO(this->get_logger(), "Initial parameters: x=%.2f, y=%.2f, theta=%.2f", state_(0), state_(1), state_(2));
    }

    void init_interfaces()
    {
        pose_publisher_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("boat_pose", 10);
        marker_publisher_ = this->create_publisher<visualization_msgs::msg::Marker>("boat_marker", 10);
        cmd_vel_subscription_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/cmd_vel", 10, std::bind(&BoatSimulation::cmd_vel_callback, this, std::placeholders::_1));
    }

    void cmd_vel_callback(const geometry_msgs::msg::Twist::SharedPtr msg)
    {
        u1_ = msg->angular.z;
        RCLCPP_INFO(this->get_logger(), "Received angular velocity command: %.2f", u1_);
    }

    void integration_euler(double u1, double dt)
    {
        Eigen::Matrix<double, 3, 1> dx;
        dx(0) = velocity_ * std::cos(state_(2)); // dx/dt = v * cos(theta)
        dx(1) = velocity_ * std::sin(state_(2)); // dy/dt = v * sin(theta)
        dx(2) = u1;                              // dtheta/dt = u1

        state_ += dx * dt; // Schéma d'Euler
    }

    void simulate_once()
    {
        integration_euler(u1_, dt_);

        auto pose_msg = geometry_msgs::msg::PoseStamped();
        pose_msg.header.stamp = this->now();
        pose_msg.header.frame_id = "map";

        pose_msg.pose.position.x = state_(0);
        pose_msg.pose.position.y = state_(1);
        pose_msg.pose.position.z = 0.0;

        tf2::Quaternion q;
        q.setRPY(0, 0, state_(2));
        pose_msg.pose.orientation = tf2::toMsg(q);

        pose_publisher_->publish(pose_msg);
        RCLCPP_INFO(this->get_logger(), "Published Pose: [x=%.2f, y=%.2f, theta=%.2f]", state_(0), state_(1), state_(2));

        publish_marker();
    }

    void publish_marker()
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
        marker.pose.position.z = 0.0;

        tf2::Quaternion q;
        q.setRPY(0, 0, state_(2));
        marker.pose.orientation = tf2::toMsg(q);

        marker.scale.x = 1.0;
        marker.scale.y = 1.0;
        marker.scale.z = 1.0;

        marker.color.r = 1.0f;
        marker.color.g = 0.0f;
        marker.color.b = 0.0f;
        marker.color.a = 1.0;

        marker.mesh_resource = "package://td2/meshes/boat.dae";

        marker_publisher_->publish(marker);
    }

    Eigen::Matrix<double, 3, 1> state_;
    double velocity_;
    double dt_;
    double u1_;
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr pose_publisher_;
    rclcpp::Publisher<visualization_msgs::msg::Marker>::SharedPtr marker_publisher_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_subscription_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BoatSimulation>());
    rclcpp::shutdown();
    return 0;
}
