#include "mesh.h"

#include <QGLFunctions>
#include <QGLShaderProgram>
#include <map>
#include <iostream>
#include <CGAL/Vector_3.h>
#include <CGAL/Origin.h>
#include <limits>

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
    // We need a temporary 1D array to give to the GC
    QVector<unsigned int> faces;
    for (int i = 0; i < m_faces.size(); ++i)
        for (int j = 0; j < m_faces.at(i).size(); ++j)
            faces.append(m_faces.at(i).at(j));

    computeNormals();

    // Building the Polyhedron from vertices and faces
    Polyhedron_builder<HalfedgeDS>builder(m_vertices, m_faces);
    m_polyhedron.delegate(builder);

    if (!m_functions->glIsBuffer(m_vertexBufferId)) {
        m_functions->glGenBuffers(1, &m_vertexBufferId);
    }
    m_functions->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    m_functions->glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

    if (!m_functions->glIsBuffer(m_indexBufferId)) {
        m_functions->glGenBuffers(1, &m_indexBufferId);
    }
    m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
    m_functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * faces.size(), &faces[0], GL_STATIC_DRAW);

    computeBoundingBox();
    m_wireBoundingBox->setFunctions(*m_functions);
    m_wireBoundingBox->setAlignedBox(*m_boundingBox);
    m_wireBoundingBox->init();

    Point3f center = m_boundingBox->center();
    m_transform.translate(-center.x(), -center.y(), -center.z());

    buildOctree();
}

void Mesh::renderMesh() const
{
    //    if (m_scenePtr != Q_NULLPTR
    //            && m_scenePtr->simpleShadingProgram()->bind())
    //    {
    //        m_functions->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    //        m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);

    //        QGLShaderProgram* program = m_scenePtr->simpleShadingProgram();
    //        program->setUniformValue("mat_obj", m_transform);
    //        program->setUniformValue("mat_view", m_scenePtr->camera()->viewMatrix());
    //        program->setUniformValue("mat_proj", m_scenePtr->camera()->projectionMatrix());

    //        const int vertexLoc = program->attributeLocation("vtx_position");
    //        const int colorLoc  = program->attributeLocation("vtx_color");
    //        const int normalLoc = program->attributeLocation("vtx_normal");

    //        if (vertexLoc >= 0)
    //        {
    //            m_functions->glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    //            m_functions->glEnableVertexAttribArray(vertexLoc);
    //        }

    //        if (colorLoc >= 0)
    //        {
    //            m_functions->glVertexAttribPointer(colorLoc, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    //            m_functions->glEnableVertexAttribArray(colorLoc);
    //        }

    //        if (normalLoc >= 0)
    //        {
    //            m_functions->glVertexAttribPointer(normalLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    //            m_functions->glEnableVertexAttribArray(normalLoc);
    //        }


    //        QVector<unsigned int> faces;
    //        for (int i = 0; i < m_faces.size(); ++i)
    //            for (int j = 0; j < m_faces.at(i).size(); ++j)
    //                faces.append(m_faces.at(i).at(j));

    //        glDrawElements(GL_POLYGON, m_faces.size(), GL_UNSIGNED_INT, (void*)0);

    //        if (vertexLoc >= 0) m_functions->glDisableVertexAttribArray(vertexLoc);
    //        if (colorLoc >= 0)  m_functions->glDisableVertexAttribArray(colorLoc);
    //        if (normalLoc >= 0) m_functions->glDisableVertexAttribArray(normalLoc);

    //        m_scenePtr->simpleShadingProgram()->release();
    //    }


    glShadeModel(GL_SMOOTH);
//    glShadeModel(GL_FLAT);

    glMultMatrixf(m_transform.constData());

    for (const FaceIndex& face: m_faces) {
        glBegin(GL_POLYGON);
        for (const uint index: face) {
            const Vertex &vertex = m_vertices.at(index);

            glColor4f(vertex.color.r, vertex.color.g, vertex.color.b, m_scenePtr->transparency());
            glVertex3f(vertex.position.x(), vertex.position.y(), vertex.position.z());
        }
        glEnd();
    }
}

void Mesh::renderBoundingBox() const
{
    if (m_wireBoundingBox != Q_NULLPTR)
        m_wireBoundingBox->render(*m_scenePtr, m_transform);
}


void Mesh::renderOctree() const
{
    if (m_octree != Q_NULLPTR)
        m_octree->render(*m_scenePtr, m_transform);
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
    for (int i = 0; i < m_faces.size(); i++)
    {
        const FaceIndex face = m_faces.at(i);

        const Point3f v0 = m_vertices.at(face.at(0)).position;
        const Point3f v1 = m_vertices.at(face.at(1)).position;
        const Point3f v2 = m_vertices.at(face.at(2)).position;

        const Vector3f normal = (v1 - v0).cross(v2 - v0);

        m_vertices[face.at(0)].normal += normal;
        m_vertices[face.at(1)].normal += normal;
        m_vertices[face.at(2)].normal += normal;
    }

    // Normalize all normals
    for (int i = 0; i < m_vertices.size(); ++i) {
        Vector3f n = m_vertices[i].normal;
        float norm = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);

        if (norm != 0)
            m_vertices[i].normal /= norm;
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
    m_octree = new Octree(m_vertices, std::max(1, m_vertices.size() / 1000));
    m_octree->setFunctions(*m_functions);
    m_octree->build();
}

