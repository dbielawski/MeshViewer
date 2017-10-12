#include "wireboundingbox.h"

#include "scene.h"
#include "camera.h"

WireBoundingBox::WireBoundingBox()
{

}

WireBoundingBox::WireBoundingBox(const AlignedBox3f& aabb)
{
    m_aabb = &aabb;
}

void WireBoundingBox::init()
{
    computeBox();

    m_functionsPtr->glGenBuffers(1, &m_vertexBufferId);
    m_functionsPtr->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
    m_functionsPtr->glBufferData(GL_ARRAY_BUFFER, sizeof(Point3f) * 8, &m_points[0], GL_STATIC_DRAW);

    m_functionsPtr->glGenBuffers(1, &m_indexBufferId);
    m_functionsPtr->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);
    m_functionsPtr->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Vector2i) * 12, &m_indices[0], GL_STATIC_DRAW);
}

void WireBoundingBox::render(const Scene& scene, const QMatrix4x4& transform) const
{
    if (scene.simpleShaderProgram()->bind())
    {
        m_functionsPtr->glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId);
        m_functionsPtr->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferId);

        QGLShaderProgram* program = scene.simpleShaderProgram();
        program->setUniformValue("mat_obj", transform);
        program->setUniformValue("mat_view", scene.camera()->viewMatrix());
        program->setUniformValue("mat_proj", scene.camera()->projectionMatrix());

        const int vertexLoc = program->attributeLocation("vtx_position");

        // TODO: creer une variable membre color et ajouter au constructeur
        float c[4] = {0.f, 1.f, 0.f, 1.f};
        program->setAttributeArray("vtx_color", GL_FLOAT, &c[0], 4);

        if (vertexLoc >= 0)
        {
            m_functionsPtr->glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Point3f), (void*)0);
            m_functionsPtr->glEnableVertexAttribArray(vertexLoc);
        }

        glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, (void*)0);

        if (vertexLoc >= 0) m_functionsPtr->glDisableVertexAttribArray(vertexLoc);
    }
}

void WireBoundingBox::computeBox()
{
    Point3f min = m_aabb->min();
    Point3f max = m_aabb->max();

    m_points[0] = min;                          // Left Bottom Back
    m_points[1] = Point3f(min.x, max.y, min.z); // Left Top Back
    m_points[2] = Point3f(max.x, max.y, min.z); // Right Top Back
    m_points[3] = Point3f(max.x, min.y, min.z); // Right Bottom Back

    m_points[4] = Point3f(max.x, min.y, max.z); // Right Bottom Front
    m_points[5] = Point3f(min.x, min.y, max.z); // Left Bottom Front
    m_points[6] = Point3f(min.x, max.y, max.z); // Left Top Front
    m_points[7] = Point3f(max.x, max.y, max.z); // Right Top Front

    m_indices[0] = Vector2i(0, 1);
    m_indices[1] = Vector2i(1, 2);
    m_indices[2] = Vector2i(2, 3);
    m_indices[3] = Vector2i(3, 0);

    m_indices[4] = Vector2i(4, 5);
    m_indices[5] = Vector2i(5, 6);
    m_indices[6] = Vector2i(6, 7);
    m_indices[7] = Vector2i(7, 4);

    m_indices[8] = Vector2i(0, 5);
    m_indices[9] = Vector2i(1, 6);
    m_indices[10] = Vector2i(2, 7);
    m_indices[11] = Vector2i(3, 4);
}
