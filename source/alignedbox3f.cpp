#include "alignedbox3f.h"


AlignedBox3f::AlignedBox3f()
{

}

AlignedBox3f::AlignedBox3f(const Vector3f &min, const Vector3f &max) :
    m_min(min), m_max(max)
{

}

void AlignedBox3f::init()
{

}

void AlignedBox3f::render() const
{

}

void AlignedBox3f::extend(const Vector3f &v)
{
    if (m_min.x > v.x)
        m_min = v.x;
    if (m_min.y > v.y)
        m_min.y = v.y;
    if (m_min.z > v.z)
        m_min.z = v.z;

    if (m_max.x < v.x)
        m_max.x = v.x;
    if (m_max.y < v.y)
        m_max.y = v.y;
    if (m_max.z < v.z)
        m_max.z = v.z;
}

bool AlignedBox3f::contain(const Vector3f &v) const
{
    return (m_min.x >= v.x) && (m_min.y >= v.y) && (m_min.z >= v.z)
            && (m_max.x <= v.x) && (m_max.y <= v.y) && (m_max.z <= v.z);
}
