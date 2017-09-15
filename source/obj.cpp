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

    //    m_vertices.clear();
    //    m_faces.clear();

    //    do
    //    {
    //        line = in.readLine();

    //        if (!line.isNull())
    //        {
    //            // Vertex
    //            if (line.at(0) == 'v')
    //            {
    //                QStringList list = line.split(' ');
    //                list.removeAt(0); // Remove the 'v' character

    //                QString x = list.at(0);
    //                QString y = list.at(1);
    //                QString z = list.at(2);

    //                Vertex v(Point3f(x.toFloat(), y.toFloat(), z.toFloat()), Color3f(.5f, .5f, .5f));
    //                m_vertices.append(v);
    //            }
    //            // Face
    //            else if (line.at(0) == 'f') // TODO: extends to 3/4 face indcides ...
    //            {
    //                QStringList list = line.split(' ');
    //                list.removeAt(0); // Remove the 'f' character

    //                QString v0 = list.at(0);
    //                QString v1 = list.at(1);
    //                QString v2 = list.at(2);
    //                QString v3 = list.at(3);

    //                FaceIndex i1(v0.toInt(), v1.toInt(), v2.toInt());
    //                FaceIndex i2(v2.toInt(), v3.toInt(), v0.toInt());

    //                m_faces.append(i1);
    //                m_faces.append(i2);
    //            }
    //        }

    //    } while (!line.isNull());

    m_vertices.clear();
    m_faces.clear();

    m_vertices.append(Vertex(Point3f(-10.f, 0.f, 0.f), Color3f(1.f, 0.f, 0.f)));
    m_vertices.append(Vertex(Point3f(10.f, 0.f, 0.f), Color3f(1.f, 0.f, 0.f)));
    m_vertices.append(Vertex(Point3f(0.f, 10.f, 0.f), Color3f(1.f, 0.f, 0.f)));
    m_faces.append(FaceIndex(1, 2, 3));
}

Mesh *obj::mesh() const
{
    Mesh* mesh = new Mesh;
    mesh->rawData(m_vertices, m_faces);
    mesh->init();

    return mesh;
}
