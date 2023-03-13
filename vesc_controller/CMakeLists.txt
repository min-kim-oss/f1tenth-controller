cmake_minimum_required(VERSION 3.5)
project(vesc_controller)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_auto REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rcl_interfaces REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

include_directories(include)

ament_auto_add_library(VescController SHARED
  src/vesc_controller.cpp
)


target_compile_features(VescController PRIVATE cxx_std_17)
ament_target_dependencies(VescController rclcpp rcl_interfaces
  std_msgs)
target_include_directories(VescController
  PUBLIC
  #$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  #$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_executable(VescController_node src/vesc_controller_node.cpp)
ament_target_dependencies(VescController_node rclcpp)
target_link_libraries(VescController_node VescController)


install(TARGETS
  VescController
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS
  VescController_node
  DESTINATION lib/${PROJECT_NAME})


ament_package()
