#ifndef ALIGNEDBOX3F_H
#define ALIGNEDBOX3F_H


#include "utils.h"


class AlignedBox3f
{
public:
    AlignedBox3f();
    AlignedBox3f(const Point3f& min, const Point3f& max);

    void extend(const Point3f& p);
    bool contain(const Point3f& p) const;
    void reset();

    Vector3f size() const;

    Point3f center() const;

    Point3f min() const { return m_min; }
    Point3f max() const { return m_max; }

    std::string toString() {
        return std::string("min(" + std::to_string(m_min.x()) + ";" + std::to_string(m_min.y()) + ";" + std::to_string(m_min.z()) + ") ; "
                         + "max(" + std::to_string(m_max.x()) + ";" + std::to_string(m_max.y()) + ";" + std::to_string(m_max.z()) + ")");
    }

private:

    Point3f    m_min;
    Point3f    m_max;
};

#endif // ALIGNEDBOX3F_H
