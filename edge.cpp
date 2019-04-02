#include "edge.h"

#include <math.h>

//minYVertIndex is the index of minYVert in the gradient color array
Edge::Edge(Gradients gradients, Vertex minYVert, Vertex maxYVert, int minYVertIndex) {
    m_x_start = ceil(minYVert.getX());
    m_x_end = ceil(maxYVert.getX());
    m_y_start = ceil(minYVert.getY());
    m_y_end = ceil(maxYVert.getY());

    float y_dist = maxYVert.getY() - minYVert.getY();
    float x_dist = maxYVert.getX() - minYVert.getX();

    float y_pre_step = m_y_start - minYVert.getY();
    m_x_step = (float)x_dist/(float)y_dist;
    m_x = minYVert.getX() + y_pre_step * m_x_step;
    float x_pre_step = m_x - minYVert.getX();

    m_texCoordX = gradients.getTexCoordX(minYVertIndex) +
                gradients.getTexCoordX_XStep() * x_pre_step +
                gradients.getTexCoordX_YStep() * y_pre_step;
    m_texCoordXStep = gradients.getTexCoordX_YStep() + gradients.getTexCoordX_XStep() * m_x_step;

    m_texCoordY = gradients.getTexCoordY(minYVertIndex) +
                gradients.getTexCoordY_XStep() * x_pre_step +
                gradients.getTexCoordY_YStep() * y_pre_step;
    m_texCoordYStep = gradients.getTexCoordY_YStep() + gradients.getTexCoordY_XStep() * m_x_step;

}

void Edge::step() {
    m_x += m_x_step;
    m_texCoordX += m_texCoordXStep;
    m_texCoordY += m_texCoordYStep;
}

float Edge::getX() {
    return m_x;
}

int Edge::getXStart() {
    return m_x_start;
}

int Edge::getXEnd() {
    return m_x_end;
}

int Edge::getYStart() {
    return m_y_start;
}

int Edge::getYEnd() {
    return m_y_end;
}

float Edge::getTexCoordX() {
    return m_texCoordX;
}

float Edge::getTexCoordY() {
    return m_texCoordY;
}

Edge::~Edge()
{
    //dtor
}
