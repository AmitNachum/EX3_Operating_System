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
#include "../Q_1/point.hpp"
#include <cctype>
#include <mutex>
#include "interface.hpp"
#include <unordered_map>
#include <algorithm>

#define PORT "9034"
#define BACKLOG 10
#define BUF_SIZE 1024
#define MAX_EVENTS 10

std::mutex ch_mutex;
std::vector<epoll_event> events(MAX_EVENTS);
std::vector<Point*> ch_points;
size_t n_points;

using std::cout;
using std::cin;
using std::endl;
using std::cerr;
std::unordered_map<int, bool> initialized_map;
std::unordered_map<int, std::string> last_command_map;

void send_menu(int i) {
    std::string menu =
        "\n=== Make your Choice ===\n"
        "1) Add Point\n"
        "2) Remove Point\n"
        "3) Quit\n"
        "4) Calculate and Print CH's Area\n";
    send(events.at(i).data.fd, menu.c_str(), menu.size(), 0);
}

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

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &ev) == -1) {
        perror("epoll_ctl");
        exit(EXIT_FAILURE);
    }

    cout << "=== Server is listening on Port " << PORT << " ===" << endl;

    while (true) {
        int n_fds = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);
        if (n_fds == -1) {
            perror("epoll_wait");
            continue;
        }

        for (int i = 0; i < n_fds; ++i) {
            int fd = events[i].data.fd;

            if (fd == server_fd) {
                sockaddr_in client_addr;
                socklen_t addr_size = sizeof(client_addr);
                int client_fd = accept(server_fd, (sockaddr*)&client_addr, &addr_size);
                if (client_fd == -1) {
                    perror("accept");
                    continue;
                }

                fcntl(client_fd, F_SETFL, O_NONBLOCK);
                ev.events = EPOLLIN;
                ev.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) == -1) {
                    perror("epoll_ctl: client");
                    close(client_fd);
                }

                cout << "New client connected: " << client_fd << endl;
                std::string welcome = "=== Welcome to Convex Hull ===\nChoose the amount of Points:\n";
                send(client_fd, welcome.c_str(), welcome.size(), 0);
                initialized_map[client_fd] = false;
                continue;
            }

            char buf[BUF_SIZE];
            ssize_t count = read(fd, buf, sizeof(buf));
            if (count <= 0) {
                close(fd);
                initialized_map.erase(fd);
                last_command_map.erase(fd);
                cout << "Client disconnected: " << fd << endl;
                continue;
            }

            std::string input(buf, count);
            input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());

            if (!initialized_map[fd]) {
                if (std::all_of(input.begin(), input.end(), ::isdigit)) {
                    n_points = std::stoul(input);
                    ch_points.reserve(n_points);
                    std::string msg = "Convex Hull has been initialized with " + std::to_string(n_points) + " points.\n";
                    send(fd, msg.c_str(), msg.size(), 0);
                    send_menu(i);
                    initialized_map[fd] = true;
                } else {
                    std::string err = "Invalid number. Please enter a number like 5:\n";
                    send(fd, err.c_str(), err.size(), 0);
                }
                continue;
            }

            if (input == "1") {
                last_command_map[fd] = "add";
                std::string prompt = "Enter point to add (x,y):\n";
                send(fd, prompt.c_str(), prompt.size(), 0);
            } else if (input == "2") {
                last_command_map[fd] = "remove";
                std::string prompt = "Enter point to remove (x,y):\n";
                send(fd, prompt.c_str(), prompt.size(), 0);
            } else if (input == "3") {
                std::string msg = "Bye!\n";
                send(fd, msg.c_str(), msg.size(), 0);
                close(fd);
                initialized_map.erase(fd);
                last_command_map.erase(fd);
                cout << "Client quit: " << fd << endl;
            } else if (input == "4") {
                std::lock_guard<std::mutex> lock(ch_mutex);
                if (ch_points.empty()) {
                    std::string msg = "No points to calculate the area.\n";
                    send(fd, msg.c_str(), msg.size(), 0);
                } else {
                    float area = calculate_area(ch_points);
                    std::string msg = "CH Area: " + std::to_string(area) + "\n";
                    send(fd, msg.c_str(), msg.size(), 0);
                }
                send_menu(i);
            } else if (input.find(',') != std::string::npos) {
                size_t comma = input.find(',');
                try {
                    float x = std::stof(input.substr(0, comma));
                    float y = std::stof(input.substr(comma + 1));
                    std::lock_guard<std::mutex> lock(ch_mutex);
                    if (last_command_map[fd] == "add") {
                        if (ch_points.size() >= n_points) {
                            std::string msg = "Cannot add more points.\n";
                            send(fd, msg.c_str(), msg.size(), 0);
                        } else {
                            add_point(ch_points, x, y);
                            std::string msg = "Point added successfully.\n";
                            send(fd, msg.c_str(), msg.size(), 0);
                        }
                    } else if (last_command_map[fd] == "remove") {
                        if (ch_points.empty()) {
                            std::string msg = "No points to remove.\n";
                            send(fd, msg.c_str(), msg.size(), 0);
                        } else {
                            bool removed = delete_ith_pointer(ch_points, x, y);
                            std::string msg = removed ? "Point removed successfully.\n" : "Point not found.\n";
                            send(fd, msg.c_str(), msg.size(), 0);
                            send_menu(i);
                        }
                    } else {
                        std::string msg = "Unknown operation. Choose option from menu first.\n";
                        send(fd, msg.c_str(), msg.size(), 0);
                    }
                } catch (...) {
                    std::string err = "Invalid format. Use x,y (like 3.5,2.1)\n";
                    send(fd, err.c_str(), err.size(), 0);
                }
                send_menu(i);
            } else {
                std::string err = "Invalid option. Try 1, 2, 3, or 4.\n";
                send(fd, err.c_str(), err.size(), 0);
                send_menu(i);
            }
        }
    }

    delete_pointers(ch_points);
    return 0;
}
