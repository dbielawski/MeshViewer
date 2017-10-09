#include "scene.h"

#include "camera.h"
#include "mesh.h"
#include "light.h"
#include <QDebug>

#include <iostream>


Scene::Scene() :
    m_shaderProgram(new QGLShaderProgram),
    m_camera(new Camera),
    m_backgroundColor(0.f, 0.f, 0.f)
{
    m_camera->lookAt(Point3f(0.f, 0.f, 3.f),
                     Point3f(0.f, 0.f, 0.f),
                     Vector3f(0.f, 1.f, 0.f));

    m_camera->setPerspective(60.f, 0.001f, 1000.f);

    m_displayBoundingBox = false;
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
    // For now we deal with only one light source
    if (m_lightList.size() > 0 && m_lightList.at(0))
    {
        Vector3f dir = m_lightList[0]->direction();
        Color4f intensity = m_lightList[0]->intensity(Point3f());
        m_shaderProgram->setUniformValue("light_dir", QVector3D(dir.x, dir.y, dir.z));
        m_shaderProgram->setUniformValue("light_intensity", QColor(intensity.r, intensity.g, intensity.b));
    }

    for (const Mesh* m : m_meshList)
    {
        m->renderMesh();
        if (m_displayBoundingBox)
            m->renderBoundingBox();
    }
}

void Scene::clear()
{
	for(int i = 0 ; i < m_lightList.size(); i++) {
		const Light* l = m_lightList.at(i);
		m_lightList.pop_front();
		delete l;
	}

	for(int i = 0 ; i < m_meshList.size(); i++) {
		const Mesh* m = m_meshList.at(i);
		m_meshList.pop_front();
		delete m;
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

	Point3f center = mesh.boundingBox()->center();
	m_camera->lookAt(Point3f(0.f, 0.f, 3.f),
                     center,
                     Vector3f(0.f, 1.f, 0.f));
}

void Scene::addLight(const Light& light)
{
    m_lightList.append(&light);
}

unsigned int Scene::verticesCount() const
{
    unsigned int totalVerticesCount = 0;

    for (const Mesh* m : m_meshList)
        totalVerticesCount += m->verticesCount();

    return totalVerticesCount;
}

unsigned int Scene::trianglesCount() const
{
    unsigned int totalTrianglesCount = 0;

    for (const Mesh* m : m_meshList)
        totalTrianglesCount += m->trianglesCount();

    return totalTrianglesCount;
}

unsigned int Scene::facesCount() const
{
    unsigned int totalFacesCount = 0;

    for (const Mesh* m : m_meshList)
        totalFacesCount += m->facesCount();

    return totalFacesCount;
}

unsigned int Scene::edgesCount() const
{
    unsigned int totalEdgesCount = 0;

    for (const Mesh* m : m_meshList)
        totalEdgesCount += m->edgesCount();

    return totalEdgesCount;
}
