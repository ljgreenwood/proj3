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
    Point(); // makes an empty point
    Point(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}; // initialize given xyz
    Point(const Point& p) { // copy constructor from another poitn
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    };
    bool operator==(const Point& rhs) const { // operator overloading for ==
        return (x == rhs.x && y == rhs.y && z == rhs.z);
    };
};

struct Face { // wrapper for vector of indices
    vector<int> indices; 
};

bool loadOFF(const string& path, vector<Point>& vertices, vector<Face>& faces); // file to load OFFs into a vector of vertices and a vector of faces
float distance(const Point& p1, const Point& p2); // generic function to calculate distance between two 3d points :: O(1)