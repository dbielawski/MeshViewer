#include "obj.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "mesh.h"

#include <iostream>


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

    do
    {
        line = in.readLine();

        if (!line.isNull() && !line.isEmpty())
        {
            if (line.at(0) == 'v')  // Vertices
            {
                QStringList list = line.split(' ');
                list.removeAt(0); // Remove the 'v' character

                QString x = list.at(0);
                QString y = list.at(1);
                QString z = list.at(2);

                // WARNING: magic number (color)
                Vertex v(Point3f(x.toFloat(), y.toFloat(), z.toFloat()),
                         Color3f(.5f, .5f, .5f));

                m_vertices.append(v);
            }
            else if (line.at(0) == 'f') // Face indices
            {
                QStringList list = line.split(' ');
                list.removeAt(0); // Remove the 'f' character

                uint listSize = list.size();
                if (listSize == 3)
                {
                    QString v0 = list.at(0);
                    QString v1 = list.at(1);
                    QString v2 = list.at(2);

                    m_faces.append(FaceIndex(v0.toInt() - 1, v1.toInt() - 1, v2.toInt() - 1));

//                    std::cout << "List 3" << std::endl;
                }
                else if (listSize == 4)
                {
                    QString v0 = list.at(0);
                    QString v1 = list.at(1);
                    QString v2 = list.at(2);
                    QString v3 = list.at(3);

                    FaceIndex i1(v0.toInt() - 1, v1.toInt() - 1, v2.toInt() - 1);
                    FaceIndex i2(v2.toInt() - 1, v3.toInt() - 1, v0.toInt() - 1);

                    m_faces.append(i1);
                    m_faces.append(i2);

//                    std::cout << "List 4" << std::endl;
                }
                else if (listSize == 18)
                {
                    // with normals
                    // maybe later...
                    QMessageBox::critical(0, "Error", "OBJ file format not supported");
                    return;
                }
                else
                {
                    QMessageBox::critical(0, "Error", "OBJ file format not supported");
                    return;
                }
            }
        }

    } while (!line.isNull());

}

Mesh *obj::mesh() const
{
    Mesh* mesh = new Mesh;
    mesh->rawData(m_vertices, m_faces);
    mesh->init();

    // TODO: update stuffs ...
    //    mesh->computeBoundingBox();
    //    mesh->buildOctree();

    return mesh;
}
