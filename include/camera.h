#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix3x3>
#include <QMatrix4x4>
#include <QQuaternion>

#include "utils.h"

class Camera
{
public:
    Camera();

    void setPerspective(float fovY, float near, float far);
    void setViewport(unsigned int oX, unsigned int oY, unsigned int width, unsigned int height);

    void lookAt(const Point3f& position, const Point3f& target, const Vector3f& up);

    QMatrix4x4 viewMatrix() const;
    QMatrix4x4 projectionMatrix();
    void rotateAround(const QQuaternion& q);
    void rotate(float angle, float x, float y, float z);

    void zoom(float z);

    void setPosition(const Point3f& p)          { m_position = p;       }
    Point3f position() const                    { return m_position;    }

    void setOrientation(const QQuaternion& q)   { m_orientation = q;    }
    QQuaternion orientation() const             { return m_orientation; }

    Vector3f up() const;
    Vector3f right() const;
    Vector3f direction() const;


private:
    void updateView();
    void updateProjection();

    QQuaternion m_orientation;
    Point3f m_position;
    Point3f m_target;
    Vector3f m_up;

    float m_fovY;
    float m_near;
    float m_far;

    unsigned int m_originX;
    unsigned int m_originY;
    unsigned int m_width;
    unsigned int m_height;

    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;
};

#endif // CAMERA_H
