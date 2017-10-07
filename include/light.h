#ifndef LIGHT_H
#define LIGHT_H

#include "utils.h"

class Light
{
public:
    Light(const Color4f& intensity) : m_intensity(intensity)
    {}

    virtual Vector3f direction() const = 0;
    virtual Color4f intensity(const Point3f& position) const = 0;

protected:
    Color4f m_intensity;
};

class DirectionalLight: public Light
{
public:
    DirectionalLight(const Color4f& intensity, const Vector3f& direction) :
        Light(intensity),
        m_direction(direction)
    {}

    // Direction is always the same
    virtual Vector3f direction() const override
    {
        return m_direction;
    }

    // Directional Light has constant intensity
    virtual Color4f intensity(const Point3f& position) const override
    {
        Q_UNUSED(position)
        return m_intensity;
    }

private:
    Vector3f m_direction;
};

#endif // LIGHT_H
