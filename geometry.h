#ifndef GEOMETRY_H
#define GEOMETRY_H

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

    Vector()
    {
        x = 0.0f;
        y = 0.0f;
    }

    Vector(T _x, T _y)
    {
        x = _x;
        y = _y;
    }

    Vector(T f)
    {
        x = y = f;
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

    Vector()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    Vector(T _x, T _y, T _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector(T f)
    {
        x = y = z = f;
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

    Vector()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
    }

    Vector(T _x, T _y, T _z, T _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    Vector(Vector<T, 3> vec3, T _w)
    {
        x = vec3.x;
        y = vec3.y;
        z = vec3.z;
        w = _w;
    }

    Vector(T f)
    {
        x = y = z = w = f;
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
        if(i != 0)
        {
            os << ",";
        }
        os << a[i];
    }
    os << ")" << std::endl;
}

#endif