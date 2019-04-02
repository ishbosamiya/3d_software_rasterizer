#include "vertex.h"

Vertex::Vertex(float x, float y)
{
    m_pos = Vector4f(x ,y, 0, 1);
}

Vertex::Vertex(Vector4f pos) {
    m_pos = pos;
}

//Cross product of the vectors formed by current vertex, b and c
float Vertex::triangleArea(Vertex b, Vertex c) {
    float x1 = b.getX() - m_pos.getX();
    float y1 = b.getY() - m_pos.getY();

    float x2 = c.getX() - m_pos.getX();
    float y2 = c.getY() - m_pos.getY();

    return (0.5 * (x1 * y2 - x2 * y1));
}

Vertex Vertex::transform(Matrix4f transform_) {
    return Vertex(transform_.transform(m_pos));
}

Vertex::~Vertex()
{
    //dtor
}
