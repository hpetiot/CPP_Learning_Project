#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <functional>
#include <iostream>
#include <numeric>

struct Point2D
{
    float values[2] {};

    Point2D() {}
    Point2D(float x, float y) : values { x, y } {}

    float& x() { return values[0]; }
    float x() const { return values[0]; }

    float& y() { return values[1]; }
    float y() const { return values[1]; }

    Point2D& operator+=(const Point2D& other)
    {
        x() += other.x();
        y() += other.y();
        return *this;
    }

    Point2D& operator*=(const Point2D& other)
    {
        x() *= other.x();
        y() *= other.y();
        return *this;
    }

    Point2D& operator*=(const float scalar)
    {
        x() *= scalar;
        y() *= scalar;
        return *this;
    }

    Point2D operator+(const Point2D& other) const
    {
        Point2D result = *this;
        result += other;
        return result;
    }

    Point2D operator*(const Point2D& other) const
    {
        Point2D result = *this;
        result *= other;
        return result;
    }

    Point2D operator*(const float scalar) const
    {
        Point2D result = *this;
        result *= scalar;
        return result;
    }
};

struct Point3D
{
    std::array<float, 3> values {};

    Point3D() {}
    Point3D(float x, float y, float z) : values { x, y, z } {}

    float& x() { return values[0]; }
    float x() const { return values[0]; }

    float& y() { return values[1]; }
    float y() const { return values[1]; }

    float& z() { return values[2]; }
    float z() const { return values[2]; }

    Point3D& operator+=(const Point3D& other)
    {
        // x() += other.x();
        // y() += other.y();
        // z() += other.z();
        // return *this;
        // values += other.values;
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), std::plus());
        return *this;
    }

    Point3D& operator-=(const Point3D& other)
    {
        std::transform(values.begin(), values.end(), other.values.begin(), values.begin(), std::minus());

        return *this;
    }

    Point3D& operator*=(const float scalar)
    {
        std::transform(values.begin(), values.end(), values.begin(),
                       [scalar](auto& val) { return val * scalar; });

        return *this;
    }

    Point3D operator+(const Point3D& other) const
    {
        Point3D result = *this;
        result += other;
        return result;
    }

    Point3D operator-(const Point3D& other) const
    {
        Point3D result = *this;
        result -= other;
        return result;
    }

    Point3D operator*(const float scalar) const
    {
        Point3D result = *this;
        result *= scalar;
        return result;
    }

    Point3D operator-() const { return Point3D { -x(), -y(), -z() }; }

    float length() const
    {
        return std::sqrt(std::accumulate(values.begin(), values.end(), 0.f,
                                         [](float acc, float value) { return value * value + acc; }));
    }

    float distance_to(const Point3D& other) const { return (*this - other).length(); }

    Point3D& normalize(const float target_len = 1.0f)
    {
        const float current_len = length();
        if (current_len == 0)
        {
            throw std::logic_error("cannot normalize vector of length 0");
        }

        *this *= (target_len / current_len);
        return *this;
    }

    Point3D& cap_length(const float max_len)
    {
        // assert(max_len > 0.01f);

        const float current_len = length();
        if (current_len > max_len)
        {
            if (max_len > 0.f)
            {
                *this *= (max_len / current_len);
            }
            else
            {
                *this *= 0;
            }
        }
        return *this;
    }
};

// our 3D-coordinate system will be tied to the airport: the runway is parallel to the x-axis, the z-axis
// points towards the sky, and y is perpendicular to both thus,
// {1,0,0} --> {.5,.5}   {0,1,0} --> {-.5,.5}   {0,0,1} --> {0,1}
inline Point2D project_2D(const Point3D& p)
{
    return { .5f * p.x() - .5f * p.y(), .5f * p.x() + .5f * p.y() + p.z() };
}

template <typename local_type, size_t size> class Point
{
private:
    std::array<local_type, size> coord;

    template <class BinOp> Point& operate(const Point& other, BinOp binary_op)
    {
        assert(other.coord.size() == size);
        std::transform(coord.begin(), coord.end(), other.coord.begin(), coord.begin(), binary_op);
        return *this;
    }

public:
    // Point() = default;
    // Point() = default;
    template <typename... Params>
    Point(Params&&... params) : coord { static_cast<local_type>(std::forward<Params>(params))... }
    {
        static_assert(size == sizeof...(params));
        // static_assert(std::is_arithmetic_v<local_type> && "only arithmetic types are allowed in this
        // class"); -> forgot we needed to use SFINAE.
    }
    // Point& operator=(const Point& original){return Point(){coord.}}
    Point& operator+=(const Point& other) { return operate(other, std::plus()); }

    Point& operator-=(const Point& other) { return operate(other, std::minus()); }

    Point& operator*=(const local_type scalar)
    {
        // return operate(other, [scalar](auto& val) { return val * scalar; });
        std::for_each(coord.begin(), coord.end(), [scalar](auto& value) { value *= scalar; });
        return *this;
    }
    Point operator+(const Point& other) const
    {
        Point result = *this;
        result += other;
        return result;
    }

    Point operator-(const Point& other) const
    {
        Point result = *this;
        result -= other;
        return result;
    }

    Point operator*(const local_type scalar) const
    {
        Point result = *this;
        result *= scalar;
        return result;
    }

    Point operator-() const
    {
        Point negativ;
        std::transform(coord.begin(), coord.end(), negativ.coord.begin(), std::negate());
        return negativ;
    }

    double length() const
    {
        return std::sqrt(std::accumulate(coord.begin(), coord.end(), 0.d,
                                         [](local_type acc, local_type value)
                                         { return value * value + acc; }));
    }
    // template <typename local_type, size_t size>
    friend std::ostream& operator<<(std::ostream& stream, const Point& point)
    {
        stream << "Point<" << typeid(local_type).name() << ", " << point.coord.size() << ">(";
        for (const auto& val : point.coord)
        {
            stream << val << " ";
        }
        stream << ")";
        return stream;
    }
};
