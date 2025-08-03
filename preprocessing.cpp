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

float distance(const Point& p1, const Point& p2) {
    float dist = 0.0;
    dist += (p1.x - p2.x) * (p1.x - p2.x);
    dist += (p1.y - p2.y) * (p1.y - p2.y);
    dist += (p1.z - p2.z) * (p1.z - p2.z);
    return dist;
}

float KDTreeComparison(const KDTree& treeA, const KDTree& treeB) {
    // distance from A to B
    float max_dist_A_to_B = 0.0;
    dataA = treeA.traverse();
     for (const auto& pA : dataA) {
         Point nearest_pB = TreeB.nearestNeighbor(pA);
         float dist = distance(pA, nearest_pB); // Euclidean distance
         if (dist > max_dist_A_to_B) {
             max_dist_A_to_B = dist;
         }
     }
    // distance from B to A
    float max_dist_B_to_A = 0.0;
    dataB = treeB.travsere();
    for (const auto& pB : dataB) {
        Point nearest_pA = treeA.nearestNeighbor(pB);
        double dist = calculateDistance(pB, nearest_pA);
        if (dist > max_dist_B_to_A) {
            max_dist_B_to_A = dist;
        }
    }
    return std::max(max_dist_A_to_B, max_dist_B_to_A);
}

void makePC(vector<Point>& vertices, vector<Face>& faces, vector<Point>& pointCloud) {
    // add some logic here to sample points from the surface and create a pointcloud
}

// ONCE WE HAVE A POINT CLOUD WE CAN PROCEED AND INSERT AS FOLLOWS

// // May need to be changed depending on how we insert data into KDTree
// KDTree<Point> fillKD(const std::vector<Point>& vertices) {
//     KDTree<Point> tree;
//     for (Point p : vertices) {
//         tree.insert(p);
//     }
//     return tree;
// }

// // May need to be changed depending on how we insert data into Octree
// Octree<Point> fillOct(const std::vector<Point>& vertices) {
//     Octree<int> tree;
//     for (Point p : vertices) {
//         tree.insert(p);
//     }
//     return tree;
// }