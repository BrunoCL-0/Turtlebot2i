# cmake_minimum_required(VERSION 3.8)
# project(kobuki_teleop_switch)

# if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#   add_compile_options(-Wall -Wextra -Wpedantic)
# endif()

# # Encontrar dependências
# find_package(ament_cmake REQUIRED)
# find_package(rclcpp REQUIRED)
# find_package(rclcpp_components REQUIRED)  # rclcpp_components necessário
# find_package(geometry_msgs REQUIRED)
# find_package(kobuki_ros_interfaces REQUIRED)
# find_package(teleop_twist_joy REQUIRED)
# find_package(kobuki_rotate_test REQUIRED)
# find_package(sensor_msgs REQUIRED)  # Adiciona a dependência do sensor_msgs

# # Criar o executável
# add_executable(teleop_switch src/teleop_switch.cpp)
# target_include_directories(teleop_switch PUBLIC
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
#   $<INSTALL_INTERFACE:include>
#   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../kobuki_rotate_test/include>  # Include do pacote kobuki_rotate_test
# )

# # Link correto das dependências
# ament_target_dependencies(teleop_switch
#   rclcpp
#   rclcpp_components  # rclcpp_components necessário
#   geometry_msgs
#   kobuki_ros_interfaces
#   teleop_twist_joy
#   kobuki_rotate_test
#   sensor_msgs  # Linka o sensor_msgs ao seu pacote
# )

# install(TARGETS
#   teleop_switch
#   DESTINATION lib/${PROJECT_NAME})

# # Testes
# if(BUILD_TESTING)
#   find_package(ament_lint_auto REQUIRED)
#   set(ament_cmake_copyright_FOUND TRUE)
#   set(ament_cmake_cpplint_FOUND TRUE)
#   ament_lint_auto_find_test_dependencies()
# endif()

# ament_package()
