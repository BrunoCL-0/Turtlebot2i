cmake_minimum_required(VERSION 3.8)
project(kobuki_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Encontrar dependências
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(kobuki_ros_interfaces REQUIRED)

# Adicionar executável
add_executable(node_manager src/node_manager.cpp)

# Vincular dependências
ament_target_dependencies(node_manager
  rclcpp
  geometry_msgs
  kobuki_ros_interfaces
)

# Instalar o executável
install(TARGETS node_manager
  DESTINATION lib/${PROJECT_NAME}
)

# Instalar arquivos de lançamento
install(DIRECTORY launch
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
