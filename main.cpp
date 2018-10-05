#include "tgaimage.cpp"
#include "geometry.h"
#include "model.h"
#include <algorithm>
#include <cstdlib>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color);
bool isInTriangle(Vec2i A, Vec2i B, Vec2i C, Vec2i P);

Model model("face.obj");

int main(int argc, char const *argv[])
{
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    triangle(Vec2i(10,10), Vec2i(100, 30), Vec2i(190, 160), image, white);
    // for (int i = 0; i < model.nfaces(); i++)
    // {
    //     std::vector<int> face = model.face(i);

    //     for (int j = 0; j < 3; j++)
    //     {
    //         Vec3f v0 = model.vertex(face[j]);
    //         Vec3f v1 = model.vertex(face[(j + 1) % 3]);

    //         int x0 = (v0.x + 1.0f) * WIDTH / 2.0f;
    //         int y0 = (v0.y + 1.0f) * HEIGHT / 2.0f;

    //         int x1 = (v1.x + 1.0f) * WIDTH / 2.0f;
    //         int y1 = (v1.y + 1.0f) * HEIGHT / 2.0f;

    //         line(x0, y0, x1, y1, image, white);
    //     }
    // }

    image.flip_vertically();
    image.write_tga_file("output.tga");

    return 0;
}

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    bool steep = std::abs(x0 - x1) < std::abs(y0 - y1);
    if (steep)
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    float denom = (x1 - x0);

    for (float x = x0; x <= x1; x++)
    {
        float t = (x - x0) / denom;
        int y = y0 * (1.0f - t) + y1 * t;
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
    }
}

void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color)
{
    int maxX = std::min(image.get_width() - 1, std::max(t0.x, std::max(t1.x, t2.x)));
    int maxY = std::min(image.get_height() - 1, std::max(t0.y, std::max(t1.y, t2.y)));

    int minX = std::max(0, std::min(t0.x, std::min(t1.x, t2.x)));
    int minY = std::max(0, std::min(t0.y, std::min(t1.y, t2.y)));

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            Vec2i p(x, y);
            if (isInTriangle(t0, t1, t2, p))
            {
                image.set(x, y, color);
            }
        }
    }
}

bool isInTriangle(Vec2i A, Vec2i B, Vec2i C, Vec2i P)
{
    Vec3i AB(B - A, 1);
    Vec3i AC(C - A, 1);
    Vec3i PA(A - P, 1);

    Vec3f X(AB.x, AC.x, PA.x);
    Vec3f Y(AB.y, AC.y, PA.y);

    Vec3f N = X.cross(Y);
    //std::cout << N << std::endl;
    Vec3f UV(1.0f - (N.x + N.y)/N.z, N.x/N.z, N.y/N.z);
    // if(UV.x >= 0 && UV.y >= 0 && UV.z >= 0)
    // {
    //     std::cout << UV << std::endl;
    // }
    return(UV.x >= 0 && UV.y >= 0 && UV.z >= 0);
    
}