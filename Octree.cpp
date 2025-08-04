#include "Octree.h"

/*
    insert:: if a node exists then return else insert recursively
    First, we start with the root node and mark it as current
    Then we find the child node in which we can store the point (calculate which it should be in)
    If node == nullptr then it is replaced with the node we want to insert (making the inserted node a leaf)
    If the node is the leaf node then make it an internal node and if it is an internal node then go to the child node
    This process is performed recursively until an empty node is not found
    O(log N) 
*/
Octree::OctreeNode* Octree::insertHelper(OctreeNode* node, const Point &point) {

    // // centerxyz is
    // // >>> backLeftBottom 000
    // // <>> frontLeftBottom 001
    // // ><> backRightBottom 010
    // // <<> frontRightBottom 011
    // // >>< backLeftTop 100
    // // <>< frontLeftTop 101
    // // ><< backRightTop 110
    // // <<< frontRightTop 111


    unsigned char index = 0b111; // start with all bits set (7)

    if (node->center.x > point.x) index &= 0b110; // back section  
    if (node->center.y > point.y) index &= 0b101; // left section  
    if (node->center.z > point.z) index &= 0b011; // bottom section         
    
    // NOW THE CHAR HAS THE INDEX



}

/*
    search node for point (midpoint of the region) - if not find which octant and search in that subtree (if the root is empty or the intended child is null then return false)
    O(log N)
*/
bool Octree::searchHelper(const OctreeNode* node, const Point &point) {
    if(node->center == point) return true;
    else {

        // use logic to find where it should be

    }
}

void Octree::traverseHelper(const OctreeNode* node, vector<Point> &points) {

}

void Octree::deleteOctree(OctreeNode* node) {
    if (!node) return; // there is no tree to delete
    if (node->size == 0) delete node; // the node has no children
    else {// it has children
        for (OctreeNode* child : node->children) {
            deleteOctree(child);
        }
        delete node; // no children after loop
    }
}

Octree::OctreeNode* Octree::getRoot() {
    return root;
}

float Octree::calculateNodeSimilarity(float node1_data, float node2_data, float tolerance) {
    if (node1_data == 0.0f && node2_data == 0.0f) return 1.0f; // Similar if both nodes data are zero
    if (node1_data == 0.0f || node2_data == 0.0f) return 0.0f; // Not similar if one is zero and the other is not
    float diff = std::abs(node1_data - node2_data);
    if (diff <= tolerance) return 1.0f - (diff / tolerance);
    return 0.0f;
}

float Octree::compareOctreeNodes(OctreeNode* node1, OctreeNode* node2, float tolerance) {
    if (node1 == nullptr && node2 == nullptr) return 1.0; // Similar if both nodes are null
    if (node1 == nullptr || node2 == nullptr) return 0.0; // Not similar if one node is null and the other is not
    if (node1->isLeaf() && node2->isLeaf()) {
        return calculateNodeSimilarity(node1->getData(), node2->getData(), tolerance);
    }
    if (node1->isLeaf() != node2->isLeaf()) return 0.0;
    float total_similarity = 0.0;
    for (int i = 0; i < 8; ++i) {
        total_similarity += compareOctreeNodes(node1->getChild(i), node2->getChild(i), tolerance);
    }
    return total_similarity / 8.0;
}
