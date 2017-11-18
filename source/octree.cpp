#include "octree.h"

#include "wireboundingbox.h"


#include <iostream>

Octree::Octree(const QVector<Vertex>& vertices, uint minObj) : m_vertices(vertices), m_minObj(minObj) {
}

void Octree::build() {
    // Creating first node which is the big one with everyone inside
    AlignedBox3f* aabb = new AlignedBox3f();
    for(const Vertex v : m_vertices) {
        aabb->extend(v.position);
    }

    Node* mainNode = new Node();
    mainNode->parent = NULL;
    mainNode->aabb = aabb;
    mainNode->objects = m_vertices;
	mainNode->box = new WireBoundingBox(*mainNode->aabb);
	mainNode->box->setFunctions(*m_functions);
	mainNode->box->init();

    m_octree = mainNode;

	m_size++;

	buildNode(mainNode);

	std::cout << "Finished Octree buildind. " << std::endl << "Octree size: " << m_size << std::endl;
}

void::Octree::buildNode(Node* parent) {
    QVector<Vertex> objects = parent->objects;
	// If the boxes are too small we stop the recursion
    if(objects.size() <= m_minObj ||
		(parent->aabb->size().x() <= 0.00001
		&& parent->aabb->size().y() <= 0.00001
		&& parent->aabb->size().z() <= 0.00001)) {
        return;
    }

	m_size++;

    AlignedBox3f* aabb = parent->aabb;

    Point3f center = aabb->center();
    Point3f min = aabb->min();
    Point3f max = aabb->max();
    //Vector3f halfV = aabb.size() / 2;
    //Point3f half = Point3f(halfV.x(), halfV.y(), halfV.z());

    /* Boxes has been written considering the following axis
     *
     *    | y
     *    |
     *    +----- x
     * z /
     *  /
     *
    */

	AlignedBox3f* aabb_nodes[8];
    // Front Top left
    aabb_nodes[0] = new AlignedBox3f(Point3f(min.x(), center.y(), min.z()), Point3f(center.x(), max.y(), center.z()));
    // Front Top Right
    aabb_nodes[1] = new AlignedBox3f(Point3f(center.x(), center.y(), min.z()), Point3f(max.x(), max.y(), center.z()));
    // Front Bot Left
    aabb_nodes[2] = new AlignedBox3f(min, center);
    // Front Bot Right
    aabb_nodes[3] = new AlignedBox3f(Point3f(center.y(), min.y(), min.z()), Point3f(max.x(), center.y(), center.z()));
    // Back Top Left
    aabb_nodes[4] = new AlignedBox3f(Point3f(min.x(), center.y(), center.z()), Point3f(center.x(), max.y(), max.z()));
    // Back Top Right
    aabb_nodes[5] = new AlignedBox3f(center, max);
    // Back Bot Left
    aabb_nodes[6] = new AlignedBox3f(Point3f(min.x(), min.y(), center.z()), Point3f(center.x(), center.y(), max.z()));
    // Back Bot Right
    aabb_nodes[7] = new AlignedBox3f(Point3f(center.x(), min.y(), center.z()), Point3f(max.x(), center.y(), max.z()));

    QVector<Vertex> objs[8];
    for(int i  = 0 ; i < objects.size() ; i++) {
        /* For each bounding box we test if the object from the list is inside it.
        *  If true : we can push it into the obj list of the node and break out the for loop
        *  since we don"t want duplicate. The first bounding box containing an object
        *  will be the only one.
        */
        for(int j = 0 ; j < 8 ; j++) {
            if(aabb_nodes[j]->contain(objects.at(i).position)) {
                objs[j].push_back(objects.at(i));
                break;
            }
        }
    }

    for(int i = 0 ; i < 8 ; i++) {
        if(objs[i].size() != 0) {
            Node* n = new Node();
            n->aabb = aabb_nodes[i];
            n->objects = objs[i];
            n->parent = parent;
			n->box = new WireBoundingBox(*n->aabb, Color4f(0.f, 0.f, 1.f));
			n->box->setFunctions(*m_functions);
			n->box->init();
            parent->childs.push_back(n);
            buildNode(n);
        }
    }
}

void Octree::render(const Scene &scene, const QMatrix4x4 &transform) const {
    m_octree->box->render(scene, transform);
    renderNode(scene, transform, m_octree);
}

void Octree::renderNode(const Scene &scene, const QMatrix4x4 &transform, Node* currentNode) const {
    for(Node* child : currentNode->childs) {;
		child->box->render(scene, transform);
        renderNode(scene, transform, child);
    }
}
