#ifndef COCTREE_H
#define COCTREE_H

#include "utils.h"
#include "alignedbox3f.h"
#include "scene.h"

#include <QVector>
#include <QGLFunctions>

class cOctree
{
public:

    /* This is a simple representation of an octree node.
     * An octree node contain
     * - A reference to its parent
     * - 8 references to its child
     * - A list of the objects it contains
    */
    struct Node {
        AlignedBox3f aabb;
        const Node* parent;
        const Node* childs[8];
        const QVector<Vertex>* objects;
        bool isLeaf;
        bool isEmpty() { return objects->size() == 0; }
    };

    cOctree(const QVector<Vertex>& vertices);
    void build();
    void buildNode(Node parent);

    void render(const Scene& scene, const QMatrix4x4& transform) const;


    void setFunctions(QGLFunctions& f)              { m_functions = &f; }

private:
    // This is all the vertices we are going to build the octree of.
    const QVector<Vertex> m_vertices;
    // This is a list of all the nodes containted in the octree
    Node m_octree;

    QGLFunctions* m_functions;
};

#endif // COCTREE_H
