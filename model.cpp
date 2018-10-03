#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include "model.h"

Model::Model(const char *filename)
{
    std::ifstream file;
    file.open(filename);

    if (!file)
    {
        std::cerr << "Unable to open " << filename << std::endl;
        return;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream stream(line);

        char trash;
        if (!line.compare(0, 2, "v "))
        {

            int index = 0;
            float val;
            Vec3f vert;
            stream >> trash;
            while (stream >> val)
            {
                vert[index++] = val;
            }

            vertices.push_back(vert);
        }
        else if (!line.compare(0, 2, "f "))
        {
            stream >> trash;
            int vertTrash;
            int index;
            std::vector<int> face;
            while (stream >> index >> trash >> vertTrash >> trash >> vertTrash)
            {
                index--;
                face.push_back(index);
            }

            faces.push_back(face);
        }
    }

    std::cout << "faces: " << nfaces() << std::endl;
    std::cout << "vertices: " << nverts() << std::endl;
}

Model::~Model()
{
}

int Model::nverts()
{
    return (int)vertices.size();
}

int Model::nfaces()
{
    return (int)faces.size();
}

Vec3f Model::vertex(int i)
{
    return vertices[i];
}

std::vector<int> Model::face(int idx)
{
    return faces[idx];
}