#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>
#include <math.h>

#include "vertex.h"
#include "matrix4f.h"
#include "vector4f.h"
#include "colour.h"

using namespace std;

class Shader
{
    Vector4f normals[3];
    Vector4f varying_intensity;
    public:
        static Vector4f light_dir;
        static int shader_type;
        static int no_of_shaders;

        virtual Vertex vertexShader(Vertex vertex, int nth_vertex, int width, int height);
        virtual bool fragmentShader(Vector4f bar, Colour &colour);
};

#endif // SHADER_H
