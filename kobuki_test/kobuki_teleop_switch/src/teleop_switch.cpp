// #include "rclcpp/rclcpp.hpp"
// #include "geometry_msgs/msg/twist.hpp"
// #include "kobuki_ros_interfaces/msg/button_event.hpp"
// #include <cstdlib>  // Para std::system()

// class TeleopSwitchNode : public rclcpp::Node
// {
// public:
//     TeleopSwitchNode() : Node("teleop_switch_node"), is_moving_(false), is_teleop_running_(false), is_rotate_running_(false)
//     {
//         // Criando publisher para enviar comandos de velocidade
//         publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("/commands/velocity", 10);

//         // Assinando o tópico /events/button para receber os eventos dos botões
//         button_subscriber_ = this->create_subscription<kobuki_ros_interfaces::msg::ButtonEvent>(
//             "/events/button", 10,
//             std::bind(&TeleopSwitchNode::button_callback, this, std::placeholders::_1));

//         RCLCPP_INFO(this->get_logger(), "Teleop switch node started");
//     }

// private:
//     void button_callback(const kobuki_ros_interfaces::msg::ButtonEvent::SharedPtr msg)
//     {
//         if (msg->state == 1) // Quando o botão for pressionado
//         {
//             if (msg->button == 0)  // Botão B0
//             {
//                 if (is_moving_)
//                 {
//                     RCLCPP_INFO(this->get_logger(), "Button B0 pressed: Stopping joystick teleop");
//                     stop_joystick_control();
//                 }
//                 else
//                 {
//                     RCLCPP_INFO(this->get_logger(), "Button B0 pressed: Starting joystick teleop");
//                     start_joystick_control();
//                 }
//                 // Alterna o estado de movimento
//                 is_moving_ = !is_moving_;
//             }
//             else if (msg->button == 1)  // Botão B1
//             {
//                 if (is_rotate_running_)
//                 {
//                     RCLCPP_INFO(this->get_logger(), "Button B1 pressed: Stopping rotate node");
//                     stop_rotate_control();
//                 }
//                 else
//                 {
//                     RCLCPP_INFO(this->get_logger(), "Button B1 pressed: Starting rotate node");
//                     start_rotate_control();
//                 }
//             }
//         }
//     }

//     void start_joystick_control()
//     {
//         if (!is_teleop_running_) // Garante que só inicie se não estiver rodando
//         {
//             RCLCPP_INFO(this->get_logger(), "Starting joystick teleop node...");
//             std::string command = "ros2 launch teleop_twist_joy teleop-launch.py config_filepath:=" 
//                                   "\"/home/turtlebot/ros2_ws/src/kobuki_test/kobuki_controller/pdp.config.yaml\" "
//                                   "joy_vel:=\"/commands/velocity\" &";
//             std::system(command.c_str());  // Inicia o teleop com os parâmetros
//             is_teleop_running_ = true;
//         }
//     }

//     void stop_joystick_control()
//     {
//         if (is_teleop_running_)  // Verifica se o processo está rodando antes de tentar parar
//         {
//             RCLCPP_INFO(this->get_logger(), "Stopping joystick teleop node...");
//             std::system("pkill -f teleop-launch.py");  // Para o teleop, matando o processo do teleop
//             is_teleop_running_ = false;
//         }
//     }

//     void start_rotate_control()
//     {
//         if (!is_rotate_running_) // Garante que só inicie se não estiver rodando
//         {
//             RCLCPP_INFO(this->get_logger(), "Starting rotate node...");
//             std::system("ros2 run kobuki_rotate_test rotate &");  // Inicia o rotate
//             is_rotate_running_ = true;
//         }
//     }

//     void stop_rotate_control()
//     {
//         if (is_rotate_running_) // Verifica se o processo está rodando antes de tentar parar
//         {
//             RCLCPP_INFO(this->get_logger(), "Stopping rotate node...");
//             std::system("pkill -f rotate");  // Para o rotate, matando o processo do rotate
//             is_rotate_running_ = false;
//         }
//     }

//     rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
//     rclcpp::Subscription<kobuki_ros_interfaces::msg::ButtonEvent>::SharedPtr button_subscriber_;
//     bool is_moving_;  // Flag para controlar o estado de movimento (mover/parar)
//     bool is_teleop_running_;  // Flag para verificar se o teleop está rodando
//     bool is_rotate_running_;  // Flag para verificar se o rotate está rodando
// };

// int main(int argc, char **argv)
// {
//     rclcpp::init(argc, argv);
//     rclcpp::spin(std::make_shared<TeleopSwitchNode>());
//     rclcpp::shutdown();
//     return 0;
// }
