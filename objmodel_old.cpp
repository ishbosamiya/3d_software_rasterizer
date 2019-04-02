#include "objmodel.h"

OBJModel::OBJModel(string file_name)
{
    m_has_texCoords = false;
    m_has_normals = false;

    ifstream meshReader;
    meshReader.open(file_name);
    if(!meshReader) {
        cerr << "File Could Not Be Opened" << endl;
        return;
    }
    string line;
    int no_of_tokens = 0;
    while(!meshReader.eof()) {
        meshReader >> line;
        no_of_tokens++;
    }
    meshReader.close();
    meshReader.open(file_name);
    string *tokens = new string[no_of_tokens];
    int temp = 0;
    while(!meshReader.eof()) {
        meshReader >> tokens[temp];
        temp++;
    }
    for(int i = 0; i < no_of_tokens; i++) {
        if(tokens[0].compare("v") == 0) {
            stringstream no_s_1(tokens[1]);
            float no_1;
            no_s_1 >> no_1;
            stringstream no_s_2(tokens[2]);
            float no_2;
            no_s_2 >> no_2;
            stringstream no_s_3(tokens[3]);
            float no_3;
            no_s_3 >> no_3;
            addElement(m_positions, Vector4f(no_1, no_2, no_3, 1));
        }
        else if(tokens[0].compare("vt") == 0) {
            stringstream no_s_1(tokens[1]);
            float no_1;
            no_s_1 >> no_1;
            stringstream no_s_2(tokens[2]);
            float no_2;
            no_s_2 >> no_2;
            addElement(m_texCoords, Vector4f(no_1, 1.0- no_2, 0, 0));
        }
        else if(tokens[0].compare("vn") == 0) {
            stringstream no_s_1(tokens[1]);
            float no_1;
            no_s_1 >> no_1;
            stringstream no_s_2(tokens[2]);
            float no_2;
            no_s_2 >> no_2;
            stringstream no_s_3(tokens[3]);
            float no_3;
            no_s_3 >> no_3;
            addElement(m_normals, Vector4f(no_1, no_2, no_3, 1));
        }
        else if(tokens[0].compare("f") == 0) {
            for(int i = 0; i < no_of_tokens - 3; i++) {
                addElement(m_indices, parseOBJIndex(tokens[1]));
                addElement(m_indices, parseOBJIndex(tokens[2 + i]));
                addElement(m_indices, parseOBJIndex(tokens[3 + i]));
            }
        }
    }
    meshReader.close();
}

void OBJModel::addElement(list<Vector4f> &m_list, Vector4f data) {
    list<Vector4f>::iterator it;
    it = m_list.end();
    m_list.insert(it, data);
}

void OBJModel::addElement(list<OBJIndex> &m_list, OBJIndex data) {
    list<OBJIndex>::iterator it;
    it = m_list.end();
    m_list.insert(it, data);
}

void OBJModel::addElement(list<int> &m_list, int data) {
    list<int>::iterator it;
    it = m_list.end();
    m_list.insert(it, data);
}

OBJModel::OBJIndex OBJModel::parseOBJIndex(string token) {
    //spitting the string into substrings with '/' as delimiter
    int no_of_substrings = 1;
    for(int i = 0; i <= token.length(); i++) {
        if(token[i]=='/') {
            no_of_substrings++;
        }
    }
    string *values = new string[no_of_substrings];
    int j = 0;
    int index = 0;
    for(int i = 0; i < token.length()+1; i++) {
        if(token[i] == '/') {
            index = 0;
            j++;
        }
        else {
            values[j][index] = token[i];
        }
    }

    OBJIndex result;
    //for converting the string to integer
    stringstream no_s_0(values[0]);
    int no_0;
    no_s_0 >> no_0;
    result.setVertexIndex(no_0 - 1);

    if(no_of_substrings > 1) {
        if(values[1].compare("") != 0) {
            m_has_texCoords = true;
            stringstream no_s_1(values[1]);
            int no_1;
            no_s_1 >> no_1;
            result.setTexCoordIndex(no_1 - 1);
        }
        if(no_of_substrings > 2) {
            m_has_normals = true;
            stringstream no_s_2(values[2]);
            int no_2;
            no_s_2 >> no_2;
            result.setNormalIndex(no_2 - 1);
        }
    }
    return result;
}

