#include <preprocessing.cpp> // Update me

class Octree {
    struct OctreeNode {
        Point point;
        vector<OctreeNode*> children = {nullptr, 8}; // each octree has eight subtree children
        OctreeNode(const Point &p) : point(p),  {}
    };
    OctreeNode* root;
    OctreeNode* insertHelper(OctreeNode* node, const Point &point, int depth); // Recursively called to insert node
    bool searchHelper(const OctreeNode* node, const Point &point, int depth); // Recursively called to find node
    void traverseHelper(OctreeNode* node, std::vector<Point> &points); // Recursively called to push each node's point into collection
    void deleteOctree(const OctreeNode* node); // Delete every node in KDTree

public:
    Octree() : root(nullptr) {};
    ~Octree() { deleteKDTree(root); }
    void insert(OctreeNode* node, const Point& point, int depth);
};


void insert(OctreeNode* node, const Point& point, int depth) {
    if (depth == MAX_DEPTH || node->points.size() < threshold) {
        node->points.push_back(point);
        return;
    }
    int octant = getOctant(node->bounds, point);
    if (!node->children[octant])
        node->children[octant] = new OctreeNode(subdivide(node->bounds, octant));
    insert(node->children[octant], point, depth+1);
    void insert(const Point &point);
    bool search(const Point &point);
    std::vector<Point> traverse();
};
