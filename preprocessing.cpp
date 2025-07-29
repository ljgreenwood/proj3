/* BASIC PROCESSING SCRIPT */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <sstream>
#include <cmath>

struct Point {
    float x, y, z;
    // Equality operator overloading 
    bool operator==(const Point& rhs) const {
        return (x == rhs.x && y == rhs.y && z == rhs.z);
    }
};

struct Face {
    std::vector<int> indices;
};

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
        for (int j = 0; j < n; ++j)j
            file >> faces[i].indices[j]; // then we can read in each vertex individually
    }
    return true;
}
  
  // we need to verify that the OFF files are normalized around the origin to ensure consistent processing - check manpage before implementing here
  /*
  void normalize(std::vector<Point>& vertices) {
      Point min = vertices[0], max = vertices[0];
      
          min.y = std::min(min.y, v.y); max.y = std::max(max.y, v.y);
          min.z = std::min(min.z, v.z); max.z = std::max(max.z, v.z);
      }
      Point center = {(max.x + min.x) / 2, (max.y + min.y) / 2, (max.z + min.z) / 2};
      float scale = std::max({max.x - min.x, max.y - min.y, max.z - min.z});

      for (auto& v : vertices) {
          v.x = (v.x - center.x) / scale;
          v.y = (v.y - center.y) / scale;
          v.z = (v.z - center.z) / scale;
      }
  }
  */

  /*
   *
   * 
  // Placeholder
  std::vector<Point> samplePoints(const std::vector<Point>& vertices, const std::vector<Face>& faces) {
      // Add uniform surface sampling here
      return vertices; // fallback: return raw vertices
  }
  */

int main() {
    std::string directory = "path/to/ModelNet10/class_name/"; // path to the directory containing the off files you want to load (by class here)
    // make a directory iterator out of the path - iterate over the "entries"
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        std::vector<Point> vertices; // vector of 3d points
        std::vector<Face> faces; // vector of face vectors
        if (!loadOFF(entry.path().string(), vertices, faces)) continue;
        // for each entry create a vector of vertices and faces
        
        // normalize(vertices); // normalizer function for the vertices
  
        /*
        std::vector<Point> pointCloud = samplePoints(vertices, faces); 
        */

        // one approach may be to make a point cloud from the vertices and faces???
        std::cout << "Processed: " << entry.path().filename() << ", Points: " << pointCloud.size() << '\n';
    }
    return 0;
}
