cmake_minimum_required(VERSION 3.8)
project(battery_status_indication)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(example_interfaces REQUIRED)
find_package(my_robot_interfaces REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)


add_executable(battery_node src/battery_node.cpp)
ament_target_dependencies(battery_node rclcpp my_robot_interfaces)

add_executable(led_node src/led_node.cpp)
ament_target_dependencies(led_node rclcpp my_robot_interfaces)

install(TARGETS
  battery_node
  led_node
  DESTINATION lib/${PROJECT_NAME}
)
