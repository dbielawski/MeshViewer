#include "camera.h"
#include <iostream>

Camera::Camera() : m_originX(0), m_originY(0), m_fovY(60.f), m_near(0.001f), m_far(1000.f),
    m_position(0.f, 0.f, 5.f),
    m_target(0.f, 0.f, 0.f)
{

}

void Camera::setSize(int w, int h)
{
    m_width = w;
    m_height = h;

    updateProjection();
}

void Camera::setPerspective(float fovY, float near, float far)
{
    m_fovY = fovY;
    m_near = near;
    m_far = far;

    updateProjection();
}

void Camera::setViewport(unsigned int oX, unsigned int oY, unsigned int width, unsigned int height)
{
    m_originX = oX;
    m_originY = oY;
    m_width = width;
    m_height = height;

    updateProjection();
}

void Camera::lookAt(const Point3f& position, const Point3f& target, const Vector3f& up)
{
    m_position = position;
    m_target = target;
    m_up = up;

    m_viewMatrix.setToIdentity();

    m_viewMatrix.lookAt(QVector3D(position.x, position.y, position.z),
                        QVector3D(target.x, target.y, target.z),
                        QVector3D(up.x, up.y, up.z));
}

QMatrix4x4 Camera::viewMatrix() const
{
    return m_viewMatrix;
}

QMatrix4x4 Camera::projectionMatrix()
{
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(m_fovY, float(m_width) / float(m_height), m_near, m_far);

    return m_projectionMatrix;
}

void Camera::rotateAround(const QQuaternion& q)
{
    Point3f center = m_target - m_position;
    m_viewMatrix.translate(QVector3D(center.x, center.y, center.z));
    m_viewMatrix.rotate(q);
    m_viewMatrix.translate(QVector3D(m_position.x, m_position.y, m_position.z));

    updateView();
}

void Camera::rotateAround(float angle, Vector3f axis)
{
    Point3f center = m_target - m_position;

//    m_viewMatrix.translate(QVector3D(-center.x, -center.y, -center.z));
    m_viewMatrix.rotate(10, axis.x, axis.y, axis.z);
//    m_viewMatrix.translate(QVector3D(center.x, center.y, center.z));

    updateView();
}

void Camera::rotate(float angle, float x, float y, float z)
{
    m_viewMatrix.rotate(angle, x, y, z);

    updateView();
}

void Camera::zoom(float z)
{
    Point3f p = (m_position - m_target);
    Vector3f v(p.x, p.y, p.z);
    float dist = v.norm();

    if (dist > z)
    {
        m_position = m_position + direction() * z;
    }

    updateView();
}

Vector3f Camera::up() const
{
    return Vector3f(m_viewMatrix(0, 1), m_viewMatrix(1, 1), m_viewMatrix(2, 1));
}

Vector3f Camera::right() const
{
    return Vector3f(m_viewMatrix(0, 0), m_viewMatrix(1, 0), m_viewMatrix(2, 0));
}

Vector3f Camera::direction() const
{
    return Vector3f(-m_viewMatrix(0, 2), -m_viewMatrix(1, 2), -m_viewMatrix(2, 2));
}

void Camera::updateView()
{
    lookAt(m_position, m_target, m_up);
}

void Camera::updateProjection()
{
    m_projectionMatrix.perspective(m_fovY, float(m_width) / float(m_height), m_near, m_far);
}
