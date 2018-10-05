#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <math.h>

template <typename T, int n>
struct Vector
{
    T data[n];

    T &operator[](int i)
    {
        return data[i];
    }
};

typedef Vector<float, 2> Vec2f;
typedef Vector<float, 3> Vec3f;
typedef Vector<float, 4> Vec4f;

typedef Vector<int, 2> Vec2i;
typedef Vector<int, 3> Vec3i;
typedef Vector<int, 4> Vec4i;

template <typename T>
struct Vector<T, 2>
{
    union {
        T data[2];
        struct
        {
            T x, y;
        };
    };

    Vector(T _x = 0, T _y = 0) : x(_x), y(_y)
    {
    }

    Vector(T f) : x(f), y(f)
    {
    }

    T &operator[](int i)
    {
        return data[i];
    }
};

template <typename T>
struct Vector<T, 3>
{
    union {
        T data[3];
        struct
        {
            T x, y, z;
        };
    };

    Vector(T _x = 0, T _y = 0, T _z = 0) : x(_x), y(_y), z(_z)
    {
    }

    Vector(T f) : x(f), y(f), z(f)
    {
    }

    Vector(Vector<T,2> A, T _z) : x(A.x), y(A.y), z(_z)
    {

    }

    Vector<T, 3> cross(Vector<T, 3> t)
    {
        Vector<T, 3> vec(y * t.z - z * t.y, z * t.x - x * t.z, x * t.y - y *t.x);

        return vec;
    }

    T &operator[](int i)
    {
        return data[i];
    }
};

template <typename T>
struct Vector<T, 4>
{
    union {
        T data[4];
        struct
        {
            T x, y, z, w;
        };
    };

    Vector(T _x = 0, T _y = 0, T _z = 0, T _w = 0) : x(_x), y(_y), z(_z), w(_w)
    {
    }

    Vector(T f) : x(f), y(f), z(f), w(f)
    {
    }

    Vector(Vector<T, 3> vec3, T _w) : x(vec3.x), y(vec3.y), z(vec3.z), w(_w)
    {
    }

    T &operator[](int i)
    {
        return data[i];
    }
};

// Addition
template <typename T, int n>
Vector<T, n> operator+(Vector<T, n> a, Vector<T, n> b)
{
    Vector<T, n> answer = a;
    for (int i = 0; i < n; i++)
    {
        answer.data[i] = a.data[i] + b.data[i];
    }

    return answer;
};

template <typename T, int n>
Vector<T, n> &operator+=(Vector<T, n> &a, Vector<T, n> b)
{
    a = a + b;
    return a;
};

// Subtraction
template <typename T, int n>
Vector<T, n> operator-(Vector<T, n> a, Vector<T, n> b)
{
    Vector<T, n> answer = a;
    for (int i = 0; i < n; i++)
    {
        answer.data[i] = a.data[i] - b.data[i];
    }

    return answer;
};

template <typename T, int n>
Vector<T, n> &operator-=(Vector<T, n> &a, Vector<T, n> b)
{
    a = a - b;
    return a;
};

// Multiplication
template <typename T, int n>
Vector<T, n> operator*(Vector<T, n> a, Vector<T, n> b)
{
    Vector<T, n> answer = a;
    for (int i = 0; i < n; i++)
    {
        answer.data[i] = a.data[i] * b.data[i];
    }

    return answer;
};

template <typename T, int n>
Vector<T, n> &operator*=(Vector<T, n> &a, Vector<T, n> b)
{
    a = a * b;
    return a;
};

// Division
template <typename T, int n>
Vector<T, n> operator/(Vector<T, n> a, Vector<T, n> b)
{
    Vector<T, n> answer = a;
    for (int i = 0; i < n; i++)
    {
        answer.data[i] = a.data[i] / b.data[i];
    }

    return answer;
};

template <typename T, int n>
Vector<T, n> &operator/=(Vector<T, n> &a, Vector<T, n> b)
{
    a = a / b;
    return a;
};

template <typename T, int n>
std::ostream &operator<<(std::ostream &os, Vector<T, n> a)
{
    os << "(";
    for (int i = 0; i < n; i++)
    {
        if (i != 0)
        {
            os << ",";
        }
        os << a[i];
    }
    os << ")" << std::endl;
}

template <typename T, int n>
float dot(Vector<T, n> a, Vector<T, n> b)
{
    float sum = 0.0f;
    for (int i = 0; i < n; i++)
    {
        sum += a[i] * b[i];
    }
    return sum;
}

template <typename T, int n>
float magnitude(Vector<T, n> a)
{
    float sum = 0.0f;
    for (int i = 0; i < n; i++)
    {
        sum += a[i] * a[i];
    }
    return sqrt(sum);
}

#endif