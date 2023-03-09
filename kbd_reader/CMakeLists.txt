cmake_minimum_required(VERSION 3.5)
project(kbd_reader)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_auto REQUIRED)
find_package(rclcpp_interfaces REQUIRED)
# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

include_directories(include)

ament_auto_add_libray(kbd_reader SHARED
  src/kbd_reader.cpp
)

target_compile_features(kbd_reader PRIVATE cxx_std_17)
ament_target_dependencies(kbd_reader rclcpp rcl_interfaces
  std_msgs )
target_include_directories(kbd_reader
  PUBLIC
  #$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  #$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

add_executable(kbd_reader_node src/kbd_reader_node.cpp)
ament_target_dependencies(kbd_reader_node rclcpp)
target_link_libraries(kbd_reader_node kbd_reader)

install(TARGETS
  kbd_reader
  DESTINATION lib/${PROJECT_NAME})

install(TARGETS
  kbd_reader_node
  DESTINATION lib/${PROJECT_NAME})
  
ament_package()
