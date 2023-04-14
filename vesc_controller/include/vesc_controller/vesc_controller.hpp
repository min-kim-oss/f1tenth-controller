#include "time.h"
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include <std_msgs/msg/float64.hpp>

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

#define KEYCODE_Q 0x71

#define CONTROL_PERIOD 0.1

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

        //for vesc controll
        float max_rsteer_arg;
        float max_lsteer_arg;
        float max_accel_arg;
        float max_brake_arg;

        int max_hit_count;
        float hit_increment;

        float cur_steer_arg;
        float cur_accel_arg;
        float cur_brake_arg;

        int accel_hitCount;
        int steer_hitCount;
        int brake_hitCount;

        //for ROS services
        rclcpp::TimerBase::SharedPtr timer_control_;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr steer_pub_;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr accel_pub_;
        rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr brake_pub_;


    public:
        VescController(const rclcpp::NodeOptions &);
        //~VescController();

        void SocketSetter();
        void socketTread();
        void acceptingClient();
        
        void ReceiveKey();
        void keyHandler(char r_key);

        void timerCallBack();
        void hitRecoverer();
        void cmd_creator(int accel_hitCount, int steer_hitCount, int brake_hitCount);
        void cmd_publisher(float cur_accel_arg, float cur_steer_arg, float cur_brake_arg);

};