string* OBJModel::removeEmptyStrings(string* data) {
    list<string> result;
    //can be going wrong here because of the size of data part
    for(int i = 0; i < sizeof(data)/sizeof(data[0]); i++) {
        if(data[i].compare("") != 0) {
            list<string>::iterator it;
            it = result.end();
            result.insert(it, data[i]);
        }
    }
    string *res = new string[result.size()];
    for(int i = 0; i < result.size(); i++) {
        list<string>::iterator it;
        advance(it, i);
        res[i] = *it;
    }
    return res;
}

IndexedModel OBJModel::toIndexedModel() {
    IndexedModel result;
    IndexedModel normal_model;
    map<OBJIndex, int> result_index_map;
    map<int, int> normal_index_map;
    map<int, int> index_map;

    for(int i = 0; i < m_indices.size(); i++) {
        OBJIndex current_index = getElement(m_indices, i);

        Vector4f current_position = getElement(m_positions, current_index.getVertexIndex());
        Vector4f current_texCoord;
        Vector4f current_normal;

        if(m_has_texCoords) {
            current_texCoord = getElement(m_texCoords, current_index.getTexCoordIndex());
        }
        else {
            current_texCoord = Vector4f(0, 0, 0, 0);
        }

        if(m_has_normals) {
            current_normal = getElement(m_normals, current_index.getNormalIndex());
        }
        else {
            current_normal = Vector4f(0, 0, 0, 0);
        }

        int model_vertex_index = result_index_map.find(current_index)->second;

        if(model_vertex_index == NULL) {
            model_vertex_index = result.getPositions().size();
            result_index_map.insert(make_pair(current_index, model_vertex_index));

            addElement(result.getPositions(), current_position);
            addElement(result.getTexCoords(), current_texCoord);
            if(m_has_normals) {
                addElement(result.getNormals(), current_normal);
            }
        }

        int normal_model_index = normal_index_map.find(current_index.getVertexIndex())->first;

        if(normal_model_index == NULL) {
            normal_model_index = normal_model.getPositions().size();
            normal_index_map.insert(make_pair(current_index.getVertexIndex(), normal_model_index));

            addElement(normal_model.getPositions(), current_position);
            addElement(normal_model.getTexCoords(), current_texCoord);
            addElement(normal_model.getNormals(), current_normal);
            addElement(normal_model.getTangents(), Vector4f(0, 0, 0, 0));
        }

        addElement(result.getIndices(), model_vertex_index);
        addElement(normal_model.getIndices(), normal_model_index);
        index_map.insert(make_pair(model_vertex_index, normal_model_index));
    }

    if(!m_has_normals) {
        normal_model.calcNormals();
        for(int i = 0; i < result.getPositions().size(); i++) {
            addElement(result.getNormals(), getElement(normal_model.getNormals(), index_map.find(i)->first));
        }
    }

    normal_model.calcTangents();
    for(int i = 0; i < result.getPositions().size(); i++) {
        addElement(result.getTangents(), getElement(normal_model.getTangents(), index_map.find(i)->first));
    }
    return result;
}

Vector4f OBJModel::getElement(list<Vector4f> &m_list, int index) {
    list<Vector4f>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

OBJModel::OBJIndex OBJModel::getElement(list<OBJIndex> &m_list, int index) {
    list<OBJIndex>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

int OBJModel::getElement(list<int> &m_list, int index) {
    list<int>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

void OBJModel::setElement(list<Vector4f> &m_list, int index, Vector4f data) {
    list<Vector4f>::iterator it;
    it = m_list.begin();
    advance(it, index);
    *it = data;
}

void OBJModel::setElement(list<int> &m_list, int index, int data) {
    list<int>::iterator it;
    it = m_list.begin();
    advance(it, index);
    *it = data;
}

void OBJModel::setElement(list<OBJIndex> &m_list, int index, OBJIndex data) {
    list<OBJIndex>::iterator it;
    it = m_list.begin();
    advance(it, index);
    *it = data;
}

OBJModel::~OBJModel()
{
    //dtor
}
