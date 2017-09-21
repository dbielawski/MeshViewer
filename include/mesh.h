#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QMatrix4x4>

#include "utils.h"
#include "alignedbox3f.h"

class QGLFunctions;
class QGLShaderProgram;

class Octree;

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

    AlignedBox3f boundingBox() const    { return m_boundingBox; }
    Octree* octree() const              { return m_octree;      }
    QMatrix4x4 transform() const        { return m_transform;   }

    // TODO: supprimer
    QMatrix4x4 mView;
    QMatrix4x4 mProjection;

private:

    QVector<Vertex>     m_vertices;
    QVector<FaceIndex>  m_faces;

    AlignedBox3f        m_boundingBox;
    Octree*             m_octree;

    uint                m_vertexBufferId;
    uint                m_indexBufferId;

    QGLFunctions*       m_functions;
    QGLShaderProgram*   m_shaderProgram;

    QMatrix4x4          m_transform;
};

#endif // MESH_H
