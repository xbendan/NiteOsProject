#include <stdcxx/math.h>
#include <stdcxx/type-traits.h>

template <typename T, uint8_t Dim>
    requires Std::IsArithmetic<T>
struct Vector
{

private:
    T m_num[Dim];
};

template <typename T>
    requires Std::IsArithmetic<T>
struct Vector<T, 2>
{
    T x, y;

    Vec2D() = default;
    Vec2D(T x, T y)
      : x(x)
      , y(y)
    {
    }

    Vec2D<T> operator+(const Vec2D<T>& other) const
    {
        return Vec2D<T>(x + other.x, y + other.y);
    }

    Vec2D<T> operator-(const Vec2D<T>& other) const
    {
        return Vec2D<T>(x - other.x, y - other.y);
    }

    Vec2D<T> operator*(const Vec2D<T>& other) const
    {
        return Vec2D<T>(x * other.x, y * other.y);
    }

    Vec2D<T> operator/(const Vec2D<T>& other) const
    {
        return Vec2D<T>(x / other.x, y / other.y);
    }

    Vec2D<T> operator+(T other) const { return Vec2D<T>(x + other, y + other); }

    Vec2D<T> operator-(T other) const { return Vec2D<T>(x - other, y - other); }

    Vec2D<T> operator*(T other) const { return Vec2D<T>(x * other, y * other); }

    Vec2D<T> operator/(T other) const { return Vec2D<T>(x / other, y / other); }

    Vec2D<T>& operator+=(const Vec2D<T>& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vec2D<T>& operator-=(const Vec2D<T>& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Vec2D<T>& operator*=(const Vec2D<T>& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    Vec2D<T>& operator/=(const Vec2D<T>& other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    Vec2D<T>& operator+=(T other)
    {
        x += other;
        y += other;
        return *this;
    }

    Vec2D<T>& operator-=(T other)
    {
        x -= other;
        y -= other;
        return *this;
    }

    Vec2D<T>& operator*=(T other)
    {
        x *= other;
        y *= other;
        return *this;
    }

    Vec2D<T>& operator/=(T other)
    {
        x /= other;
        y /= other;
        return *this;
    }

    bool operator==(const Vec2D<T>& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vec2D<T>& other) const
    {
        return x != other.x || y != other.y;
    }

    T dot(const Vec2D<T>& other) const { return x * other.x + y * other.y; }

    T length() const { return sqrt(x * x + y * y); }

    T lengthSquared() const { return x * x + y * y; }
};

template <typename T>
    requires Std::IsArithmetic<T>
struct Vector<T, 3>
{
    T x, y, z;
};