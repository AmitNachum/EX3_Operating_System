#include "point.hpp"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::pair;
#define NUM_POINTS 4


void delete_pointers(std::vector<Point *> &vec);


void delete_pointers(std::vector<Point *> &vec){
    for(auto* point : vec){
        delete point;
    }
}


int main(){
    cout<< "==== Convex Hull ====" << endl << endl;

    std::vector<Point *> convex_hull_points;
    int i = 0;
    cout<<"Enter 4 points for the convex hall:"<< endl;

    while( i < NUM_POINTS){
        cout << "Point number " << i + 1 <<":" <<endl;
        float x;
        float y;
        cin >> x >> y;
        pair<float,float> x_y = {x,y};
        Point *point = new Point(x_y);
        convex_hull_points.emplace_back(point);
        cout <<"(" << point->_point.first << ", " << point->_point.second << ")" << endl;
        i++;
    }

    cout<<"CH's Area:" << calculate_area(convex_hull_points) << endl; 
    delete_pointers(convex_hull_points);
return 0;

}