#include <memory>
#include <iostream>
#include "kbd_reader/kbd_reader.hpp"

using namespace std;

struct termios cooked, raw;

void quit(int sig)
{
    tcsetattr(0, TCSANOW, &cooked);
    rclcpp::shutdown();
    exit(0);
}


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
    KbdReader kbdReader(node, ip, port, &cooked, &raw);

    signal(SIGINT,quit);

    kbdReader.keyLoop();

    rclcpp::shutdown();
    return 0;
}