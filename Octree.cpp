#include <Octree.h> 

/*
    insert:: if a node exists then return else insert recursively
    First, we start with the root node and mark it as current
    Then we find the child node in which we can store the point (calculate which it should be in)
    If node == nullptr then it is replaced with the node we want to insert (making the inserted node a leaf)
    If the node is the leaf node then make it an internal node and if it is an internal node then go to the child node
    This process is performed recursively until an empty node is not found
    O(log N) 
*/
Octree::OctreeNode* Octree::insertHelper(OctreeNode* node, const Point &point) {
}

/*
    search node for point (midpoint of the region) - if not find which octant and search in that subtree (if the root is empty or the intended child is null then return false)
    O(log N)
*/
bool Octree::searchHelper(const OctreeNode* node, const Point &point) {
    if(node->point == point) return true;
    else {
        // use logic to find where it should be

    }
}

void Octree::traverseHelper(const OctreeNode* node, vector<Point> &points) {

}

void Octree::deleteOctree(OctreeNode* node) {
    if (!node) return; // there is no tree to delete
    if (node->children.size() == 0) delete node; // the node has no children
    else {// it has children
        for (OctreeNode* child : node->children) {
            deleteOctree(child);
        }
        delete node; // no children after loop
    }
}

