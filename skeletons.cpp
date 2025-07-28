#include <preprocessing.cpp> // make a header file out of this instead obviously

struct KDNode {
    Point point;
    KDNode *left, *right;
    int axis;
};

/*
  KDNode* buildKDTree(std::vector<Point>& points, int depth) {
      if (points.empty()) return nullptr;
      int axis = depth % 3;
      std::nth_element(...);  // Partition by axis
      KDNode* node = new KDNode{median, ..., ..., axis};
      node->left = buildKDTree(left_half, depth+1);
      node->right = buildKDTree(right_half, depth+1);
      return node;
  }
*/

struct OctreeNode {
    AABB bounds;
    std::vector<Point> points;
    OctreeNode* children[8];
};

/*
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
*/

