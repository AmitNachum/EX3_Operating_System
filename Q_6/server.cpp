#include <iostream>
#include <unistd.h>
#include <cstring>
#include <cstdlib>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h> 
#include "../Q_1/point.hpp"
#include <cctype>
#include <mutex>
#include "interface_Q6.hpp"
#include <unordered_map>
#include <algorithm>
#include "reactor_pattern.hpp"

#define PORT "9034"
#define BACKLOG 10
#define BUF_SIZE 1024
#define MAX_EVENTS 10

std::mutex ch_mutex;
std::vector<Point*> ch_points;
size_t n_points;
Reactor reactor;

using std::cout;
using std::cin;
using std::endl;
using std::cerr;
std::unordered_map<int, bool> initialized_map;
std::unordered_map<int, std::string> last_command_map;



int main() {
    int server_fd;
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(sockaddr_in));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int status = getaddrinfo(nullptr, PORT, &hints, &res);
    if (status != 0) {
        cerr << "getaddrinfo:" << gai_strerror(status) << endl;
        exit(EXIT_FAILURE);
    }

    server_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (server_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int yes = 1;
    if (setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR,&yes, sizeof(yes)) < 0) {
        perror("setsockopt(SO_REUSEADDR)");
        exit(1);
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


    

    cout << "=== Server is listening on Port " << PORT << " ===" << endl;

    reactor.add_fd(server_fd,[server_fd](int fd) {
        sockaddr_in client_addr;
        socklen_t addr_size = sizeof(client_addr);
        int client_fd = accept(server_fd,(sockaddr *)&client_addr,&addr_size);
        if (client_fd == -1) {
            perror("accept");
            return;
        }
        fcntl(client_fd,F_SETFL,O_NONBLOCK);

        cout <<"New client connected: " <<client_fd <<std::endl;
        initialized_map[client_fd] = false;

        std::string welcome = "=== Welcome to Convex Hull ===\nChoose the amount of Points";
        send(client_fd,welcome.c_str(),welcome.size(),0);


        reactor.add_fd(client_fd,handle_client);
    });
    reactor.start();
    delete_pointers(ch_points);
    return 0;
}
