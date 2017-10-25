#include "obj.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <tiny_obj_loader.h>

#include "mesh.h"
#include "polyhedron.h"


obj::obj(const QString &fileName)
{
    loadFromFile(fileName);
}

void obj::loadFromFile(const QString &fileName) {
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    bool success = tinyobj::LoadObj(shapes, materials, err, fileName.toStdString().c_str());

    if (!success) {
        QMessageBox::critical(0, "Error", QString::fromStdString(err));
    }

    bool needNormals = false;

    // copy vertices
    int currentShapeIndex = 0;
    for (size_t i = 0; i < shapes.size(); i++) {
        m_vertices.resize(currentShapeIndex + shapes[i].mesh.positions.size()/3);
        for (size_t v = 0; v < shapes[i].mesh.positions.size()/3; v++) {
            m_vertices[v+currentShapeIndex] = Vertex(Point3f(shapes[i].mesh.positions[3*v],
                                                             shapes[i].mesh.positions[3*v+1],
                                                             shapes[i].mesh.positions[3*v+2]), Color4f(0.5, 0.5, 0.5));

            if(!shapes[i].mesh.normals.empty())
                m_vertices[v+currentShapeIndex].normal = Vector3f(shapes[i].mesh.normals[3*v],
                                                                 shapes[i].mesh.normals[3*v+1],
                                                                 shapes[i].mesh.normals[3*v+2]);
            else
                needNormals = true;

            if(!shapes[i].mesh.texcoords.empty())
                m_vertices[v+currentShapeIndex].texcoord = Vector2f(shapes[i].mesh.texcoords[2*v],
                                                                 shapes[i].mesh.texcoords[2*v+1]);

        }
        currentShapeIndex += shapes[i].mesh.positions.size()/3;
    }

    // copy faces
    currentShapeIndex = 0;
    int vertexIndexOffset = 0;
    for (size_t i = 0; i < shapes.size(); i++) {
        m_faces.resize(currentShapeIndex + shapes[i].mesh.indices.size()/3);
        for (size_t f = 0; f < shapes[i].mesh.indices.size()/3; f++) {
            m_faces[f+currentShapeIndex] = FaceIndex(shapes[i].mesh.indices[3*f]+vertexIndexOffset,
                                                   shapes[i].mesh.indices[3*f+1]+vertexIndexOffset,
                                                   shapes[i].mesh.indices[3*f+2]+vertexIndexOffset);
        }
        currentShapeIndex += shapes[i].mesh.indices.size()/3;
        vertexIndexOffset += shapes[i].mesh.positions.size()/3;
    }

    /* Building the Polyhedron from vertices and faces */
    Polyhedron P;
    Polyhedron_builder<HalfedgeDS> builder(m_vertices, m_faces);
    P.delegate( builder );

    /* Predicat testing to check if the Polyhedron is build properly */
    if(P.is_valid()) {
        QMessageBox::critical(0, "Success", "The mesh is valid !");
    } else {
        QMessageBox::critical(0, "Error", "The mesh isn't' valid !");
    }

    if(P.is_closed()) {
        QMessageBox::critical(0, "Closed", "The mesh is closed !");
    } else {
        QMessageBox::critical(0, "Open", "The mesh is open, need to process Hole Filling !");
    }
}

Mesh *obj::mesh() const
{
    Mesh* mesh = new Mesh;
    mesh->displayableData(m_vertices, m_faces);
    mesh->init();

    return mesh;
}
