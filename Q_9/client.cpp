#include <iostream>        
#include <cstring>         
#include <unistd.h>        
#include <sys/socket.h>    
#include <netinet/in.h>    
#include <arpa/inet.h>     
#include <netdb.h>
#define PORT "9034"

int main(){
addrinfo hints,*res;
memset(&hints,0,sizeof(addrinfo));
hints.ai_family = AF_INET;
hints.ai_socktype = SOCK_STREAM;
hints.ai_flags = AI_PASSIVE;


if(getaddrinfo(nullptr,PORT,&hints,&res)){
    perror("Get Addr Info");
    freeaddrinfo(res);
    exit(EXIT_FAILURE);
}

int client_fd = socket(res->ai_family, res->ai_socktype,res->ai_protocol);
if(client_fd == -1){
    perror("socket");
    freeaddrinfo(res);
    exit(EXIT_FAILURE);
}

if(connect(client_fd,res->ai_addr,res->ai_addrlen)){
    perror("connect");
    close(client_fd);
    freeaddrinfo(res);
    exit(EXIT_FAILURE);
}
freeaddrinfo(res);

while (true) {
    char buffer[1024];
    ssize_t receive = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    
    if (receive > 0) {
        buffer[receive] = '\0';
        std::cout << buffer << std::endl;

        // Prompt user to respond
        std::string input;
        std::getline(std::cin, input);
        input += '\n';  // Ensure newline as server expects it
        send(client_fd, input.c_str(), input.size(), 0);
    } else if (receive == 0) {
        std::cout << "Server closed the connection." << std::endl;
        break;
    } else {
        perror("recv");
        break;
    }
}






}
