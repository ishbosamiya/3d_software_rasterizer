#ifndef STARS3D_H
#define STARS3D_H

#include "bitmap.h"

class Stars3D
{
    float m_spread;
    float m_speed;
    float *m_starX;
    float *m_starY;
    float *m_starZ;
    int m_numStars;
    float m_fov;
    float m_tanHalfFov;

    public:
        Stars3D(int numStars, float spread, float speed, float fov = 70);
        void initStar(int index);
        void updateAndRender(Bitmap &target, float delta);
        ~Stars3D();

    protected:
};

#endif // STARS3D_H
