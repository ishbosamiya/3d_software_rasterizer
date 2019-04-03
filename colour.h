#ifndef COLOUR_H
#define COLOUR_H

#include <iostream>

using namespace std;

union Colour {
    struct {
        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };
    long int packed;
    void fill(char shade) {
        r = g = b = shade;
        a = 255;
    }
    void fill(char r, char g, char b) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = 255;
    }
    void fill(char r, char g, char b, char a) {
        this->r = r;
        this->g = g;
        this->b = b;
        this->a = a;
    }
    void display() {
        cout << (int)r << " " << (int)g << " " << (int)b << " " << (int)a << endl;
    }
    void multiply(float value) {
        r *= value;
        g *= value;
        b *= value;
    }
};

#endif // COLOUR_H
