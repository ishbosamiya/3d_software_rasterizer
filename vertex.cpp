#include "vertex.h"

Vertex::Vertex(float x, float y)
{
    m_x = x;
    m_y = y;
}

//Cross product of the vectors formed by current vertex, b and c
float Vertex::triangleArea(Vertex b, Vertex c) {
    float x1 = b.getX() - m_x;
    float y1 = b.getY() - m_y;

    float x2 = c.getX() - m_x;
    float y2 = c.getY() - m_y;

    return (x1 * y2 - x2 * y1);
}

Vertex::~Vertex()
{
    //dtor
}
