#include "obj.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <tiny_obj_loader.h>

#include <fstream>
#include "mesh.h"
#include "polyhedron.h"


#include "CGAL/Simple_cartesian.h"
#include "CGAL/Polyhedron_items_3.h"
#include "CGAL/HalfedgeDS_list.h"
#include "CGAL/Polyhedron_3.h"

#include "CGAL/Polyhedron_incremental_builder_3.h"
#include "CGAL/Modifier_base.h"
#include "CGAL/exceptions.h"


obj::obj(const QString &fileName)
{
    loadFromFile(fileName);
}

void obj::loadFromFile(const QString &fileName) {
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, "Error", file.errorString());
        return;
    }

    m_vertices.clear();
    m_faces.clear();

    QTextStream in(&file);
    QString line = in.readLine();

    QVector<Vector3f> normals;
    QVector<QVector<unsigned int>> normalsIndices;

    while (!line.isNull()) {
        if (line.isEmpty()) {
            line = in.readLine();
            continue;
        }

        QStringList list = line.split(' ');

        // delete empty characters
        uint listSize = list.size();
        if (listSize > 4) {
            for (int i = listSize - 1; i >= 0; i--) {
                QString string = list.at(i);
                if (string.isEmpty()) {
                    list.removeAt(i);
                }
            }
            listSize = list.size();
        }

        // get vertices, normals and faces
        if (list.at(0) == "v") {  // Vertices
            float x = list.at(1).toFloat();
            float y = list.at(2).toFloat();
            float z = list.at(3).toFloat();
            Vertex v(Point3f(x, y, z), Color4f::gray());
            m_vertices.append(v);
        } else if (list.at(0) == "vn") { // Normals
            float n0 = list.at(1).toFloat();
            float n1 = list.at(2).toFloat();
            float n2 = list.at(3).toFloat();
            Vector3f normal(n0, n1, n2);
            normals.append(normal);
        } else if (list.at(0) == "f") { // Faces
            FaceIndex face;
            QVector<uint> normalIndices;
            for (int i = 1; i < listSize; ++i) {
                QStringList elementArray = list.at(i).split('/');
                face.append(elementArray.at(0).toInt() - 1);

                uint textCoord, normalIndex;
                Q_UNUSED(textCoord);
                if (elementArray.size() == 2) {
                    normalIndex = elementArray.at(1).toInt() - 1;
                } else if (elementArray.size() == 3) {
                    //textCoord = elementArray.at(1).toInt() - 1;  // We don't use textCoords for now
                    normalIndex = elementArray.at(2).toInt() - 1;
                }
            }
            normalsIndices.append(normalIndices);
            m_faces.append(face);
        }

        line = in.readLine();
    }

    file.close();

    // Set normals previously loaded
    // vertex/textCoord/normals format
    //TODO: Damien fix this
//    if (!normalIndices.isEmpty())
//    {
//        for (int i = 0; i < m_faces.size(); ++i)
//        {
//            for (int j = 0; j < m_faces.at(i).size(); ++j)
//            {
//                m_vertices.value(m_faces.at(i).at(0)).normal = normals.at(normalsIndices.at(i).x);
//                m_vertices.value(m_faces.at(i).at(1)).normal = normals.at(normalsIndices.at(i).y);
//                m_vertices.value(m_faces.at(i).at(2)).normal = normals.at(normalsIndices.at(i).z);
//            }
//        }
//    }
//    else
//    {
//        // classical format
//        for (int i = 0; i < normals.size(); ++i)
//            m_vertices.value(i).normal = normals.at(i);
//    }
}

Mesh *obj::mesh() const
{
    Mesh* mesh = new Mesh;
    mesh->displayableData(m_vertices, m_faces);
    mesh->init();

    return mesh;
}
