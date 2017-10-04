#include "scene.h"

#include "camera.h"
#include "mesh.h"
#include "light.h"
#include <QDebug>


Scene::Scene() :
    m_shaderProgram(new QGLShaderProgram),
    m_camera(new Camera),
    m_backgroundColor(0.f, 0.f, 0.f)
{
    m_camera->lookAt(Point3f(3.f, 0.f, 3.f),
                     Point3f(0.f, 0.f, 0.f),
                     Vector3f(0.f, 1.f, 0.f));

    m_camera->setPerspective(60.f, 0.001f, 1000.f);
}

Scene::~Scene()
{
    clear();
    delete m_camera;
    delete m_shaderProgram;
}

void Scene::init()
{
    if (!m_shaderProgram->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/simpleshader.vert"))
    {
        qDebug() << "Shaders cannot be Vertex";
        exit(-1);
    }

    if (!m_shaderProgram->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/simpleshader.frag"))
    {
        qDebug() << "Fragment Shaders cannot be loader";
        exit(-1);
    }

    if (!m_shaderProgram->link())
    {
        qDebug() << "Shaders cannot be linked";
        exit(-1);
    }

    if (!m_shaderProgram->bind())
    {
        qDebug() << "Shaders cannot be binded";
        exit(-1);
    }
}

void Scene::render() const
{
    for (const Mesh* m : m_meshList)
        m->render();
}

void Scene::clear()
{
    for (const Light* l : m_lightList)
        delete l;

    for (const Mesh* m : m_meshList)
        delete m;
}

void Scene::addMesh(Mesh& mesh)
{
    mesh.attachScene(this);

    m_meshList.append(&mesh);
}

void Scene::addLight(const Light& light)
{
    m_lightList.append(&light);
}
