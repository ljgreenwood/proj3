#include "Octree.h"
#include "KDTree.h"

// Variables that are used for tree comparisons
const float KD_TOLERANCE = 0.1; // Tolerance for point distance
const float OCT_TOLERANCE = 0.1; // Results with lower than tolerance mean similar points
const float OCT_THRESHOLD = 0.65; // Similarity threshold percentage, results with higher percentage are more similar

/**
 * FUNCTION TO COMPARE KDTrees
 * Time :: O(n^2)
 */
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
    if (max(max_dist_A_to_B, max_dist_B_to_A) <= KD_TOLERANCE) {
        return true;
    }
    return false;
}

/**
 * FUNCTION TO COMPARE OCTTREES (class method)
 * Time :: 
 */
bool OctTreeComparison(Octree& treeA, Octree& treeB) {
    return Octree::compareOctree(treeA.getRoot(), treeB.getRoot(), OCT_TOLERANCE, OCT_THRESHOLD);
}

/**
 * FUNCTION TO CREATE A KDTREE
 */
void fillKD(const std::vector<Point>& vertices, KDTree& tree) {
    for (Point p : vertices) {
        tree.insert(p);
    }
}

/**
 * FUNCTION TO CREATE AN OCTREE
 */
void fillOct(const std::vector<Point>& vertices, Octree& tree) {
    for (Point p : vertices) {
        tree.insert(p);
    }
}

/**
 * ARGUMENTS ARE :: <PROGRAM NAME> <SOURCEDIRECTORY> <TREETOGGLE>
 */
int main(int argc, char* argv[]) {
    string source_dir = argv[1];
    string tree_toggle = argv[2];
    int count = stoi(argv[3]);

    vector<Point> source_vertices; // vector of 3d points
    vector<Face> source_faces; // vector of face vectors

    KDTree source_KDTree;
    Octree source_Octree;
    if (tree_toggle == "kdtree") {
        fillKD(source_vertices, source_KDTree);
    }
    else if (tree_toggle == "octree") {
        fillOct(source_vertices, source_Octree);
    }
    if (!loadOFF(source_dir, source_vertices, source_faces)) return -1;

    string directory = "path/to/ModelNet10/class_name/"; // path to the directory containing the off files you want to load (by class here)
    // make a directory iterator out of the path - iterate over the "entries"
    int iteration = 0;
    vector<string> filenames;
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        vector<Point> vertices; // vector of 3d points
        vector<Face> faces; // vector of face vectors

        if (!loadOFF(entry.path().string(), vertices, faces)) continue;
        if (tree_toggle == "kdtree") {
            KDTree kd; fillKD(vertices, kd);
            if (KDTreeComparison(source_KDTree,  kd)) {
                filenames.push_back(entry.path().string());
            }
        }
        else if (tree_toggle == "octree") {
            Octree oct; fillOct(vertices, oct);
             if (OctTreeComparison(source_Octree, oct)) {
                 filenames.push_back(entry.path().string());
             }
        }
        iteration++;
        if (iteration == count) {
            break;
        }
    }
    return 0;
}