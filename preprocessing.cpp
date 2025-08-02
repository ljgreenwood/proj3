#include "preprocessing.h"

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
    return true;
}

// Requires external library to be implemented
void viewOFF(const std::string &path) {
    return;
}

void normalize(std::vector<Point> vertices) {
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
}

// May need to be changed depending on how we insert data into KDTree
KDTree<Point> createKDTree(const std::vector<Point>& vertices) {
    KDTree<Point> tree;
    for (auto p : vertices) {
        tree.insert(p);
    }
    return tree;
}

// May need to be changed depending on how we insert data into Octree
/*
  Octree<data> createOctree(const std::vector<data>& data) {
    //Octree tree;
    for (auto p : vertices) {
        tree.insert(p);
    }
    return tree;
}
*/
/*

std::vector<std::string>> findSimilarTrees(maybe input map<file, tree>, std::string source) {
    return collection of names of similar files
}

*/