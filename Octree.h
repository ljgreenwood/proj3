#include "preprocessing.h"

class Octree {
    struct OctreeNode { // each of these is effectively a tree but this is just encapsulation to have the helpers in one wrapper class
        bool empty; 
        Point point; // this is the midpoint of the region
        vector<OctreeNode*> children ; // each octree has eight subtree children
        OctreeNode() : empty(true) {};
        OctreeNode(const Point &p) : point(p), children(8, nullptr), empty(false){
        };

    };
    OctreeNode* root;
    OctreeNode* insertHelper(const OctreeNode* node, const Point &point);
    bool searchHelper(const OctreeNode* node, const Point &point); 
    void traverseHelper(const OctreeNode* node, vector<Point> &points); 
    void deleteOctree(const OctreeNode* node); // postorder traversal to delete all nodes

public:
    Octree() : root(nullptr) {};
    ~Octree() { deleteOctree(root); }
    void insert(OctreeNode* node, const Point& point);
};


/*
    insert:: if a node exists then return else insert recursively
    First, we start with the root node and mark it as current
    Then we find the child node in which we can store the point (calculate which it should be in)
    If node == nullptr then it is replaced with the node we want to insert (making the inserted node a leaf)
    If the node is the leaf node then make it an internal node and if it is an internal node then go to the child node
    This process is performed recursively until an empty node is not found
    O(log N) 
*/

/*
    search node for point (midpoint of the region) - if not find which octant and search in that subtree (if the root is empty or the intended child is null then return false)
    O(log N)
*/