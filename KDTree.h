#pragma once
#include "generic.h"

class KDTree {
    struct KDNode {
        Point point;
        KDNode *left, *right;
        KDNode(const Point &p) : point(p), left(nullptr), right(nullptr) {}
    };

    KDNode* root;
    KDNode* insertHelper(KDNode* node, const Point &point, int depth);
    bool searchHelper(const KDNode* node, const Point &point, int depth); 
    void traverseHelper(const KDNode* node, std::vector<Point> &points);
    void deleteKDTree(KDNode* node);
    void nearestNeighborHelper(KDNode* node, const Point &point, Point &bestPoint, float &dist, int depth);

public:
    KDTree() : root(nullptr) {}
    ~KDTree() { deleteKDTree(root); }
    void insert(const Point &point);
    bool search(const Point &point);
    Point nearestNeighbor(const Point &point);
    vector<Point> traverse();
};
