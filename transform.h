#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <iostream>
#include <math.h>

#include "vector4f.h"
#include "quarternion.h"
#include "matrix4f.h"

using namespace std;

class Transform
{
    Vector4f m_pos;
    Quarternion m_rot;
    Vector4f m_scale;

    public:
        Transform();
        Transform(Vector4f pos);
        Transform(Vector4f pos, Quarternion rot, Vector4f scale);

        Transform setPos(Vector4f pos);
        Transform setScale(Vector4f scale);
        Transform rotate(Quarternion rotation);
        Transform lookAt(Vector4f point, Vector4f up);
        Quarternion getLookAtRotation(Vector4f point, Vector4f up);
        Matrix4f getTransformation();
        Vector4f getTransformedPos();
        Quarternion getTransformedRot();
        Vector4f getPos();
        Quarternion getRot();
        Vector4f getScale();

        virtual ~Transform();

    protected:
};

#endif // TRANSFORM_H
