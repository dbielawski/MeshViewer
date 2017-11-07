#ifndef POLYHEDRON_H
#define POLYHEDRON_H

#include <QVector>
#include "mesh.h"


#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Polyhedron_incremental_builder_3.h>

typedef CGAL::Simple_cartesian<double>                  Kernel;
typedef Kernel::Point_3                                 Point_3;
typedef CGAL::Polyhedron_3<Kernel>                      Polyhedron;

// Used in the builder (obj.h)
typedef Polyhedron::HalfedgeDS                          HalfedgeDS;

template <class HDS>
class Polyhedron_builder: public CGAL::Modifier_base<HDS> {

public:
    typedef HDS Half_edge_data_structure;

    Polyhedron_builder(QVector<Vertex> vertices, QVector<FaceIndex> faces) :
        m_vertices(vertices), m_faces(faces){}

    void operator()(HDS& hds);

private:
    QVector<Vertex> m_vertices;
    QVector<FaceIndex> m_faces;
};

template <class HDS>
void Polyhedron_builder<HDS>::operator()(HDS& hds) {
    CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);
    B.begin_surface(m_vertices.size(), m_faces.size());

    for (Vertex v: m_vertices)
        B.add_vertex(Point_3(v.position.x, v.position.y, v.position.z));

    for (FaceIndex face: m_faces) {
        B.begin_facet();
        for (int i = 0 ; i < face.size(); ++i)
        {
            B.add_vertex_to_facet(face.at(i));
        }
        B.end_facet();
    }
}


#endif // POLYHEDRON_H

