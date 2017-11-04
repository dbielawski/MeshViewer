#ifndef WIREBOUNDINGBOX_H
#define WIREBOUNDINGBOX_H

#include <QGLFunctions>

#include "utils.h"
#include "alignedbox3f.h"
#include <QVector4D>

class Scene;

class WireBoundingBox
{
public:
    WireBoundingBox(const Color4f& color = Color4f(0.f, 1.f, 0.f));
    WireBoundingBox(const AlignedBox3f& aabb, const Color4f& color = Color4f(0.f , 1.f , 0.f));

    void init();
    void render(const Scene& scene, const QMatrix4x4& transform) const;

    void setAlignedBox(const AlignedBox3f& aabb)    { m_aabb = &aabb; }
    void setColor(const Color4f& color)             { m_color = color; }
    void setFunctions(QGLFunctions& f)              { m_functionsPtr = &f; }

private:
    void computeBox();

    QGLFunctions* m_functionsPtr;
    const AlignedBox3f* m_aabb;
    Color4f m_color;

    uint    m_vertexBufferId;
    uint    m_indexBufferId;

    Point3f  m_points[8];
    Vector2i m_indices[12];
};

#endif // WIREBOUNDINGBOX_H
