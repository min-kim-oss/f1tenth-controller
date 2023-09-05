cmake_minimum_required(VERSION 3.5)
project(remocon_client)

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

ament_auto_add_library(RemoconClient SHARED
  src/remocon_client.cpp
)

target_compile_features(RemoconClient PRIVATE cxx_std_17)
ament_target_dependencies(RemoconClient rclcpp rcl_interfaces
  std_msgs )
target_include_directories(RemoconClient
  PUBLIC
  #$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  #$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_executable(RemoconClient_node src/remocon_client_node.cpp)
ament_target_dependencies(RemoconClient_node rclcpp)
target_link_libraries(RemoconClient_node RemoconClient)

install(TARGETS
  RemoconClient
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS
  RemoconClient_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()
