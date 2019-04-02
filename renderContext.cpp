#include "renderContext.h"

#include <iostream>
#include <math.h>
using namespace std;

RenderContext::RenderContext() {
    //ctor
}

RenderContext::RenderContext(unsigned int width, unsigned int height, unsigned int channels): Bitmap(width, height, channels)
{
    //For the fill concave shape algorithm to work, it needs a start starting x coordinate and the ending x coordinate for all the y coordinates of the image
    m_scan_buffer = new int[height * 2];
}

//same as the overloaded constructor
void RenderContext::initialize(unsigned int width, unsigned int height, unsigned int channels) {
    Bitmap::initialize(width, height, channels);
    m_scan_buffer = new int[height * 2];
}

//to add the starting x coordinate and the ending y coordinate to the respective y coordinate
void RenderContext::drawScanBuffer(int yCoord, int xMin, int xMax) {
    m_scan_buffer[yCoord * 2] = xMin;
    m_scan_buffer[yCoord * 2 + 1] = xMax;
}

//running across the given y coordinate of pixels to draw the shape
void RenderContext::fillShape(int yMin, int yMax) {
    for(int i = yMin; i < yMax; i++) {
        int xMin = m_scan_buffer[i * 2];
        int xMax = m_scan_buffer[i * 2 + 1];

        //moving across the x axis and drawing those particular pixels
        for(int j = xMin; j < xMax; j++) {
            drawPixel(j, i, 255, 255, 255);
        }
    }
}

//converting the the given vertices into a line for the algorithm to use
void RenderContext::scanConvertLine(Vertex minYVert, Vertex maxYVert, int whichSide) {
    //getting the starting and x and y coordinates for the line
    int y_start = (int)ceil(minYVert.getY());
    int y_end = (int)ceil(maxYVert.getY());
    int x_start = (int)ceil(minYVert.getX());
    int x_end = (int)ceil(maxYVert.getX());

    //getting the x axis and y axis distance between the 2 vertices
    float y_dist = maxYVert.getY() - minYVert.getY();
    float x_dist = maxYVert.getX() - minYVert.getX();

    if(y_dist <= 0) {
        return;
    }

    //having a step to move along the x axis as the y coordinate changes
    float x_step = (float)x_dist/(float)y_dist;
    float y_prestep = y_start - minYVert.getY();
    float cur_x = minYVert.getX() + y_prestep * x_step;

    //adding the starting and ending x coordinate for entire y axis (starting at y_start, ending at y_end)
    for(int i = y_start; i < y_end; i++) {
        m_scan_buffer[i * 2 + whichSide] = (int)ceil(cur_x);
        cur_x += x_step;
    }
}

//to convert the given 3 vertices in order into the algorithm ready scan buffer
void RenderContext::scanConvertTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert, int handedness) {
    scanConvertLine(minYVert, maxYVert, 0 + handedness);
    scanConvertLine(minYVert, midYVert, 1 - handedness);
    scanConvertLine(midYVert, maxYVert, 1 - handedness);
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
    //Old Implementation
    //Creating the triangle
    //scanConvertTriangle(minYVert, midYVert, maxYVert, handedness);
    //fillShape((int)ceil(minYVert.getY()), (int)ceil(maxYVert.getY()));
}

void RenderContext::scanTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert, bool handedness) {
    Edge top_to_bottom(minYVert, maxYVert);
    Edge top_to_middle(minYVert, midYVert);
    Edge middle_to_bottom(midYVert, maxYVert);

    scanEdges(top_to_bottom, top_to_middle, handedness);
    scanEdges(top_to_bottom, middle_to_bottom, handedness);
}

//getting information for (horizontal) lines from edge to edge
void RenderContext::scanEdges(Edge &a, Edge &b, bool handedness) {
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
        drawScanLine(left, right, i);
        left.step();
        right.step();
    }
}

void RenderContext::drawScanLine(Edge &left, Edge &right, int j) {
    //basic sweep render of all the pixels between the starting x coordinate and the ending x coordinate
    int x_min = ceil(left.getX());
    int x_max = ceil(right.getX());

    for(int i = x_min; i < x_max; i++) {
        drawPixel(i, j, 255, 255, 255);
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

    //creating all the edges
    Edge top_to_bottom(minYVert, maxYVert);
    Edge top_to_middle(minYVert, midYVert);
    Edge middle_to_bottom(midYVert, maxYVert);

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
