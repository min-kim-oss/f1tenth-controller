#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>

#include "rclcpp/rclcpp.hpp"

#include "string.h"
#include "arpa/inet.h"
#include "sys/socket.h"

/**************************************************************/
/************************ key code ****************************/
/**************************************************************/

#define KEYCODE_A 0x61
#define KEYCODE_W 0x77
#define KEYCODE_S 0x73
#define KEYCODE_D 0x64


class KbdReader
{
    private:
        std::shared_ptr<rclcpp::Node> nh_;

        //for socket connetion to vehicle
        struct sockaddr_in addr_vehicle;
        int client_sock;

        //for reading key code from console
        int kfd;
        struct termios cooked, raw;
        bool key_dirty;

    public:        
        KbdReader(std::shared_ptr<rclcpp::Node> nh, char* ip, char* port);
        int keyLoop();
        void key_sender(char key);
};
