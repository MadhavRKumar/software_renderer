#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "geometry.h"

class Model {
    private:
        std::vector<Vec3f> vertices;
        std::vector<std::vector<int> > faces;

    public:
        Model(const char *filename);
        ~Model();
        int nverts();
        int nfaces();
        Vec3f vertex(int i);
        std::vector<int> face(int idx);
};


#endif