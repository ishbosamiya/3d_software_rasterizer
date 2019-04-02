#ifndef MATRIX4F_H
#define MATRIX4F_H

#include "vector4f.h"

class Vector4f;
class Matrix4f
{
    float **m;

    public:
        Matrix4f();
        Matrix4f(const Matrix4f &obj);
        void operator=(const Matrix4f &obj);

        void initIdentity();
        void initScreenSpaceTransform(float halfWidth, float halfHeight);
        void initTranslation(float x, float y, float z);
        void initRotation(float x, float y, float z, float angle);
        void initRotation(float x, float y, float z);
        void initScale(float x, float y, float z);
        void initPerspective(float fov, float aspect_ratio, float z_near, float z_far);
        void initOrthographic(float left, float right, float bottom, float top, float near, float far);
        Matrix4f initRotation(Vector4f forward, Vector4f up);
        Matrix4f initRotation(Vector4f forward, Vector4f up, Vector4f right);
        Vector4f transform(Vector4f r);
        Matrix4f mul(Matrix4f r);
        float** getM();
        float get(int x, int y);
        void setM(float **m);
        void set(int x, int y, float value);
        void print();
        ~Matrix4f();

    protected:
};

#endif // MATRIX4F_H
