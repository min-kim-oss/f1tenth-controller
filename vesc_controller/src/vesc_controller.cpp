#include "vesc_controller/vesc_controller.hpp"

VescController::VescController(const rclcpp::NodeOptions & options)
  :  rclcpp::Node("vesc_controller", options) 
{
    addr_server = {};
    addr_client = {};
    SocketSetting();
    ReceiveKey();
}

void VescController::SocketSetting()
{
    char port[6];
    std::cout << "write port for connection : ";
    std::cin >> port;

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
    }
}