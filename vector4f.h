#ifndef VECTOR4F_H
#define VECTOR4F_H

#include <iostream>
#include <math.h>

#include "quarternion.h"
using namespace std;

class Quarternion;
class Vector4f
{
    float x;
    float y;
    float z;
    float w;

    public:
        Vector4f();
        Vector4f(float x, float y, float z);
        Vector4f(float x, float y, float z, float w);
        void initialize(float x, float y, float z);
        void initialize(float x, float y, float z, float w);

        float length();
        float max();
        float dot(Vector4f r);
        Vector4f cross(Vector4f r);
        Vector4f normalized();
        Vector4f rotate(Vector4f axis, float angle);
        Vector4f rotate(Quarternion rotation);
        Vector4f lerp(Vector4f dest, float lerp_factor);
        Vector4f add(Vector4f r);
        Vector4f add(float r);
        Vector4f sub(Vector4f r);
        Vector4f sub(float r);
        Vector4f mul(Vector4f r);
        Vector4f mul(float r);
        Vector4f div(Vector4f r);
        Vector4f div(float r);
        Vector4f abs();
        float getX();
        float getY();
        float getZ();
        float getW();
        void setX(float value);
        void setY(float value);
        void setZ(float value);
        void setW(float value);
        Vector4f returnThis();
        Vector4f invert();
        bool operator == (Vector4f r);
        ~Vector4f();

        void printALL();

    protected:
};

#endif // VECTOR4F_H
