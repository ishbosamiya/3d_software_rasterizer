#include "gradients.h"

Gradients::Gradients(Vertex minYVert, Vertex midYVert, Vertex maxYVert)
{
    m_texCoordX = new float[3];
    m_texCoordY = new float[3];

    m_texCoordX[0] = minYVert.getTexCoords().getX();
    m_texCoordX[1] = midYVert.getTexCoords().getX();
    m_texCoordX[2] = maxYVert.getTexCoords().getX();

    m_texCoordY[0] = minYVert.getTexCoords().getY();
    m_texCoordY[1] = midYVert.getTexCoords().getY();
    m_texCoordY[2] = maxYVert.getTexCoords().getY();

    float one_over_dx = 1.0 /
                        (((midYVert.getX() - maxYVert.getX()) *
                        (minYVert.getY() - maxYVert.getY())) -
                        ((minYVert.getX() - maxYVert.getX()) *
                        (midYVert.getY() - maxYVert.getY())));

    float one_over_dy = -one_over_dx;

    m_texCoordX_XStep = (((m_texCoordX[1] - m_texCoordX[2]) *
                         (minYVert.getY() - maxYVert.getY())) -
                         ((m_texCoordX[0] - m_texCoordX[2]) *
                          (midYVert.getY() - maxYVert.getY()))) * one_over_dx;

    m_texCoordX_YStep = (((m_texCoordX[1] - m_texCoordX[2]) *
                         (minYVert.getX() - maxYVert.getX())) -
                         ((m_texCoordX[0] - m_texCoordX[2]) *
                          (midYVert.getX() - maxYVert.getX()))) * one_over_dy;

    m_texCoordY_XStep = (((m_texCoordY[1] - m_texCoordY[2]) *
                         (minYVert.getY() - maxYVert.getY())) -
                         ((m_texCoordY[0] - m_texCoordY[2]) *
                          (midYVert.getY() - maxYVert.getY()))) * one_over_dx;

    m_texCoordY_YStep = (((m_texCoordY[1] - m_texCoordY[2]) *
                         (minYVert.getX() - maxYVert.getX())) -
                         ((m_texCoordY[0] - m_texCoordY[2]) *
                          (midYVert.getX() - maxYVert.getX()))) * one_over_dy;
}

Gradients::~Gradients()
{
    //delete m_texCoordX;
    //delete m_texCoordY;
}
