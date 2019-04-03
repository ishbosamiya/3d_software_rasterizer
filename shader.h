#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <fstream>

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
        Shader() {
            //light_dir.initialize(1, 1, 1);
        }

        virtual Vertex vertexShader(Vertex vertex, int nth_vertex, int width, int height);
        virtual bool fragmentShader(Vector4f bar, Colour &colour);
};

class Gouraud_Shader : public Shader {
    Vector4f varying_intensity;
    Vector4f light_dir;
    public:
        Gouraud_Shader() : Shader() {
            light_dir.initialize(1, 1, 1);
        }

        virtual Vertex vertexShader(Vertex vertex, int nth_vertex, int width, int height);
        virtual bool fragmentShader(Vector4f bar, Colour &colour);
};

#endif // SHADER_H
