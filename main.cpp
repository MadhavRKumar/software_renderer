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
const unsigned int DEPTH = 225;
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);
void triangle(Vec3f *pts, Vec2f *vts, float *zbuffer, TGAImage &image, float *intensity);
Vec3f barycenter(Vec3f A, Vec3f B, Vec3f C, Vec3f P);
Matrix<4, 1> v2m(Vec3f v);
Vec3f m2v(Matrix<4, 1> m);
Matrix<4, 4> viewport(int x, int y, int w, int h);

Model model("african_head.obj");

int main(int argc, char const *argv[])
{
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);
    Vec3f light_dir(0.0f, 0.0f, 1.0f);
    Vec3f camera(0.0f, 0.0f, 3.0f);
    float *zbuffer = new float[WIDTH * HEIGHT];

    Matrix<4, 4> Projection(1.0f);
    Matrix<4, 4> Viewport = viewport(WIDTH / 8, HEIGHT / 8, WIDTH * 3 / 4, HEIGHT * 3 / 4);
    Projection[3][2] = -1.0f / camera.z;
    for (int i = 0; i < WIDTH * HEIGHT; i++)
    {
        zbuffer[i] = -std::numeric_limits<float>::max();
    }
    for (int i = 0; i < model.nfaces(); i++)
    {
        std::vector<Vec3i> face = model.face(i);
        Vec3f screen_coords[3];
        Vec3f world_coords[3];
        Vec2f vert_texts[3];
        float intensities[3];
        for (int j = 0; j < 3; j++)
        {
            Vec3f v0 = model.vertex(face[j][0]);
            Vec2f vt = model.uv(i, j);
            intensities[j] = dot(model.normal(i, j), light_dir);
            world_coords[j] = v0;
            Matrix<4, 1> m = v2m(Vec3f((v0.x + 1.0f) * WIDTH / 2.0, (v0.y + 1.0f) * HEIGHT / 2.0f, v0.z));
            Vec3f v((v0.x + 1.0f) * WIDTH / 2.0, (v0.y + 1.0f) * HEIGHT / 2.0f, v0.z);
            screen_coords[j] = m2v(Viewport * Projection * v2m(v0));
            vert_texts[j] = vt;
        }

        // Vec3f normal = (world_coords[2] - world_coords[0]).cross(world_coords[1] - world_coords[0]);
        // normalize(normal);
        // float intensity = dot(normal, light_dir);
        triangle(screen_coords, vert_texts, zbuffer, image, intensities);
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

void triangle(Vec3f *pts, Vec2f *vts, float *zbuffer, TGAImage &image, float *intensities)
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
                float intensity = 0.0f;
                for (int i = 0; i < 3; i++)
                {
                    u += vts[i][0] * UV[i];
                    v += vts[i][1] * UV[i];
                    z += pts[i][2] * UV[i];

                    intensity += intensities[i] * UV[i];
                }
                TGAColor diff = model.diffuse(Vec2f(u, v));
                TGAColor c(diff.r * intensity, diff.g * intensity, diff.b * intensity, 255);
                if (zbuffer[int(x + y * WIDTH)] < z)
                {
                    zbuffer[int(x + y * WIDTH)] = z;
                    if (intensity > 0)
                    {
                        image.set(x, y, c);
                    }
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

Matrix<4, 1> v2m(Vec3f v)
{
    Matrix<4, 1> m;
    for (int i = 0; i < 3; i++)
    {
        m[i][0] = v[i];
    }
    m[3][0] = 1.0f;

    return m;
}

Vec3f m2v(Matrix<4, 1> m)
{
    Vec3f v;
    for (int i = 0; i < 3; i++)
    {
        v[i] = m[i][0] / m[3][0];
    }

    return v;
}

Matrix<4, 4> viewport(int x, int y, int w, int h)
{
    Matrix<4, 4> m(1.0f);

    m[0][3] = x + w / 2.0f;
    m[1][3] = y + h / 2.0f;
    m[2][3] = DEPTH / 2.f;

    m[0][0] = w / 2.0f;
    m[1][1] = h / 2.0f;
    m[2][2] = DEPTH / 2.0f;
    return m;
}