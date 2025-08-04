#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <cmath>
#include <iostream>
#include <fstream>
using namespace std;

struct Point {
    float x, y, z; // we may need to make the doubles 
    // Equality operator overloading
    
    Point() : x(0.0f), y(0.0f), z(0.0f) {}
    Point(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {};
    Point(const Point& p) { // copy constructor
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