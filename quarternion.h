#ifndef QUARTERNION_H
#define QUARTERNION_H

#include <iostream>
#include <math.h>

#include "matrix4f.h"
#include "vector4f.h"
using namespace std;

class Matrix4f;
class Vector4f;
class Quarternion
{
    float m_x;
    float m_y;
    float m_z;
    float m_w;

    public:
        Quarternion();
        Quarternion(float x, float y, float z, float w);
        Quarternion(Vector4f axis, float angle);
        Quarternion(Matrix4f rot);
        void initialize(float x, float y, float z, float w);

        float length();
        Quarternion normalized();
        Quarternion conjugate();
        Quarternion mul(float r);
        Quarternion mul(Quarternion r);
        Quarternion mul(Vector4f r);
        Quarternion sub(Quarternion r);
        Quarternion add(Quarternion r);
        Matrix4f toRotationMatrix();
        float dot(Quarternion r);
        Quarternion nLerp(Quarternion dest, float lerp_factor, bool shortest);
        Quarternion sLerp(Quarternion dest, float lerp_factor, bool shortest);
        Vector4f getForward();
        Vector4f getBack();
        Vector4f getUp();
        Vector4f getDown();
        Vector4f getLeft();
        Vector4f getRight();
        float getX();
        float getY();
        float getZ();
        float getW();
        bool equals(Quarternion r);

        virtual ~Quarternion();

    protected:
};

#endif // QUARTERNION_H
