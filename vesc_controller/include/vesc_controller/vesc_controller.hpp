#include "time.h"
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>

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


class VescController : public rclcpp::Node
{
    private:

        //for socket connetion
        struct sockaddr_in addr_server;
        struct sockaddr_in addr_client;
        socklen_t addr_client_len;
        int server_sock;
        int client_sock;
        char r_buff[256];
        char r_key;

    public:
        VescController(const rclcpp::NodeOptions &);
        void SocketSetting();
        void ReceiveKey();

};
