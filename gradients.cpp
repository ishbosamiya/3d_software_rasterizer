#include "gradients.h"

Gradients::Gradients(Vertex minYVert, Vertex midYVert, Vertex maxYVert)
{
    m_texCoordX = new float[3];
    m_texCoordY = new float[3];
    m_one_over_z = new float[3];
    m_depth = new float[3];

    m_depth[0] = minYVert.getPosition().getZ();
    m_depth[1] = midYVert.getPosition().getZ();
    m_depth[2] = maxYVert.getPosition().getZ();

    m_one_over_z[0] = 1.0/minYVert.getPosition().getW();
    m_one_over_z[1] = 1.0/midYVert.getPosition().getW();
    m_one_over_z[2] = 1.0/maxYVert.getPosition().getW();

    m_texCoordX[0] = minYVert.getTexCoords().getX() * m_one_over_z[0];
    m_texCoordX[1] = midYVert.getTexCoords().getX() * m_one_over_z[1];
    m_texCoordX[2] = maxYVert.getTexCoords().getX() * m_one_over_z[2];

    m_texCoordY[0] = minYVert.getTexCoords().getY() * m_one_over_z[0];
    m_texCoordY[1] = midYVert.getTexCoords().getY() * m_one_over_z[1];
    m_texCoordY[2] = maxYVert.getTexCoords().getY() * m_one_over_z[2];

    float one_over_dx = 1.0 /
                        (((midYVert.getX() - maxYVert.getX()) *
                        (minYVert.getY() - maxYVert.getY())) -
                        ((minYVert.getX() - maxYVert.getX()) *
                        (midYVert.getY() - maxYVert.getY())));

    float one_over_dy = -one_over_dx;

    m_texCoordX_XStep = calcXStep(m_texCoordX, minYVert, midYVert, maxYVert, one_over_dx);
    m_texCoordX_YStep = calcYStep(m_texCoordX, minYVert, midYVert, maxYVert, one_over_dy);
    m_texCoordY_XStep = calcXStep(m_texCoordY, minYVert, midYVert, maxYVert, one_over_dx);
    m_texCoordY_YStep = calcYStep(m_texCoordY, minYVert, midYVert, maxYVert, one_over_dy);
    m_one_over_z_XStep = calcXStep(m_one_over_z, minYVert, midYVert, maxYVert, one_over_dx);
    m_one_over_z_YStep = calcYStep(m_one_over_z, minYVert, midYVert, maxYVert, one_over_dy);
    m_depth_x_step = calcXStep(m_depth, minYVert, midYVert, maxYVert, one_over_dx);
    m_depth_y_step = calcYStep(m_depth, minYVert, midYVert, maxYVert, one_over_dy);

}

Gradients::Gradients(const Gradients &obj) {
    m_texCoordX = new float[3];
    m_texCoordY = new float[3];
    m_one_over_z = new float[3];
    m_depth = new float[3];
    for(int i = 0; i < 3; i++) {
        m_texCoordX[i] = obj.m_texCoordX[i];
        m_texCoordY[i] = obj.m_texCoordY[i];
        m_one_over_z[i] = obj.m_one_over_z[i];
        m_depth[i] = obj.m_depth[i];
    }

    m_texCoordX_XStep = obj.m_texCoordX_XStep;
    m_texCoordX_YStep = obj.m_texCoordX_YStep;
    m_texCoordY_XStep = obj.m_texCoordY_XStep;
    m_texCoordY_YStep = obj.m_texCoordY_YStep;
    m_one_over_z_XStep = obj.m_one_over_z_XStep;
    m_one_over_z_YStep = obj.m_one_over_z_YStep;
    m_depth_x_step = obj.m_depth_x_step;
    m_depth_y_step = obj.m_depth_y_step;
}

float Gradients::calcXStep(float *values, Vertex minYVert, Vertex midYVert, Vertex maxYVert, float one_over_dx) {
    return (((values[1] - values[2]) *
             (minYVert.getY() - maxYVert.getY())) -
             ((values[0] - values[2]) *
              (midYVert.getY() - maxYVert.getY()))) * one_over_dx;
}

float Gradients::calcYStep(float *values, Vertex minYVert, Vertex midYVert, Vertex maxYVert, float one_over_dy) {
    return (((values[1] - values[2]) *
             (minYVert.getX() - maxYVert.getX())) -
             ((values[0] - values[2]) *
              (midYVert.getX() - maxYVert.getX()))) * one_over_dy;
}

Gradients::~Gradients()
{
    delete [] m_texCoordX;
    delete [] m_texCoordY;
    delete [] m_one_over_z;
    delete [] m_depth;
}
