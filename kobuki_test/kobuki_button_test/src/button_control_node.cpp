#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "kobuki_ros_interfaces/msg/button_event.hpp"

class RotateNode : public rclcpp::Node
{
public:
    RotateNode() : Node("rotate_node")
    {
        // Criando o publisher para enviar comandos de movimento
        publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/commands/velocity", 10);

        // Criando o timer que envia comandos de rotação a cada 100ms
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&RotateNode::publish_move_command, this));

        // Assinando o tópico /events/button para receber os eventos dos botões
        button_subscriber_ = this->create_subscription<kobuki_ros_interfaces::msg::ButtonEvent>(
            "/events/button", 10, std::bind(&RotateNode::button_callback, this, std::placeholders::_1));

        RCLCPP_INFO(this->get_logger(), "Ready to rotate on button press");
    }

private:
    // Callback para processar a entrada do botão
    void button_callback(const kobuki_ros_interfaces::msg::ButtonEvent::SharedPtr msg)
    {
        if (msg->state == 1) {  // Quando o botão for pressionado (estado 1)
            if (msg->button == 0) {  // B0 - rotaciona para a direita
                rotating_ = true;
                direction_ = -1.0;  // Sentido horário
                RCLCPP_INFO(this->get_logger(), "Button B0 pressed, rotating left");
            }
            else if (msg->button == 2) {  // B2 - rotaciona para a esquerda
                rotating_ = true;
                direction_ = 1.0;  // Sentido anti-horário
                RCLCPP_INFO(this->get_logger(), "Button B2 pressed, rotating right");
            }
            else if (msg->button == 1) {  // B1 - para a rotação
                rotating_ = false;
                RCLCPP_INFO(this->get_logger(), "Button B1 pressed, stopping rotation");
            }
        }
    }

    // Função para publicar o comando de rotação
    void publish_move_command()
    {
        geometry_msgs::msg::Twist cmd;

        if (rotating_) {
            // Se o botão foi pressionado e a rotação está ativa
            cmd.linear.x = 0.0;
            cmd.angular.z = direction_ * 1.0;  // Velocidade angular para rotação
        } else {
            // Se a rotação estiver parada
            cmd.linear.x = 0.0;
            cmd.angular.z = 0.0;
        }

        publisher_->publish(cmd);  // Publicando o comando
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::Subscription<kobuki_ros_interfaces::msg::ButtonEvent>::SharedPtr button_subscriber_;
    rclcpp::TimerBase::SharedPtr timer_;

    bool rotating_ = false;  // Flag que indica se o robô deve rotacionar
    double direction_ = 0.0;  // Direção da rotação: 1.0 para direita, -1.0 para esquerda
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<RotateNode>());
    rclcpp::shutdown();
    return 0;
}
