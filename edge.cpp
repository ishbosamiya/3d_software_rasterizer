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

    m_color = gradients.getColor(minYVertIndex).add(
                gradients.getColorYStep().mul(y_pre_step)).add(
                gradients.getColorXStep().mul(x_pre_step));

    m_color_step = gradients.getColorYStep().add(gradients.getColorXStep().mul(m_x_step));
}

void Edge::step() {
    m_x += m_x_step;
    m_color = m_color.add(m_color_step);
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

Vector4f Edge::getColor() {
    return m_color;
}

Edge::~Edge()
{
    //dtor
}
