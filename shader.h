#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <math.h>

#include "vertex.h"
#include "matrix4f.h"
#include "vector4f.h"
#include "colour.h"
#include "bitmap.h"
#include "cubemap.h"

using namespace std;

float saturate(float intensity);
float addAmbientFactor(float intensity, float ambient_factor);
float contrast(float intensity, float factor);

Vector4f reflect(Vector4f incident, Vector4f normal);

class Shader
{
    Vector4f positions[3];
    Vector4f normals[3];
    Vector4f varying_intensity;

    Vector4f interpolation(Vector4f v1, Vector4f v2, Vector4f v3, Vector4f barycentric);

    public:
        static Vector4f light_dir;
        static int shader_type;
        static int no_of_shaders;
        static float ambient_factor;
        static Colour base_colour;
        static Cubemap cubemap;

        virtual Vertex vertexShader(Vertex vertex, int nth_vertex, int width, int height);
        virtual bool fragmentShader(Vector4f screen_coords, Vector4f barycentric_coords, Colour &colour);
};

#endif // SHADER_H
