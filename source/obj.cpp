#include "obj.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "mesh.h"


obj::obj(const QString &fileName)
{
    loadFromFile(fileName);
}

void obj::loadFromFile(const QString &fileName)
{
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
    QVector<Vector3i> normalIndices;

    do
    {
        line = in.readLine();

        if (!line.isNull() && !line.isEmpty())
        {
            QStringList list = line.split(' ');

            if (list.at(0) == 'v')  // Vertices
            {
                list.removeAt(0); // Remove the 'v' character

                QString x = list.at(0);
                QString y = list.at(1);
                QString z = list.at(2);

                // WARNING: magic number (color)
                Vertex v(Point3f(x.toFloat(), y.toFloat(), z.toFloat()),
                         Color4f(.5f, .5f, .5f));

                m_vertices.append(v);
            }
            else if (list.at(0) == 'f') // Face indices
            {
                list.removeAt(0); // Remove the 'f' character

                uint listSize = list.size();
                QString firstElement = list.at(0);
                QStringList firstElementArray = firstElement.split('/');

                //We want to know if he has this format => (f 5/1/1 1/2/1 4/3/1)
                bool hasVertTexNor = (firstElementArray.size() == 3);

                // It's this format => (f 1 2 3 4)
                if (listSize == 3 && !hasVertTexNor)
                {
                    QString v0 = list.at(0);
                    QString v1 = list.at(1);
                    QString v2 = list.at(2);

                    m_faces.append(FaceIndex(v0.toInt() - 1, v1.toInt() - 1, v2.toInt() - 1));
                }
                // It's this format => (f 2 4 1)
                else if (listSize == 4 && !hasVertTexNor)
                {
                    QString v0 = list.at(0);
                    QString v1 = list.at(1);
                    QString v2 = list.at(2);
                    QString v3 = list.at(3);

                    FaceIndex i1(v0.toInt() - 1, v1.toInt() - 1, v2.toInt() - 1);
                    FaceIndex i2(v2.toInt() - 1, v3.toInt() - 1, v0.toInt() - 1);

                    m_faces.append(i1);
                    m_faces.append(i2);
                }
                // It's this format => (f 5/1/1 1/2/1 4/3/1)
                else if (hasVertTexNor)
                {
                    int listSize = list.size();
                    int face[listSize];
                    int faceNormals[listSize];

                    for (int i = 0; i < listSize; ++i)
                    {
                        QString firstElement = list.at(i);
                        QStringList elementArray = firstElement.split('/');

                        QString vertexIndex = elementArray.at(0);
                        //QString textCoord = elementArray.at(1);       // We don't use textCoords
                        QString normalIndex = elementArray.at(2);

                        face[i] = vertexIndex.toInt();
                        faceNormals[i] = normalIndex.toInt();
                    }
                    m_faces.append(FaceIndex(face[0] - 1, face[1] - 1, face[2] - 1));
                    normalIndices.append(Vector3i(faceNormals[0] - 1, faceNormals[1] - 1, faceNormals[2] - 1));
                }
                else
                {
                    QMessageBox::critical(0, "Error", "OBJ file format not supported");
                    return;
                }
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
        }

    } while (!line.isNull());

    // set normals
    // vertex/textCoord/normals format
    if (!normalIndices.isEmpty())
    {
        for (int i = 0; i < m_faces.size(); ++i)
        {
            m_vertices.value(m_faces.at(i).v0).normal = normals.at(normalIndices.at(i).x);
            m_vertices.value(m_faces.at(i).v1).normal = normals.at(normalIndices.at(i).y);
            m_vertices.value(m_faces.at(i).v2).normal = normals.at(normalIndices.at(i).z);
        }
    }
    else
    {
        // classical format
        for (int i = 0; i < normals.size(); ++i)
            m_vertices.value(i).normal = normals.at(i);
    }
}

Mesh *obj::mesh() const
{
    Mesh* mesh = new Mesh;
    mesh->rawData(m_vertices, m_faces);
    mesh->init();

    return mesh;
}
