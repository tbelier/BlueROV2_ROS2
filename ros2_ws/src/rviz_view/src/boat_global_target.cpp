#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/point.hpp"

class TargetPublisher : public rclcpp::Node
{
public:
    TargetPublisher() : Node("target_publisher_node")
    {
        target_publisher_ = this->create_publisher<geometry_msgs::msg::Point>("/target_position", 10);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(1000),
            std::bind(&TargetPublisher::publish_target, this));
    }

private:
    void publish_target()
    {
        auto target_msg = geometry_msgs::msg::Point();
        target_msg.x = 25.0;
        target_msg.y = 25.0;
        target_msg.z = 25.0;

        target_publisher_->publish(target_msg);
        RCLCPP_INFO(this->get_logger(), "Published target position: [x=%.2f, y=%.2f]", target_msg.x, target_msg.y);
    }

    rclcpp::Publisher<geometry_msgs::msg::Point>::SharedPtr target_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TargetPublisher>());
    rclcpp::shutdown();
    return 0;
}
