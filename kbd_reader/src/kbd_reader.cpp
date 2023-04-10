#include "kbd_reader/kbd_reader.hpp"

KbdReader::KbdReader(std::shared_ptr<rclcpp::Node> nh, char* ip, char* port, struct termios* cooked, struct termios* raw) : nh_(nh)
{
    //setting addr struct
    addr_vehicle = {};

    memset(&addr_vehicle, 0, sizeof(addr_vehicle));
    addr_vehicle.sin_family = AF_INET;
    addr_vehicle.sin_addr.s_addr = inet_addr(ip);
    addr_vehicle.sin_port = htons(atoi(port));

    client_sock = socket(AF_INET, SOCK_STREAM,0);
    if(client_sock == -1){
        std::cout<<"creating socket error"<<std::endl;
        std::cout<<"closing socket"<<std::endl;
        close(client_sock);
        exit(1);
    }

    if(connect(client_sock, (struct sockaddr*) &addr_vehicle, sizeof(addr_vehicle)) == -1){
        std::cout<<"connecting socket error"<<std::endl;
        std::cout<<"closing socket"<<std::endl;
        close(client_sock);
        exit(1);
    }

    std::cout<<"socket connecting completed"<<std::endl;

    //console reading setting
    //get the console in raw mode  
    kfd = 0;
    tcgetattr(kfd, cooked);
    memcpy(raw, cooked, sizeof(struct termios));
    raw->c_lflag &=~ (ICANON | ECHO);
    // Setting a new line, then end of file                         
    raw->c_cc[VEOL] = 1;
    raw->c_cc[VEOF] = 2;
    tcsetattr(kfd, TCSANOW, raw);

    std::cout<<"setting colsole reading completed"<<std::endl;

}

KbdReader::~KbdReader()
{
    tcsetattr(kfd, TCSANOW, cooked);
}

int KbdReader::keyLoop()
{
    char c;
    bool key_dirty;

    std::cout<<"Reading from keyboard"<<std::endl;
    std::cout<<"-----------------------------"<<std::endl;
    std::cout<<"Use w a s d to move vehicle."<<std::endl;

    for(;;)
    {
        //reading from keyboard
        if(read(kfd, &c, 1) < 0)
        {
            perror("read() : ");
            exit(-1);
        }

        switch(c)
        {
            case KEYCODE_W:
            case KEYCODE_A:
            case KEYCODE_S:
            case KEYCODE_D:
                key_dirty = true;
                break;
        }

        if(key_dirty == true)
        {
            key_sending(c);
        }
    }

}

void KbdReader::key_sending(char key)
{
    memset(w_buff, 0 ,256);
    w_buff[0] = key;
    int write_check = 0;
    write_check = write(client_sock, w_buff,1);
}