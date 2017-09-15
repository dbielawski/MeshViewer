#include "pgm3d.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "mesh.h"

pgm3d::pgm3d(): m_width(0), m_height(0), m_depth(0), m_data(nullptr)
{
}

pgm3d::pgm3d(const QString& s)
{
    pgm3d();

    if (s != "")
        loadFromFile(s);
}

pgm3d::~pgm3d()
{
    delete m_data;
}

void pgm3d::loadFromFile(const QString& fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(0, "Error", file.errorString());
        return;
    }

    QTextStream in(&file);

    QString magicNumber;
    in >> magicNumber;

    if (magicNumber != "PGM3D")
    {
        QMessageBox::critical(0, "Error", "not a PGM3D file");
        return;
    }

    in >> m_width;
    in >> m_height;
    in >> m_depth;
    in >> m_maxGrayscaleValue;

    unsigned long long voxelCount = m_width * m_height * m_depth;

    if (voxelCount > 0)
    {
        m_data = new unsigned int[voxelCount];

        for (int i = 0; i < voxelCount; ++i)
            in >> m_data[i];
    }
}

Point3i indexTo3Dcoordinates(unsigned int index, unsigned int width, unsigned int height)
{
    unsigned int x = index % width;
    unsigned int y = index / width;
    unsigned int z = index / (width * height);

    return Point3i(x, y, z);
}

Mesh* pgm3d::mesh() const
{
    unsigned long long voxelCount = m_width * m_height * m_depth;

    //          z
    //         /
    //        /___x
    //        |
    //        |
    //        y

    Mesh* mesh = new Mesh;

    QVector<Vertex> vertices;
    QVector<FaceIndex> faces;
    // TODO: reserve
    // vertices.reserve();
    // faces.reserve();

    for (unsigned long long i = 0; i < voxelCount; ++i)
    {
        if (m_data[i] != 0) // if the voxel is not black
        {
            // Compute the centered position of the Voxel
            Point3i offset(indexTo3Dcoordinates(i, m_width, m_height));

            float grayValue = m_data[i] / float(m_maxGrayscaleValue);
            Color3f color(grayValue, grayValue, grayValue);

            Vertex frontBottomLeft(offset + Point3f(-.5f, .5f, -.5f), color);
            Vertex frontBottomRight(offset + Point3f(.5f, .5f, -.5f), color);
            Vertex frontTopRight(offset + Point3f(.5f, -.5f, -.5f), color);
            Vertex frontTopLeft(offset + Point3f(-.5f, -.5f, -.5f), color);

            Vertex backTopLeft(offset + Point3f(-.5f, -.5f, .5f), color);
            Vertex backTopRight(offset + Point3f(.5f, -.5f, .5f), color);
            Vertex backBottomLeft(offset + Point3f(-.5f, .5f, .5f), color);
            Vertex backBottomRight(offset + Point3f(.5f, .5f, .5f), color);

            vertices.append(frontBottomLeft);
            vertices.append(frontBottomRight);
            vertices.append(frontTopRight);
            vertices.append(frontTopLeft);

            vertices.append(backTopLeft);
            vertices.append(backTopRight);
            vertices.append(backBottomLeft);
            vertices.append(backBottomRight);

            faces.append(FaceIndex(i + 0, i + 1, i + 2)); // Front
            faces.append(FaceIndex(i + 2, i + 3, i + 0));

            faces.append(FaceIndex(i + 1, i + 4, i + 7)); // Right
            faces.append(FaceIndex(i + 7, i + 2, i + 1));

            faces.append(FaceIndex(i + 0, i + 3, i + 6)); // Left
            faces.append(FaceIndex(i + 6, i + 5, i + 0));

            faces.append(FaceIndex(i + 3, i + 2, i + 7)); // UP
            faces.append(FaceIndex(i + 7, i + 6, i + 3));

            faces.append(FaceIndex(i + 1, i + 0, i + 5)); // Bottom
            faces.append(FaceIndex(i + 5, i + 4, i + 1));

            faces.append(FaceIndex(i + 4, i + 5, i + 0)); // Back
            faces.append(FaceIndex(i + 6, i + 7, i + 4));
        }
    }

    mesh->rawData(vertices, faces);
    mesh->init();

    return mesh;
}

