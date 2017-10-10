#include "mesh.h"

#include <QGLFunctions>
#include <QGLShaderProgram>


Mesh::Mesh() :
    m_octree(Q_NULLPTR),
    m_scenePtr(Q_NULLPTR),
    m_boundingBox(new AlignedBox3f),
    m_wireBoundingBox(new WireBoundingBox),
    m_functions(new QGLFunctions)
{
    m_functions->initializeGLFunctions();

    m_transform.setToIdentity();
}

Mesh::~Mesh()
{
    m_functions->glDeleteBuffers(1, &m_vertexBufferId);
    m_functions->glDeleteBuffers(1, &m_indexBufferId);

    delete m_functions;
    delete m_boundingBox;
    delete m_wireBoundingBox;
    delete m_octree;
}

void Mesh::init()
{
    //computeNormals();

    if (!m_functions->glIsBuffer(m_vertexBufferId))
        m_functions->glGenBuffers(1, &m_vertexBufferId);
    m_functions->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    m_functions->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

    if (!m_functions->glIsBuffer(m_indexBufferId))
        m_functions->glGenBuffers(1, &m_indexBufferId);
    m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
    m_functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(FaceIndex) * m_faces.size(), &m_faces[0], GL_STATIC_DRAW);

    computeBoundingBox();
    m_wireBoundingBox->setFunctions(*m_functions);
    m_wireBoundingBox->setAlignedBox(*m_boundingBox);
    m_wireBoundingBox->init();

//    Point3f center = m_boundingBox->center();
//    m_transform.translate(center.x, center.y, center.z);

    // buildOctree();
}

void Mesh::renderMesh() const
{
    if (m_scenePtr != Q_NULLPTR
            && m_scenePtr->simpleShadingProgram()->bind())
    {
        m_functions->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
        m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);

        m_scenePtr->simpleShadingProgram()->setUniformValue("mat_obj", m_transform);
        m_scenePtr->simpleShadingProgram()->setUniformValue("mat_view", m_scenePtr->camera()->viewMatrix());
        m_scenePtr->simpleShadingProgram()->setUniformValue("mat_proj", m_scenePtr->camera()->projectionMatrix());

        const int vertexLoc = m_scenePtr->simpleShadingProgram()->attributeLocation("vtx_position");
        const int colorLoc  = m_scenePtr->simpleShadingProgram()->attributeLocation("vtx_color");
        const int normalLoc = m_scenePtr->simpleShadingProgram()->attributeLocation("vtx_normal");

        if (vertexLoc >= 0)
        {
            m_functions->glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            m_functions->glEnableVertexAttribArray(vertexLoc);
        }

        if (colorLoc >= 0)
        {
            m_functions->glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
            m_functions->glEnableVertexAttribArray(colorLoc);
        }

        if (normalLoc >= 0)
        {
            m_functions->glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            m_functions->glEnableVertexAttribArray(normalLoc);
        }

        glDrawElements(GL_TRIANGLES, 3 * m_faces.size(), GL_UNSIGNED_INT, (void*)0);

        if (vertexLoc >= 0) m_functions->glDisableVertexAttribArray(vertexLoc);
        if (colorLoc >= 0)  m_functions->glDisableVertexAttribArray(colorLoc);
        if (normalLoc >= 0) m_functions->glDisableVertexAttribArray(normalLoc);
    }
    else
        Q_ASSERT(false);
}

void Mesh::renderBoundingBox() const
{
    m_wireBoundingBox->render(*m_scenePtr);
}

void Mesh::rawData(const QVector<Vertex>& vertices, const QVector<FaceIndex>& faces)
{
    m_vertices = vertices;
    m_faces = faces;
}

void Mesh::rawData(const QVector<Vertex>& vertices,
			const QVector<FaceIndex>& faces,
			const QVector<FaceIndex>& allFaces,
			const QVector<EdgeIndex>& edges)
{
	rawData(vertices, faces);
	m_allFaces = allFaces;
	m_edges = edges;
}

void Mesh::computeNormals()
{
    // Set all normals to 0
    for (int i = 0; i < m_vertices.size(); ++i)
        m_vertices.value(i).normal = Vector3f(0.f, 0.f, 0.f);

    // Compute normals
    for (int i = 0; i < m_vertices.size(); ++i)
    {
        const FaceIndex face = m_faces.at(i);

        const Point3f v0 = m_vertices.at(face.v0).position;
        const Point3f v1 = m_vertices.at(face.v1).position;
        const Point3f v2 = m_vertices.at(face.v2).position;

        const Vector3f normal = (v1 - v0).cross(v2 - v0);

        m_vertices[face.v0].normal += normal;
        m_vertices[face.v1].normal += normal;
        m_vertices[face.v2].normal += normal;
    }

    // Normalize all normals
    for (int i = 0; i < m_vertices.size(); ++i)
        m_vertices[i].normal.normalise();
}

void Mesh::computeBoundingBox()
{
    m_boundingBox->reset();

    for (int i = 0; i < m_vertices.size(); ++i)
        m_boundingBox->extend(m_vertices.at(i).position);
}

void Mesh::buildOctree()
{
    // TODO: implementer
}
