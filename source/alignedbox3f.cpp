#include "alignedbox3f.h"
#include <QtMath>

AlignedBox3f::AlignedBox3f()
{

}

AlignedBox3f::AlignedBox3f(const Point3f& min, const Point3f& max) :
    m_min(min), m_max(max)
{

}

void AlignedBox3f::extend(const Point3f& p)
{
    m_min.x = qMin(m_min.x, p.x);
    m_min.y = qMin(m_min.y, p.y);
    m_min.z = qMin(m_min.z, p.z);

    m_max.x = qMax(m_max.x, p.x);
    m_max.y = qMax(m_max.y, p.y);
    m_max.z = qMax(m_max.z, p.z);
}

bool AlignedBox3f::contain(const Point3f& p) const
{
    return (m_min.x >= p.x) && (m_min.y >= p.y) && (m_min.z >= p.z)
            && (m_max.x <= p.x) && (m_max.y <= p.y) && (m_max.z <= p.z);
}

void AlignedBox3f::reset()
{
    // TODO: implementer reset
}
