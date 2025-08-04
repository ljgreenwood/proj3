#pragma once
#include "generic.h"

class Octree {
    struct OctreeNode { // each of these is effectively a tree but this is just encapsulation to have the helpers in one wrapper class
        static const int MAXCHILDREN = 8;               // size limit for pushing back new children
        Point center, frontRightTop, backLeftBottom;    // the center of the region, and the two bounds of the region
        vector<Point> contents;                         // vector the points in the octree
        OctreeNode* children[MAXCHILDREN] = {nullptr};  // initialize the children with nullptrs (for a Octree with subtrees these will be OctreeNode*)
        OctreeNode(const Point &frontRightTop_, const Point& backLeftBottom_) : frontRightTop(frontRightTop_), backLeftBottom(backLeftBottom_) { 
            center.x = (frontRightTop.x + backLeftBottom.x) / 2.0f;
            center.y = (frontRightTop.y + backLeftBottom.y) / 2.0f;
            center.z = (frontRightTop.z + backLeftBottom.z) / 2.0f;
        }; // constructor with box bounds (finding center of box)
        bool isLeaf() const {return children[0] == nullptr;} ;  // if the children array has no subtrees then it is a leaf and we can access the contents vector
        OctreeNode* getChild(int i) {return children[i];};      // getter for the ith octant subtree of the Octree
        vector<Point> getData() {return contents;};             // returns the vector of points (should only be used on leaves)
    };

    OctreeNode* root;
    OctreeNode* insertHelper(OctreeNode* node, const Point &point); // given a point and node, try to insert - else recurse to proper suboctant
    bool searchHelper(const OctreeNode* node, const Point &point); // given a point and node, check if the point is in the contents, else go to proper suboctant
    void traverseHelper(const OctreeNode* node, vector<Point> &accum); // accumulate the nodes from left to right (octants 1->8) 
    void deleteOctree(OctreeNode* node);  // delete the octree (called by destructor)
    void subdivide(OctreeNode* node); // function which will subdivide an octant into sub octants and create the subtrees dma
    unsigned char getIndex(const OctreeNode* node, const Point &point) const; // function which takes a node and a point and returns the index of that node's children that the point should be inserted into

public:
    Octree() : root(nullptr) {};        // initializes the root to null (when inserting the first node - the bounds are needed)
    ~Octree() { deleteOctree(root); }   // destructor calls helper
    void insert(const Point& point) { insertHelper(root, point); };         // insertion public function calls helper
    bool search(const Point& point) { return searchHelper(root, point); };  // search public function calls helper
    OctreeNode* getRoot() const;        // const getter to get the Root for comparisons

    // static members to run comparison
    static bool calculatePointSimilarity(const vector<Point>& points1, const vector<Point>& points2, float tolerance);                          // point similarity
    static void calculateNodeSimilarity(OctreeNode* node1, OctreeNode* node2, float tolerance, float &result, int &nodes, int &similar_nodes);  // node similarity calls point similarity
    static bool compareOctree(OctreeNode* node1, OctreeNode* node2, float tolerance, float threshold);                                          // highest level comparison logic
};