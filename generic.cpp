#include "generic.h"

bool loadOFF(const std::string& path, std::vector<Point>& vertices, std::vector<Face>& faces) {
    std::ifstream file(path); // since OFF are text files this is fine
    if (!file.is_open()) return false;

    std::string header;
    file >> header;
    if (header != "OFF") return false;

    int vCount, fCount, _;
    file >> vCount >> fCount >> _; // get header info and read out a dummy value from the header;
    vertices.resize(vCount); // initialize the vector to the size of the number of vertices we will be adding
    for (int i = 0; i < vCount; ++i)
        file >> vertices[i].x >> vertices[i].y >> vertices[i].z; // read in line by line until there are no more....
    faces.resize(fCount);   // same but for faces
    for (int i = 0; i < fCount; ++i) {
        int n; file >> n; // line starts with the number of vertices in each face
        faces[i].indices.resize(n);
        for (int j = 0; j < n; ++j)
            file >> faces[i].indices[j]; // then we can read in each vertex individually
    }
    file.close();
    return true;
}

float distance(const Point& p1, const Point& p2) {
    float dist = 0.0;
    dist += (p1.x - p2.x) * (p1.x - p2.x);
    dist += (p1.y - p2.y) * (p1.y - p2.y);
    dist += (p1.z - p2.z) * (p1.z - p2.z);
    return dist;
}