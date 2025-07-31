#include "point.hpp"

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
    std::string line;

    while( i < NUM_POINTS){
        cout << "Point number " << i + 1 <<"(format: x y)" <<endl;

        std::getline(std::cin, line);
        if(line.empty()){--i; continue;}

        std::istringstream iss(line);
        float x,y;

        if(!(iss >> x >> y)){
            std::cout <<"Invalid format. Please enter two numbers."<< endl;
            --i;
            continue;
        }

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