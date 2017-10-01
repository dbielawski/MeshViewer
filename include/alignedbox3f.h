#ifndef ALIGNEDBOX3F_H
#define ALIGNEDBOX3F_H


#include "utils.h"


class QGLFunctions;
class QGLShaderProgram;

class AlignedBox3f
{
public:
    AlignedBox3f();
    AlignedBox3f(const Vector3f& min, const Vector3f& max);

    void init();
    void render() const;
    void extend(const Vector3f& v);
    bool contain(const Vector3f& v) const;

private:
    void setupPoints();

    Vector3f    m_min;
    Vector3f    m_max;

    uint        m_vertexBufferId;
    uint        m_indexBufferId;

    Color4f     m_color;

    QGLFunctions*       m_functions;
    QGLShaderProgram*   m_shaderProgram;

    Point3f             m_points[8];
};

#endif // ALIGNEDBOX3F_H
