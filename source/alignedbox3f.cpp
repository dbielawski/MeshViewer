#include "alignedbox3f.h"
#include <QtMath>

AlignedBox3f::AlignedBox3f()
{

}

AlignedBox3f::AlignedBox3f(const Vector3f &min, const Vector3f &max) :
    m_min(min), m_max(max)
{

}

void AlignedBox3f::init()
{
//    m_functions->glGenBuffers(1, &m_vertexBufferId);
//    m_functions->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
//    m_functions->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

//    m_functions->glGenBuffers(1, &m_indexBufferId);
//    m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
//    m_functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(FaceIndex) * m_faces.size(), &m_faces[0], GL_STATIC_DRAW);
}

void AlignedBox3f::render() const
{
//    m_functions->glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
//    m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

//    int vertex_loc = prg->attrib("vtx_position");
//    int color_loc = prg->attrib("vtx_color");

//    m_functions->glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
//    m_functions->glEnableVertexAttribArray(vertex_loc);

//    m_functions->glVertexAttribPointer(vertex_loc, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
//    m_functions->glEnableVertexAttribArray(color_loc);

//    m_functions->glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, (void*)0);

//    if (vertexLoc >= 0) m_functions->glDisableVertexAttribArray(vertexLoc);
//    if (colorLoc >= 0)  m_functions->glDisableVertexAttribArray(colorLoc);
}

void AlignedBox3f::extend(const Vector3f &v)
{
    m_min = qMin(m_min.x, v.x);
    m_min = qMin(m_min.y, v.y);
    m_min = qMin(m_min.z, v.z);

    m_max.x = qMin(m_max.x, v.x);
    m_max.x = qMin(m_max.y, v.y);
    m_max.x = qMin(m_max.z, v.z);
}

bool AlignedBox3f::contain(const Vector3f &v) const
{
    return (m_min.x >= v.x) && (m_min.y >= v.y) && (m_min.z >= v.z)
            && (m_max.x <= v.x) && (m_max.y <= v.y) && (m_max.z <= v.z);
}

void AlignedBox3f::setupPoints()
{
//    m_points[8] = { Point3f(), Point3f(), Point3f(), Point3f()
//                    Point3f(), Point3f(), Point3f(), Point3f() };
}
