#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include "model.h"

Model::Model(const char *filename) : vertices(), faces(), uvs(), normals()
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
        else if (!line.compare(0, 2, "vt"))
        {
            stream >> trash >> trash;
            Vec2f vecText;
            for (int i = 0; i < 2; i++)
            {
                stream >> vecText[i];
            }

            uvs.push_back(vecText);
        }
        else if (!line.compare(0, 2, "vn"))
        {
            stream >> trash >> trash;
            Vec3f vertNorm;
            for(int i = 0; i < 3; i++)
            {
                stream >> vertNorm[i];
            }
            normalize(vertNorm);
            normals.push_back(vertNorm);

        }
        else if (!line.compare(0, 2, "f "))
        {
            stream >> trash;
            int vt;
            int vn;
            int index;
            std::vector<Vec3i> face;
            //                 v        /      vt      /      vn
            while (stream >> index >> trash >> vt >> trash >> vn)
            {
                vt--;
                index--;
                vn--;
                Vec3i vec(index, vt, vn);
                face.push_back(vec);
            }

            faces.push_back(face);
        }
    }

    loadTexture(filename, "_diffuse.tga", diffuseMap);

    std::cout << "faces: " << nfaces() << std::endl;
    std::cout << "vertices: " << nverts() << std::endl;
    std::cout << "uvs: " << uvs.size() << std::endl;
    std::cout << "normals: " << normals.size() << std::endl;
}

Model::~Model()
{
}

void Model::loadTexture(std::string filename, std::string suffix, TGAImage &texture)
{
    int dot = filename.find_last_of(".");
    std::string texname = (filename.substr(0, dot) + suffix);
    if (!texture.read_tga_file(texname.c_str()))
    {
        std::cerr << texname << " failed to load" << std::endl;
    }
    else
    {
        std::cerr << texname << " loaded successfully" << std::endl;
        texture.flip_vertically();
    }
}

TGAColor Model::diffuse(Vec2f uv)
{
    Vec2i uvi(uv[0] * diffuseMap.get_width(), uv[1] * diffuseMap.get_height());
    return diffuseMap.get(uvi.x, uvi.y);
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

std::vector<Vec3i> Model::face(int idx)
{
    return faces[idx];
}

Vec2f Model::uv(int i, int vert)
{
    return uvs[faces[i][vert][1]];
}

Vec3f Model::normal(int i, int vert)
{
    return normals[faces[i][vert][2]];
}