#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "kobuki_ros_interfaces/msg/button_event.hpp"

class TeleopControlNode : public rclcpp::Node
{
public:
    TeleopControlNode() : Node("teleop_control_node"), is_teleop_active_(false), is_rotate_active_(false), first_b1_press_(true)
    {
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/commands/velocity", 10);

        button_subscriber_ = this->create_subscription<kobuki_ros_interfaces::msg::ButtonEvent>(
            "/events/button", 10,
            std::bind(&TeleopControlNode::button_switch, this, std::placeholders::_1));

        joy_subscriber_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/joy/velocity", 10,
            std::bind(&TeleopControlNode::move_joy, this, std::placeholders::_1));

        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&TeleopControlNode::publish_velocity, this));

        RCLCPP_INFO(this->get_logger(), "Teleop Control Node started");
    }

private:
    void button_switch(const kobuki_ros_interfaces::msg::ButtonEvent::SharedPtr msg)
    {
        if (msg->state == 1) // Quando o botão for pressionado
        {
            if (msg->button == 0) // B0 ativa/desativa o teleop
            {
                is_teleop_active_ = !is_teleop_active_;
                RCLCPP_INFO(this->get_logger(), "Joystick control %s", is_teleop_active_ ? "activated" : "deactivated");
                if (is_rotate_active_)
                {
                    is_rotate_active_ = false;
                    RCLCPP_INFO(this->get_logger(), "Sequence movement deactivated due to B0 press");
                }
            }
            else if (msg->button == 1) // B1 ativa/desativa o movimento predefinido
            {
                if (is_teleop_active_)
                {
                    is_teleop_active_ = false;
                    RCLCPP_INFO(this->get_logger(), "Joystick control deactivated due to B1 press");
                }
                if (first_b1_press_ || !is_rotate_active_)
                {
                    first_b1_press_ = false;
                    is_rotate_active_ = true;
                    start_time_ = this->now();
                    RCLCPP_INFO(this->get_logger(), "Sequence movement activated");
                }
                else
                {
                    is_rotate_active_ = false;
                    RCLCPP_INFO(this->get_logger(), "Sequence movement deactivated");
                }
            }
        }
    }

    void move_joy(const geometry_msgs::msg::Twist::SharedPtr msg)
    {
        last_joy_cmd_ = *msg;
        joy_received_ = true;
    }

    void move_sequence()
    {
        auto now = this->now();
        rclcpp::Duration elapsed = now - start_time_;
        geometry_msgs::msg::Twist cmd;

        if (elapsed.seconds() < 2.0)
        {
            cmd.linear.x = 0.2;
            cmd.angular.z = 0.0;
        }
        else if (elapsed.seconds() < 4.0)
        {
            cmd.linear.x = 0.0;
            cmd.angular.z = 2.0;
        }
        else if (elapsed.seconds() < 6.0)
        {
            cmd.linear.x = 0.2;
            cmd.angular.z = 0.0;
        }
        else if (elapsed.seconds() < 11.0) // Parado por 5 segundos
        {
            cmd.linear.x = 0.0;
            cmd.angular.z = 0.0;
        }
        else
        {
            start_time_ = this->now(); // Reinicia o ciclo
            RCLCPP_INFO(this->get_logger(), "Restarting movement sequence...");
        }

        publisher_->publish(cmd);
    }

    void publish_velocity()
    {
        if (is_teleop_active_ && joy_received_)
        {
            publisher_->publish(last_joy_cmd_);
        }
        else if (is_rotate_active_)
        {
            move_sequence();
        }
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::Subscription<kobuki_ros_interfaces::msg::ButtonEvent>::SharedPtr button_subscriber_;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr joy_subscriber_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Time start_time_;

    bool is_teleop_active_;
    bool is_rotate_active_;
    bool joy_received_ = false;
    bool first_b1_press_;
    geometry_msgs::msg::Twist last_joy_cmd_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<TeleopControlNode>());
    rclcpp::shutdown();
    return 0;
}
