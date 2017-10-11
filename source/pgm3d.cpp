#include "pgm3d.h"

#include <fstream>
#include <iostream>

#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <assert.h>

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
	uint x = 0,y = 0, z = 0;
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
				float vValue = value/float(m_maxGrayscaleValue);
				Vertex v(Point3f(x, y, z), Color4f(vValue, vValue, vValue));
				x++;
				if(x == m_width) {
					x = 0;
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

	if(!(m_width * m_height * m_depth == uint(m_data.size())))
	{
		QMessageBox::critical(0, "Error", "Bad file fromat : Missing data.");
		return;
	}
}


// TODO: Move this two functions in utils ?
static void indexToCoord(uint index, uint width, uint height, uint depth, uint& x, uint& y, uint& z) {
	assert(width != 0 && height != 0 && depth != 0);

	x = (index % (width * height)) % width;
	y = (index % (width * height)) / width;
	z =  index / (width * height);
}

static int coordToIndex(uint width, uint height, uint depth, uint x, uint y, uint z) {
	assert(width != 0 && height != 0 && depth != 0);
	return x + y * width + z * height * width;
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

	// Create raw data
	QVector<Vertex> allVertices;
	QVector<EdgeIndex> allEdges;
	QVector<FaceIndex> allFaces;
	allVertices.reserve(voxelCount * 8);  // For each voxel we build 8 vertices
	allEdges.reserve(voxelCount * 18);    // For each voxel we build 18 edges
	allFaces.reserve(voxelCount * 12); // For each voxel we build 12 faces (6 * 2 triangles)

	for (unsigned long long i = 0; i < voxelCount; ++i) {
		Point3f pos = m_dataVertex[i].position;
		Color4f color = m_dataVertex[i].color;

		for (float x = -0.5; x <= 0.5; x++) {
			for (float y = -0.5; y <= 0.5; y++) {
				for (float z = -0.5; z <= 0.5; z++) {
					Vertex v(Point3f(x+pos.x,y+pos.y,z+pos.z), color);
					allVertices.push_back(v);
				}
			}
		}

		/*1--------5
		./|       /|
		0-+------4 |
		| |      | |
		| 3------+-7
		|/       |/
		2--------6 */
		uint offset = i * 8;

		// Index of Vertices to build the Faces : IVF
		int ivf[12][3] = {{0,1,3}, {0,2,3},  // left
						  {4,5,7}, {4,6,7},  // right
						  {0,1,5}, {0,4,5},  // top
						  {2,3,7}, {2,6,7},  // bot
						  {0,2,6}, {0,4,6},  // front
						  {1,3,7}, {1,5,7}}; // back
		for (int i = 0; i < 12; i++) {
			FaceIndex face(offset+ivf[i][0], offset+ivf[i][1], offset+ivf[i][2]);
			allFaces.push_back(face);
		}

		// Index of Vertices to build the Edges : IVE
		int ive[18][2]= {{0,1}, {1,5}, {5,4}, {4,0}, {0,5}, {4,7},
		                 {0,2}, {1,3}, {5,7}, {4,6}, {0,6}, {0,3},
						 {2,3}, {3,7}, {7,6}, {6,2}, {2,7}, {1,7}};
		for(int i = 0; i < 18; i++) {
			EdgeIndex edge(offset+ive[i][0], offset+ive[i][1]);
			allEdges.push_back(edge);
		}
	}

	// Create simplify data
	QVector<FaceIndex> faces;
	QVector<Vertex> vertices;

	uint x = 0, y = 0, z = 0, index = 0, offset;
	bool left, right, top, bot, front, back;
	bool newVertex;

	for(uint i = 0 ; i < voxelCount ; i++) {
		if(m_data[i] == 0) { continue; }
		newVertex = false;
		offset = vertices.size();

		indexToCoord(i, m_width, m_height, m_depth, x, y, z);

		left  = (x == 0 ? true : false);
		right = (x == m_width - 1 ? true : false);
		top   = (y == 0 ? true : false);
		bot   = (y == m_height - 1 ? true : false);
		front = (z == 0 ? true : false);
		back  = (z == m_depth - 1 ? true : false);

		if(!left) {
			index = coordToIndex(m_width, m_height, m_depth, x-1, y, z);
			if(m_dataVertex[i].color != m_dataVertex[index].color) {
				FaceIndex face1(offset, offset+1, offset+3); FaceIndex face2(offset, offset+2, offset+3);
				faces.push_back(face1); faces.push_back(face2);
				newVertex = true;
			}
		}

		if(!right) {
			index = coordToIndex(m_width, m_height, m_depth, x+1, y, z);
			if(m_dataVertex[i].color != m_dataVertex[index].color) {
				FaceIndex face1(offset+4, offset+5 , offset+7); FaceIndex face2(offset+4, offset+6, offset+7);
				faces.push_back(face1); faces.push_back(face2);
				newVertex = true;
			}
		}

		if(!top) {
			index = coordToIndex(m_width, m_height, m_depth, x, y-1, z);
			if(m_dataVertex[i].color != m_dataVertex[index].color) {
				FaceIndex face1(offset, offset+1, offset+5); FaceIndex face2(offset, offset+4, offset+5);
				faces.push_back(face1); faces.push_back(face2);
				newVertex = true;
			}
		}

		if(!bot) {
			index = coordToIndex(m_width, m_height, m_depth, x, y+1, z);
			if(m_dataVertex[i].color != m_dataVertex[index].color) {
				FaceIndex face1(offset + 2, offset + 3, offset + 7); FaceIndex face2(offset+2, offset+6, offset+7);
				faces.push_back(face1); faces.push_back(face2);
				newVertex = true;
			}
		}

		if(!front) {
			index = coordToIndex(m_width, m_height, m_depth, x, y, z-1);
			if(m_dataVertex[i].color != m_dataVertex[index].color) {
				FaceIndex face1(offset, offset+2, offset+6); FaceIndex face2(offset, offset+4, offset+6);
				faces.push_back(face1); faces.push_back(face2);
				newVertex = true;
			}
		}

		if(!back) {
			index = coordToIndex(m_width, m_height, m_depth, x, y, z+1);
			if(m_dataVertex[i].color != m_dataVertex[index].color) {
				FaceIndex face1(offset+1, offset+3, offset+7); FaceIndex face2(offset+1, offset+5, offset+7);
				faces.push_back(face1); faces.push_back(face2);
				newVertex = true;
			}
		}

		if (newVertex) {
			Point3f pos = m_dataVertex[i].position;
			Color4f color = m_dataVertex[i].color;
			for (float dx = -0.5; dx <= 0.5; dx++) {
				for (float dy = -0.5; dy <= 0.5; dy++) {
					for (float dz = -0.5; dz <= 0.5; dz++) {
						Vertex v(Point3f(dx+pos.x,dy+pos.y,dz+pos.z), color);
						vertices.push_back(v);
					}
				}
			}
		}
	}

	mesh->rawData(allVertices, allEdges, allFaces);
    mesh->simplifyData(vertices, faces);
	mesh->init();

	return mesh;
}
