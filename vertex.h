#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <math.h>
#include "vector4f.h"
#include "matrix4f.h"

class Vertex
{
    Vector4f m_pos;
    Vector4f m_texCoords;
    Vector4f m_normal;

    public:
        Vertex();
        Vertex(float x, float y, float z);
        Vertex(Vector4f pos, Vector4f texCoords, Vector4f normal);
        float get(int index);
        float getX() {return m_pos.getX();}
        float getY() {return m_pos.getY();}
        Vector4f getPosition() {return m_pos;}
        Vector4f getTexCoords() {return m_texCoords;}
        Vector4f getNormal() {return m_normal;}
        float triangleArea(Vertex b, Vertex c);
        Vertex transform(Matrix4f transform_);
        Vertex perspectiveDivide();
        Vertex lerp(Vertex other, float lerp_amount);
        bool isInsideViewFrustum();
        void print() {
            std::cout << "x: " << m_pos.getX() << " y:" << m_pos.getY() << " z:" << m_pos.getZ() << " w:" << m_pos.getW() << std::endl;
        }
        ~Vertex();

    protected:
};

#endif // VERTEX_H
