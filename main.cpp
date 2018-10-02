#include "tgaimage.cpp"
#include "geometry.h"
#include "model.h"
#include <cstdlib>

const TGAColor white = TGAColor(255, 255,255,255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);

Model model("face.obj");

int main(int argc, char const *argv[])
{
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);

    for (int i = 0; i < model.nfaces(); i++)
    {
        std::vector<int> face = model.face(i);

        for(int j = 0; j < 3; j++)
        {
            Vec3 v0 = model.vertex(face[j]);
            Vec3 v1 = model.vertex(face[(j+1)%3]);

            int x0 = (v0.x + 1.0f)*WIDTH/2.0f;
            int y0 = (v0.y + 1.0f)*HEIGHT/2.0f;


            int x1 = (v1.x + 1.0f)*WIDTH/2.0f;
            int y1 = (v1.y + 1.0f)*HEIGHT/2.0f;

            line(x0, y0, x1, y1, image, white);
        }
    }

    image.flip_vertically();
    image.write_tga_file("output.tga");
    


    return 0;
}

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    bool steep = std::abs(x0-x1) < std::abs(y0 - y1);
    if(steep) 
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if(x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    float denom = (x1- x0);
    
    for(float x=x0; x <= x1; x++)
    {
        float t = (x - x0) / denom;
        int y = y0*(1.0f - t) + y1*t;
        if(steep)
        {
            image.set(y, x, color);
        } else {
            image.set(x,y, color);
        }
    }
}