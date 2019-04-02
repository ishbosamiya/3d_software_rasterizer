#ifndef GRADIENTS_H
#define GRADIENTS_H

#include "vector4f.h"
#include "vertex.h"

class Gradients
{
    Vector4f *m_color;
    Vector4f m_color_x_step;
    Vector4f m_color_y_step;

    public:
        Gradients(Vertex minYVert, Vertex midYVert, Vertex maxYVert);
        Vector4f getColor(int loc) {return m_color[loc];}
        Vector4f getColorXStep() {return m_color_x_step;}
        Vector4f getColorYStep() {return m_color_y_step;}
        ~Gradients();

    protected:
};

#endif // GRADIENTS_H
