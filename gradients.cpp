#include "gradients.h"

Gradients::Gradients(Vertex minYVert, Vertex midYVert, Vertex maxYVert)
{
    m_color = new Vector4f[3];

    m_color[0] = minYVert.getColor();
    m_color[1] = midYVert.getColor();
    m_color[2] = maxYVert.getColor();

    float one_over_dx = 1.0 /
                        (((midYVert.getX() - maxYVert.getX()) *
                        (minYVert.getY() - maxYVert.getY())) -
                        ((minYVert.getX() - maxYVert.getX()) *
                        (midYVert.getY() - maxYVert.getY())));

    float one_over_dy = -one_over_dx;

    m_color_x_step = (((m_color[1].sub(m_color[2])).mul(
			(minYVert.getY() - maxYVert.getY()))).sub(
			((m_color[0].sub(m_color[2])).mul(
			(midYVert.getY() - maxYVert.getY()))))).mul(one_over_dx);

    m_color_y_step = (((m_color[1].sub(m_color[2])).mul(
			(minYVert.getX() - maxYVert.getX()))).sub(
			((m_color[0].sub(m_color[2])).mul(
			(midYVert.getX() - maxYVert.getX()))))).mul(one_over_dy);
}

Gradients::~Gradients()
{
    //dtor
}
