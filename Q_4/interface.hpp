#ifndef IMPROVED_HPP
#define IMRPOVED_HPP
#include "../Q_1/point.hpp"
#include <iostream>
#include <string>
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





#endif