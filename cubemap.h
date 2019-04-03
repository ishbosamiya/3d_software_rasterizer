#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <string.h>

#include "bitmap.h"
#include "vector4f.h"
#include "colour.h"

using namespace std;

class Cubemap
{
    Bitmap pos_x;
    Bitmap neg_x;
    Bitmap pos_y;
    Bitmap neg_y;
    Bitmap pos_z;
    Bitmap neg_z;

    void xyzToCubeUV(Vector4f &position_xyz, Vector4f &position_uv, int &index);

    public:
        Cubemap(char *folder_path);
        void initialize(char *folder_path);

        Colour textureCube(Vector4f position_xyz);

        virtual ~Cubemap();

    protected:
};

#endif // CUBEMAP_H
