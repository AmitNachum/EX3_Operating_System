#ifndef POINT_HPP
#define POINT_HPP
#include <vector>
#include <utility>
#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>


struct Point {
    std::pair<float,float> _point;

    inline explicit Point(std::pair<float,float>& x_y):_point(x_y){};

    ~Point()= default;
};


template <typename Container>
inline float calculate_area(const Container& points){
    if (points.size() < 3) return 0.0f;
    float area = 0.0f;

    

    for(auto it = points.begin() ; it != points.end() ; ++it){
    
        auto it2 = std::next(it);
        if(it2 == points.end()) it2 = points.begin();
        float x1 = (*it)->_point.first;
        float y1 = (*it)->_point.second;
        float x2 = (*it2)->_point.first;
        float y2 = (*it2)->_point.second;
        area += (x1 * y2) - (x2 * y1);
    }

    return std::abs(area) * 0.5f;
}

#endif