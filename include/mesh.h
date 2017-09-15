#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QMatrix4x4>

#include "utils.h"

class Octree;
class QGLShaderProgram;
class QGLFunctions;

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void init();
    void render() const;
    void render(QGLShaderProgram& shader) const;
    void rawData(const QVector<Vertex>& vertices, const QVector<FaceIndex>& indices);



    void clear();
    void computeBoundingBox();
    void buildOctree();

    AABB boundingBox() const        { return m_boundingBox; }
    Octree* octree() const          { return m_octree;      }
    QMatrix4x4 transform() const    { return m_transform;   }

private:

    QVector<Vertex>     m_vertices;
    QVector<FaceIndex>  m_faces;

    AABB                m_boundingBox;
    Octree*             m_octree;

    uint m_vao;
    uint        m_vertexBufferId;
    uint        m_indexBufferId;

    QGLFunctions*       m_functions;
    QGLShaderProgram*   m_shaderProgram;

    QMatrix4x4          m_transform;

    QMatrix4x4 mView;
    QMatrix4x4 mProjection;
};

#endif // MESH_H
