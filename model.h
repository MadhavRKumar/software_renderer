#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "geometry.h"
#include "tgaimage.h"
class Model
{
  private:
    std::vector<Vec3f> vertices;
    std::vector<std::vector<Vec3i> > faces;
    std::vector<Vec2f> uvs;
    std::vector<Vec3f> normals;
    TGAImage diffuseMap;
    void loadTexture(std::string filename, std::string suffix, TGAImage &texture);

  public:
    Model(const char *filename);
    ~Model();
    int nverts();
    int nfaces();
    Vec3f vertex(int i);
    std::vector<Vec3i> face(int idx);
    Vec2f uv(int i, int vert);
    Vec3f normal(int i, int vert);
    TGAColor diffuse(Vec2f uv);
};

#endif