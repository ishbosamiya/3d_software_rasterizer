#ifndef FACE_H
#define FACE_H

#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
//stores only the vertex indices, so acts somewhat like a pointer
class Face
{
    vector<int> m_positions;
    vector<int> m_texCoords;
    vector<int> m_normals;

    public:
        Face();

        template <typename T>
        void setElement(vector<T> &m_list, int index, T data) {
            typename vector<T>::iterator it;
            it = m_list.begin();
            advance(it, index);
            m_list.insert(it, data);
        }

        void writeFaceToFile(ofstream &fout);
        void readFaceFromFile(ifstream &fin);

        void addPosition(int position_index) { m_positions.push_back(position_index);}
        int getPosition(int loc) { return m_positions[loc];}
        vector<int> getPositions() { return m_positions;}

        void addTexCoord(int texCoord_index) { m_texCoords.push_back(texCoord_index);}
        int getTexCoord(int loc) { return m_texCoords[loc];}
        vector<int> getTexCoords() { return m_texCoords;}

        void addNormal(int normal_index) { m_normals.push_back(normal_index);}
        int getNormal(int loc) { return m_normals[loc];}
        vector<int> getNormals() { return m_normals;}

        int getNumOfVerts() { return m_positions.size();}
        virtual ~Face();

    protected:
};

#endif // FACE_H
