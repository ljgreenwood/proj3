#include "KDTree.h"

/* ===== Private KDTree Functions ===== */
KDNode* KDTree::insertHelper(KDNode* node, const Point &point, int depth) {
    // If node does not exist, create new node
    if (node == nullptr) return new KDNode(point);
    int dim = depth % 3;
    std::vector val_new = {point.x, point.y, point.z};
    std::vector val_curr = {node->point.x, node->point.y ,node->point.z};
    // Compare current node's point with point, if less go left, if greater go right
    if (val_new[dim] < val_curr[dim])
        node->left = insertHelper(node->left, point, depth + 1);
    else
        node->right = insertHelper(node->right, point, depth + 1);
    return node;
}

bool KDTree::searchHelper(const KDNode* node, const Point &point, int depth) {
    if (node == nullptr) // If current node is null, point does not exist
        return false; 
    if (node->point == point) // Point is found
        return true;
    int dim = depth % 3;
    std::vector val_new = {point.x, point.y, point.z};
    std::vector val_curr = {node->point.x, node->point.y ,node->point.z};
    // Compare current node's point with point, if less go left, if greater go right
    if (val_new[dim] < val_curr[dim])
       return searchHelper(node->left, point, depth + 1);
    else
        return searchHelper(node->right, point, depth + 1);
}

void KDTree::traverseHelper(KDNode* node, std::vector<Point> &points) {
    if (node == nullptr)
        return;
    // Add current node's point to collection
    points.emplace_back(node->point);
    // Traverse left
    traverseHelper(node->left, points);
    // Traverse right
    traverseHelper(node->right, points);
}

void KDTree::deleteKDTree(const KDNode* node) {
    if (node == nullptr)
        return;
    deleteKDTree(node->left);
    deleteKDTree(node->right);
    delete node;
}
/* ===== Public KDTree Functions ===== */
void KDTree::insert(const Point &point) {
    root = insertHelper(root, point, 0);
}
bool KDTree::search(const Point &point) {
    return searchHelper(root, point, 0);
}
std::vector<Point> KDTree::traverse() {
    std::vector<Point> points;
    traverseHelper(root, points);
    return points;
}