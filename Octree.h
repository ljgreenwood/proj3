#pragma once
#include "generic.h"

class Octree {
    struct OctreeNode { // each of these is effectively a tree but this is just encapsulation to have the helpers in one wrapper class
        static const int MAXCHILDREN = 8; // size limit for pushing back new children
        Point center, frontRightTop, backLeftBottom; // this is the midpoint of the region
        vector<Point> contents;
        OctreeNode* children[MAXCHILDREN] = {nullptr};
        OctreeNode(const Point &frontRightTop_, const Point& backLeftBottom_) : frontRightTop(frontRightTop_), backLeftBottom(backLeftBottom_) { 
            center.x = (frontRightTop.x + backLeftBottom.x) / 2.0f;
            center.y = (frontRightTop.y + backLeftBottom.y) / 2.0f;
            center.z = (frontRightTop.z + backLeftBottom.z) / 2.0f;
        }; // constructor with box bounds (finding center of box)
        // if in inserting we have found that we need to create a node in order to insert at a subdivision - we create with empty = true
        // when the node we are inserting IS the node that's midpoint defines the associated octant - empty = false
        bool isLeaf() const {return children[0] == nullptr;} ;
        OctreeNode* getChild(int i) {return children[i];};
        vector<Point> getData() {return contents;};
    };

    OctreeNode* root;
    void insertHelper(OctreeNode* node, const Point &point);
    bool searchHelper(const OctreeNode* node, const Point &point); 
    void traverseHelper(const OctreeNode* node, vector<Point> &accum);
    void deleteOctree(OctreeNode* node); 
    void subdivide(OctreeNode* node); // function which will subdivide an octant into sub octants and create the subtrees dma
    unsigned char getIndex(const OctreeNode* node, const Point &point) const; // function which takes a node and a point and returns the index of that node's children that the point should be inserted into

public:
    Octree() : root(nullptr) {};
    ~Octree() { deleteOctree(root); }
    void insert(const Point& point) { insertHelper(root, point); };
    bool search(const Point& point) { return searchHelper(root, point); };
    OctreeNode* getRoot() const;

    static bool calculatePointSimilarity(const vector<Point>& points1, const vector<Point>& points2, float tolerance);
    static void calculateNodeSimilarity(OctreeNode* node1, OctreeNode* node2, float tolerance, float &result, int &nodes, int &similar_nodes);
    static bool compareOctree(OctreeNode* node1, OctreeNode* node2, float tolerance, float threshold);
};