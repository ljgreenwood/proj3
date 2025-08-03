#include "preprocessing.h"

int main() {
    std::string directory = "path/to/ModelNet10/class_name/"; // path to the directory containing the off files you want to load (by class here)
    // make a directory iterator out of the path - iterate over the "entries"
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        std::vector<Point> vertices; // vector of 3d points
        std::vector<Face> faces; // vector of face vectors
        if (!loadOFF(entry.path().string(), vertices, faces)) continue;
        // for each entry create a vector of vertices and faces
        
            normalize(vertices); // normalizer function for the vertices
  
        /*
        std::vector<Point> pointCloud = samplePoints(vertices, faces); 
        */

        // one approach may be to make a point cloud from the vertices and faces???
        // std::cout << "Processed: " << entry.path().filename() << ", Points: " << pointCloud.size() << '\n';
    }
    // TEMP FRAMEWORK:
    // create kd/octrees of each file
    // Start user driven program with UI, when user selects an OFF file, viewOFF(selected_file)
    // When user wants to find similar off files, using search and create collection of OFF files
    // viewOFF(collection[0]) if user hits next/prev viewOFF(collection[i + or - 1])

    // KDTree<Point> createKDTree(const std::vector<Point>& vertices);
    //Octree<data> createOctree(const std::vector<data>& data);
    //std::vector<std::string>> findSimilarTrees(maybe input map<file, tree>, std::string source);


    return 0;
}
