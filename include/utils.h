#ifndef UTILS_H
#define UTILS_H

#include <math.h>
#include <QVector>

namespace arm
{
struct Color3f;
struct Vector3f;
struct Point3i;
struct Point3f;
struct Vertex;
struct FaceIndex;
}


struct FaceIndex
{
    unsigned int v0, v1, v2;

    FaceIndex(unsigned int x_ = 1, unsigned int y_ = 2, unsigned int z_ = 3):
        v0(x_), v1(y_), v2(z_)
    {
    }
};

struct Color3f
{
    float r, g, b;

    Color3f(float v): r(v), g(v), b(v)
    {}

    Color3f(float red = 0.f, float green = 0.f, float blue = 0.f) :
        r(red), g(green), b(blue)
    {}

    Color3f(const Color3f& c):
        r(c.r), g(c.g), b(c.b)
    {}

    inline Color3f& operator += (const Color3f& c)
    {
        r += c.r;
        g += c.g;
        b += c.b;
        return *this;
    }
};

inline Color3f operator +(const Color3f& l, const Color3f& r)
{
    return Color3f(l.r + r.r, l.g + r.g, l.b + r.b);
}

struct Vector3f
{
    float x, y, z;

    Vector3f(float x_ = 0.f, float y_ = 0.f, float z_ = 0.f) :
        x(x_), y(y_), z(z_)
    {}

    inline float dot(const Vector3f& v) const
    {
        return (x * v.x) + (y * v.y) + (z * v.z);
    }

    inline Vector3f cross(const Vector3f& v) const
    {
        return Vector3f((y * v.z - z * v.y) + (z * v.x - x * v.z) + (x * v.y - y * v.x));
    }

    inline float squaredNorm() const
    {
        return (x * x) + (y * y) + (z * z);
    }

    inline float norm() const
    {
        return sqrt(squaredNorm());
    }

    inline void normalise()
    {
        *this /= norm();
    }

    inline Vector3f normalised() const
    {
        float n = norm();
        return Vector3f(x / n, y / n, z / n);
    }

    inline Vector3f& operator +=(const Vector3f& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    inline Vector3f& operator /=(float f)
    {
        if (f == 0.f)
            return *this;

        x /= f;
        y /= f;
        z /= f;
        return *this;
    }
};

inline Vector3f operator +(const Vector3f& l, const Vector3f& r)
{
    return Vector3f(l.x + r.x, l.y + r.y, l.z + r.z);
}

inline Vector3f operator *(const Vector3f& l, float f)
{
    return Vector3f(l.x * f, l.y * f, l.z * f);
}

inline Vector3f operator -(const Vector3f& l, const Vector3f& r)
{
    return Vector3f(l.x - r.x, l.y - r.y, l.z - r.z);
}

inline Vector3f operator /(const Vector3f& l, float f)
{
    if (f == 0.f) return l;

    return Vector3f(l.x / f, l.y / f, l.z / f);
}

struct Point3i
{
    int x, y, z;

    Point3i(int x_ = 0, int y_ = 0, int z_ = 0):
        x(x_), y(y_), z(z_)
    {}

    Point3i(const Point3i& p):
        x(p.x), y(p.y), z(p.z)
    {}
};

struct Point3f
{
    float x, y, z;

    Point3f(float x_ = 0.f, float y_ = 0.f, float z_ = 0.f) :
        x(x_), y(y_), z(z_)
    {}

    Point3f(const Point3f& p) : x(p.x), y(p.y), z(p.z)
    {}
};

inline Point3f operator +(const Point3f& l, const Point3f& r)
{
    return Point3f(l.x + r.x, l.y + r.y, l.z + r.z);
}

inline Point3f operator +(const Point3f& l, const Point3i& r)
{
    return Point3f(l.x + r.x, l.y + r.y, l.z + r.z);
}

inline Point3f operator +(const Point3i& l, const Point3f& r)
{
    return Point3f(l.x + r.x, l.y + r.y, l.z + r.z);
}

inline Point3f operator +(const Point3f& l, const Vector3f& r)
{
    return Point3f(l.x + r.x, l.y + r.y, l.z + r.z);
}

inline Point3f operator +(const Vector3f& l, const Point3f& r)
{
    return Point3f(l.x + r.x, l.y + r.y, l.z + r.z);
}

inline Point3f operator -(const Point3f& l, const Point3f& r)
{
    return Point3f(l.x - r.x, l.y - r.y, l.z - r.z);
}


struct Vertex
{
    Point3f position;
    Color3f color;

    Vertex() :
        position(Point3f(0.f, 0.f, 0.f)), color(Color3f(0.f, 0.f, 0.f))
    {}

    Vertex(const Point3f& p, const Color3f& c) :
        position(p), color(c)
    {}

    Vertex(const Vertex& v):
        position(v.position), color(v.color)
    {}
};

#endif // UTILS_H
