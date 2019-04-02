#ifndef EDGE_H
#define EDGE_H

#include "vertex.h"
#include "vector4f.h"
#include "gradients.h"

class Edge
{
    float m_x;
    float m_x_step;
    int m_x_start;
    int m_x_end;
    int m_y_start;
    int m_y_end;
    float m_texCoordX;
    float m_texCoordXStep;
    float m_texCoordY;
    float m_texCoordYStep;

    public:
        Edge(Gradients gradients, Vertex minYVert, Vertex maxYVert, int minYVertIndex);
        void step();
        float getX();
        int getXStart();
        int getXEnd();
        int getYStart();
        int getYEnd();
        float getTexCoordX();
        float getTexCoordY();
        ~Edge();

    protected:
};

#endif // EDGE_H
