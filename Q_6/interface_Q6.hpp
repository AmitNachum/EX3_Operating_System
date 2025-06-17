#ifndef IMPROVED_HPP
#define IMRPOVED_HPP
#include "../Q_1/point.hpp"
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>
#include <mutex>
#include <sys/socket.h>
#include <unistd.h>

using std::cout;
using std::endl;

#define BUF_SIZE 1024
extern std::unordered_map<int, bool> initialized_map;
extern std::unordered_map<int, std::string> last_command_map;
extern std::mutex ch_mutex;
extern size_t n_points;
extern std::vector<Point *> ch_points;


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

void handle_client(int fd) {
    char buf[BUF_SIZE];
    ssize_t count = read(fd, buf, sizeof(buf));
    if (count <= 0) {
        close(fd);
        initialized_map.erase(fd);
        last_command_map.erase(fd);
        cout << "Client disconnected: " << fd << endl;
        return;
    }

    std::string input(buf, count);
    input.erase(std::remove(input.begin(), input.end(), '\n'), input.end());

    if (!initialized_map[fd]) {
        if (std::all_of(input.begin(), input.end(), ::isdigit)) {
            n_points = std::stoul(input);
            ch_points.reserve(n_points);
            std::string msg = "Convex Hull has been initialized with " + std::to_string(n_points) + " points.\n";
            send(fd, msg.c_str(), msg.size(), 0);
            send_menu(fd);
            initialized_map[fd] = true;
        } else {
            std::string err = "Invalid number. Please enter a number like 5:\n";
            ssize_t bytes = send(fd, err.c_str(), err.size(), 0);
            if(bytes == -1) perror("menu");
        }
        return;
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
        send_menu(fd);
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
                }
            } else {
                std::string msg = "Unknown operation. Choose option from menu first.\n";
                send(fd, msg.c_str(), msg.size(), 0);
            }
        } catch (...) {
            std::string err = "Invalid format. Use x,y (like 3.5,2.1)\n";
            send(fd, err.c_str(), err.size(), 0);
        }
        send_menu(fd);
    } else {
        std::string err = "Invalid option. Try 1, 2, 3, or 4.\n";
        send(fd, err.c_str(), err.size(), 0);
        send_menu(fd);
    }
}
#endif