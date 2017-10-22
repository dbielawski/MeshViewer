#include "coctree.h"

#include "wireboundingbox.h"


#include <iostream>

cOctree::cOctree(const QVector<Vertex>& vertices) : m_vertices(vertices)
{

}

void cOctree::build() {
    // Creating first node which is the big one with everyone inside
    AlignedBox3f aabb;
    for(const Vertex v : m_vertices) {
        aabb.extend(v.position);
    }

    Node mainNode;
    mainNode.parent = NULL;
    mainNode.aabb = aabb;
    mainNode.objects = &m_vertices;

    m_octree = mainNode;
    buildNode(mainNode);
}

void::cOctree::buildNode(Node parent) {

    // If we have no more objects we can stop the recursion
    const QVector<Vertex>* objects = parent.objects;
    if(objects->size() <= 1) {
        std::cout << "Empty" << std::endl;
        return;
    }

    AlignedBox3f aabb = parent.aabb;

    // TODO: Create constructor for Vector3f from Point3f and reverse
    Point3f center = aabb.center();
    Point3f min = aabb.min();
    Point3f max = aabb.max();
    //Vector3f halfV = aabb.size() / 2;
    //Point3f half = Point3f(halfV.x, halfV.y, halfV.z);

    /* Boxes had be written considering the following axis
     *
     *    | y
     *    |
     *    +----- x
     * z /
     *  /
     *
    */

    // TODO: Try to wrap this in for a loop ? Maybe impossible using min and max pos that's why I rewritten the expression using half vector
    AlignedBox3f aabb_nodes[8];
    // Front Top left
    //aabb_nodes[0] = AlignedBox3f(Point3f(center.x - half.x, center.y, center.z + half.z), Point3f(center.x, center.y + half.y, center.z));
    aabb_nodes[0] = AlignedBox3f(Point3f(min.x, center.y, min.z), Point3f(center.x, max.y, center.z));
    // Front Top Right
    //aabb_nodes[1] = AlignedBox3f(Point3f(center.x, center.y, center.z + half.z), Point3f(center.x + half.x, center.y + half.y, center.z));
    aabb_nodes[1] = AlignedBox3f(Point3f(center.x, center.y, min.z), Point3f(max.x, max.y, center.z));
    // Front Bot Left
    aabb_nodes[2] = AlignedBox3f(min, center);
    // Front Bot Right
    //aabb_nodes[3] = AlignedBox3f(Point3f(center.x, center.y - half.y, center.z - half.z), Point3f(center.x + half.x, center.y, center.z));
    aabb_nodes[3] = AlignedBox3f(Point3f(center.y, min.y, min.z), Point3f(max.x, center.y, center.z));
    // Back Top Left
    //aabb_nodes[4] = AlignedBox3f(Point3f(center.x - half.x, center.y, center.z), Point3f(center.x, center.y + half.y, center.z - half.z));
    aabb_nodes[4] = AlignedBox3f(Point3f(min.x, center.y, center.z), Point3f(center.x, max.y, max.z));
    // Back Top Right
    aabb_nodes[5] = AlignedBox3f(center, max);
    // Back Bot Left
    //aabb_nodes[6] = AlignedBox3f(Point3f(center.x - half.x, center.y - half.y, center.z), Point3f(center.x, center.y, center.z - half.z));
    aabb_nodes[6] = AlignedBox3f(Point3f(min.x, min.y, center.z), Point3f(center.x, center.y, max.z));
    // Back Bot Right
    //aabb_nodes[7] = AlignedBox3f(Point3f(center.x, center.y - half.y, center.z), Point3f(center.x + half.x, center.y, center.y + half.z));
    aabb_nodes[7] = AlignedBox3f(Point3f(center.x, min.y, center.z), Point3f(max.x, center.y, max.z));

    QVector<Vertex> objs[8];
    for(int i  = 0 ; i < objects->size() ; i++) {
        /* For each bounding box we test if the object from the list is inside it.
        *  If true : we can push it into the obj list of the node and break out the for loop
        *  since we don"t want duplicate. The first bounding box containing an object
        *  will be the only one.
        */
        for(int j = 0 ; j < 8 ; j++) {
            if(aabb_nodes[j].contain(objects->at(i).position)) {
                objs[j].push_back(objects->at(i));
                break;
            }
        }
    }

    for(int i = 0 ; i < 8 ; i++) {
        if(objs[i].size() != 0) {
            Node n;
            n.aabb = aabb_nodes[i];
            n.objects = &objs[i];
            n.parent = &parent;
            parent.childs[i] = &n;

            buildNode(n);
        }
    }
}

void cOctree::render(const Scene &scene, const QMatrix4x4 &transform) const {
    std::cerr << "Not implemented yet" << std::endl;
}
