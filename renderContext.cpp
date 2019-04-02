#include "renderContext.h"

#include <iostream>
#include <math.h>
using namespace std;

RenderContext::RenderContext() {
    //ctor
}

RenderContext::RenderContext(unsigned int width, unsigned int height, unsigned int channels): Bitmap(width, height, channels)
{
}

//same as the overloaded constructor
void RenderContext::initialize(unsigned int width, unsigned int height, unsigned int channels) {
    Bitmap::initialize(width, height, channels);
}

//creating a global triangle generation function that works with any ordering of the vertices
void RenderContext::fillTriangle(Vertex v1, Vertex v2, Vertex v3) {
    //randomly assigning values to the min, mid and max vertices
    //will later be sorted for the correct order
    //adding the perspective transforms to the triangle vertices
    Matrix4f screen_space_transform;
    screen_space_transform.initScreenSpaceTransform(getWidth()/2.0, getHeight()/2.0);
    Vertex minYVert = v1.transform(screen_space_transform).perspectiveDivide();
    Vertex midYVert = v2.transform(screen_space_transform).perspectiveDivide();
    Vertex maxYVert = v3.transform(screen_space_transform).perspectiveDivide();

    //Sorting the vertices
    if(maxYVert.getY() < midYVert.getY()) {
        Vertex temp = maxYVert;
        maxYVert = midYVert;
        midYVert = temp;
    }
    if(midYVert.getY() < minYVert.getY()) {
        Vertex temp = midYVert;
        midYVert = minYVert;
        minYVert = temp;
    }
    if(maxYVert.getY() < midYVert.getY()) {
        Vertex temp = maxYVert;
        maxYVert = midYVert;
        midYVert = temp;
    }

    //to figure out the side at which the triangle should be rendered, the cross product of the vectors is done
    float area = minYVert.triangleArea(maxYVert, midYVert);
    int handedness;
    if(area >= 0) {
        handedness = true;
    }
    else {
        handedness = false;
    }

    //rendering the triangle without using memory buffers
    scanTriangle(minYVert, midYVert, maxYVert, handedness);
}

void RenderContext::scanTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert, bool handedness) {
    Gradients gradients(minYVert, midYVert, maxYVert);

    Edge top_to_bottom(gradients, minYVert, maxYVert, 0);
    Edge top_to_middle(gradients, minYVert, midYVert, 0);
    Edge middle_to_bottom(gradients, midYVert, maxYVert, 1);

    scanEdges(gradients, top_to_bottom, top_to_middle, handedness);
    scanEdges(gradients, top_to_bottom, middle_to_bottom, handedness);
}

//getting information for (horizontal) lines from edge to edge
void RenderContext::scanEdges(Gradients gradients, Edge &a, Edge &b, bool handedness) {
    //making sure that the left and right edges are on their correct sides
    Edge left = a;
    Edge right = b;
    if(handedness) {
        Edge temp = left;
        left = right;
        right = temp;
    }

    int y_start = b.getYStart();
    int y_end = b.getYEnd();

    int a_y_start = a.getYStart();

    //very very important
    //moving the m_x counter so that it aligns with the starting vertex of the other edge
    if(!handedness) {
        for(int i = a_y_start; i < y_start; i++) {
            left.step();
        }
    }
    else {
        for(int i = y_start; i > a_y_start; i--) {
            right.step();
        }
    }

    //sending data to render the horizontal line
    for(int i = y_start; i < y_end; i++) {
        drawScanLine(gradients, left, right, i);
        left.step();
        right.step();
    }
}

void RenderContext::drawScanLine(Gradients gradients, Edge &left, Edge &right, int j) {
    //basic sweep render of all the pixels between the starting x coordinate and the ending x coordinate
    int x_min = ceil(left.getX());
    int x_max = ceil(right.getX());

    //for correct colors based on the position, otherwise there is a difference between the actual x and the ceil(x) which causes the colors to be slightly off
    float x_pre_step = x_min - left.getX();

    Vector4f min_color = left.getColor().add(gradients.getColorXStep().mul(x_pre_step));
    Vector4f max_color = right.getColor().add(gradients.getColorXStep().mul(x_pre_step));


    float lerp_amt = 0.0;
    float lerp_step = 1.0 / (float)(x_max - x_min);

    for(int i = x_min; i < x_max; i++) {
        Vector4f color = min_color.lerp(max_color, lerp_amt);

        char r = (char)(color.getX() * 255.0 + 0.5);
        char g = (char)(color.getY() * 255.0 + 0.5);
        char b = (char)(color.getZ() * 255.0 + 0.5);

        drawPixel(i, j, r, g, b);
        lerp_amt += lerp_step;
    }
}

