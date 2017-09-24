#include "mesh.h"

#include <QGLFunctions>
#include <QGLShaderProgram>
#include <iostream>
#include <assert.h>

Mesh::Mesh() : m_octree(nullptr)
{
    m_functions = new QGLFunctions;
    m_functions->initializeGLFunctions();

    m_shaderProgramPtr = new QGLShaderProgram;
    m_shaderProgramPtr->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/simpleshader.vert");
    m_shaderProgramPtr->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/simpleshader.frag");
    m_shaderProgramPtr->link();

    if (!m_shaderProgramPtr->isLinked())
    {
        std::cout << "Shaders cannot be linked" << std::endl;
        exit(-1);
    }

    if (!m_shaderProgramPtr->bind())
    {
        std::cout << "Shaders cannot be binded" << std::endl;
        exit(-1);
    }

    m_transform.setToIdentity();
}

Mesh::~Mesh()
{
    m_functions->glDeleteBuffers(1, &m_vertexBufferId);
    m_functions->glDeleteBuffers(1, &m_indexBufferId);
    delete m_functions;
}

void Mesh::init()
{
    m_functions->glGenBuffers(1, &m_vertexBufferId);
    m_functions->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    m_functions->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

    m_functions->glGenBuffers(1, &m_indexBufferId);
    m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
    m_functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(FaceIndex) * m_faces.size(), &m_faces[0], GL_STATIC_DRAW);
}

void Mesh::render() const
{
    // TODO: enlever
    assert(m_shaderProgramPtr->bind());

    m_functions->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);

    m_shaderProgramPtr->setUniformValue("mat_obj", m_transform);
    m_shaderProgramPtr->setUniformValue("mat_view", m_scenePtr->camera()->viewMatrix());
    m_shaderProgramPtr->setUniformValue("mat_proj", m_scenePtr->camera()->projectionMatrix());

    int vertexLoc = m_shaderProgramPtr->attributeLocation("vtx_position");
    int colorLoc  = m_shaderProgramPtr->attributeLocation("vtx_color");

    if (vertexLoc >= 0)
    {
        m_functions->glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        m_functions->glEnableVertexAttribArray(vertexLoc);
    }
    else // TODO: enlever
        assert(false);

    if (colorLoc >= 0)
    {
        m_functions->glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
        m_functions->glEnableVertexAttribArray(colorLoc);
    }
    else // TODO: enlever
        assert(false);

    glDrawElements(GL_TRIANGLES, 3 * m_faces.size(), GL_UNSIGNED_INT, (void*)0);

    if (vertexLoc >= 0) m_functions->glDisableVertexAttribArray(vertexLoc);
    if (colorLoc >= 0)  m_functions->glDisableVertexAttribArray(colorLoc);
}

void Mesh::rawData(const QVector<Vertex>& vertices, const QVector<FaceIndex>& faces)
{
    m_vertices = vertices;
    m_faces = faces;
}

