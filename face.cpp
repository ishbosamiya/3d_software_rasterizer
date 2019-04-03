#include "face.h"

Face::Face()
{
    //ctor
}

void Face::writeFaceToFile(ofstream &fout) {
    if(!fout.is_open()) {
        cout << "Not working" << endl;
    }

    int no_of_positions = m_positions.size();
    int no_of_texCoords = m_texCoords.size();
    int no_of_normals = m_normals.size();
    fout.write((char *)&no_of_positions, sizeof(no_of_positions));
    fout.write((char *)&no_of_texCoords, sizeof(no_of_texCoords));
    fout.write((char *)&no_of_normals, sizeof(no_of_normals));
    for(int i = 0; i < no_of_positions; i++) {
        int data = m_positions[i];
        fout.write((char *)&data, sizeof(data));
    }
    for(int i = 0; i < no_of_texCoords; i++) {
        int data = m_texCoords[i];
        fout.write((char *)&data, sizeof(data));
    }
    for(int i = 0; i < no_of_normals; i++) {
        int data = m_normals[i];
        fout.write((char *)&data, sizeof(data));
    }
}

void Face::readFaceFromFile(ifstream &fin) {
    if(!fin.is_open()) {
        cout << "Not working" << endl;
    }
    int no_of_positions;
    int no_of_texCoords;
    int no_of_normals;
    fin.read((char *)&no_of_positions, sizeof(no_of_positions));
    fin.read((char *)&no_of_texCoords, sizeof(no_of_texCoords));
    fin.read((char *)&no_of_normals, sizeof(no_of_normals));
    m_positions.reserve(no_of_positions);
    m_texCoords.reserve(no_of_texCoords);
    m_normals.reserve(no_of_normals);
    for(int i = 0; i < no_of_positions; i++) {
        int data;
        fin.read((char *)&data, sizeof(data));
        m_positions.push_back(data);
    }
    for(int i = 0; i < no_of_texCoords; i++) {
        int data;
        fin.read((char *)&data, sizeof(data));
        m_texCoords.push_back(data);
    }
    for(int i = 0; i < no_of_normals; i++) {
        int data;
        fin.read((char *)&data, sizeof(data));
        m_normals.push_back(data);
    }
}

void Face::printNormalIndices() {
    for(int i = 0; i < m_normals.size(); i++) {
        cout << m_normals[i] << " ";
    }
}

Face::~Face()
{
    m_positions.clear();
    m_texCoords.clear();
    m_normals.clear();
}
