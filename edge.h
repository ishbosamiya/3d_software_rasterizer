#ifndef EDGE_H
#define EDGE_H

#include "vertex.h"

class Edge
{
    float m_x;
    float m_x_step;
    int m_x_start;
    int m_x_end;
    int m_y_start;
    int m_y_end;

    public:
        Edge(Vertex minYVert, Vertex maxYVert);
        void step();
        float getX();
        int getXStart();
        int getXEnd();
        int getYStart();
        int getYEnd();
        ~Edge();

    protected:
};

#endif // EDGE_H
