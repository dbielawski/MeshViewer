#include "scene.h"

#include "camera.h"
#include "mesh.h"
#include "light.h"
#include <iostream>
#include <assert.h>


Scene::Scene() :
    m_shaderProgram(new QGLShaderProgram),
    m_camera(new Camera),
    m_backgroundColor(.9f, .9f, .9f)
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
    //    m_shaderProgram = new QGLShaderProgram;

    if (!m_shaderProgram->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/simpleshader.vert"))
    {
        std::cout << "Shaders cannot be Vertex" << std::endl;
        exit(-1);
    }

    if (!m_shaderProgram->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/simpleshader.frag"))
    {
        std::cout << "Shaders cannot be Fragment" << std::endl;
        exit(-1);
    }
    m_shaderProgram->link();

    if (!m_shaderProgram->isLinked())
    {
        std::cout << "Shaders cannot be linked" << std::endl;
        exit(-1);
    }

    if (!m_shaderProgram->bind())
    {
        std::cout << "Shaders cannot be binded" << std::endl;
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
    mesh.attachCamera(m_camera);

//    mesh.attachShader(m_shaderProgram);
    mesh.attachScene(this);

    m_meshList.append(&mesh);
}

void Scene::addLight(const Light& light)
{
    m_lightList.append(&light);
}
