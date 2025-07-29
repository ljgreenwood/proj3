#include <preprocessing.cpp> // Update me

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
