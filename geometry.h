#ifndef GEOMETRY_H
#define GEOMETRY_H

template <int n>
struct Vector 
{
    float data[n]; 
};


typedef Vector<2> Vec2;
typedef Vector<3> Vec3;
typedef Vector<4> Vec4;

 template <int n>  Vector<n> operator+(Vector<n> a, Vector<n> b)
{
    Vector<n> answer = a;
    for(int i = 0; i < n; i++) 
    {
        answer.data[i] = a.data[i] + b.data[i];
    }

    return answer;
};

#endif