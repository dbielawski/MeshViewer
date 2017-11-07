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

    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0, "Error", file.errorString());
        return;
    }

    QTextStream in(&file);
    QString line;

    m_vertices.clear();
    m_faces.clear();

    QVector<Vector3f> normals;
    QVector<QVector<unsigned int>> normalsIndices;

    do
    {
        line = in.readLine();

        if (!line.isNull() && !line.isEmpty())
        {
            QStringList list = line.split(' ');

            if (list.at(0) == "v")  // Vertices
            {
                list.removeAt(0); // Remove the 'v' character

                // Test if we have empty characters
                // We remove them
                QString string = list.at(0);
                while (string.isEmpty())
                {
                    list.removeAt(0);
                    string = list.at(0);
                }

                QString x = list.at(0);
                QString y = list.at(1);
                QString z = list.at(2);

                Vertex v(Point3f(x.toFloat(), y.toFloat(), z.toFloat()),
                         Color4f::gray());

                m_vertices.append(v);
            }

            else if (list.at(0) == "vn") // Normals
            {
                list.removeAt(0); // Remove 'vn' characters

                QString n0 = list.at(0);
                QString n1 = list.at(1);
                QString n2 = list.at(2);
                Vector3f normal(n0.toFloat(), n1.toFloat(), n2.toFloat());
                normals.append(normal);
            }

            else if (list.at(0) == "f") // Face indices
            {
                list.removeAt(0); // Remove the 'f' character

                uint listSize = list.size();

                QString firstElement = list.at(0);
                QStringList firstElementArray = firstElement.split('/');

                // We want to know if he has this format => (f 5/1/1 1/2/1 4/3/1)
                bool hasVertTexNor = (firstElementArray.size() == 2 || firstElementArray.size() == 3);

                // Format f 1 2 3 ...
                if (!hasVertTexNor)
                {
                    FaceIndex face;

                    for (int i = 0; i < listSize; ++i)
                    {
                        unsigned int faceIndex = list.at(i).toInt() - 1;
                        face.append(faceIndex);
                    }

                    m_faces.append(face);
                }
                // It's this format => (f 5/1 ou 1/2/1 ...)
                else if (hasVertTexNor)
                {
                    FaceIndex face;
                    QVector<unsigned int> normalIndices;

                    for (int i = 0; i < listSize; ++i)
                    {
                        QString firstElement = list.at(i);
                        QStringList elementArray = firstElement.split('/');

                        QString vertexIndex, textCoord, normalIndex;
                        Q_UNUSED(textCoord);

                        // 5/1 5/2 5/3
                        if (elementArray.size() == 2)
                        {
                            vertexIndex = elementArray.at(0);
                            normalIndex = elementArray.at(1);
                        }
                        // 5/1/1 5/2/1 5/3/1
                        else if (elementArray.size() == 3)
                        {
                            vertexIndex = elementArray.at(0);
                            //textCoord = elementArray.at(1);       // We don't use textCoords for now
                            normalIndex = elementArray.at(2);
                        }

                        int vertexIndexValue = vertexIndex.toInt();
                        // Check for invalid charatcter
                        if (vertexIndexValue > 0)
                            face.append(vertexIndexValue - 1);

                        int normalIndexValue = normalIndex.toInt();
                        // Check for invalid charatcter
                        if (normalIndexValue > 0)
                            normalIndices.append(normalIndexValue - 1);
                    }

                    m_faces.append(face);

                    // We actually don't need to retrieve the correct normal per
                    // vertex, we recompute all normals later.
                    normalsIndices.append(normalIndices);
                }
                else
                {
                    QMessageBox::critical(0, "Error", "OBJ file format not supported");
                    return;
                }
            }
        }

    } while (!line.isNull());

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
