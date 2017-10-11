#ifndef MESH_H
#define MESH_H

#include <QVector>
#include <QMatrix4x4>

#include "utils.h"
#include "camera.h"
#include "scene.h"
#include "wireboundingbox.h"

#include <surface_mesh/surface_mesh.h>
#include <QMessageBox>
#include <QFile>


class Octree;

class Mesh
{
public:
    Mesh();
    ~Mesh();

    void init();
    void renderMesh() const;
    void renderBoundingBox() const;
	void rawData(const QVector<Vertex>& vertices, const QVector<FaceIndex>& indices);
    void rawData(const QVector<Vertex>& vertices, const QVector<FaceIndex>& faces, const QVector<FaceIndex>& allFaces, const QVector<EdgeIndex>& edges);

    void computeNormals();
    void clear();
    void computeBoundingBox();
    void buildOctree();

    AlignedBox3f* boundingBox() const   { return m_boundingBox; }
    Octree* octree() const              { return m_octree;      }
    QMatrix4x4 transform() const        { return m_transform;   }

    void attachScene(Scene* scene)      { m_scenePtr = scene; }

	surface_mesh::Surface_mesh& halfEdgeMesh() { return m_halfEdge; }

    unsigned int verticesCount() const  { return m_vertices.size(); }
    unsigned int trianglesCount() const { return m_faces.size();    }
	unsigned int facesCount() const     { return m_faces.size();    }
    unsigned int edgesCount() const     { return m_edges.size();    }


	void saveAsObj(const QString& fileName) const
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

		// Write Face (as quads)
		for (int i = 0; i < m_faces.size(); i++)
		{
			unsigned int v0, v1, v2;
			v0 = m_faces.at(i).v0+1;
			v1 = m_faces.at(i).v1+1;
			v2 = m_faces.at(i).v2+1;

			QString line ="f ";
			line += QString::number(v0) + " "
			+ QString::number(v1) + " "
			+ QString::number(v2) + " "
			+ '\n';
			out << line;
		}

		file.close();
	}


private:
    QGLFunctions*       m_functions;

    uint                m_vertexBufferId;
    uint                m_indexBufferId;

    QVector<Vertex>     m_vertices;
    QVector<FaceIndex>  m_allFaces;
	QVector<FaceIndex> 	m_faces;
	QVector<EdgeIndex> 	m_edges;

	surface_mesh::Surface_mesh m_halfEdge;

    QMatrix4x4          m_transform;

    AlignedBox3f*       m_boundingBox;
    WireBoundingBox*    m_wireBoundingBox;

    Octree*             m_octree;

    const Scene*        m_scenePtr;
};

#endif // MESH_H
