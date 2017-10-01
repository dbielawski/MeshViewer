#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QMatrix4x4>

#include "utils.h"
#include "camera.h"
#include "scene.h"
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
    void rawData(const QVector<Vertex>& vertices, const QVector<FaceIndex>& indices);


    void computeNormals();
    void clear();
    void computeBoundingBox();
    void buildOctree();

    AlignedBox3f boundingBox() const                { return m_boundingBox; }
    Octree* octree() const                          { return m_octree;      }
    QMatrix4x4 transform() const                    { return m_transform;   }

    void attachShader(QGLShaderProgram* shaderProgram)  { m_shaderProgramPtr = shaderProgram;   }
    QGLShaderProgram* shader() const                    { return m_shaderProgramPtr;            }

    void attachCamera(Camera* camera)               { m_cameraPtr = camera; }
    Camera* camera() const                          { return m_cameraPtr;   }

    void attachScene(Scene* scene) { m_scenePtr = scene; }

private:
    QGLFunctions*       m_functions;

    QGLShaderProgram*   m_shaderProgramPtr;
    uint                m_vertexBufferId;
    uint                m_indexBufferId;

    QVector<Vertex>     m_vertices;
    QVector<FaceIndex>  m_faces;
    QMatrix4x4          m_transform;

    AlignedBox3f        m_boundingBox;
    Octree*             m_octree;
    Camera*             m_cameraPtr;

    Scene*              m_scenePtr;
};

#endif // MESH_H

