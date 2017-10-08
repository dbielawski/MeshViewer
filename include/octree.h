#ifndef OCTREE_H
#define OCTREE_H

#include <QVector>

#include "utils.h"

#include "alignedbox3f.h"

class Octree
{
public:

    struct OctreeNode
    {
        AlignedBox3f boundingBox;
        OctreeNode* childs[8];
        /*QVector<> list;

        bool isLeaf() const     { return childs == nullptr;             }
        bool isEmpty() const    { return list.isEmpty();                }
        bool isValid() const    { return isLeaf() ? !isEmpty() : false; }
        */
    };

    Octree(const QVector<Vertex>& vertices);
    ~Octree();

    void build();

    void render() const;

private:
    void renderNode() const;
    void buildNode();

    unsigned int m_depth;
    unsigned int m_maxDepth;

    const QVector<Vertex>* m_verticesPtr;
};

#endif // OCTREE_H
