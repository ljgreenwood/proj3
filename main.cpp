#include "Octree.h"
#include "KDTree.h"

// Variables that are used for tree comparisons
float KD_TOLERANCE = 50.0; // Tolerance for point distance (loosened for testing)
float OCT_TOLERANCE = 0.5; // Results with lower than tolerance mean similar points

float OCT_THRESHOLD = 0.3; // Similarity threshold percentage (loosened for testing)

float KDTreeSimilarity(KDTree& treeA, KDTree& treeB) {
    // const float EARLY_TERMINATION_THRESHOLD = 500.0f; // Early exit for very different models - DISABLED
    
    // distance from A to B
    float max_dist_A_to_B = 0.0;
    vector<Point> dataA = treeA.traverse();
    for (const auto& pA : dataA) {
        Point nearest_pB = treeB.nearestNeighbor(pA);
        float dist = distance(pA, nearest_pB);
        if (dist > max_dist_A_to_B) {
            max_dist_A_to_B = dist;
        }
        // Early termination for clearly dissimilar models - DISABLED
        // if (max_dist_A_to_B > EARLY_TERMINATION_THRESHOLD) {
        //     std::cerr << "DEBUG: Early termination - Max distance exceeded " << EARLY_TERMINATION_THRESHOLD << std::endl;
        //     return 0.01f; // Very low similarity score
        // }
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
        // Early termination for clearly dissimilar models - DISABLED
        // if (max_dist_B_to_A > EARLY_TERMINATION_THRESHOLD) {
        //     std::cerr << "DEBUG: Early termination - Max distance exceeded " << EARLY_TERMINATION_THRESHOLD << std::endl;
        //     return 0.01f; // Very low similarity score
        // }
    }
    
    float max_distance = std::max(max_dist_A_to_B, max_dist_B_to_A);
    
    // Convert distance to similarity score (0-100%)
    // Normalize by model scale to handle large coordinate systems
    float normalized_distance = max_distance / 1000.0f; // Scale down large distances
    float similarity = 100.0f / (1.0f + normalized_distance);
    std::cerr << "DEBUG: Max distance = " << max_distance << ", Similarity = " << similarity << "%" << std::endl;
    
    return similarity;
}

float OctTreeSimilarity(Octree& treeA, Octree& treeB) {
    float result = 0.0f;
    int total_nodes = 0;
    int similar_nodes = 0;
    
    // Get detailed similarity metrics using existing function
    Octree::calculateNodeSimilarity(treeA.getRoot(), treeB.getRoot(), 
                                   OCT_TOLERANCE, result, total_nodes, similar_nodes);
    
    // Convert to meaningful similarity percentage
    float similarity;
    if (total_nodes > 0) {
        // Base similarity on ratio of similar nodes
        float node_ratio = (float)similar_nodes / (float)total_nodes;
        // Scale to 0-100% range with some weighting
        similarity = node_ratio * 100.0f;
        
        // Ensure reasonable range (avoid 0% for all comparisons)
        similarity = std::max(0.1f, similarity);
    } else {
        similarity = 0.1f; // Fallback for empty trees
    }
    
    std::cerr << "DEBUG: Octree - Total nodes: " << total_nodes 
              << ", Similar nodes: " << similar_nodes 
              << ", Similarity: " << similarity << "%" << std::endl;
    
    return similarity;
}

KDTree fillKD(const std::vector<Point>& vertices) {
    KDTree tree;
    // Sample vertices to improve performance (max 200 vertices)
    int step = std::max(1, (int)vertices.size() / 200);
    std::cerr << "DEBUG: Sampling " << vertices.size() << " vertices with step " << step 
              << " (using ~" << (vertices.size() / step) << " vertices)" << std::endl;
    
    for (int i = 0; i < vertices.size(); i += step) {
        tree.insert(vertices[i]);
    }
    return tree;
}

Octree fillOct(const std::vector<Point>& vertices) {
    Octree tree;
    // Sample vertices to improve performance (max 200 vertices)
    int step = std::max(1, (int)vertices.size() / 200);
    std::cerr << "DEBUG: Sampling " << vertices.size() << " vertices with step " << step 
              << " (using ~" << (vertices.size() / step) << " vertices)" << std::endl;
    
    for (int i = 0; i < vertices.size(); i += step) {
        tree.insert(vertices[i]);
    }
    return tree;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        cerr << "Usage: ./similarity_search <source_file> <compare_file> <algorithm>" << endl;
        return -1;
    }
    
    string source_file = argv[1];     // First model to compare
    string compare_file = argv[2];    // Second model to compare  
    string algorithm = argv[3];       // "kdtree" or "octree"

    // Load source model
    vector<Point> source_vertices;
    vector<Face> source_faces;
    if (!loadOFF(source_file, source_vertices, source_faces)) {
        cerr << "Error loading source file: " << source_file << endl;
        return -1;
    }

    // Load comparison model
    vector<Point> compare_vertices;
    vector<Face> compare_faces;
    if (!loadOFF(compare_file, compare_vertices, compare_faces)) {
        cerr << "Error loading compare file: " << compare_file << endl;
        return -1;
    }

    // Build trees and compare
    float similarity = 0.0f;
    
    if (algorithm == "kdtree") {
        KDTree sourceTree = fillKD(source_vertices);
        KDTree compareTree = fillKD(compare_vertices);
        similarity = KDTreeSimilarity(sourceTree, compareTree);
    }
    else if (algorithm == "octree") {
        Octree sourceTree = fillOct(source_vertices);
        Octree compareTree = fillOct(compare_vertices);
        similarity = OctTreeSimilarity(sourceTree, compareTree);
    }
    else {
        cerr << "Invalid algorithm. Use 'kdtree' or 'octree'" << endl;
        return -1;
    }

    // Output similarity score (0-100%)
    cout << similarity << endl;
    return 0;
}