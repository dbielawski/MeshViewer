#include "include/octree.h"

Octree::Octree(const QVector<Vertex>& vertices) :
    m_verticesPtr(&vertices)
{

}

Octree::~Octree()
{

}

void Octree::build()
{
    AlignedBox3f aabb;
    for (int i = 0; i < m_verticesPtr->size(); ++i)
    {
        aabb.extend(m_verticesPtr->at(i).position);
    }

    //buildNode(aabb);
}

void Octree::render() const
{

}

void Octree::buildNode()
{

}
