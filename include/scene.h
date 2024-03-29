#ifndef SCENE_H
#define SCENE_H

#include <QGLShaderProgram>
#include <QFile>
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
    void fillHoles();

    void toggleDisplayBoundingBox();
    void toggleDisplayOctree();

    void addMesh(Mesh& model);
    void addLight(const Light& light);

	void setBackgroundColor(const Color4f& color)   { m_backgroundColor = color; }
    Color4f backgroundColor() const                 { return m_backgroundColor; }

    Camera* camera() const                          { return m_camera; }
    QGLShaderProgram* simpleShaderProgram() const   { return m_simpleShaderProgram;  }
    QGLShaderProgram* simpleShadingProgram() const  { return m_simpleshadingProgram; }

    void setTransparency(float t);
    float transparency() const { return m_transparency; }

    void toggleFilledLinesMode() { m_filledAndLinesMode = !m_filledAndLinesMode; }

    uint vertexCount() const;
    uint triangleCount() const;
    uint faceCount() const;
    uint edgeCount() const;
    uint lightCount() const { return m_lightList.size();    }
    uint meshCount() const  { return m_meshList.size();     }
    bool isValid() const;
    bool isClosed() const;

    void detectHoles();

    void saveMeshes(const QString fileName) const;

    void drawAxis() const;

private:
    void loadShader(QGLShaderProgram* program, const QString& fileName);

    QVector<Mesh*> m_meshList;
    QVector<const Light*> m_lightList;

    // Used to draw AABB, octree..
    QGLShaderProgram* m_simpleShaderProgram;

    // Used to draw models with effects
    QGLShaderProgram* m_simpleshadingProgram;

    Camera* m_camera;
    Color4f m_backgroundColor;

    bool m_displayBoundingBox;
    bool m_displayOctree;
    uint m_octreeMode;

    float m_transparency;

    bool m_filledAndLinesMode;
};

#endif // SCENE_H
