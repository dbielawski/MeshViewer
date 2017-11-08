#include "mesh.h"

#include <QGLFunctions>
#include <QGLShaderProgram>
#include <map>
#include <iostream>
#include <CGAL/Vector_3.h>
#include <CGAL/Origin.h>


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
    // TODO: clean
    // We need a temporary 1D array to give to the GC
    QVector<unsigned int> faces;
    for (int i = 0; i < m_faces.size(); ++i)
        for (int j = 0; j < m_faces.at(i).size(); ++j)
            faces.append(m_faces.at(i).at(j));

    //computeNormals();

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
    m_transform.translate(-center.x, -center.y, -center.z);

    buildOctree();

    /* Building the Polyhedron from vertices and faces */
    Polyhedron_builder<HalfedgeDS>builder(m_vertices, m_faces);
    m_polyhedron.delegate(builder);
    fillHoles();

    // TODO: Maybe we should move this call ?
    //toHalfedge();
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


    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glMultMatrixf(m_transform.constData());

    for (const FaceIndex& face: m_faces)
    {
        glBegin(GL_POLYGON);
        for (const uint index: face)
        {
            const Vertex &vertex = m_vertices.at(index);

            glColor4f(vertex.color.r, vertex.color.g, vertex.color.b, m_scenePtr->transparency());
            glVertex3f(vertex.position.x, vertex.position.y, vertex.position.z);
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
    //TODO: fixe this

    // Set all normals to 0
    //    for (int i = 0; i < m_vertices.size(); ++i)
    //        m_vertices.value(i).normal = Vector3f(0.f, 0.f, 0.f);

    //    qDebug()<< "computenormals1";
    //    // Compute normals
    //    for (int i = 0; i < m_faces.size(); i++)
    //    {
    //        const FaceIndex face = m_faces.at(i);
    //        const FaceIndex face2 = m_faces.at(i+1);

    //        const Point3f v0 = m_vertices.at(face.at(0)).position;
    //        const Point3f v1 = m_vertices.at(face.at(1)).position;
    //        const Point3f v2 = m_vertices.at(face.at(2)).position;

    //        const Vector3f normal = (v1 - v0).cross(v2 - v0);

    //        m_vertices[face.at(0)].normal += normal;
    //        m_vertices[face.at(1)].normal += normal;
    //        m_vertices[face.at(2)].normal += normal;

    //        // The reversed face should have the same normal.
    //        m_vertices[face2.at(0)].normal += normal;
    //        m_vertices[face2.at(1)].normal += normal;
    //        m_vertices[face2.at(2)].normal += normal;
    //    }
    //    qDebug()<< "computenormals2";
    //    // Normalize all normals
    //    for (int i = 0; i < m_vertices.size(); ++i) {
    //        m_vertices[i].normal.normalise();
    //    }
}

void Mesh::computeBoundingBox()
{
    m_boundingBox->reset();

    for (int i = 0; i < m_vertices.size(); ++i)
        m_boundingBox->extend(m_vertices.at(i).position);
}

void Mesh::buildOctree()
{
    m_octree = new cOctree(m_vertices);
    m_octree->setFunctions(*m_functions);
    m_octree->build();
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

    QString applicationName = "MeshViewer";
    QString header = "# This file was generated with " + applicationName + "\n";
    out << header;

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
        QString line ="f ";
        for (int j = 0; j < m_faces.at(i).size(); ++j)
        {
            line += QString::number(m_faces.at(i).at(j) + 1) + " ";
        }
        line += '\n';
        out << line;
    }

    file.close();
}

// TODO : Rename ?
void Mesh::toHalfedge() {
    //    m_halfEdge.clear();
    //    QVector<surface_mesh::Surface_mesh::Vertex> vertices;
    //    for(int i = 0 ; i < m_vertices.size(); i++) {
    //        const Vertex& v = m_vertices[i];
    //        surface_mesh::Surface_mesh::Vertex v_he = m_halfEdge.add_vertex(surface_mesh::Point(v.position.x, v.position.y, v.position.z));
    //        vertices.push_back(v_he);
    //    }

    //    for(int i = 0 ; i < m_faces.size(); i++) {
    //        surface_mesh::Surface_mesh::Vertex v0, v1, v2;
    //        const FaceIndex& f = m_faces.at(i);
    //        v0 = vertices.at(f.v0);
    //        v1 = vertices.at(f.v1);
    //        v2 = vertices.at(f.v2);
    //        m_halfEdge.add_triangle(v0, v1, v2);
    //    }
}


/* ====================================== */

typedef CGAL::Simple_cartesian<double>                  Kernel;
typedef CGAL::Polyhedron_3<Kernel>                      Polyhedron;
typedef Polyhedron::Halfedge_handle                     Halfedge_handle;
typedef Polyhedron::Halfedge_iterator                   Halfedge_iterator;
typedef Polyhedron::Vertex_iterator                     Vertex_iterator;
typedef Polyhedron::Face_iterator                       Face_iterator;
typedef Polyhedron::Halfedge_around_facet_circulator    Halfedge_facet_circulator;

// http://cgal-discuss.949826.n4.nabble.com/Using-Polyhedron-fill-hole-td4657972.html

void Mesh::fillHoles() {
    // Fill hole in the polyhedron
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
}
