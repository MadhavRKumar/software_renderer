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
void triangle(Vec3f *pts, Vec2f *vts, float *zbuffer, TGAImage &image, float intensity);
Vec3f barycenter(Vec3f A, Vec3f B, Vec3f C, Vec3f P);

Model model("african_head.obj");

int main(int argc, char const *argv[])
{
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    Vec3f light_dir(0.0f, 0.0f, -1.0f);
    float *zbuffer = new float[WIDTH * HEIGHT];
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }
    for (int i = 0; i < model.nfaces(); i++)
    {
        std::vector<Vec2i> face = model.face(i);
        Vec3f screen_coords[3];
        Vec3f world_coords[3];
        Vec2f vert_texts[3];
        for (int j = 0; j < 3; j++)
        {
            Vec3f v0 = model.vertex(face[j][0]);
            Vec2f vt = model.uv(i, j);
     
            world_coords[j] = v0;
            screen_coords[j] = Vec3f((v0.x + 1.0f) * WIDTH / 2.0, (v0.y + 1.0f) * HEIGHT / 2.0f, v0.z);
            vert_texts[j] = vt;
        }

        Vec3f normal = (world_coords[2] - world_coords[0]).cross(world_coords[1] - world_coords[0]);
        normalize(normal);
        float intensity = dot(normal, light_dir);
        if (intensity >= 0)
        {
            triangle(screen_coords, vert_texts, zbuffer, image, intensity);
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

void triangle(Vec3f *pts, Vec2f *vts, float *zbuffer, TGAImage &image, float intensity)
{
    float maxX = -std::numeric_limits<float>::max();
    float maxY = -std::numeric_limits<float>::max();
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    for (int i = 0; i < 3; i++)
    {
        maxX = std::min(image.get_width() - 1.0f, std::max(maxX, pts[i].x));
        maxY = std::min(image.get_height() - 1.0f, std::max(maxY, pts[i].y));

        minX = std::max(0.0f, std::min(minX, pts[i].x));
        minY = std::max(0.0f, std::min(minY, pts[i].y));
    }
    for (int x = minX; x <= maxX; x++)
    {
        for (int y = minY; y <= maxY; y++)
        {
            Vec3f p(x, y);
            Vec3f UV = barycenter(pts[0], pts[1], pts[2], p);
            if (UV.x >= 0 && UV.y >= 0 && UV.z >= 0)
            {   
                float u, v, z;
                u = v = z = 0.0f;
                for (int i = 0; i < 3; i++)
                {
                    u += vts[i][0] * UV[i];
                    v += vts[i][1] * UV[i];
                    z += pts[i][2] * UV[i];
                }
                TGAColor diff = model.diffuse(Vec2f(u, v));
                TGAColor c(diff.r * intensity, diff.g * intensity, diff.b *intensity, 255);
                if (zbuffer[int(x + y * WIDTH)] < z)
                {
                    zbuffer[int(x + y * WIDTH)] = z;
                    image.set(x, y, c);
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