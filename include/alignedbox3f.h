#ifndef ALIGNEDBOX3F_H
#define ALIGNEDBOX3F_H


#include "utils.h"


class QGLFunctions;
class QGLShaderProgram;

class AlignedBox3f
{
public:
    AlignedBox3f();
    AlignedBox3f(const Point3f& min, const Point3f& max);

    void init();
    void render() const;
    void extend(const Point3f& p);
    bool contain(const Point3f& p) const;
    void reset();

private:
    void setupPoints();

    Point3f    m_min;
    Point3f    m_max;

    uint        m_vertexBufferId;
    uint        m_indexBufferId;

    Color4f     m_color;

    QGLFunctions*       m_functions;
    QGLShaderProgram*   m_shaderProgram;

    Point3f             m_points[8];
};

#endif // ALIGNEDBOX3F_H
