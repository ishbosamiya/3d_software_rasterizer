#include <iostream>
#include "display.h"
#include "bitmap.h"
using namespace std;

int main(int argc, char *argv[]) {
    //creating the display and updating it
    Display display("Software Rendering", 800, 600);
    display.updateWindow();
}
