#include <Octree.h> 

void Octree::insert(OctreeNode* node, const Point& point) {
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
