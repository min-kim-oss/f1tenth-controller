#include <memory>
#include <iostream>
#include "kbd_reader/kbd_reader.hpp"

using namespace std;

int main(int argc, char** argv)
{
    char port[6];
    char ip[16];
    cout << "write ip to connect : ";
    cin >> ip;
    cout << "write port to connect : ";
    cin >> port;

    rclcpp::init(argc,argv);
    auto node = rclcpp::Node::make_shared("kbd_reader");
    KbdReader kbdReader(node, ip, port);

    kbdReader.keyLoop();

    rclcpp::shutdown();
    return 0;
}