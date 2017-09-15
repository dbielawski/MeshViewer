#ifndef PGM3D_H
#define PGM3D_H

#include "model3d.h"
#include "utils.h"


class pgm3d : public Model3d
{
public:
    pgm3d();
    pgm3d(const QString& fileName);
    virtual ~pgm3d();

    unsigned int width() const              { return m_width;   }
    unsigned int height() const             { return m_height;  }
    unsigned int depth() const              { return m_depth;   }
    unsigned int maxGrayscaleValue() const  { return m_maxGrayscaleValue; }
    unsigned int* data() const              { return m_data; }


    virtual void loadFromFile(const QString& fileName) override;
    virtual Mesh* mesh() const override;

private:

    //    v6----- v5
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v7---|-|v4
    //  |/      |/
    //  v2------v3

    //    const Vector3f m_voxel[] = {
    //        Vector3f(),
    //        Vector3f(),
    //        Vector3f(),
    //        Vector3f(),

    //        Vector3f(),
    //        Vector3f(),
    //        Vector3f(),
    //        Vector3f()
    //    };

    unsigned int m_width;
    unsigned int m_height;
    unsigned int m_depth;
    unsigned int m_maxGrayscaleValue;
    unsigned int* m_data;
};

#endif // PGM3D_H
