#ifndef OCTREE_H
#define OCTREE_H

#include <QGLFunctions>
#include "utils.h"
#include "alignedbox3f.h"
#include "wireboundingbox.h"

class Octree
{
public:

    /* This is a simple representation of an octree node.
     * An octree node contain
     * - A reference to its parent
     * - 8 references to its child
     * - A list of the objects it contains
    */
    struct Node {
        AlignedBox3f* aabb;
        Node* parent;
		WireBoundingBox* box;
        QVector<Node*> childs;
        QVector<Vertex> objects;

        Node() {}
        Node(AlignedBox3f* aabb, Node* parent, WireBoundingBox* box, QVector<Vertex> objects) :
            aabb(aabb), parent(parent), box(box), objects(objects)
        {}

        ~Node() {
			delete aabb;
			delete box;
			for (Node* child : childs) {
				delete child;
			}
		}

        bool isEmpty() { return objects.size() == 0; }
        bool isLeaf()  { return childs.size() == 0; }
    };

    Octree(const QVector<Vertex>& vertices, uint m_minObj = 5);
    ~Octree() { delete m_octree; }

    void build();
    void buildNode(Node* parent);

    void buildShapeFromOctree();
    void buildShape(Node* currentNode);

    void render(const Scene& scene, const QMatrix4x4& transform, bool renderFullOctree) const;
    void renderNode(const Scene &scene, const QMatrix4x4 &transform, Node* currentNode, bool renderFullOctree) const;

    void setFunctions(QGLFunctions& f) { m_functions = &f; }

private:
    // This is all the vertices we are going to build the octree of.
    const QVector<Vertex> m_vertices;
    // This is a list of all the nodes containted in the octree
    Node* m_octree;

	uint m_minObj;
	uint m_size;
    QGLFunctions* m_functions;
};

#endif // COCTREE_H
