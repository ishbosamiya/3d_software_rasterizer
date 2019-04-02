#include "vertex.h"

Vertex::Vertex(float x, float y, float z)
{
    m_pos = Vector4f(x ,y, z, 1);
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

    return (float)(x1 * y2 - x2 * y1);
    //return (0.5 * (x1 * y2 - x2 * y1));
}

//to perform a matrix transform to the vertex
Vertex Vertex::transform(Matrix4f transform_) {
    return Vertex(transform_.transform(m_pos));
}

//To add perspective to the vertex
Vertex Vertex::perspectiveDivide() {
    return Vertex(Vector4f(m_pos.getX()/m_pos.getW(), m_pos.getY()/m_pos.getW(), m_pos.getZ()/m_pos.getW(), m_pos.getW()));
}

Vertex::~Vertex()
{
    //dtor
}
