#include "tgaimage.cpp"
#include "geometry.h"
#include "model.h"
#include <algorithm>
#include <cstdlib>
#include <limits>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 800;

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void triangle(Vec3f t0, Vec3f t1, Vec3f t2, float *zbuffer, TGAImage &image, TGAColor color);
Vec3f barycenter(Vec3f A, Vec3f B, Vec3f C, Vec3f P);

Model model("face.obj");

int main(int argc, char const *argv[])
{
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    Vec3f light_dir(0.0f, 0.0f, -1.0f);
    float *zbuffer = new float[WIDTH*HEIGHT];
    for(int i = 0; i < WIDTH*HEIGHT; i++) {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }
    for (int i = 0; i < model.nfaces(); i++)
    {
        std::vector<Vec2i> face = model.face(i);
        Vec3f screen_coords[3];
        Vec3f world_coords[3];
        for (int j = 0; j < 3; j++)
        {
            Vec3f v0 = model.vertex(face[j][0]);
            world_coords[j] = v0;
            screen_coords[j] = Vec3f((v0.x + 1.0f) * WIDTH / 2.0, (v0.y + 1.0f) * HEIGHT / 2.0f, v0.z);
        }

        Vec3f normal = (world_coords[2] - world_coords[0]).cross(world_coords[1] - world_coords[0]);
        normalize(normal);
        float intensity = dot(normal, light_dir);
        if (intensity >= 0)
        {
            triangle(screen_coords[0], screen_coords[1], screen_coords[2], zbuffer, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
        }
    }

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

void triangle(Vec3f t0, Vec3f t1, Vec3f t2, float *zbuffer, TGAImage &image, TGAColor color)
{
    int maxX = std::min(image.get_width() - 1.0f, std::max(t0.x, std::max(t1.x, t2.x)));
    int maxY = std::min(image.get_height() - 1.0f, std::max(t0.y, std::max(t1.y, t2.y)));

    int minX = std::max(0.0f, std::min(t0.x, std::min(t1.x, t2.x)));
    int minY = std::max(0.0f, std::min(t0.y, std::min(t1.y, t2.y)));

    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            Vec3f p(x, y);
            Vec3f UV = barycenter(t0, t1, t2, p);
            float z = 0;
            if (UV.x >= 0 && UV.y >= 0 && UV.z >= 0)
            {
                Vec3f mul(t0.z, t1.z, t2.z);
                z += (t0.z * UV.x) + (t1.z * UV.y) + (t2.z * UV.z);
                if(zbuffer[int(x+y*WIDTH)] < z) {
                    zbuffer[int(x+y*WIDTH)] = z;
                    image.set(x, y, color);
                }
            }
        }
    }
}

Vec3f barycenter(Vec3f A, Vec3f B, Vec3f C, Vec3f P)
{
    Vec3f AB(B - A);
    Vec3f AC(C - A);
    Vec3f PA(A - P);

    Vec3f X(AB.x, AC.x, PA.x);
    Vec3f Y(AB.y, AC.y, PA.y);

    Vec3f N = X.cross(Y);

    Vec3f UV(1.0f - (N.x + N.y) / N.z, N.x / N.z, N.y / N.z);

    return UV;
}