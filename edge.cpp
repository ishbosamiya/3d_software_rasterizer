#include "edge.h"

#include <math.h>

Edge::Edge(Vertex minYVert, Vertex maxYVert)
{
    m_x_start = ceil(minYVert.getX());
    m_x_end = ceil(maxYVert.getX());
    m_y_start = ceil(minYVert.getY());
    m_y_end = ceil(maxYVert.getY());

    float y_dist = maxYVert.getY() - minYVert.getY();
    float x_dist = maxYVert.getX() - minYVert.getX();

    float pre_step = m_y_start - minYVert.getY();
    m_x_step = (float)x_dist/(float)y_dist;
    m_x = minYVert.getX() + pre_step * m_x_step;
}

void Edge::step() {
    m_x += m_x_step;
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

Edge::~Edge()
{
    //dtor
}
