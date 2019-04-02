#include "stars3D.h"
#include <stdlib.h>
#include <iostream>
#include <time.h>
#include <math.h>
using namespace std;

Stars3D::Stars3D(int numStars, float spread, float speed, float fov)
{
    m_spread = spread;
    m_speed = speed;
    m_numStars = numStars;
    m_fov = fov;
    m_tanHalfFov = tan(fov/2.0 * 3.141592 / 180.0);

    m_starX = new float[numStars];
    m_starY = new float[numStars];
    m_starZ = new float[numStars];

    for(int i = 0; i < m_numStars; i++) {
        initStar(i);
    }
    srand(time(NULL));
}

void Stars3D::initStar(int index) {
    //adding a random value to the x y and z co-ordinates of the points
    m_starX[index] = 2.0 * ((float)rand()/RAND_MAX - 0.5) * (float)m_spread;
    m_starY[index] = 2.0 * ((float)rand()/RAND_MAX - 0.5) * (float)m_spread;
    m_starZ[index] = ((float)rand()/RAND_MAX + 0.001f) * m_spread;
}

void Stars3D::updateAndRender(Bitmap &target, float delta) {
    target.clear(0);

    float halfWidth = target.getWidth()/2.0f;
    float halfHeight = target.getHeight()/2.0f;
    for(int i = 0; i < m_numStars; i++) {
        //moving the star's in the negative z direction (towards the user)
        m_starZ[i] -= delta * m_speed;

        //if the star has gone beyond the "camera" then reinitialize that point
        if(m_starZ[i] <= 0) {
            initStar(i);
        }

        //getting the x and y co-ordinates of the points for the camera
        //dividing by tanHalfFov * z coordinate of the star to give it a sense of perspective
        int x = (int)(m_starX[i]/(m_tanHalfFov * m_starZ[i]) * halfWidth + halfWidth);
        int y = (int)(m_starY[i]/(m_tanHalfFov * m_starZ[i]) * halfHeight + halfHeight);

        //if the star has gone beyond the "camera" then reinitialize that point
        if(x < 0 || x >= target.getWidth() || (y < 0 || y >= target.getHeight())) {
            //acout << "x: " << x << " y: " << y << endl;
            initStar(i);
        }
        //otherwise draw that point on the screen (in this case the image)
        else {
            target.drawPixel(x, y, 255, 255, 255);
        }
    }
}

Stars3D::~Stars3D()
{
    //dtor
}
