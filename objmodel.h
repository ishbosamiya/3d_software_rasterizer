#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <list>
#include <map>
#include <iostream>
#include <fstream>

#include "vector4f.h"
#include "indexedmodel.h"
using namespace std;

class OBJModel
{
    class OBJIndex {
        int m_positions_index;
        int m_texCoords_index;
        int m_normals_index;
        public:
            OBJIndex() {
                m_positions_index = 0;
                m_texCoords_index = 0;
                m_normals_index = 0;
            }
            OBJIndex(int positions_index, int texCoords_index, int normals_index) {
                m_positions_index = positions_index;
                m_texCoords_index = texCoords_index;
                m_normals_index = normals_index;
            }

            void printALL() {   cout << "Position Index: " << m_positions_index;
                                cout << " TexCoords Index: " << m_texCoords_index;
                                cout << " Normals Index: " << m_normals_index;
                                cout << endl;}

            setPositionsIndex(int positions_index) { m_positions_index = positions_index;}
            setTexCoordsIndex(int texCoords_index) { m_texCoords_index = texCoords_index;}
            setNormalsIndex(int normals_index) { m_normals_index = normals_index;}

            getPositionsIndex() { return m_positions_index;}
            getTexCoordsIndex() { return m_texCoords_index;}
            getNormalsIndex() { return m_normals_index;}
    };

    bool m_has_normals;
    bool m_has_texCoords;
    int no_of_triangles;
    bool has_loaded;

    list<Vector4f> m_positions;
    list<Vector4f> m_texCoords;
    list<Vector4f> m_normals;
    list<OBJIndex> m_indices;

    void addElement(list<Vector4f> &m_list, Vector4f data);
    Vector4f getElement(list<Vector4f> &m_list, int index);

    void addElement(list<int> &m_list, int data);
    int getElement(list<int> &m_list, int index);

    void addElement(list<OBJIndex> &m_list, OBJIndex data);
    OBJIndex getElement(list<OBJIndex> &m_list, int index);
    void deleteElement(list<OBJIndex> &m_list, int index);
    void deleteElement(list<OBJIndex> &m_list);

    char** getSplit(char *data, char delimiter);
    void addIndices(OBJIndex &index, char **result);

    public:
        OBJModel(char *file_name);

        IndexedModel toIndexedModel();
        void printALL();

        bool hasLoaded() { return has_loaded;}

        virtual ~OBJModel();

    protected:
};

#endif // OBJMODEL_H
