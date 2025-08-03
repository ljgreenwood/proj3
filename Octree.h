#include "preprocessing.h"

class Octree {
    struct OctreeNode { // each of these is effectively a tree but this is just encapsulation to have the helpers in one wrapper class
        int maxchildren = 8; // size limit for pushing back new children
        bool empty; 
        Point point; // this is the midpoint of the region
        vector<OctreeNode*> children; // each octree has eight subtree children
        OctreeNode(const Point &p, bool empty_) : point(p), empty(empty_) {};
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
};