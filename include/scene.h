#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include "utils.h"


class Mesh;
class Light;
class Camera;

#include <QGLShaderProgram>

class Scene
{
public:
    Scene();
    ~Scene();

    void render() const;
    void render(QGLShaderProgram& sahder) const;
    void clear();

    void addMesh(const Mesh& model);
    void addLight(const Light& light);

    void setBackgroundColor(const Color3f& color)   { m_backgroundColor = color; }
    Color3f backgroundColor() const                 { return m_backgroundColor; }

    Camera* camera() const                          { return m_camera; }

private:
    QVector<const Mesh*>    m_meshList;
    QVector<const Light*>   m_lightList;

    Camera*                 m_camera;
    Color3f                 m_backgroundColor;
};

#endif // SCENE_H
