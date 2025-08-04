#include "Octree.h"
#include "KDTree.h"

float KD_TOLERANCE = 0.1;
float OCT_TOLERANCE = 0.7;

bool KDTreeComparison(KDTree& treeA, KDTree& treeB) {
    // distance from A to B
    float max_dist_A_to_B = 0.0;
    vector<Point> dataA = treeA.traverse();
    for (const auto& pA : dataA) {
        Point nearest_pB = treeB.nearestNeighbor(pA);
        float dist = distance(pA, nearest_pB);
        if (dist > max_dist_A_to_B) {
            max_dist_A_to_B = dist;
        }
    }
    // distance from B to A
    float max_dist_B_to_A = 0.0;
    vector<Point> dataB = treeB.traverse();
    for (const auto& pB : dataB) {
        Point nearest_pA = treeA.nearestNeighbor(pB);
        float dist = distance(pB, nearest_pA);
        if (dist > max_dist_B_to_A) {
            max_dist_B_to_A = dist;
        }
    }
    if (std::max(max_dist_A_to_B, max_dist_B_to_A) <= KD_TOLERANCE) {
        return true;
    }
    return false;
}

bool OctTreeComparison(Octree& treeA, Octree& treeB) {
    if (Octree::compareOctreeNodes(treeA.getRoot(), treeB.getRoot(), OCT_TOLERANCE * 10) >= OCT_TOLERANCE) {
        return true;
    }
    return false;
}

int main() {
    string directory = "path/to/ModelNet10/class_name/"; // path to the directory containing the off files you want to load (by class here)
    // make a directory iterator out of the path - iterate over the "entries"
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        vector<Point> vertices; // vector of 3d points
        vector<Face> faces; // vector of face vectors
        if (!loadOFF(entry.path().string(), vertices, faces)) continue;
        // for each entry create a vector of vertices and faces
        
        vector<Point> pointCloud;
        makePC(vertices, faces, pointCloud); 

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
