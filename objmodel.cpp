#include "objmodel.h"

OBJModel::OBJModel(char *file_name)
{
    initialize(file_name);
}

void OBJModel::initialize(char *file_name) {
    m_has_normals = false;
    m_has_texCoords = false;
    no_of_triangles = 0;
    cout << file_name << " getting created" << endl;
    ifstream fin;
    fin.open(file_name);
    if(!fin) {
        cout << "OBJ file did not open!!!" << endl;
        has_loaded = false;
        return;
    }
    char type[3];
    while(!fin.eof()) {
        fin >> type;
        //cout << type << endl;
        if(type[0] == 'v' && type[1] == '\0') {
            //cout << "v seen" << endl;
            float position1;
            float position2;
            float position3;
            fin >> position1 >> position2 >> position3;
            //cout << position1 << position2 << position3 << endl;
            addElement(m_positions, Vector4f(position1, position2, position3, 1));
        }
        else if(type[0] == 'v' && type[1] == 't') {
            //cout << "vt seen" << endl;
            float position1;
            float position2;
            fin >> position1 >> position2;
            addElement(m_texCoords, Vector4f(position1, 1.0 - position2, 0, 0));
            m_has_texCoords = true;
        }
        else if(type[0] == 'v' && type[1] == 'n') {
            //cout << "vn seen" << endl;
            float position1;
            float position2;
            float position3;
            fin >> position1 >> position2 >> position3;
            addElement(m_normals, Vector4f(position1, position2, position3, 1));
            m_has_normals = true;
        }
        else if(type[0] == 'f' && type[1] == '\0') {
            //cout << "f seen" << endl;
            int no_components = 1;
            if(m_has_normals == true) {
                no_components++;
            }
            if(m_has_texCoords == true) {
                no_components++;
            }

            //to get face
            int no_of_vertices = 0;
            int start = fin.tellg();
            if(start == -1) {
                break;
            }
            {
                char temp[500];
                fin.get(temp, 500);
                if(temp[0] == '\0') {
                    break;
                }
                for(int i = 0; temp[i]!='\0'; i++) {
                    if(temp[i] == ' ') {
                        no_of_vertices++;
                    }
                }
                fin.seekg(start);
            }
            Face face;
            for(int i = 0; i < no_of_vertices; i++) {
                char info[100];
                fin >> info;
                char **result = getSplit(info, '/');
                for(int i = 0; i < 3; i++) {
                    int temp = 0;
                    for(int j = 0; result[i][j] != '\0'; j++) {
                        temp *= 10;
                        temp += (int)result[i][j] - (int)'0';
                    }
                    //cout << "temp: " << temp << endl;
                    if(i == 0) {
                        face.addPosition(temp - 1);
                    }
                    else if(i == 1) {
                        face.addTexCoord(temp - 1);
                    }
                    else if(i == 2) {
                        face.addNormal(temp - 1);
                    }
                }
            }
            addElement(m_faces, face);

            no_of_triangles += no_of_vertices - 2;
        }
        else {
            char temp[100];
            fin.get(temp, 100);
            //cout << temp << endl;
        }
    }
    cout << file_name << " created with " << no_of_triangles << " triangles" << endl;
    has_loaded = true;
}

void OBJModel::addIndices(Face &index, char **result, int insertion_loc) {
    for(int i = 0; i < 3; i++) {
        int temp = 0;
        for(int j = 0; result[i][j] != '\0'; j++) {
            temp *= 10;
            temp += (int)result[i][j] - (int)'0';
        }
        //cout << "temp: " << temp << endl;
        if(i == 0) {
            vector<int> position = index.getPositions();
            index.setElement(position, insertion_loc, temp - 1);
        }
        else if(i == 1) {
            vector<int> texCoords = index.getTexCoords();
            index.setElement(texCoords, insertion_loc, temp - 1);
        }
        else if(i == 2) {
            vector<int> normals = index.getNormals();
            index.setElement(normals, insertion_loc, temp - 1);
        }
    }
}

char** OBJModel::getSplit(char *data, char delimiter) {
    int no_of_sub_strings = 1;
    int max_sub_string = 0;
    int curr_length = 0;
    for(int i = 0; data[i] != '\0'; i++) {
        if(data[i] == delimiter) {
            no_of_sub_strings++;
            if(max_sub_string < curr_length) {
                max_sub_string = curr_length;
            }
            curr_length = 0;
        }
        else {
            curr_length++;
        }
    }
    char **result = new char*[no_of_sub_strings];
    for(int i = 0; i < no_of_sub_strings; i++) {
        result[i] = new char[max_sub_string];
    }
    int line_no = 0;
    int repeater = 0;
    for(int i = 0; data[i] != '\0'; i++) {
        if(data[i] == delimiter || data[i] == '\0') {
            result[line_no][repeater] = '\0';
            line_no++;
            repeater = 0;
        }
        else {
            result[line_no][repeater] = data[i];
            repeater++;
        }
    }
    return result;
}

template <typename T>
void OBJModel::addElement(list<T> &m_list, T data) {
    typename list<T>::iterator it;
    it = m_list.end();
    m_list.insert(it, data);
}

template <typename T>
T OBJModel::getElement(list<T> &m_list, int index) {
    typename list<T>::iterator it;
    it = m_list.begin();
    advance(it, index);
    return *it;
}

void OBJModel::printALL() {
    for(int i = 0; i < m_positions.size(); i++) {
        cout << "Position: ";
        getElement(m_positions, i).printALL();
    }
    for(int i = 0; i < m_texCoords.size(); i++) {
        cout << "texCoords: ";
        getElement(m_texCoords, i).printALL();
    }
    for(int i = 0; i < m_normals.size(); i++) {
        cout << "Normals: ";
        getElement(m_normals, i).printALL();
    }
}


IndexedModel OBJModel::toIndexedModel() {
    IndexedModel result;
    for(int i = 0; i < m_positions.size(); i++) {
        addElement(result.getPositions(), getElement(m_positions, i));
    }
    for(int i = 0; i < m_texCoords.size(); i++) {
        addElement(result.getTexCoords(), getElement(m_texCoords, i));
    }
    for(int i = 0; i < m_normals.size(); i++) {
        addElement(result.getNormals(), getElement(m_normals, i));
    }
    for(int i = 0; i < m_faces.size(); i++) {
        addElement(result.getFaces(), getElement(m_faces, i));
    }

    cout << "IndexedModel has been made with " << m_faces.size() << " number of faces. " << endl;
    return result;
}

void OBJModel::toIndexedModel(IndexedModel &result) {
    for(int i = 0; i < m_positions.size(); i++) {
        addElement(result.getPositions(), getElement(m_positions, i));
    }
    for(int i = 0; i < m_texCoords.size(); i++) {
        addElement(result.getTexCoords(), getElement(m_texCoords, i));
    }
    for(int i = 0; i < m_normals.size(); i++) {
        addElement(result.getNormals(), getElement(m_normals, i));
    }
    for(int i = 0; i < m_faces.size(); i++) {
        addElement(result.getFaces(), getElement(m_faces, i));
    }

    cout << "IndexedModel has been made with " << m_faces.size() << " number of faces. " << endl;
}

OBJModel::~OBJModel()
{
    m_faces.clear();
    m_positions.clear();
    m_texCoords.clear();
    m_normals.clear();
}
