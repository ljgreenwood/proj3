#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;

struct Point { // all operations O(1)
    float x, y, z;
    Point(float x_ = 0.0f, float y_ = 0.0f, float z_= 0.0f) : x(x_), y(y_), z(z_) {}; // initialize given xyz
    Point(const Point& p) { // copy constructor from another poitn
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    };
    bool operator==(const Point& rhs) const {
        return (x == rhs.x && y == rhs.y && z == rhs.z);
    };
    Point operator-(const Point& rhs) const {
        return Point(x - rhs.x, y - rhs.y, z - rhs.z);
    }
    
};

struct Face {
    vector<int> indices;
};

bool loadOFF(const string& path, vector<Point>& vertices, vector<Face>& faces);
float distance(const Point& p1, const Point& p2);