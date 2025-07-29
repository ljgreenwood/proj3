#include <preprocessing.cpp> // Update me
class KDTree {
    struct KDNode {
        Point point;
        KDNode *left, *right;
        // KDNode Constructor
        KDNode(const Point &p) : point(p), left(nullptr), right(nullptr) {}
    };

    // KDTree root node
    KDNode* root;
    // KDTree Private Functions
    KDNode* insertHelper(KDNode* node, const Point &point, int depth); // Recursively called to insert node
    bool searchHelper(KDNode* node, const Point &point, int depth); // Recursively called to find node
    void traverseHelper(KDNode* node, std::vector<Point> &points); // Recursively called to push each node's point into collection
    void deleteKDTree(KDNode* node); // Delete every node in KDTree

public:
    // KDTree Constructor
    KDTree() : root(nullptr) {}
    // KDTree Destructor
    ~KDTree() { deleteKDTree(root); }
    // KDTree Operations
    void insert(const Point &point);
    bool search(const Point &point);
    std::vector<Point> traverse();
];