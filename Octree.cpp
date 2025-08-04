#include <Octree.h> 

void Octree::subdivide(OctreeNode* node) { // O(1)
     
    Point blbs[8] = {
        {node->backLeftBottom.x, node->backLeftBottom.y, node->backLeftBottom.z},
        {node->center.x,         node->backLeftBottom.y, node->backLeftBottom.z},
        {node->backLeftBottom.x, node->center.y,         node->backLeftBottom.z},
        {node->center.x,         node->center.y,         node->backLeftBottom.z},
        {node->backLeftBottom.x, node->backLeftBottom.y, node->center.z},
        {node->center.x,         node->backLeftBottom.y, node->center.z},
        {node->backLeftBottom.x, node->center.y,         node->center.z},
        {node->center.x,         node->center.y,         node->center.z}
    };

    Point frt[8] = {
        {node->center.x,         node->center.y,         node->center.z},
        {node->frontRightTop.x,  node->center.y,         node->center.z},
        {node->center.x,         node->frontRightTop.y,  node->center.z},
        {node->frontRightTop.x,  node->frontRightTop.y,  node->center.z},
        {node->center.x,         node->center.y,         node->frontRightTop.z},
        {node->frontRightTop.x,  node->center.y,         node->frontRightTop.z},
        {node->center.x,         node->frontRightTop.y,  node->frontRightTop.z},
        {node->frontRightTop.x,  node->frontRightTop.y,  node->frontRightTop.z}
    };
    

    for (int i = 0; i < OctreeNode::MAXCHILDREN; ++i) {
        node->children[i] = new OctreeNode(blbs[i], frt[i]);
    }
}

Octree::OctreeNode* Octree::insertHelper(OctreeNode* node, const Point &point) { // O(log n) 

    if(!node->isLeaf()) { // node is internal (has children)
        insertHelper(node->children[getIndex(node, point)], point); // insert in the proper subquadrant
    }
    else { // node is a leaf
        if(node->contents.size() < OctreeNode::MAXCHILDREN) {
            node->contents.push_back(point); // now the point has been added
        }
        else {
            subdivide(node); // modifies the children
            node->children[getIndex(node, point)]->contents.push_back(point);
        }
    }
}

bool Octree::searchHelper(const OctreeNode* node, const Point &point) {
    if(node->center == point) return true;
    else {

        // use logic to find where it should be

    }
}

void Octree::traverseHelper(const OctreeNode* node, vector<Point> &points) {

}

void Octree::deleteOctree(OctreeNode* node) {
    if (!node) return; // there is no tree to delete
    if (node->isLeaf()) delete node; // the node has no children
    else {// it has children
        for (OctreeNode* child : node->children) {
            deleteOctree(child);
        }
        delete node; // no children after loop
    }
}

unsigned char Octree::getIndex(OctreeNode* node, const Point &point){ 
    // centerxyz is
    // >>> backLeftBottom 000
    // <>> frontLeftBottom 001
    // ><> backRightBottom 010
    // <<> frontRightBottom 011
    // >>< backLeftTop 100
    // <>< frontLeftTop 101
    // ><< backRightTop 110
    // <<< frontRightTop 111

    unsigned char index = 0b111; // start with all bits set (7)

    if (node->center.x > point.x) index &= 0b110; // back section  
    if (node->center.y > point.y) index &= 0b101; // left section  
    if (node->center.z > point.z) index &= 0b011; // bottom section       

    return index;
}

Octree::OctreeNode* Octree::getRoot() {
    return root;
}

float Octree::calculateNodeSimilarity(Point node1_data, Point node2_data, float tolerance) {
    if (node1_data == Point(0.0f, 0.0f, 0.0f) && node2_data == Point(0.0f, 0.0f, 0.0f)) return 1.0f; // Similar if both nodes data are zero
    if (node1_data == Point(0.0f, 0.0f, 0.0f) || node2_data == Point(0.0f, 0.0f, 0.0f)) return 0.0f; // Not similar if one is zero and the other is not
    float diff = distance(node1_data, node2_data);
    if (diff <= tolerance) return 1.0f - (diff / tolerance);
    return 0.0f;
}

float Octree::compareOctreeNodes(OctreeNode* node1, OctreeNode* node2, float tolerance) {
    if (node1 == nullptr && node2 == nullptr) return 1.0; // Similar if both nodes are null
    if (node1 == nullptr || node2 == nullptr) return 0.0; // Not similar if one node is null and the other is not
    if (node1->isLeaf() && node2->isLeaf()) { // if theyre both leafs then they have data
        return calculateNodeSimilarity(node1->getData(), node2->getData(), tolerance);
    }
    if (node1->isLeaf() != node2->isLeaf()) return 0.0; // if one is leaf and the other isnt
    float total_similarity = 0.0;
    for (int i = 0; i < 8; ++i) {
        total_similarity += compareOctreeNodes(node1->getChild(i), node2->getChild(i), tolerance);
    }
    return total_similarity / 8.0;
}