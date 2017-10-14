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

void Mesh::init(const bool& hasNormals)
{
    // If the mesh already has its normals, don't compute them.
    if(!hasNormals) {
        computeNormals();
    } else {
        for(int i = 0 ; i < m_vertices.size() ; i++) {
            std::cout << m_vertices.at(i).normal.x << ";" << m_vertices.at(i).normal.y << ";" << m_vertices.at(i).normal.z << std::endl;
        }
    }

    if (!m_functions->glIsBuffer(m_vertexBufferId)) {
        m_functions->glGenBuffers(1, &m_vertexBufferId);
    }
    m_functions->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    m_functions->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

    if (!m_functions->glIsBuffer(m_indexBufferId)) {
        m_functions->glGenBuffers(1, &m_indexBufferId);
    }
    m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
    m_functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(FaceIndex) * m_faces.size(), &m_faces[0], GL_STATIC_DRAW);

    computeBoundingBox();
    m_wireBoundingBox->setFunctions(*m_functions);
    m_wireBoundingBox->setAlignedBox(*m_boundingBox);
    m_wireBoundingBox->init();

    Point3f center = m_boundingBox->center();
    m_transform.translate(-center.x, -center.y, -center.z);

    // buildOctree();
}

void Mesh::renderMesh() const
{
    if (m_scenePtr != Q_NULLPTR
            && m_scenePtr->simpleShadingProgram()->bind())
    {
        m_functions->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
        m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);

        QGLShaderProgram* program = m_scenePtr->simpleShadingProgram();
        program->setUniformValue("mat_obj", m_transform);
        program->setUniformValue("mat_view", m_scenePtr->camera()->viewMatrix());
        program->setUniformValue("mat_proj", m_scenePtr->camera()->projectionMatrix());

        const int vertexLoc = program->attributeLocation("vtx_position");
        const int colorLoc  = program->attributeLocation("vtx_color");
        const int normalLoc = program->attributeLocation("vtx_normal");

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
}

void Mesh::renderBoundingBox() const
{
    m_wireBoundingBox->render(*m_scenePtr, m_transform);
}

void Mesh::rawData(const QVector<Vertex>& allVertices, const QVector<EdgeIndex>& allEdges, const QVector<FaceIndex>& allFaces)
{
    m_allVertices = allVertices;
    m_allEdges = allEdges;
    m_allFaces = allFaces;
}

void Mesh::displayableData(const QVector<Vertex>& vertices, const QVector<FaceIndex>& faces)
{
    m_vertices = vertices;
    m_faces = faces;
}

void Mesh::computeNormals()
{
    // Set all normals to 0
    for (int i = 0; i < m_vertices.size(); ++i)
        m_vertices.value(i).normal = Vector3f(0.f, 0.f, 0.f);

    // Compute normals
    for (int i = 0; i < m_faces.size(); i+=2)
    {
        const FaceIndex face = m_faces.at(i);
        const FaceIndex face2 = m_faces.at(i+1);

        const Point3f v0 = m_vertices.at(face.v0).position;
        const Point3f v1 = m_vertices.at(face.v1).position;
        const Point3f v2 = m_vertices.at(face.v2).position;

        const Vector3f normal = (v1 - v0).cross(v2 - v0);

        m_vertices[face.v0].normal += normal;
        m_vertices[face.v1].normal += normal;
        m_vertices[face.v2].normal += normal;

        // The reversed face should have the same normal.
        m_vertices[face2.v0].normal += normal;
        m_vertices[face2.v1].normal += normal;
        m_vertices[face2.v2].normal += normal;
    }

    // Normalize all normals
    for (int i = 0; i < m_vertices.size(); ++i) {
        m_vertices[i].normal.normalise();
    }
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

void Mesh::saveAsObj(const QString& fileName) const
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(0, "Error while writing file", file.errorString());
		return;
	}

	QTextStream out(&file);

	// Write Vertex
	for (int i = 0; i < m_vertices.size(); ++i)
	{
		QString line = "v ";
		line += QString::number(m_vertices.at(i).position.x) + " "
		+ QString::number(m_vertices.at(i).position.y) + " "
		+ QString::number(m_vertices.at(i).position.z)
		+ '\n';
		out << line;
	}


    // Write Normal
	for (int i = 0; i < m_vertices.size(); ++i)
	{
		QString line = "vn ";
		line += QString::number(m_vertices.at(i).normal.x) + " "
		+ QString::number(m_vertices.at(i).normal.y) + " "
		+ QString::number(m_vertices.at(i).normal.z)
		+ '\n';
		out << line;
	}


    // Write Faces
	for (int i = 0; i < m_faces.size(); i++)
	{
        	unsigned int v0, v1, v2, vn0, vn1, vn2;
		v0 = m_faces.at(i).v0+1;
		v1 = m_faces.at(i).v1+1;
		v2 = m_faces.at(i).v2+1;

        	// TODO: Debug the normal writing in the obj
		QString line ="f ";
		// Vertice and Normal of this vertice are in the same order so we can use vertice index as normal index.
        	/*line += QString::number(v0) + "//" + QString::number(v0) + " "
        	+ QString::number(v1) + "//" + QString::number(v1) + " "
        	+ QString::number(v2) + "//" + QString::number(v2) + " "
        	+ '\n';*/
        	line += QString::number(v0) + " "
                	+ QString::number(v1) + " "
                	+ QString::number(v2)
                	+ '\n';
		out << line;
	}

	file.close();
}
