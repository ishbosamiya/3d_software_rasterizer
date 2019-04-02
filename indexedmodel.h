#ifndef INDEXEDMODEL_H
#define INDEXEDMODEL_H

#include <list>

#include "face.h"
#include "vector4f.h"

using namespace std;

class IndexedModel
{
    list<Vector4f> m_positions;
    list<Vector4f> m_texCoords;
    list<Vector4f> m_normals;
    list<Vector4f> m_tangents;
    list<Face> m_faces;

    public:
        IndexedModel();

        void calcNormals();
        void calcTangents();

        template <typename T>
        T getElement(list<T> &m_list, int index);
        template <typename T>
        void setElement(list<T> &m_list, int index, T data);

        list<Vector4f>& getPositions() { return m_positions;}
        list<Vector4f>& getTexCoords() { return m_texCoords;}
        list<Vector4f>& getNormals() { return m_normals;}
        list<Vector4f>& getTangents() { return m_tangents;}
        list<Face>& getFaces() { return m_faces;}


        virtual ~IndexedModel();

    protected:
};

#endif // INDEXEDMODEL_H
