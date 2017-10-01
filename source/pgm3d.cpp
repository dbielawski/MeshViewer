#include "pgm3d.h"

#include <fstream>
#include <iostream>

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include "mesh.h"


#define HEADER_SIZE 5


pgm3d::pgm3d(): m_width(0), m_height(0), m_depth(0), m_data()
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
}


void pgm3d::loadFromFile(const QString& fileName)
{
    std::ifstream inputFile(fileName.toStdString());

    if(!inputFile.is_open())
    {
        QMessageBox::critical(0, "Error", QString("Couldn't open file " + fileName));
        return;
    }

    std::string data;
    std::string format;

    int headerCount(0);
    int x = 0,y = 0, z = 0;
    while(inputFile >> data && inputFile)
    {
        if(data[0] == '#')
        {
            inputFile.ignore(INT_MAX, '\n');
        }
        else
        {
            /* If we have already read the header, we store the raw data in m_data */
            if(headerCount == HEADER_SIZE)
            {
                uint8_t value = (uint8_t)atoi(data.c_str());
                m_data.push_back(value);

                if(value > m_maxGrayscaleValue) {
                    QMessageBox::critical(0, "Error", "Bad gray value.");
                    return;
                }
                Vertex v(Point3f(x, y, z), Color4f(value, 0.f, 0.f));
                x++;
                if(x == m_width) {
                    x=0;
                    y++;
                }
                if(y == m_height) {
                    y = 0;
                    z++;
                }
                m_dataVertex.append(v);
            }
            else
            {
                /* We need to read the HEADER_SIZE values for header informations
                We assume that they're in the right order -> format width height depth grayscales */
                headerCount++;
                switch(headerCount)
                {
                case 1:
                    format = data.c_str();
                    if(format != "PGM3D") {
                        QMessageBox::critical(0, "Error", "Bad file format : Bad header.");
                        return;
                    }
                    break;
                case 2:
                    m_width = atoi(data.c_str());
                    break;
                case 3:
                    m_height = atoi(data.c_str());
                    break;
                case 4:
                    m_depth = atoi(data.c_str());
                    break;
                case 5:
                    m_maxGrayscaleValue = (uint8_t)atoi(data.c_str());
                    break;
                default:
                    break;
                }
            }
        }
    }

    if( !(m_width * m_height * m_depth == m_data.size()))
    {
        QMessageBox::critical(0, "Error", "Bad file fromat : Missing data.");
        return;
    }
}

// TODO: deplacer la fonction
/*
Point3i indexTo3Dcoordinates(unsigned int index, unsigned int width, unsigned int height, unsigned int depth)
{
unsigned int x = index % width;
unsigned int y = index % height;
unsigned int z = index % depth;

return Point3i(x, y, z);
}
*/

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
    // TODO: reserve pour l'optimisation
    // vertices.reserve();
    // faces.reserve();

    for (unsigned long long i = 0; i < voxelCount; ++i)
    {
        if (m_data[i] != 0) // if the voxel is not black
        {

            // Compute the centered position of the Voxel
            Point3f offset = m_dataVertex[i].position;
            Color4f color = m_dataVertex[i].color;

            /*
            1--------5
            /|       /|
            0-+------4 |
            | |      | |
            | 3------+-7
            |/       |/
            2--------6
            */
            int index[8];
            for (size_t i = 0; i < 8; i++)
            {
                index[i] = -1;
            }
            int cpt = 0;
            for (float x = -0.5; x <= 0.5; x++)
            {
                for (float y = -0.5; y <= 0.5; y++)
                {
                    for (float z = -0.5; z <= 0.5; z++)
                    {
                        Vertex v(offset + Point3f(x,y,z), color);
                        std::cerr << v.position.x << " " << v.position.y << " " << v.position.z << '\n';

                        for (int verticeIndex = 0; verticeIndex < vertices.size(); verticeIndex++)
                        {
                            if ((v.position.x == vertices[verticeIndex].position.x) &&
                                    (v.position.y == vertices[verticeIndex].position.y) &&
                                    (v.position.z == vertices[verticeIndex].position.z))
                            {
                                index[cpt] = verticeIndex;
                                // TODO : que faire si la couleur est différente?
                                break;
                            }
                        }

                        if (index[cpt] == -1) {
                            index[cpt] = vertices.size();
                            vertices.append(v);
                        }

                        cpt++;
                    }
                }
            }
            std::cerr << '\n';
            int index_vertice[12][3] = {{0,3,1}, {0,3,2},  // left
                                        {0,5,1}, {0,5,4},  // up
                                        {0,6,2}, {0,6,4},  // front
                                        {7,1,3}, {7,1,5},  // back
                                        {7,2,3}, {7,2,6},  // down
                                        {7,4,5}, {7,4,6}}; // right
            for (int i = 0; i < 12; i++) {
                bool face_exist = false;
                for (int faceIndex = 0; faceIndex < faces.size(); faceIndex++) {
                    if (((unsigned)index_vertice[i][0] == faces[faceIndex].v0) &&
                            ((unsigned)index_vertice[i][1] == faces[faceIndex].v1) &&
                            ((unsigned)index_vertice[i][2] == faces[faceIndex].v2)) {
                        face_exist = true;
                        continue;
                    }
                }

                if (!face_exist) {
                    faces.append(FaceIndex(index_vertice[i][0],
                                 index_vertice[i][1],
                            index_vertice[i][2]));
                }
            }
        }
    }

    mesh->rawData(vertices, faces);
    mesh->init();

    return mesh;
}
