#include"server.cpp"
#include <string.h>
#include"request.cpp"
#include"response.cpp"
#include<thread>

//utility function to log the serving of a file.
void logServingFile(const std::string& path, const std::string& mimetype) {
    std::cout << "Serving file: " << path << " with MIME type: " << mimetype << std::endl;
}

void handleClient(int client_socket_fd){
     char client_req_buffer[1024]; 
      //handle client request.
        //read from the client.
        read(client_socket_fd, client_req_buffer, 1024);
        HttpRequest req = HttpRequest();
        
        req.parseRequest(client_req_buffer);
        // for(auto it = req.headers.begin(); it != req.headers.end(); it++){
        //     std::cout << it->first << " : " << it->second << std::endl;
        // }
        std::string mimetype = req.getMimeType(req.path);
        // std::cout << "Mimetype: " << mimetype << std::endl;
        HttpResponse res= HttpResponse();
        std::string body = req.readHtmlFile(req.path);
        std::string response = res.frameHttpResponse("200","OK",req.headers,body,mimetype);
        logServingFile(req.path, mimetype);
   
        //write to client.
        write(client_socket_fd, response.c_str(), response.length());
        //close client socket.
        close(client_socket_fd);

}
int main(int argc, char* argv[]){
    Server server = Server(argv[1]);
    struct sockaddr_in client_addr;
    socklen_t client_addr_size;
    int s_fd;
    s_fd = server.start();
    client_addr_size = sizeof(struct sockaddr_in); 
  
   



    while(1){
        //handle client requests.
        //Accept system call .
      
        int client_socket_fd = accept(s_fd, (struct sockaddr*)&client_addr, &client_addr_size);
        if(client_socket_fd < 0){
            std::cerr << "Failed to accept client request." << std::endl;
            exit(1);
        }
       
        // Create a new thread to handle the client
        std::thread clientThread(handleClient, client_socket_fd);
        clientThread.detach(); // Detach the thread to allow it to run independently
       
      


    }


    return 0;
}