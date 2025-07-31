#ifndef IMPROVED_HPP
#define IMRPOVED_HPP
#include "../Q_1/point.hpp"
#include <iostream>
#include <string>
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
#include <thread>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <condition_variable>
#include <chrono>


#define BUF_SIZE 1024
extern std::unordered_map<int, bool> initialized_map;
extern std::unordered_map<int, std::string> last_command_map;
extern std::mutex ch_mutex;
extern std::vector<Point*> ch_points;
extern size_t n_points;
extern size_t thread_count;
static bool graph_been_created = false;
extern bool area_over_100;
extern std::condition_variable cv;


using std::cout;
using std::cin;
using std::endl;
using std::cerr;

void watcher(){
bool prev_state = area_over_100;

    std::unique_lock<std::mutex> lock(ch_mutex);
    
    while(true){
        cv.wait(lock, [=](){return area_over_100 != prev_state;});

        if(area_over_100){
            std::cout << "At least 100 units belongs to CH" << std::endl;
        } else{
            std::cout << "At least 100 units are no longer belong to CH" << std::endl;
        }

        prev_state = area_over_100;
    }

}





void check_area(float area){
    bool new_state = area >= 100;
    if(new_state != area_over_100){
        area_over_100 = new_state;
        cv.notify_all();
    }
}

void send_menu(int fd) {
    std::string menu =
        "\n=== Make your Choice ===\n"
        "1) Add Point\n"
        "2) Remove Point\n"
        "3) Quit\n"
        "4) Calculate and Print CH's Area\n";
    send(fd, menu.c_str(), menu.size(), 0);
}

void delete_pointers(std::vector<Point *> &vec){
    for(auto* point : vec){
        if(point != nullptr) delete point;
    }
}
bool delete_ith_pointer(std::vector<Point*> &vec, float x, float y) {
    for (auto it = vec.begin(); it != vec.end(); ++it) {
        if (*it && (*it)->_point.first == x && (*it)->_point.second == y) {
            delete *it;             
            vec.erase(it);          
            return true;            
        }
    }
    return false;
}


void add_point(std::vector<Point *> &vec,float i, float j){
    std::pair<float,float> i_j(i,j);
    Point *point_ij = new Point(i_j);
    vec.emplace_back(point_ij);

}



void* handle_client(int fd) {
    char buf[BUF_SIZE];
    ssize_t count;

    while (true) {
       

        count = read(fd, buf, sizeof(buf));
        if (count <= 0) {
            close(fd);
            initialized_map.erase(fd);
            last_command_map.erase(fd);
            std::cout << "Client disconnected: " << fd << std::endl;
            --thread_count;
            return nullptr;
        }

        std::string input(buf, count);
        input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());

        if (!initialized_map.count(fd) || !initialized_map[fd]) {
            if (std::all_of(input.begin(), input.end(), ::isdigit)) {
                std::lock_guard<std::mutex> lock(ch_mutex);

                if (ch_points.empty() && !graph_been_created) {
                    n_points = std::stoul(input);
                    ch_points.reserve(n_points);
                    std::string msg = "Convex Hull has been initialized with " + std::to_string(n_points) + " points.\n";
                    send(fd, msg.c_str(), msg.size(), 0);
                    initialized_map[fd] = true;
                    graph_been_created = true;
                    
                } else {
                    std::string msg = "Graph already initialized by another client.\n";
                    send(fd, msg.c_str(), msg.size(), 0);
                    
                    initialized_map[fd] = true;
                    
                }
            } else {
                std::string err = "Invalid number. Please enter a number like 5:\n";
                send(fd, err.c_str(), err.size(), 0);
            }
            send_menu(fd);
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
            std::cout << "Client quit: " << fd << std::endl;
            return nullptr;  

        } else if (input == "4") {
            std::lock_guard<std::mutex> lock(ch_mutex);
            std::string msg;
            if (ch_points.empty()) {
                msg = "No points to calculate the area.\n";
            } else {
                float area = calculate_area(ch_points);
                msg = "CH Area: " + std::to_string(area) + "\n";
                check_area(area);
            }
            send(fd, msg.c_str(), msg.size(), 0);
            send_menu(fd);

        } else if (input.find(',') != std::string::npos) {
            size_t comma = input.find(',');
            try {
                float x = std::stof(input.substr(0, comma));
                float y = std::stof(input.substr(comma + 1));


                std::string msg;
                if (last_command_map.count(fd) && last_command_map[fd] == "add") {
                    if (ch_points.size() >= n_points) {
                        msg = "Cannot add more points.\n";
                    } else {
                        add_point(ch_points, x, y);
                        msg = "Point added successfully.\n";
                    }
                } else if (last_command_map.count(fd) && last_command_map[fd] == "remove") {
                    if (ch_points.empty()) {
                        msg = "No points to remove.\n";
                    } else {
                        bool removed = delete_ith_pointer(ch_points, x, y);
                        msg = removed ? "Point removed successfully.\n" : "Point not found.\n";
                    }
                } else {
                    msg = "Unknown operation. Choose option from menu first.\n";
                }

                send(fd, msg.c_str(), msg.size(), 0);
                send_menu(fd);
            } catch (...) {
                std::string err = "Invalid format. Use x,y (like 3.5,2.1)\n";
                send(fd, err.c_str(), err.size(), 0);
                send_menu(fd);
            }
            
        } else {
            std::string err = "Invalid option. Try 1, 2, 3, or 4.\n";
            send(fd, err.c_str(), err.size(), 0);
            send_menu(fd);
            
        }
       
        
    }

    return nullptr;
}





#endif