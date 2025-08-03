#pragma once
#include "preprocessing.h"

class KDTree {
    struct KDNode {
        Point point;
        KDNode *left, *right;
        KDNode(const Point &p) : point(p), left(nullptr), right(nullptr) {}
    };

    KDNode* root;
    KDNode* insertHelper(KDNode* node, const Point &point, int depth); 
    bool searchHelper(const KDNode* node, const Point &point, int depth); 
    void traverseHelper(KDNode* node, std::vector<Point> &points);
    void deleteKDTree(KDNode* node); 

public:
    KDTree() : root(nullptr) {}
    ~KDTree() { deleteKDTree(root); }
    void insert(const Point &point);
    bool search(const Point &point);
    std::vector<Point> traverse();
};
