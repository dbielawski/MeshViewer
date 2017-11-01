#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include <QGLShaderProgram>
#include <QGLFunctions>

#include "utils.h"

class Mesh;
class Light;
class Camera;

class Scene
{
public:
    Scene();
    ~Scene();

    void init();
    void render() const;
    void removeLights();
    void removeModels();

    void toggleDisplayBoundingBox();
    void toggleDisplayOctree();

    void addMesh(Mesh& model);
    void addLight(const Light& light);

	void setBackgroundColor(const Color4f& color)   { m_backgroundColor = color; }
    Color4f backgroundColor() const                 { return m_backgroundColor; }

    Camera* camera() const                          { return m_camera; }
    QGLShaderProgram* simpleShaderProgram() const   { return m_simpleShaderProgram;  }
    QGLShaderProgram* simpleShadingProgram() const  { return m_simpleshadingProgram; }

    unsigned int vertexCount() const;
    unsigned int triangleCount() const;
    unsigned int faceCount() const;
    unsigned int edgeCount() const;
    unsigned int lightCount() const { return m_lightList.size();    }
    unsigned int meshCount() const  { return m_meshList.size();     }
    bool isValid() const;
    bool isClosed() const;

    void saveMesh(const QStringList& fileNames);

private:
    void loadShader(QGLShaderProgram* program, const QString& fileName);

    QVector<const Mesh*>    m_meshList;
    QVector<const Light*>   m_lightList;

    // Used to draw AABB, octree..
    QGLShaderProgram*       m_simpleShaderProgram;

    // Used to draw models with effects
    QGLShaderProgram*       m_simpleshadingProgram;

    Camera*                 m_camera;
    Color4f                 m_backgroundColor;

    bool m_displayBoundingBox;
    bool m_displayOctree;
};

#endif // SCENE_H
