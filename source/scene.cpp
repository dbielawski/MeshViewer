#include "scene.h"

#include <iostream>
#include <QDebug>
#include <QMessageBox>

#include "camera.h"
#include "mesh.h"
#include "light.h"

Scene::Scene() :
    m_simpleShaderProgram(new QGLShaderProgram),
    m_simpleshadingProgram(new QGLShaderProgram),
    m_camera(new Camera),
    m_backgroundColor(0.f, 0.f, 0.f)
{
    m_camera->lookAt(Point3f(0.f, 0.f, 3.f),
                     Point3f(0.f, 0.f, 0.f),
                     Vector3f::UnitY());

    m_camera->setPerspective(60.f, 0.001f, 1000.f);

    m_displayBoundingBox = false;
    m_displayOctree = false;

    m_transparency = 1.f;

    // Add some lights
    m_lightList.append(new DirectionalLight(Color4f::white(),    Vector3f::UnitZ()));
    m_lightList.append(new DirectionalLight(Color4f::white(), -1*Vector3f::UnitZ()));
    m_lightList.append(new DirectionalLight(Color4f::white(),    Vector3f::UnitX()));
    m_lightList.append(new DirectionalLight(Color4f::white(), -1*Vector3f::UnitX()));
    m_lightList.append(new DirectionalLight(Color4f::random(),   Vector3f::UnitY()));
    m_lightList.append(new DirectionalLight(Color4f::white(), -1*Vector3f::UnitY()));
}

Scene::~Scene()
{
    removeModels();
    removeLights();
    delete m_camera;
    delete m_simpleShaderProgram;
    delete m_simpleshadingProgram;
}

void Scene::saveMeshes(const QString fileName) const
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(0, "Error while writing file", file.errorString());
        return;
    }

    QTextStream out(&file);

    QString applicationName = "MeshViewer";
    QString header = "# This file was generated with " + applicationName + "\n";
    out << header;

    int nb_vertices = 0;
    for (const Mesh* m : m_meshList) {
        m->saveAsObj(out, nb_vertices);
        nb_vertices += m->verticesCount();
    }

    file.close();
}

void Scene::fillHoles()
{
    for (Mesh* mesh : m_meshList) {
        if (!mesh->isClosed()) {
            mesh->fillHoles();
        }
    }
}

void Scene::init()
{
    loadShader(m_simpleShaderProgram, QString("simpleshader"));
    loadShader(m_simpleshadingProgram, QString("simpleshading"));

    // TODO: Clean this and use the light in the lightlist ??
    glEnable(GL_LIGHT0);
    GLfloat pos0[] = {-10.0, 0.0, 0.0, 1.0};
    GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

    glEnable(GL_LIGHT1);
    GLfloat pos1[] = {10.0, 0.0, 0.0, 1.0};
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);

    glEnable(GL_LIGHT2);
    GLfloat pos2[] = {0.0, 10.0, 0.0, 1.0};
    glLightfv(GL_LIGHT2, GL_POSITION, pos2);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse);

    glEnable(GL_LIGHT3);
    GLfloat pos3[] = {0.0, -10.0, 0.0, 1.0};
    glLightfv(GL_LIGHT3, GL_POSITION, pos3);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuse);

    glEnable(GL_LIGHT4);
    GLfloat pos4[] = {0.0, 0.0, 10.0, 1.0};
    glLightfv(GL_LIGHT4, GL_POSITION, pos4);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, diffuse);

    glEnable(GL_LIGHT5);
    GLfloat pos5[] = {0.0, 0.0, -10.0, 1.0};
    glLightfv(GL_LIGHT5, GL_POSITION, pos5);
    glLightfv(GL_LIGHT5, GL_DIFFUSE, diffuse);
}

void Scene::render() const
{
    for (int i = 0; i < m_lightList.size(); ++i) {
        const Light* l = m_lightList.at(i);

        if (l != Q_NULLPTR && m_simpleshadingProgram->bind()) {
            Vector3f dir = l->direction();
            Color4f intensity = l->intensity(Point3f());
            QString color = "lights[" + QString::number(i) + "].intensity";
            QString direction = "lights[" + QString::number(i) + "].direction";

            m_simpleshadingProgram->setUniformValue(direction.toStdString().c_str(), dir.x(), dir.y(), dir.z());
            m_simpleshadingProgram->setUniformValue(color.toStdString().c_str(), intensity.r, intensity.g, intensity.b);
            m_simpleshadingProgram->release();
        }
    }

    for (const Mesh* m : m_meshList) {
        if (m != Q_NULLPTR) {
            m->renderMesh();

            if (m_displayBoundingBox) {
                m->renderBoundingBox();
            }
            if(m_displayOctree) {
                m->renderOctree();
            }
        }
    }
}

void Scene::removeLights()
{
    for (int i = m_lightList.size() - 1; i >= 0; i--)
    {
        const Light* l = m_lightList.at(i);
        m_lightList.pop_back();
        delete l;
        l = Q_NULLPTR;
    }
}

void Scene::removeModels()
{
    for (int i = m_meshList.size() - 1; i >= 0; i--)
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

void Scene::toggleDisplayOctree()
{
    m_displayOctree = !m_displayOctree;
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

void Scene::setTransparency(float t)
{
    m_transparency = t;
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

bool Scene::isValid() const
{
    for (const Mesh* m : m_meshList)
        if (!m->isValid())
            return false;

    return true;
}

bool Scene::isClosed() const
{
    for (const Mesh* m : m_meshList)
        if (!m->isClosed())
            return false;

    return true;
}

void Scene::detectHoles()
{
    for (Mesh* m : m_meshList)
        m->detectHoles();
}

void Scene::loadShader(QGLShaderProgram* program, const QString& fileName)
{
    if (!program->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/" + fileName + ".vert"))
    {
        QMessageBox::critical(0, "Shader", "Vertex Shaders cannot be loaded!");
        return;
    }

    if (!program->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/" + fileName + ".frag"))
    {
        QMessageBox::critical(0, "Shader", "Fragment Shaders cannot be loaded " + fileName + "!");
        return;
    }

    if (!program->link())
    {
        QMessageBox::critical(0, "Shader", "Shaders cannot be linked " + fileName + "!");
        return;
    }

    if (!program->bind())
    {
        QMessageBox::critical(0, "Shader", "Shaders cannot be binded " + fileName + "!");
        return;
    }
    program->release();
}
