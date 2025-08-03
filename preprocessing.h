#pragma once
#include "KDTree.h"
#include "Octree.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <sstream>
#include <cmath>
using namespace std;

struct Point {
    float x, y, z; // we may need to make the doubles 
    // Equality operator overloading
    bool operator==(const Point& rhs) const {
        return (x == rhs.x && y == rhs.y && z == rhs.z);
    }
    Point(const Point& p) { // copy constructor
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    };
};

struct Face {
    std::vector<int> indices;
};

bool loadOFF(const string& path, vector<Point>& vertices, vector<Face>& faces);
void viewOFF(const string& path);
void makePC(vector<Point>& vertices, vector<Face>& faces, vector<Point>& pointCloud); 
// void normalize(std::vector<Point> vertices);


