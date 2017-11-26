#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>
#include <QQuaternion>

#include "utils.h"

class Camera
{
public:
    Camera();

    void setSize(int w, int h);
    Vector2i size() const { return Vector2i(m_width, m_height); }

    void setPerspective(float fovY, float near, float far);
    void setViewport(uint oX, uint oY, uint width, uint height);
    void setTarget(const Point3f& point);

    void lookAt(const Point3f& position, const Point3f& target, const Vector3f& up);

    QMatrix4x4 viewMatrix();
    QMatrix4x4 projectionMatrix();

    void rotateAroundTarget(float angle, const Vector3f axis);

    void zoom(float z);
    
    Vector3f up() const;
    Vector3f right() const;
    Vector3f direction() const;

    void setPosition(const Point3f& p)        { m_position = p;       }
    Point3f position() const                  { return m_position;    }

    void setOrientation(const QQuaternion& q) { m_orientation = q;    }
    QQuaternion orientation() const           { return m_orientation; }

private:
    void updateView();
    void updateProjection();

    QQuaternion m_orientation;
    Point3f m_position;
    Point3f m_target;
    Vector3f m_up;

    uint m_originX;
    uint m_originY;
    float m_fovY;
    float m_near;
    float m_far;

    uint m_width;
    uint m_height;

    QMatrix4x4 m_viewMatrix;
    QMatrix4x4 m_projectionMatrix;
};

#endif // CAMERA_H
