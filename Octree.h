#include "preprocessing.h"

class Octree {
    struct OctreeNode {
        int height;
        Point point;
        vector<OctreeNode*> children ; // each octree has eight subtree children
        OctreeNode(const Point &p) : point(p), children(8, nullptr), height(0) {

        }
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