void RenderContext::fillWireframe(Vertex v1, Vertex v2, Vertex v3, char r, char g, char b, int thickness) {
    //performing the necessary transformations on the vertices to convert them from ratio space to the screen space
    Matrix4f screen_space_transform;
    screen_space_transform.initScreenSpaceTransform(getWidth()/2.0, getHeight()/2.0);
    Vertex minYVert = v1.transform(screen_space_transform).perspectiveDivide();
    Vertex midYVert = v2.transform(screen_space_transform).perspectiveDivide();
    Vertex maxYVert = v3.transform(screen_space_transform).perspectiveDivide();

    //Sorting the vertices
    if(maxYVert.getY() < midYVert.getY()) {
        Vertex temp = maxYVert;
        maxYVert = midYVert;
        midYVert = temp;
    }
    if(midYVert.getY() < minYVert.getY()) {
        Vertex temp = midYVert;
        midYVert = minYVert;
        minYVert = temp;
    }
    if(maxYVert.getY() < midYVert.getY()) {
        Vertex temp = maxYVert;
        maxYVert = midYVert;
        midYVert = temp;
    }

    Gradients gradients(minYVert, midYVert, maxYVert);

    //creating all the edges
    Edge top_to_bottom(gradients, minYVert, maxYVert, 0);
    Edge top_to_middle(gradients, minYVert, midYVert, 0);
    Edge middle_to_bottom(gradients, midYVert, maxYVert, 1);

    //drawing all the edges
    drawWire(top_to_bottom, thickness, r, g, b);
    drawWire(top_to_middle, thickness, r, g, b);
    drawWire(middle_to_bottom, thickness, r, g, b);
}

void RenderContext::drawWire(Edge edge, int thickness, char r, char g, char b) {
//    for getting a certain thickness to each of the pixel points of the line
//    for(int j = 0; j < thickness; j++) {
//        for(int k = 0; k < thickness; k++) {
//            drawPixel(x + j, y_start + k, r, g, b);
//        }
//    }

    //prev_x as a buffer to be able to complete the line
    int y_start = edge.getYStart();
    int y_end = edge.getYEnd();
    int prev_x = ceil(edge.getXStart());

    //when the line is horizontal, it needs a special case due to the way edge step() works
    if(y_start == y_end) {
        int x_start = edge.getXStart();
        int x_end = edge.getXEnd();
        if(x_end < x_start) {
            int temp = x_start;
            x_start = x_end;
            x_end = temp;
        }
        for(int x = x_start; x <= x_end; x++) {
            for(int j = 0; j < thickness; j++) {
                for(int k = 0; k < thickness; k++) {
                    drawPixel(x + j, y_start + k, r, g, b);
                }
            }
        }
        return;
    }

    //other cases
    for(int i = y_start; i < y_end; i++) {
        for(int j = 0; j < thickness; j++) {
            for(int k = 0; k < thickness; k++) {
                //to draw the line from the previous x location to the current x location so that no parts of the lines are missed out
                //the step() gives x coordinates that are not necessarily next to each other, hence the buffer is needed
                int curr_x = ceil(edge.getX());
                if(curr_x > prev_x) {
                    for(int x = prev_x; x <= curr_x; x++) {
                        drawPixel(x + j, i + k, r, g, b);
                    }
                }
                else {
                    for(int x = curr_x; x <= prev_x; x++) {
                        drawPixel(x + j, i + k, r, g, b);
                    }
                }
                //drawPixel(curr_x + j, i + k, r, g, b);
            }
        }
        prev_x = ceil(edge.getX());
        edge.step();
    }


    //to make the ends meet properly
    int x_start = prev_x;
    int x_end = edge.getXEnd();
    if(x_end < x_start) {
        int temp = x_start;
        x_start = x_end;
        x_end = temp;
    }
    for(int x = x_start; x <= x_end; x++) {
        for(int j = 0; j < thickness; j++) {
            for(int k = 0; k < thickness; k++) {
                drawPixel(x + j, y_end + k, r, g, b);
            }
        }
    }
}

//while needing to render the triangles with wireframe around them
void RenderContext::fillTriangle(Vertex v1, Vertex v2, Vertex v3, bool wireframe) {
    fillTriangle(v1, v2, v3);
    fillWireframe(v1, v2, v3, 100, 100, 100, 2);
}

RenderContext::~RenderContext()
{
    //dtor
}
