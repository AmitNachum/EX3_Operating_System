#include "../Q_1/point.hpp"
#include <iostream>
#include <deque>
#include <random>
#include <chrono>  

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::pair;
#define NUM_POINTS 10000000

std::random_device rd;
std::mt19937 gen(rd()); 
std::uniform_real_distribution<float> dist(0, 1);

template <typename Cont>
void delete_pointers(Cont &container){
    for (auto it = container.begin(); it != container.end(); ++it){
        if(*it){
            delete *it;
        }
    }
}

void delete_deque(std::deque<Point*>& dq) {
    while (!dq.empty()){
        delete dq.front();
        dq.pop_front(); 
    }
}

int main(){
    cout << "==== Convex Hull ====" << endl << endl;

    std::vector<Point *> con_hull_vec;
    std::deque<Point *> con_hull_deque;
    
    for (int i = 0; i < NUM_POINTS; ++i){
        float x = dist(gen);
        float y = dist(gen);
        pair<float,float> x_y = {x, y};
        Point *point_vec = new Point(x_y);
        con_hull_vec.push_back(point_vec);
        Point *point_deque = new Point(x_y);
        con_hull_deque.push_back(point_deque); 
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    cout << "CH's Area Vector: " << calculate_area(con_hull_vec) << endl; 
    auto t2 = std::chrono::high_resolution_clock::now();
    cout << "Vector time: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " ms" << endl<<endl;
    delete_pointers(con_hull_vec);
    
    auto t3 = std::chrono::high_resolution_clock::now();
    cout << "CH's Area Deque: " << calculate_area(con_hull_deque) << endl;
    auto t4 = std::chrono::high_resolution_clock::now();
    cout << "Deque time: " << std::chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count() << " ms" << endl;
    delete_deque(con_hull_deque); 

    return 0;
}
