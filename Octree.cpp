#include "Octree.h"

void Octree::subdivide(OctreeNode* node) { // O(1)
    
    /* create an array of lower bounds points */
    Point blbs[8] = {
        {node->backLeftBottom.x, node->backLeftBottom.y, node->backLeftBottom.z},
        {node->center.x,         node->backLeftBottom.y, node->backLeftBottom.z},
        {node->backLeftBottom.x, node->center.y,         node->backLeftBottom.z},
        {node->center.x,         node->center.y,         node->backLeftBottom.z},
        {node->backLeftBottom.x, node->backLeftBottom.y, node->center.z},
        {node->center.x,         node->backLeftBottom.y, node->center.z},
        {node->backLeftBottom.x, node->center.y,         node->center.z},
        {node->center.x,         node->center.y,         node->center.z}
    };

    /* create an array of upper bound points */
    Point frt[8] = {
        {node->center.x,         node->center.y,         node->center.z},
        {node->frontRightTop.x,  node->center.y,         node->center.z},
        {node->center.x,         node->frontRightTop.y,  node->center.z},
        {node->frontRightTop.x,  node->frontRightTop.y,  node->center.z},
        {node->center.x,         node->center.y,         node->frontRightTop.z},
        {node->frontRightTop.x,  node->center.y,         node->frontRightTop.z},
        {node->center.x,         node->frontRightTop.y,  node->frontRightTop.z},
        {node->frontRightTop.x,  node->frontRightTop.y,  node->frontRightTop.z}
    };
    
    /* create the OctreeNode children according to the bounds given in the array */
    for (int i = 0; i < OctreeNode::MAXCHILDREN; ++i) {
        node->children[i] = new OctreeNode(blbs[i], frt[i]); // DYNAMICALLY ALLOCATED
    }
}

void Octree::insertHelper(OctreeNode* node, const Point &point) { // O(log n)

    if(node->isLeaf()) { // node is a leaf
        if(node->contents.size() < OctreeNode::MAXCHILDREN) {
            node->contents.push_back(point); // now the point has been added
        }
        else {
            subdivide(node); // modifies the children
            node->children[getIndex(node, point)]->contents.push_back(point);
        }
    }
    else {   // node is internal
        insertHelper(node->children[getIndex(node, point)], point); // insert in the proper subquadrant
    }
}

bool Octree::searchHelper(const OctreeNode* node, const Point &param) {
    if (node->isLeaf()){
       for (const Point& point : node->contents) {
        if (point == param) return true;
       }
       return false;
    }
    else {
        return searchHelper(node->children[getIndex(node, param)], param);
    }
}

void Octree::traverseHelper(const OctreeNode* node, vector<Point> &accum) {
    if (node->isLeaf()) {
       for(const Point& point : node->contents) {
            accum.push_back(point);
       }
    }
    else {
        for(const OctreeNode* child : node->children) {
            traverseHelper(child, accum);
        }
    }
}

void Octree::deleteOctree(OctreeNode* node) {
    if (!node) return; // there is no tree to delete
    if (node->isLeaf()) delete node; // the node has no children
    else {// it has children
        for (OctreeNode* child : node->children) {
            deleteOctree(child);
        }
        delete node; // no children after loop
    }
}

unsigned char Octree::getIndex(const OctreeNode* node, const Point &point) const {
    // centerxyz is
    // >>> backLeftBottom 000
    // <>> frontLeftBottom 001
    // ><> backRightBottom 010
    // <<> frontRightBottom 011
    // >>< backLeftTop 100
    // <>< frontLeftTop 101
    // ><< backRightTop 110
    // <<< frontRightTop 111

    unsigned char index = 0b111; // start with all bits set (7)

    if (node->center.x > point.x) index &= 0b110; // back section  
    if (node->center.y > point.y) index &= 0b101; // left section  
    if (node->center.z > point.z) index &= 0b011; // bottom section       

    return index;
}

Octree::OctreeNode* Octree::getRoot() const {
    return root;
}

bool Octree::calculatePointSimilarity(const vector<Point> &points1, const vector<Point> &points2, float tolerance) {
    if (points1.size() != 8 || points2.size() != 8) return false; //

    for (int i = 0; i < 8; ++i) {
        if (sqrt(distance(points1[i],points2[i])) > tolerance)
            return false;
    }
    return true;
}

void Octree::calculateNodeSimilarity(OctreeNode* node1, OctreeNode* node2, float tolerance, float &result, int &nodes, int &similar_nodes) {
    if (node1 == nullptr && node2 == nullptr) return; // both nullptr, there similar
    if (node1 == nullptr || node2 == nullptr) {
        // One is null, the other isn't then count only the non-null as a node
        nodes += (node1 != nullptr ? 1 : 0) + (node2 != nullptr ? 1 : 0);
        return;
    }
    nodes++;
    if (calculatePointSimilarity(node1->contents, node2->contents, tolerance)) {
        similar_nodes++;
    }
    for (int i = 0; i < 8; i++) {
        calculateNodeSimilarity(node1->getChild(i), node2->getChild(i), tolerance, result, nodes, similar_nodes);
    }
}

bool Octree::compareOctree(OctreeNode* node1, OctreeNode* node2, float tolerance, float threshold) {
    float result = 0;
    int nodes = 0;
    int similar_nodes = 0;
    calculateNodeSimilarity(node1, node2, tolerance, result, nodes, similar_nodes);
    if (nodes == 0) return true;
    float similarity = 100.0f * similar_nodes / nodes;
    return similarity >= threshold;
}