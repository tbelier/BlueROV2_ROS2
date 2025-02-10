#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/point.hpp"
#include <cmath>
#include <string>

class BoatController : public rclcpp::Node {
public:
    BoatController() : Node("boat_controller_node") {
        // Récupérer le namespace actuel
        namespace_ = this->get_namespace();
        RCLCPP_INFO(this->get_logger(), "BoatController namespace: %s", namespace_.c_str());

        // Initialiser les cibles avec un décalage en fonction du namespace
        namespace_id_ = std::stoi(namespace_.substr(namespace_.size() - 1));
        target_offset_x_ = 0.5 * (namespace_id_ - 1);
        target_offset_y_ = 0.5 * (namespace_id_ - 1);

        // Souscrire au topic de position de la cible
        target_subscription_ = this->create_subscription<geometry_msgs::msg::Point>(
            "target", 10, std::bind(&BoatController::target_callback, this, std::placeholders::_1));

        // Souscrire au topic de la pose du bateau
        pose_subscription_ = this->create_subscription<geometry_msgs::msg::PoseStamped>(
            "boat_pose", 10, std::bind(&BoatController::pose_callback, this, std::placeholders::_1));

        // Publier les commandes de vitesse
        cmd_vel_publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);

        // Initialiser la cible
        target_x_ = 0.0;
        target_y_ = 0.0;

        // Timer pour la boucle de contrôle
        control_timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&BoatController::control_loop, this));
    }

private:
    void target_callback(const geometry_msgs::msg::Point::SharedPtr msg) {
        // Ajouter un décalage spécifique au namespace pour éviter la superposition
        target_x_ = msg->x + target_offset_x_;
        target_y_ = msg->y + target_offset_y_;
        RCLCPP_INFO(this->get_logger(), "Target updated: x=%.2f, y=%.2f", target_x_, target_y_);
    }

    void pose_callback(const geometry_msgs::msg::PoseStamped::SharedPtr msg) {
        // Mettre à jour la position actuelle
        current_x_ = msg->pose.position.x;
        current_y_ = msg->pose.position.y;

        // Calculer l'orientation actuelle (theta) à partir du quaternion
        double qx = msg->pose.orientation.x;
        double qy = msg->pose.orientation.y;
        double qz = msg->pose.orientation.z;
        double qw = msg->pose.orientation.w;
        current_theta_ = std::atan2(2.0 * (qw * qz + qx * qy), 1.0 - 2.0 * (qy * qy + qz * qz));
    }

    void control_loop() {
        // Calculer la distance à la cible
        double distance_to_target = std::hypot(target_x_ - current_x_, target_y_ - current_y_);

        // Si le bateau est proche de la cible, arrêter le mouvement
        if (distance_to_target < 0.5) {
            RCLCPP_INFO(this->get_logger(), "Boat reached target! Stopping.");
            auto stop_msg = geometry_msgs::msg::Twist();
            cmd_vel_publisher_->publish(stop_msg);
            return;
        }

        // Calculer l'angle vers la cible
        double desired_theta = std::atan2(target_y_ - current_y_, target_x_ - current_x_);
        double error_theta = desired_theta - current_theta_;

        // Normaliser l'angle pour qu'il reste dans [-π, π]
        error_theta = std::fmod(error_theta + M_PI, 2 * M_PI) - M_PI;

        // Contrôle proportionnel pour l'angle et la vitesse linéaire
        double angular_velocity = 1.0 * error_theta; // Gain proportionnel pour la rotation
        double linear_velocity = std::min(1.0, 0.2 * distance_to_target); // Vitesse limitée

        // Publier les commandes de vitesse
        auto twist_msg = geometry_msgs::msg::Twist();
        twist_msg.angular.z = angular_velocity;
        twist_msg.linear.x = linear_velocity;
        cmd_vel_publisher_->publish(twist_msg);

        // Log des informations de contrôle
        RCLCPP_INFO(this->get_logger(),
                    "Target: (%.2f, %.2f), Pose: (%.2f, %.2f, %.2f), Error: %.2f, Linear: %.2f, Angular: %.2f",
                    target_x_, target_y_, current_x_, current_y_, current_theta_,
                    error_theta, linear_velocity, angular_velocity);
    }

    // Variables
    rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr target_subscription_;
    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr pose_subscription_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_publisher_;
    rclcpp::TimerBase::SharedPtr control_timer_;

    // Informations sur la cible et le déplacement
    double target_x_, target_y_;
    double current_x_, current_y_, current_theta_;
    double target_offset_x_, target_offset_y_;
    int namespace_id_;
    std::string namespace_;
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<BoatController>());
    rclcpp::shutdown();
    return 0;
}
