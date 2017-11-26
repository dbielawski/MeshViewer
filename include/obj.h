#ifndef OBJ_H
#define OBJ_H

#include <QVector>
#include "model3d.h"
#include "utils.h"

class obj : public Model3d
{
public:
    obj(const QString& fileName);

    virtual void loadFromFile(const QString& fileName) override;

    virtual Mesh* mesh() const override;

    QVector<Vertex> vertices() const;
    QVector<FaceIndex> faces() const;

private:
    QVector<Vertex>     m_vertices;
    QVector<FaceIndex>  m_faces;
};

#endif // OBJ_H
