#include "cubemap.h"

Cubemap::Cubemap(char *folder_path) {
    initialize(folder_path);
}

void Cubemap::initialize(char *folder_path) {
    struct stat info;
    if(stat(folder_path, &info) != 0) {
        cout << "Folder Not Found, Generating Noise!!!" << endl;
        pos_x.generateNoise();
        neg_x.generateNoise();
        pos_y.generateNoise();
        neg_y.generateNoise();
        pos_z.generateNoise();
        neg_z.generateNoise();
        return;
    }

    char c_pos_x[100];
    char c_neg_x[100];
    char c_pos_y[100];
    char c_neg_y[100];
    char c_pos_z[100];
    char c_neg_z[100];

    strcpy(c_pos_x, folder_path);
    strcpy(c_neg_x, folder_path);
    strcpy(c_pos_y, folder_path);
    strcpy(c_neg_y, folder_path);
    strcpy(c_pos_z, folder_path);
    strcpy(c_neg_z, folder_path);

    strcat(c_pos_x, "\\posx.jpg");
    strcat(c_neg_x, "\\negx.jpg");
    strcat(c_pos_y, "\\posy.jpg");
    strcat(c_neg_y, "\\negy.jpg");
    strcat(c_pos_z, "\\posz.jpg");
    strcat(c_neg_z, "\\negz.jpg");

    pos_x.initialize(c_pos_x);
    neg_x.initialize(c_neg_x);
    pos_y.initialize(c_pos_y);
    neg_y.initialize(c_neg_y);
    pos_z.initialize(c_pos_z);
    neg_z.initialize(c_neg_z);
}

void Cubemap::xyzToCubeUV(Vector4f &position_xyz, Vector4f &position_uv, int &index) {
    float x = position_xyz.getX();
    float y = position_xyz.getY();
    float z = position_xyz.getZ();

    float abs_x = fabs(x);
    float abs_y = fabs(y);
    float abs_z = fabs(z);

    bool is_x_positive = x > 0 ? true : false;
    bool is_y_positive = y > 0 ? true : false;
    bool is_z_positive = z > 0 ? true : false;

    float max_axis, uc, vc;
    if(is_x_positive && abs_x >= abs_y && abs_x >= abs_z) {
        max_axis = abs_x;
        uc = -z;
        vc = y;
        index = 0;
    }
    else if(!is_x_positive && abs_x >= abs_y && abs_x >= abs_z) {
        max_axis = abs_x;
        uc = z;
        vc = y;
        index = 1;
    }
    else if(is_y_positive && abs_y >= abs_x && abs_y >= abs_z) {
        max_axis = abs_y;
        uc = x;
        vc = -z;
        index = 2;
    }
    else if(!is_y_positive && abs_y >= abs_x && abs_y >= abs_z) {
        max_axis = abs_x;
        uc = x;
        vc = z;
        index = 3;
    }
    else if(is_z_positive && abs_z >= abs_x && abs_z >= abs_y) {
        max_axis = abs_z;
        uc = x;
        vc = y;
        index = 4;
    }
    else if(!is_z_positive && abs_z >= abs_x && abs_z >= abs_y) {
        max_axis = abs_z;
        uc = -x;
        vc = y;
        index = 5;
    }

    position_uv.setX(0.5f * (uc/max_axis + 1.0f));
    position_uv.setY(1.0f - (0.5f * (vc/max_axis + 1.0f)));
}

Colour Cubemap::textureCube(Vector4f position_xyz) {
    Vector4f position_uv;
    int index;

    xyzToCubeUV(position_xyz, position_uv, index);

    Colour result;
    switch(index) {
        case 0:
            result = pos_x.getPixel(position_uv);
            break;
        case 1:
            result = neg_x.getPixel(position_uv);
            break;
        case 2:
            result = pos_y.getPixel(position_uv);
            break;
        case 3:
            result = neg_y.getPixel(position_uv);
            break;
        case 4:
            result = pos_z.getPixel(position_uv);
            break;
        case 5:
            result = neg_z.getPixel(position_uv);
            break;
        default:
            result.fill(255, 0, 255);
            break;
    }

    return result;
}

Cubemap::~Cubemap() {

}
