#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include "vector4f.h"
#include "matrix4f.h"

class Vertex
{
    Vector4f m_pos;

    public:
        Vertex(float x, float y, float z);
        Vertex(Vector4f pos);
        float getX() {return m_pos.getX();}
        float getY() {return m_pos.getY();}
        float triangleArea(Vertex b, Vertex c);
        Vertex transform(Matrix4f transform_);
        Vertex perspectiveDivide();
        void print() {
            std::cout << "x: " << m_pos.getX() << " y:" << m_pos.getY() << " z:" << m_pos.getZ() << " w:" << m_pos.getW() << std::endl;
        }
        ~Vertex();

    protected:
};

#endif // VERTEX_H