// Offset is the number of vertices already written in the out stream.
void Mesh::saveAsObj(QTextStream& out, int offset) const
{
    // Write Vertex
    for (int i = 0; i < m_vertices.size(); ++i) {
        QString line = "v ";
        line += QString::number(m_vertices.at(i).position.x()) + " "
                + QString::number(m_vertices.at(i).position.y()) + " "
                + QString::number(m_vertices.at(i).position.z())
                + '\n';
        out << line;
    }

    // Write Normal
    for (int i = 0; i < m_vertices.size(); ++i) {
        QString line = "vn ";
        line += QString::number(offset + m_vertices.at(i).normal.x()) + " "
                + QString::number(offset + m_vertices.at(i).normal.y()) + " "
                + QString::number(offset + m_vertices.at(i).normal.z())
                + '\n';
        out << line;
    }

    // Write Faces
    for (int i = 0; i < m_faces.size(); i++) {
        QString line ="f ";
        for (int j = 0; j < m_faces.at(i).size(); ++j) {
            line += QString::number(offset + m_faces.at(i).at(j) + 1) + " ";
        }
        line += '\n';
        out << line;
    }
}

typedef Polyhedron::Halfedge_handle                     Halfedge_handle;
typedef Polyhedron::Halfedge_iterator                   Halfedge_iterator;
typedef Polyhedron::Vertex_iterator                     Vertex_iterator;
typedef Polyhedron::Edge_iterator                       Edge_iterator;
typedef Polyhedron::Face_iterator                       Face_iterator;
typedef Polyhedron::Halfedge_around_facet_circulator    Halfedge_facet_circulator;

void Mesh::fillHoles()
{
    for (Halfedge_iterator heit = m_polyhedron.halfedges_begin(); heit != m_polyhedron.halfedges_end(); heit++) {
        if (heit->is_border()) { // If there is a hole
            Kernel::Vector_3 vec(0.0, 0.0, 0.0);
            size_t order = 0;
            Halfedge_iterator heit2 = heit;
            do {
                vec = vec + (heit2->vertex()->point() - CGAL::ORIGIN);
                order++;
                heit2 = heit2->next();
            } while (heit2 != heit);
            CGAL_assertion(order >= 3);
            Kernel::Point_3 center = CGAL::ORIGIN + (vec / static_cast<double>(order));

            // Fill hole
            m_polyhedron.fill_hole(heit);
            Halfedge_handle new_center = m_polyhedron.create_center_vertex(heit);
            new_center->vertex()->point() = center;
        }
    }

    m_polyhedron.normalize_border(); // Normalize all the borders just to be sure

    // Update m_vertices & m_faces with the new data from the filled polyhedron
    m_vertices.clear();
    unsigned int nb_vertices = 0;
    std::map<const Polyhedron::Vertex *, unsigned int> mapping;
    for(Vertex_iterator vit = m_polyhedron.vertices_begin(); vit != m_polyhedron.vertices_end(); vit++) {
        m_vertices.push_back(Vertex(Point3f(vit->point().x(), vit->point().y(), vit->point().z()), Color4f(0.5, 0.5, 0.5)));
        mapping[&*vit] = nb_vertices++;
    }

    m_faces.clear();
    for(Face_iterator fit = m_polyhedron.facets_begin(); fit != m_polyhedron.facets_end(); fit++) {
        Halfedge_facet_circulator hfc = fit->facet_begin();

        QVector<unsigned int> face;
        do {
            face.append(mapping[&*(hfc->vertex())]);
            hfc++;
        } while (hfc != fit->facet_begin());

        m_faces.append(face);
    }

    if(m_octree != NULL) {
        delete m_octree;
    }

    buildOctree();
}

void Mesh::detectHoles()
{
    for (Halfedge_iterator heit = m_polyhedron.halfedges_begin(); heit != m_polyhedron.halfedges_end(); heit++)
    {
        if (heit->is_border()) // If there is a hole
        {
            Kernel::Point_3 vec(0.0, 0.0, 0.0);
            vec = heit->vertex()->point();
            Point3f p0(vec.x(), vec.y(), vec.z());
            vec = heit->opposite()->vertex()->point();
            Point3f p1(vec.x(), vec.y(), vec.z());

            for (int i = 0; i < m_vertices.size(); ++i)
            {
                Point3f p  = m_vertices.at(i).position;

                if ((p.x() == p0.x() && p.y() == p0.y() && p.z() == p0.z()) ||
                        (p.x() == p1.x() && p.y() == p1.y() && p.z() == p1.z()))
                {
                    m_vertices[i].color = Color4f(Color4f::red());
                }
            }
        }
    }

    //init(); // Send new colors to
}
