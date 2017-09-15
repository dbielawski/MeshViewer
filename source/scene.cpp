#include "scene.h"

#include "camera.h"
#include "mesh.h"


Scene::Scene():
    m_camera(new Camera),
    m_backgroundColor(.8f, .8f, .8f)
{
    m_camera->lookAt(Point3f(0.f, 5.f, -5.f),
                     Point3f(0.f, 0.f, 0.f),
                     Point3f(0.f, 1.f, 0.f));

    m_camera->setPerspective(60.f, 0.001f, 1000.f);
}

Scene::~Scene()
{
    clear();
    delete m_camera;
}

void Scene::render() const
{
    for (const Mesh* m : m_meshList)
        m->render();
}

void Scene::render(QGLShaderProgram& shader) const
{
    for (const Mesh* m : m_meshList)
        m->render(shader);
}

void Scene::clear()
{
    for (const Light* l : m_lightList)
        delete l;

    for (const Mesh* m : m_meshList)
        delete m;
}

void Scene::addMesh(const Mesh& mesh)
{
    m_meshList.append(&mesh);
}

void Scene::addLight(const Light& light)
{
    m_lightList.append(&light);
}
