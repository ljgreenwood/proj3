#include <preprocessing.cpp> // Update me

class Octree {

public:
}

struct OctreeNode {
    Point point;
    vector<OctreeNode*> children = {nullptr, 8}; // each octree has eight subtree children
    OctreeNode(const Point &p) : point(p),  {}
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
}

class KDTree {

    KDNode* root;
    KDNode* insertHelper(KDNode* node, const Point &point, int depth); // Recursively called to insert node
    bool searchHelper(const KDNode* node, const Point &point, int depth); // Recursively called to find node
    void traverseHelper(KDNode* node, std::vector<Point> &points); // Recursively called to push each node's point into collection
    void deleteKDTree(const KDNode* node); // Delete every node in KDTree

public:
    // KDTree Constructor
    KDTree() : root(nullptr) {}
    // KDTree Destructor
    ~KDTree() { deleteKDTree(root); }
    // KDTree Operations
    void insert(const Point &point);
    bool search(const Point &point);
    std::vector<Point> traverse();
};
