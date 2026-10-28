#include"server.h"
class Server{

public:
    std::string port;
    Server(::std::string port){
        this->port = port;
    };
    int start(){
        int server_socket_fd;
        struct sockaddr_in server_addr;
        server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if(server_socked_fd < 0){
            std:cerr << "Failed to create socket" << std::endl;
            exit(1);
        }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(std::stoi(port));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(server_socket_fd,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0){
        std::cerr << "fail bind socket" << std::endl;
        end(1);
    }

    if(listen(server_socket_fd, 5) < 0){
        std:cerr << "failed listen on socket" << std::endl;
        exit(1);
    }
    
    std::cout << "server started at port : " << port << std::endl;

    return server_socket_fd;
    };
void stop(){
    
}
