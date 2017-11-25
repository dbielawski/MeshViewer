#ifndef UTILS_H
#define UTILS_H

#include <Eigen/Dense>
#include <QVector>
#include <cmath>


typedef Eigen::Matrix<float, 3, 1> Point3f;
typedef Eigen::Matrix<float, 3, 1> Vector3f;
typedef Eigen::Matrix<float, 2, 1> Vector2f;
typedef Eigen::Matrix<int, 2, 1> Vector2i;

typedef QVector<uint> FaceIndex;


struct Color4f
{
    float r, g, b, a;

    Color4f(float red = 0.f, float green = 0.f, float blue = 0.f, float alpha = 1.f) :
        r(red), g(green), b(blue), a(alpha)
    {}

    Color4f(const Color4f& c):
        r(c.r), g(c.g), b(c.b), a(c.a)
    {}

    static Color4f gray()
    {
        return Color4f(0.5, 0.5, 0.5, 1.f);
    }
    static Color4f red()
    {
        return Color4f(1.f, 0.f, 0.f, 1.f);
    }

    inline Color4f& operator += (const Color4f& c)
    {
        r += c.r;
        g += c.g;
        b += c.b;
        return *this;
    }

	inline bool operator == (const Color4f& c) const {
        return (std::abs(this->r - c.r) < 0.001 &&
                std::abs(this->b - c.b) < 0.001 &&
                std::abs(this->g - c.g) < 0.001 &&
                std::abs(this->a - c.a) < 0.001);
	}

	inline bool operator != (const Color4f& c) const {
		return !(*this == c);
	}

    inline bool operator > (const Color4f& c) const {
        if ((this->r > c.r) ||
            (this->r == c.r && this->g > c.g) ||
            (this->r == c.r && this->g == c.g && this->b > c.b)) {
            return true;
        }
        return false;
    }

};

inline Color4f operator +(const Color4f& l, const Color4f& r)
{
    return Color4f(l.r + r.r, l.g + r.g, l.b + r.b);
}

struct Vertex
{
    Point3f position;
    Color4f color;
    Vector3f normal;
    Vector2f texcoord;

    Vertex() :
        position(Point3f(0.f, 0.f, 0.f)), color(Color4f(0.f, 0.f, 0.f))
    {}

    Vertex(const Point3f& p, const Color4f& c) :
        position(p), color(c)
    {}

    Vertex(const Vertex& v):
        position(v.position), color(v.color)
    {}
};

struct EdgeIndex
{
	uint v0, v1;
	EdgeIndex(uint v0 = 0, uint v1 = 0):
	v0(v0), v1(v1)
	{}
};


#endif // UTILS_H
