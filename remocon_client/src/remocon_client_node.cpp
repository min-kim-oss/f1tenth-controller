#include <memory>
#include <iostream>
#include "remocon_client/remocon_client.hpp"

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
    auto node = rclcpp::Node::make_shared("remocon_client");
    RemoconClient remoconClient(node, ip, port, &cooked, &raw);

    signal(SIGINT,quit);

    remoconClient.keyLoop();
    cout<<"closing program"<<endl;
    quit(0);
    
    //rclcpp::shutdown();
    return 0;
}