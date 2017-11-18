#include "camera.h"


Camera::Camera() : m_originX(0), m_originY(0),
    m_fovY(60.f),
    m_near(0.001f), m_far(1000.f),
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

void Camera::setTarget(const Point3f& target)
{
    m_target = target;
}

void Camera::lookAt(const Point3f& position, const Point3f& target, const Vector3f& up)
{
    m_position = position;
    m_target = target;
    m_up = up;

    m_viewMatrix.setToIdentity();
}

QMatrix4x4 Camera::viewMatrix()
{
    m_viewMatrix.setToIdentity();

    m_viewMatrix.lookAt(QVector3D(m_position.x(), m_position.y(), m_position.z()),
                        QVector3D(m_target.x(), m_target.y(), m_target.z()),
                        QVector3D(m_up.x(), m_up.y(), m_up.z()));
    return m_viewMatrix;
}

QMatrix4x4 Camera::projectionMatrix()
{
    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(m_fovY, float(m_width) / float(m_height), m_near, m_far);

    return m_projectionMatrix;
}

void Camera::rotateAroundTarget(float angle, Vector3f axis)
{
    QQuaternion p(0, m_position.x(), m_position.y(), m_position.z());
    QQuaternion r = QQuaternion::fromAxisAndAngle(axis.x(), axis.y(), axis.z(), -angle);
    QQuaternion pp = r * (p * r.conjugate());

    m_position.x() = pp.toVector4D().x();
    m_position.y() = pp.toVector4D().y();
    m_position.z() = pp.toVector4D().z();
}

void Camera::zoom(float z)
{
    // Get the direction
    const Vector3f p = m_position - m_target;

    // Get the distance between the camera and the target
    const float dist = p.norm();

    // Check if the distance is greater
    // Don't want to go through the target
    if (dist > z)
    {
        m_position = m_position + direction() * z;
        updateView();
    }
}

Vector3f Camera::up() const
{
    QMatrix4x4 inverted = m_viewMatrix.inverted();

    return Vector3f(inverted(0, 1), inverted(1, 1), inverted(2, 1));
}

Vector3f Camera::right() const
{
    QMatrix4x4 inverted = m_viewMatrix.inverted();

    return Vector3f(inverted(0, 0), inverted(1, 0), inverted(2, 0));
}

Vector3f Camera::direction() const
{
    QMatrix4x4 inverted = m_viewMatrix.inverted();

    return Vector3f(-inverted(0, 2), -inverted(1, 2), -inverted(2, 2));
}

void Camera::updateView()
{
    lookAt(m_position, m_target, m_up);
}

void Camera::updateProjection()
{
    m_projectionMatrix.perspective(m_fovY, float(m_width) / float(m_height), m_near, m_far);
}
