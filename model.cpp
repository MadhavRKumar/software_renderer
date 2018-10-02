#include <vector>
#include "model.h"

Model::Model(const char *filename)
{
}

Model::~Model()
{
}

int Model::nverts()
{
    return vertices.size;
}

int Model::nfaces()
{
    return faces.size;
}

Vec3 Model::vertex(int i)
{
    return vertices[i];
}

std::vector<int> Model::face(int idx)
{
    return faces[idx];
}