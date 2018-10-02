#ifndef GEOMETRY_H
#define GEOMETRY_H

template <int n>
struct Vector
{
    float data[n];

    float &operator[](int i)
    {
        return data[i];
    }
};

typedef Vector<2> Vec2;
typedef Vector<3> Vec3;
typedef Vector<4> Vec4;

template <>
struct Vector<2>
{
    union {
        float data[2];
        struct
        {
            float x, y;
        };
    };

    Vector()
    {
        x = 0.0f;
        y = 0.0f;
    }

    Vector(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    Vector(float f)
    {
        x = y = f;
    }

    float &operator[](int i)
    {
        return data[i];
    }
};

template <>
struct Vector<3>
{
    union {
        float data[3];
        struct
        {
            float x, y, z;
        };
    };

    Vector()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    Vector(float _x, float _y, float _z)
    {
        x = _x;
        y = _y;
        z = _z;
    }

    Vector(float f)
    {
        x = y = z = f;
    }

    float &operator[](int i)
    {
        return data[i];
    }
};

template <>
struct Vector<4>
{
    union {
        float data[4];
        struct
        {
            float x, y, z, w;
        };
    };

    Vector()
    {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
        w = 0.0f;
    }

    Vector(float _x, float _y, float _z, float _w)
    {
        x = _x;
        y = _y;
        z = _z;
        w = _w;
    }

    Vector(Vec3 vec3, float _w)
    {
        x = vec3.x;
        y = vec3.y;
        z = vec3.z;
        w = _w;
    }

    Vector(float f)
    {
        x = y = z = w = f;
    }

    float &operator[](int i)
    {
        return data[i];
    }
};

// Addition
template <int n>
Vector<n> operator+(Vector<n> a, Vector<n> b)
{
    Vector<n> answer = a;
    for (int i = 0; i < n; i++)
    {
        answer.data[i] = a.data[i] + b.data[i];
    }

    return answer;
};

template <int n>
Vector<n> &operator+=(Vector<n> &a, Vector<n> b)
{
    a = a + b;
    return a;
};

// Subtraction
template <int n>
Vector<n> operator-(Vector<n> a, Vector<n> b)
{
    Vector<n> answer = a;
    for (int i = 0; i < n; i++)
    {
        answer.data[i] = a.data[i] - b.data[i];
    }

    return answer;
};

template <int n>
Vector<n> &operator-=(Vector<n> &a, Vector<n> b)
{
    a = a - b;
    return a;
};

// Multiplication
template <int n>
Vector<n> operator*(Vector<n> a, Vector<n> b)
{
    Vector<n> answer = a;
    for (int i = 0; i < n; i++)
    {
        answer.data[i] = a.data[i] * b.data[i];
    }

    return answer;
};

template <int n>
Vector<n> &operator*=(Vector<n> &a, Vector<n> b)
{
    a = a * b;
    return a;
};

// Division
template <int n>
Vector<n> operator/(Vector<n> a, Vector<n> b)
{
    Vector<n> answer = a;
    for (int i = 0; i < n; i++)
    {
        answer.data[i] = a.data[i] / b.data[i];
    }

    return answer;
};

template <int n>
Vector<n> &operator/=(Vector<n> &a, Vector<n> b)
{
    a = a / b;
    return a;
};

template <int n>
std::ostream &operator<<(std::ostream &os, Vector<n> a)
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