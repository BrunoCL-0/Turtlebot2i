#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/battery_state.hpp"
#include "kobuki_ros_interfaces/msg/led.hpp"
#include "kobuki_ros_interfaces/msg/sound.hpp"
#include <csignal>

using std::placeholders::_1;

class BatteryMonitor : public rclcpp::Node
{
public:
  BatteryMonitor()
  : Node("kobuki_battery_test"), led_state_(false), warning_triggered_(false), sound_timer_started_(false), status_printed_(false)
  {
    battery_sub_ = this->create_subscription<sensor_msgs::msg::BatteryState>(
      "/sensors/battery_state", 10, std::bind(&BatteryMonitor::battery_callback, this, _1));

    led1_pub_ = this->create_publisher<kobuki_ros_interfaces::msg::Led>("/commands/led1", 10);
    led2_pub_ = this->create_publisher<kobuki_ros_interfaces::msg::Led>("/commands/led2", 10);
    sound_pub_ = this->create_publisher<kobuki_ros_interfaces::msg::Sound>("/commands/sound", 10);
  }

  void turn_off_leds()
  {
    RCLCPP_INFO(this->get_logger(), "Desligando LEDs antes do encerramento...");

    if (led_timer_) led_timer_->cancel();
    if (sound_delay_timer_) sound_delay_timer_->cancel();
    if (sound_loop_timer_) sound_loop_timer_->cancel();

    auto led_msg = kobuki_ros_interfaces::msg::Led();
    led_msg.value = led_msg.BLACK; // Apagar LEDs

    led1_pub_->publish(led_msg);
    led2_pub_->publish(led_msg);

    RCLCPP_INFO(this->get_logger(), "LEDs desligados com sucesso.");
  }

private:
  void battery_callback(const sensor_msgs::msg::BatteryState::SharedPtr msg)
  {
    if (msg->percentage <= 0.20 && !warning_triggered_) {
      RCLCPP_INFO(this->get_logger(), "Analyzing battery...");
      RCLCPP_WARN(this->get_logger(), "Warning! Low battery!");
      warning_triggered_ = true;

      // Timer para piscar LEDs
      led_timer_ = this->create_wall_timer(
        std::chrono::seconds(1),
        std::bind(&BatteryMonitor::blink_leds, this));

      // Timer para iniciar som após 10s
      sound_delay_timer_ = this->create_wall_timer(
        std::chrono::seconds(10),
        std::bind(&BatteryMonitor::start_sound_timer, this));
    } else if (msg->percentage > 0.20 && !status_printed_) {
      RCLCPP_INFO(this->get_logger(), "Battery level is sufficient.");
      status_printed_ = true;
    }
  }

  void blink_leds()
  {
    auto led_msg = kobuki_ros_interfaces::msg::Led();
    led_msg.value = led_state_ ? led_msg.RED : led_msg.BLACK;

    led1_pub_->publish(led_msg);
    led2_pub_->publish(led_msg);

    led_state_ = !led_state_;
  }

  void start_sound_timer()
  {
    if (!sound_timer_started_) {
      sound_timer_started_ = true;
      sound_loop_timer_ = this->create_wall_timer(
        std::chrono::seconds(2),
        std::bind(&BatteryMonitor::play_warning_sound, this));
    }
  }

  void play_warning_sound()
  {
    auto sound_msg = kobuki_ros_interfaces::msg::Sound();
    sound_msg.value = sound_msg.RECHARGE;
    sound_pub_->publish(sound_msg);
  }

  rclcpp::Subscription<sensor_msgs::msg::BatteryState>::SharedPtr battery_sub_;
  rclcpp::Publisher<kobuki_ros_interfaces::msg::Led>::SharedPtr led1_pub_;
  rclcpp::Publisher<kobuki_ros_interfaces::msg::Led>::SharedPtr led2_pub_;
  rclcpp::Publisher<kobuki_ros_interfaces::msg::Sound>::SharedPtr sound_pub_;

  rclcpp::TimerBase::SharedPtr led_timer_;
  rclcpp::TimerBase::SharedPtr sound_delay_timer_;
  rclcpp::TimerBase::SharedPtr sound_loop_timer_;

  bool led_state_;
  bool warning_triggered_;
  bool sound_timer_started_;
  bool status_printed_;
};

std::shared_ptr<BatteryMonitor> node_ptr = nullptr;

void signal_handler(int signum)
{
  (void)signum;  // Evita warning de parâmetro não utilizado

  if (node_ptr && rclcpp::ok()) {
    node_ptr->turn_off_leds();
    node_ptr.reset(); // Libera a memória do nó antes de encerrar
    rclcpp::shutdown();
  }
}

int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  node_ptr = std::make_shared<BatteryMonitor>();

  // Registrar manipulador de sinal para SIGINT (Ctrl + C)
  std::signal(SIGINT, signal_handler);

  rclcpp::spin(node_ptr);
  node_ptr.reset(); // Libera recursos corretamente antes de encerrar
  rclcpp::shutdown();
  return 0;
}
