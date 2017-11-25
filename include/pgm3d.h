#ifndef PGM3D_H
#define PGM3D_H

#include <QVector>

#include "model3d.h"
#include "utils.h"

class pgm3d : public Model3d
{
public:
    pgm3d();
    pgm3d(const QString& fileName);
    virtual ~pgm3d();

    uint width()  const             { return m_width;  }
    uint height() const             { return m_height; }
    uint depth()  const             { return m_depth;  }
    uint maxGrayscaleValue() const  { return m_maxGrayscaleValue; }

    virtual void loadFromFile(const QString& fileName) override;
    virtual Mesh* mesh() const override;

private:
    void indexToCoord(uint index, uint& x, uint& y, uint& z) const;
    int  coordToIndex(uint x, uint y, uint z) const;

    uint m_width;
    uint m_height;
    uint m_depth;
    uint m_maxGrayscaleValue;
    QVector<Vertex> m_data;
};

#endif // PGM3D_H
