#include <memory>
#include <iostream>
#include "vesc_controller/vesc_controller.hpp"

int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    
    auto node = std::make_shared<VescController>(
        rclcpp::NodeOptions());
    //rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}