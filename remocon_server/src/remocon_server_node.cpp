#include <memory>
#include <iostream>
#include "remocon_server/remocon_server.hpp"


int main(int argc, char** argv)
{
    rclcpp::init(argc, argv);
    
    auto node = std::make_shared<RemoconServer>(
        rclcpp::NodeOptions());

    rclcpp::spin(node);

    rclcpp::shutdown();
    return 0;
}