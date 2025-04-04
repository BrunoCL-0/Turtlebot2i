cmake_minimum_required(VERSION 3.8)
project(kobuki_battery_test)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Encontrar dependências
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(kobuki_ros_interfaces REQUIRED)

# Adicionar o executável
add_executable(kobuki_battery_test src/kobuki_battery_test.cpp)
ament_target_dependencies(kobuki_battery_test rclcpp sensor_msgs kobuki_ros_interfaces)

# Instalar binário
install(TARGETS kobuki_battery_test
  DESTINATION lib/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
