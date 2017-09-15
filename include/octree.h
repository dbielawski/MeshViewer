#ifndef OCTREE_H
#define OCTREE_H

#include <QVector>

#include "utils.h"

class Octree
{
public:

    struct OctreeNode
    {
        AABB boundingBox;
        OctreeNode* childs[8];
        /*QVector<> list;

        bool isLeaf() const     { return childs == nullptr;             }
        bool isEmpty() const    { return list.isEmpty();                }
        bool isValid() const    { return isLeaf() ? !isEmpty() : false; }
        */
    };

    Octree();

    void render() const;

private:
    void renderNode() const;

    unsigned int m_depth;
    unsigned int m_maxDepth;
};

#endif // OCTREE_H
