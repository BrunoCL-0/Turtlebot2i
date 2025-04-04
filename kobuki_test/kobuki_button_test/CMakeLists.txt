cmake_minimum_required(VERSION 3.8)
project(kobuki_button_test)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(kobuki_ros_interfaces REQUIRED)


# Add executable for the button control node
add_executable(button_control_node src/button_control_node.cpp)

# Link dependencies to the executable
ament_target_dependencies(button_control_node
  rclcpp
  geometry_msgs
  kobuki_ros_interfaces
 
)

# Install the executable
install(TARGETS
  button_control_node
  DESTINATION lib/${PROJECT_NAME}
)

# For testing (optional)
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # Skip the linter checking for copyrights if necessary
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
