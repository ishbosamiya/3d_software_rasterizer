#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <list>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>

#include "vector4f.h"
#include "indexedmodel.h"

using namespace std;

class OBJModel
{
    class OBJIndex {
        int m_vertex_index;
        int m_texCoord_index;
        int m_normal_index;

        public:
            int getVertexIndex() { return m_vertex_index;}
            int getTexCoordIndex() { return m_texCoord_index;}
            int getNormalIndex() { return m_normal_index;}

            void setVertexIndex(int val) { m_vertex_index = val;}
            void setTexCoordIndex(int val) { m_texCoord_index = val;}
            void setNormalIndex(int val) { m_normal_index = val;}

            virtual bool operator == (OBJIndex &obj) {
                if(m_vertex_index == obj.m_vertex_index && m_texCoord_index == obj.m_texCoord_index && m_normal_index == obj.m_normal_index) {
                    return true;
                }
                else {
                    return false;
                }
            }

            bool operator < (const OBJIndex &other) const {return m_vertex_index < other.m_vertex_index;}

        protected:
    };

    list<Vector4f> m_positions;
    list<Vector4f> m_texCoords;
    list<Vector4f> m_normals;
    list<OBJIndex> m_indices;
    bool m_has_texCoords;
    bool m_has_normals;

    Vector4f getElement(list<Vector4f> &m_list, int index);
    int getElement(list<int> &m_list, int index);
    OBJIndex getElement(list<OBJIndex> &m_list, int index);
    void setElement(list<Vector4f> &m_list, int index, Vector4f data);
    void setElement(list<int> &m_list, int index, int data);
    void setElement(list<OBJIndex> &m_list, int index, OBJIndex data);

    OBJIndex parseOBJIndex(string token);
    string* removeEmptyStrings(string data[]);

    void addElement(list<Vector4f> &m_list, Vector4f data);
    void addElement(list<OBJIndex> &m_list, OBJIndex data);
    void addElement(list<int> &m_list, int data);

    public:
        OBJModel(string file_name);
        IndexedModel toIndexedModel();
        virtual ~OBJModel();

    protected:
};

#endif // OBJMODEL_H
