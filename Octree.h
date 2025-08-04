#pragma once
#include "generic.h"

class Octree {
    struct OctreeNode { // each of these is effectively a tree but this is just encapsulation to have the helpers in one wrapper class
        static const int MAXCHILDREN = 8; // size limit for pushing back new children
        int size; 
        bool empty;
        Point center, frontRightTop, backLeftBottom; // this is the midpoint of the region
        OctreeNode* children[MAXCHILDREN] = {nullptr};
        OctreeNode(const Point &frontRightTop_, const Point& backLeftBottom_, bool empty_) : frontRightTop(frontRightTop_), backLeftBottom(backLeftBottom_), empty(empty_), size(0){ 
            center.x = (frontRightTop.x + backLeftBottom.x) / 2.0f;
            center.y = (frontRightTop.y + backLeftBottom.y) / 2.0f;
            center.z = (frontRightTop.z + backLeftBottom.z) / 2.0f;
        }; // constructor with box bounds (finding center of box)
        // if in inserting we have found that we need to create a node in order to insert at a subdivision - we create with empty = true
        // when the node we are inserting IS the node that's midpoint defines the associated octant - empty = false
    };

    OctreeNode* root;
    OctreeNode* insertHelper(OctreeNode* node, const Point &point);
    bool searchHelper(const OctreeNode* node, const Point &point); 
    void traverseHelper(const OctreeNode* node, vector<Point> &points); 
    void deleteOctree(OctreeNode* node); // postorder traversal to delete all nodes

public:
    Octree() : root(nullptr) {};
    ~Octree() { deleteOctree(root); }
    void insert(const Point& point) { root = insertHelper(root, point); };
    bool search(const Point& point) { return searchHelper(root, point); };
    OctreeNode* getRoot();

    static float calculateNodeSimilarity(float node1_data, float node2_data, float tolerance);
    static float compareOctreeNodes(OctreeNode* node1, OctreeNode* node2, float tolerance);
};