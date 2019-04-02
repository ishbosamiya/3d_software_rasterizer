#ifndef VERTEX_H
#define VERTEX_H


class Vertex
{
    float m_x;
    float m_y;

    public:
        Vertex(float x, float y);
        float getX() {return m_x;}
        float getY() {return m_y;}
        float setX(float x) {m_x = x;}
        float setY(float y) {m_y = y;}
        float triangleArea(Vertex b, Vertex c);
        ~Vertex();

    protected:
};

#endif // VERTEX_H
