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
    void clear();

    void addMesh(Mesh& model);
    void addLight(const Light& light);

    void setBackgroundColor(const Color3f& color)   { m_backgroundColor = color; }
    Color3f backgroundColor() const                 { return m_backgroundColor; }

    Camera* camera() const                          { return m_camera; }
    QGLShaderProgram* shaderProgram() const         { return m_shaderProgram; }

private:
    QVector<const Mesh*>    m_meshList;
    QVector<const Light*>   m_lightList;

    QGLShaderProgram*       m_shaderProgram;

    Camera*                 m_camera;
    Color3f                 m_backgroundColor;
};

#endif // SCENE_H
