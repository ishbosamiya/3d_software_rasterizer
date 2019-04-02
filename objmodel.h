#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <list>
#include <map>
#include <iostream>
#include <fstream>

#include "vector4f.h"
#include "face.h"
#include "indexedmodel.h"
using namespace std;

class OBJModel
{
    bool m_has_normals;
    bool m_has_texCoords;
    int no_of_triangles;
    bool has_loaded;

    list<Vector4f> m_positions;
    list<Vector4f> m_texCoords;
    list<Vector4f> m_normals;
    list<Face> m_faces;

    template <typename T>
    void addElement(list<T> &m_list, T data);
    template <typename T>
    T getElement(list<T> &m_list, int index);

    char** getSplit(char *data, char delimiter);
    void addIndices(Face &index, char **result, int insertion_loc);

    public:
        OBJModel(char *file_name);

        void initialize(char *file_name);

        IndexedModel toIndexedModel();
        void toIndexedModel(IndexedModel &result);
        void printALL();

        bool hasLoaded() { return has_loaded;}

        virtual ~OBJModel();

    protected:
};

#endif // OBJMODEL_H
