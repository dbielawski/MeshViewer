#include "scene.h"

#include "camera.h"
#include "mesh.h"
#include "light.h"
#include <QDebug>

#include <iostream>
#include <QDebug>


Scene::Scene() :
    m_simpleShaderProgram(new QGLShaderProgram),
    m_simpleshadingProgram(new QGLShaderProgram),
    m_camera(new Camera),
    m_backgroundColor(0.f, 0.f, 0.f)
{
    m_camera->lookAt(Point3f(0.f, 0.f, 3.f),
                     Point3f(0.f, 0.f, 0.f),
                     Vector3f(0.f, 1.f, 0.f));

    m_camera->setPerspective(60.f, 0.001f, 1000.f);

    m_displayBoundingBox = false;

    // Add some lights
    const DirectionalLight* dl1 = new DirectionalLight(Color4f(1, 1, 1), Vector3f(0, 0, 1));
    const DirectionalLight* dl2 = new DirectionalLight(Color4f(1, 1, 1), Vector3f(0, 0, -1));
    const DirectionalLight* dl3 = new DirectionalLight(Color4f(1, 1, 1), Vector3f(1, 0, 0));
    const DirectionalLight* dl4 = new DirectionalLight(Color4f(1, 1, 1), Vector3f(-1, 0, 0));

    m_lightList.append(dl1);
    m_lightList.append(dl2);
    m_lightList.append(dl3);
    m_lightList.append(dl4);
}

Scene::~Scene()
{
    removeModels();
    removeLights();
    delete m_camera;
    delete m_simpleShaderProgram;
    delete m_simpleshadingProgram;
}

void Scene::init()
{
    loadSahder(m_simpleShaderProgram, QString("simpleshader"));
    loadSahder(m_simpleshadingProgram, QString("simpleshading"));
}

void Scene::render() const
{
    for (int i = 0; i < m_lightList.size(); ++i)
    {
        const Light* l = m_lightList.at(i);

        if (l != Q_NULLPTR && m_simpleshadingProgram->bind())
        {
            Vector3f dir = l->direction();
            Color4f intensity = l->intensity(Point3f());

            QString color = "lights[";
            color += QString::number(i);
            color += "].intensity";

            QString direction;
            direction = "lights[";
            direction += QString::number(i);
            direction +="].direction";

            m_simpleshadingProgram->setUniformValue(direction.toStdString().c_str(), dir.x, dir.y, dir.z);
            m_simpleshadingProgram->setUniformValue(color.toStdString().c_str(), intensity.r, intensity.g, intensity.b);
        }
    }

    for (const Mesh* m : m_meshList)
    {
        if (m != Q_NULLPTR)
        {
            m->renderMesh();

            if (m_displayBoundingBox)
                m->renderBoundingBox();
        }
    }
}

void Scene::removeLights()
{
    for(int i = m_lightList.size() - 1; i >= 0; i--)
    {
        const Light* l = m_lightList.at(i);
        m_lightList.pop_back();
        delete l;
        l = Q_NULLPTR;
    }
}

void Scene::removeModels()
{
    for(int i = m_meshList.size() - 1; i >= 0; i--)
    {
        const Mesh* m = m_meshList.at(i);
        m_meshList.pop_back();
        delete m;
        m = Q_NULLPTR;
    }

    this->render();
}

void Scene::toggleDisplayBoundingBox()
{
    m_displayBoundingBox = !m_displayBoundingBox;
}

void Scene::addMesh(Mesh& mesh)
{
    mesh.attachScene(this);
    m_meshList.append(&mesh);

    //	Point3f center = mesh.boundingBox()->center();
    //	m_camera->lookAt(Point3f(0.f, 0.f, 3.f),
    //                     center,
    //                     Vector3f(0.f, 1.f, 0.f));
}

void Scene::addLight(const Light& light)
{
    m_lightList.append(&light);
}

unsigned int Scene::vertexCount() const
{
    unsigned int totalVerticesCount = 0;

    for (const Mesh* m : m_meshList)
        totalVerticesCount += m->verticesCount();

    return totalVerticesCount;
}

unsigned int Scene::triangleCount() const
{
    unsigned int totalTrianglesCount = 0;

    for (const Mesh* m : m_meshList)
        totalTrianglesCount += m->trianglesCount();

    return totalTrianglesCount;
}

unsigned int Scene::faceCount() const
{
    unsigned int totalFacesCount = 0;

    for (const Mesh* m : m_meshList)
        totalFacesCount += m->facesCount();

    return totalFacesCount;
}

unsigned int Scene::edgeCount() const
{
    unsigned int totalEdgesCount = 0;

    for (const Mesh* m : m_meshList)
        totalEdgesCount += m->edgesCount();

    return totalEdgesCount;
}

void Scene::loadSahder(QGLShaderProgram* program, const QString& fileName)
{
    if (!program->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/" + fileName + ".vert"))
    {
        qDebug() << "Shaders cannot be Vertex" << fileName;
        exit(-1);
    }

    if (!program->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/" + fileName + ".frag"))
    {
        qDebug() << "Fragment Shaders cannot be loader" << fileName;
        exit(-1);
    }

    if (!program->link())
    {
        qDebug() << "Shaders cannot be linked" << fileName;
        exit(-1);
    }

    if (!program->bind())
    {
        qDebug() << "Shaders cannot be binded" << fileName;
        exit(-1);
    }
}
