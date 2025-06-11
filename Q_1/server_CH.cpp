#include <iostream>
#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netdb.h> 

#include "point.hpp"
#define PORT "5555"
#define BACKLOG 10
#define BUF_SIZE 1024
#define MAX_EVENTS 10

std::vector <epoll_event> events(MAX_EVENTS);

using std::cout;
using std::cin;
using std::endl;
using std::cerr;


int main(){
    int server_fd;
    struct addrinfo hints, *res;
    memset(&hints,0,sizeof(sockaddr_in));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status = getaddrinfo(nullptr,PORT,&hints,&res);
    if(status != 0){
        cerr << "getaddrinfo:" <<gai_strerror(status) <<endl;
        exit(EXIT_FAILURE);
    }


    server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    

    if (bind(server_fd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);

    if (listen(server_fd, BACKLOG) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    struct epoll_event ev{};
    ev.events = EPOLLIN;
    ev.data.fd = server_fd;

    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD,server_fd,&ev) == -1){
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    while(true){
        
        int n_fds = epoll_wait(epoll_fd,events.data(),MAX_EVENTS,-1);
        if(n_fds == -1){
            perror("epoll_wait");
            continue;
        }

        for(int i =0; i < n_fds ; ++i){
            if(events.at(i).data.fd == server_fd){

                sockaddr_in client_addr;
                socklen_t addr_size = sizeof(client_addr);
                int client_fd = accept(server_fd, 
                    (sockaddr *)&client_addr,&addr_size);
                 
                    if(client_fd == -1){
                        perror("accept");
                        continue;
                    }
                    
                    fcntl(client_fd,F_SETFL,O_NONBLOCK);

                    ev.events = EPOLLIN;
                    ev.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    perror("epoll_ctl: client");
                    close(client_fd);
                }
                cout << "New client connected: " << client_fd << endl;
            } else {
                char buf[BUF_SIZE];

                ssize_t count = read(events.at(i).data.fd,buf,sizeof(buf));

                if(count <= 0){
                    close(events.at(i).data.fd);
                    cout <<"Client disconnected: " << events.at(i).data.fd << endl;
                } else {
                    cout << "Recived: " << std::string(buf,count);
                }

            }
        }
    }




}