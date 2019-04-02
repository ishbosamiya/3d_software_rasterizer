#ifndef GRADIENTS_H
#define GRADIENTS_H

#include "vector4f.h"
#include "vertex.h"

class Gradients
{
    float *m_texCoordX;
    float *m_texCoordY;

    float m_texCoordX_XStep;
    float m_texCoordX_YStep;
    float m_texCoordY_XStep;
    float m_texCoordY_YStep;

    public:
        Gradients(Vertex minYVert, Vertex midYVert, Vertex maxYVert);
        float getTexCoordX(int loc) {return m_texCoordX[loc];}
        float getTexCoordY(int loc) {return m_texCoordY[loc];}
        float getTexCoordX_XStep() { return m_texCoordX_XStep;}
        float getTexCoordX_YStep() { return m_texCoordX_YStep;}
        float getTexCoordY_XStep() { return m_texCoordY_XStep;}
        float getTexCoordY_YStep() { return m_texCoordY_YStep;}
        ~Gradients();

    protected:
};

#endif // GRADIENTS_H
