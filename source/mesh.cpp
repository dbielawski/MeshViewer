#include "mesh.h"

#include <QGLFunctions>
#include <QGLShaderProgram>
#include <iostream>


Mesh::Mesh() : m_octree(nullptr)
{
    m_functions = new QGLFunctions;
    m_functions->initializeGLFunctions();

    m_shaderProgram = new QGLShaderProgram;
    m_shaderProgram->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/simpleshader.vert");
    m_shaderProgram->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/simpleshader.frag");
    m_shaderProgram->link();

    if (!m_shaderProgram->isLinked())
    {
        std::cout << "Shaders not linked" << std::endl;
        exit(-1);
    }

    if (!m_shaderProgram->bind())
    {
        std::cout << "Shaders can't bind" << std::endl;
        exit(-1);
    }

    m_transform.setToIdentity();
}

Mesh::~Mesh()
{
    m_functions->glDeleteBuffers(1, &m_vertexBufferId);
    m_functions->glDeleteBuffers(1, &m_indexBufferId);
}

void Mesh::init()
{
    static const GLfloat g_vertex_buffer_data[] = {
       -1.0f, -1.0f, 0.0f,
       1.0f, -1.0f, 0.0f,
       0.0f,  1.0f, 0.0f,
    };

    m_functions->glGenBuffers(1, &m_vertexBufferId);
    m_functions->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    m_functions->glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//    m_functions->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

//    m_functions->glGenBuffers(1, &m_indexBufferId);
//    m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
//    m_functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(FaceIndex) * m_faces.size(), &m_faces[0], GL_STATIC_DRAW);

    // TODO: supprimer
    mView.lookAt(QVector3D(4.f, 3.f, 3.f), QVector3D(0.f, 0.f, 0.f), QVector3D(0.f, 1.f, 0.f));
    mProjection.perspective(60.f /** 3.14 / 180.f*/, 1200.f / 800.f, 0.001, 1000.f);
}

void Mesh::render() const
{
    m_shaderProgram->bind();
//    m_functions->glUseProgram(m_shaderProgram->programId());
    m_functions->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
//    m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);

//    m_shaderProgram->setUniformValue("mat_obj", m_transform);
//    m_shaderProgram->setUniformValue("mat_view", mView);
//    m_shaderProgram->setUniformValue("mat_proj", mProjection);

//    int vertexLoc = m_shaderProgram->attributeLocation("vtx_position");
//    int colorLoc  = m_shaderProgram->attributeLocation("vtx_color");

//    if (vertexLoc >= 0)
//    {
//        m_functions->glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//        m_functions->glEnableVertexAttribArray(vertexLoc);
//    }

//    if (colorLoc >= 0)
//    {
//        m_functions->glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
//        m_functions->glEnableVertexAttribArray(colorLoc);
//    }

    //glDrawElements(GL_TRIANGLES, 3 * m_faces.size(), GL_UNSIGNED_INT, (void*)0);

    m_functions->glEnableVertexAttribArray(0);
    m_functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    m_functions->glDisableVertexAttribArray(0);

//    if (vertexLoc >= 0) m_functions->glDisableVertexAttribArray(vertexLoc);
//    if (colorLoc >= 0)  m_functions->glDisableVertexAttribArray(colorLoc);
}

void Mesh::render(QGLShaderProgram& shader) const
{
    m_functions->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);

    int vertexLoc = shader.attributeLocation("vtx_position");
    int colorLoc  = shader.attributeLocation("vtx_color");

    shader.setUniformValue("mat_obj", m_transform);

    if (vertexLoc >= 0)
    {
        m_functions->glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        m_functions->glEnableVertexAttribArray(vertexLoc);
    }

    if (colorLoc >= 0)
    {
        m_functions->glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3f));
        m_functions->glEnableVertexAttribArray(colorLoc);
    }

    glDrawElements(GL_TRIANGLES, 3 * m_faces.size(), GL_UNSIGNED_INT, (void*)0);

    if (vertexLoc >= 0) m_functions->glDisableVertexAttribArray(vertexLoc);
    if (colorLoc >= 0)  m_functions->glDisableVertexAttribArray(colorLoc);
}

void Mesh::rawData(const QVector<Vertex>& vertices, const QVector<FaceIndex>& faces)
{
    m_vertices = vertices;
    m_faces = faces;
}

