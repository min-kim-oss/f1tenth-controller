#include "vesc_controller/vesc_controller.hpp"

VescController::VescController(const rclcpp::NodeOptions & options)
  :  rclcpp::Node("vesc_controller", options) 
{
    //setting vesc parameter
    max_rsteer_arg = 1;
    max_lsteer_arg = -1;
    max_accel_arg = 3000;
    max_brake_arg = 200000;

    max_hit_count = 150;
    hit_increment = 1.0;

    cur_steer_arg = 0;
    cur_accel_arg = 0;
    cur_brake_arg = 0;

    accel_hitCount = 0;
    steer_hitCount = 0;
    brake_hitCount = 0;

    accel_pub_ = this->create_publisher<std_msgs::msg::Float64>(
        "commands/motor/speed",1);
    steer_pub_ = this->create_publisher<std_msgs::msg::Float64>(
        "commands/servo/position",1);
    brake_pub_ = this->create_publisher<std_msgs::msg::Float64>(
        "commands/motor/brake",1);

    // Timer
    {
        const auto period_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
          std::chrono::duration<double>(CONTROL_PERIOD));
        timer_control_ = rclcpp::create_timer(
          this,get_clock(), period_ns, std::bind(&VescController::timerCallBack, this));
    }

    addr_server = {};
    addr_client = {};

    std::thread t1(&VescController::SocketSetter, this);
    t1.detach();
}


void VescController::SocketSetter()
{
    while(true){
        SocketSetting();
        ReceiveKey();
    }    
}

void VescController::SocketSetting()
{
    char port[6] = "9000";
    //std::cout << "write port for connection : ";
    //std::cin >> port;

    memset(&addr_server, 0 , sizeof(addr_server));
    addr_server.sin_family = AF_INET;
    addr_server.sin_addr.s_addr = htons(INADDR_ANY);
    addr_server.sin_port = htons(atoi(port));
    
    server_sock = socket(AF_INET, SOCK_STREAM,0);
    if(server_sock == -1){
        std::cout<<"creating socket error"<<std::endl;
        std::cout<<"closing socket"<<std::endl;
        close(server_sock);
        exit(1);
    }

    if(bind(server_sock, (sockaddr*)&addr_server, sizeof(addr_server)) == -1){
        std::cout<<"binding socket error"<<std::endl;
        std::cout<<"closing socket"<<std::endl;
        close(server_sock);
        exit(1);
    }

    if(listen(server_sock, 5) == -1){
        std::cout<<"listening error"<<std::endl;
        std::cout<<"closing socket"<<std::endl;
        close(server_sock);
        exit(1);
    }

    addr_client_len = sizeof(addr_client_len);

    client_sock = accept(server_sock, (sockaddr*) &addr_client, &addr_client_len);
    if(client_sock == -1){
        std::cout<<"accepting error"<<std::endl;
        std::cout<<"closing socket"<<std::endl;
        close(server_sock);
        exit(1);
    }

}

void VescController::ReceiveKey()
{
    while(true){
        memset(r_buff, 0 ,256);
        int read_chk = 0;
        read_chk == read(client_sock, r_buff, 1);

        if(read_chk == -1){
            std::cout<<"reading error"<<std::endl;
        }
        else{
            r_buff[strlen(r_buff)] = '\n';
        
        }
        r_key = r_buff[0];
        std::cout<<"received key : "<< r_key <<std::endl;

        keyHandler(r_key);
    }
}

void VescController::keyHandler(char r_key){
    switch(r_key)
    {
        case KEYCODE_W:
            std::cout << "w key" << std::endl;
            if(accel_hitCount + hit_increment < max_hit_count){
                accel_hitCount += hit_increment;
            }
            break;
        case KEYCODE_S:
            std::cout << "s key" << std::endl;
            brake_hitCount = max_hit_count;
            accel_hitCount = 0;
            break;
        case KEYCODE_A:
            std::cout << "a key" << std::endl;
            if(steer_hitCount - (2 * hit_increment) > -max_hit_count){
                steer_hitCount -= (2 * hit_increment);
            }
            break;
        case KEYCODE_D:
            std::cout << "d key" << std::endl;
            if(steer_hitCount + (2 * hit_increment) < max_hit_count){
                steer_hitCount += (2 * hit_increment);
            }
            break;
    }
    cmd_creator(accel_hitCount, steer_hitCount, brake_hitCount);
}

void VescController::timerCallBack()
{
    hitRecoverer();
}

void VescController::hitRecoverer()
{
    //recovering accel hit count
    if(accel_hitCount > 0)
    {
        accel_hitCount -= hit_increment;
    }
    else{
        accel_hitCount = 0;
    }

    //recovering steer hit count
    if(steer_hitCount > hit_increment)
    {
        steer_hitCount -= (2 * hit_increment);        
    }
    else if(steer_hitCount < -hit_increment)
    {
        steer_hitCount += (2 * hit_increment);
    }
    else
    {
        steer_hitCount = 0;
    }

    //recovering brake hit count
    brake_hitCount = 0;

    cmd_creator(accel_hitCount, steer_hitCount, brake_hitCount);
}

void VescController::cmd_creator(int accel_hitCount, int steer_hitCount, int brake_hitCount)
{
    cur_accel_arg = max_accel_arg * ((float)accel_hitCount / max_hit_count);
    cur_steer_arg = 0.5 + 0.5 * ((float)steer_hitCount / max_hit_count);
    cur_brake_arg = max_brake_arg * ((float)brake_hitCount / max_hit_count);
    std::cout<<"cur accel : "<< cur_accel_arg <<std::endl;
    std::cout<<"cur steer : "<< cur_steer_arg <<std::endl;
    std::cout<<"cur brake : "<< cur_brake_arg <<std::endl;

    cmd_publisher(cur_accel_arg, cur_steer_arg, cur_brake_arg);
}

void VescController::cmd_publisher(float cur_accel_arg, float cur_steer_arg, float cur_brake_arg)
{
    std_msgs::msg::Float64 accel_msg;
    std_msgs::msg::Float64 steer_msg;
    std_msgs::msg::Float64 brake_msg;
    accel_msg.data = cur_accel_arg;
    steer_msg.data = cur_steer_arg;
    brake_msg.data = cur_brake_arg;

    accel_pub_->publish(accel_msg);
    steer_pub_->publish(steer_msg);
    if(cur_brake_arg != 0){
        brake_pub_->publish(brake_msg);
    }   
}