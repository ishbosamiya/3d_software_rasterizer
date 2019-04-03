#ifndef SHADER_H
#define SHADER_H

#include "vertex.h"
#include "matrix4f.h"
#include "vector4f.h"
#include "colour.h"


class Shader
{
    public:
        virtual Vertex vertexShader() = 0;
        virtual bool fragmentShader() = 0;
        virtual ~Shader();

    protected:
};

class Gouraud_Shader : public Shader {
    Vector4f varying_intensity;
    Vector4f light_dir;
    public:
        Gouraud_Shader() {
            light_dir.initialize(1, 1, 1);
        }

        virtual Vertex vertexShader();
        virtual bool fragmentShader();

        virtual Vertex vertexShader(Vertex vertex, int nth_vertex, int width, int height);
        virtual bool fragmentShader(Vector4f bar, Colour &colour);
};

#endif // SHADER_H
