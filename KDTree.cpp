#include "KDTree.h"

/* ===== Private KDTree Functions ===== */
KDTree::KDNode* KDTree::insertHelper(KDNode* node, const Point &point, const int depth) {
    // If node does not exist, create new node
    if (node == nullptr) return new KDNode(point);
    int dim = depth % 3;
    vector val_new = {point.x, point.y, point.z};
    vector val_curr = {node->point.x, node->point.y ,node->point.z};
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
    vector val_new = {point.x, point.y, point.z};
    vector val_curr = {node->point.x, node->point.y ,node->point.z};
    // Compare current node's point with point, if less go left, if greater go right
    if (val_new[dim] < val_curr[dim])
       return searchHelper(node->left, point, depth + 1);
    else
        return searchHelper(node->right, point, depth + 1);
}

void KDTree::traverseHelper(KDNode* node, vector<Point> &points) {
    if (node == nullptr)
        return;
    // Add current node's point to collection
    points.emplace_back(node->point);
    // Traverse left
    traverseHelper(node->left, points);
    // Traverse right
    traverseHelper(node->right, points);
}

void KDTree::deleteKDTree(KDNode* node) {
    if (node == nullptr)
        return;
    deleteKDTree(node->left);
    deleteKDTree(node->right);
    delete node;
}

void KDTree::nearestNeighborHelper(KDNode* node, const Point &point, const Point &bestPoint, float &dist, int depth) {
    if (node == nullptr) return;
    // Get distance from the current point to the target point
    float currDist = distance(node->point, point);
    // Update best point if current point if closer
    if (currDist < dist) {
        dist = currDist;
        bestPoint = nodePoint;
    }
    int dim = depth % 3;
    std::vector target = {point.x, point.y, point.z};
    std::vector curr = {node->point.x, node->point.y, node->point.z};
    KDNode* closer = nullptr;
    KDNode* farther = nullptr;
    // Deterime farther and closer node
    if (target[dim] < curr[dim]) {
        closer = node->left;
        farther = node->right;
    }
    else {
        closer = node->right;
        farther = node->left;
    }
    // Neighbor search closer
    nearestNeighborHelper(closer, point, bestPoint, dist, depth + 1);
    // Check points on the other side of the splitting plane
    float distPlane = target[dim] - curr[dim];
    if (distPlane * distPlane < dist) { // Neighbor search farther
        nearestNeighborHelper(farther, point, bestPoint, dist, depth + 1);
    }
}

/* ===== Public KDTree Functions ===== */
void KDTree::insert(const Point &point) {
    root = insertHelper(root, point, 0);
}

bool KDTree::search(const Point &point) {
    return searchHelper(root, point, 0);
}

vector<Point> KDTree::traverse() {
    vector<Point> points;
    traverseHelper(root, points);
    return points;
}

Point KDTree::nearestNeighbor(const Point &point) {
    Point bestPoint = root->point;
    float dist = distance(root->point, point);
    nearestNeighborHelper(root, point, bestPoint, dist, 0);
    return bestPoint;
}