#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QMatrix4x4>

#include "utils.h"
#include "camera.h"
#include "scene.h"
#include "wireboundingbox.h"
#include "octree.h"
#include "polyhedron.h"

class Octree;

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void init();
    void renderMesh() const;
    void renderBoundingBox() const;
    void renderOctree() const;

    void rawData(const QVector<Vertex>& vertices,  const QVector<EdgeIndex>& edges, const QVector<FaceIndex>& faces);
    void displayableData(const QVector<Vertex>& vertices, const QVector<FaceIndex>& faces);

    void computeNormals();
    void clear();
    void computeBoundingBox();
    void buildOctree();

    AlignedBox3f* boundingBox() const   { return m_boundingBox; }
    Octree* octree() const              { return m_octree;      }
    QMatrix4x4 transform() const        { return m_transform;   }

    void attachScene(Scene* scene)      { m_scenePtr = scene; }

    unsigned int verticesCount() const  { return m_vertices.size(); }
    unsigned int trianglesCount() const { return m_faces.size();    }
    unsigned int facesCount() const     { return m_faces.size();    }
    unsigned int edgesCount() const     { return m_allEdges.size(); }
    bool isValid() const { return m_polyhedron.is_valid(); }
    bool isClosed() const { return m_polyhedron.is_closed(); }
    void fillHoles();
    void detectHoles();

    void saveAsObj(QTextStream& out, int offset) const;


private:
    QGLFunctions*       m_functions;

    uint                m_vertexBufferId;
    uint                m_indexBufferId;

    QVector<Vertex>     m_vertices;
    QVector<FaceIndex> 	m_faces;

    QVector<Vertex>     m_allVertices;
    QVector<EdgeIndex> 	m_allEdges;
    QVector<FaceIndex>  m_allFaces;

    QMatrix4x4          m_transform;

    AlignedBox3f*       m_boundingBox;
    WireBoundingBox*    m_wireBoundingBox;

    Octree*             m_octree;

    const Scene*        m_scenePtr;
    Polyhedron          m_polyhedron;
};

#endif // MESH_H
