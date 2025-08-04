#pragma once
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
    Point();
    Point(const Point& p) { // copy constructor
        this->x = p.x;
        this->y = p.y;
        this->z = p.z;
    };
};

struct Face {
    std::vector<int> indices;
};

bool loadOFF(const std::string& path, std::vector<Point>& vertices, std::vector<Face>& faces);
float distance(const Point& p1, const Point& p2);
void makePC(vector<Point>& vertices, vector<Face>& faces, vector<Point>& pointCloud);
// void normalize(std::vector<Point> vertices);
// void viewOFF(const string& path);