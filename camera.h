#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <math.h>

#include "SDL.h"
#include "vector4f.h"
#include "matrix4f.h"
#include "transform.h"

using namespace std;

class Camera
{
    Vector4f Y_AXIS;

    Transform m_transform;
    Matrix4f m_projection;

    Transform getTransform() { return m_transform;}

    public:
        Camera(Matrix4f projection);

        Matrix4f getViewProjection();
        void update(SDL_Event &event, float delta, int x, int y);
        void move(Vector4f dir, float amt);
        void rotate(Vector4f axis, float angle);

        virtual ~Camera();

    protected:
};

#endif // CAMERA_H
