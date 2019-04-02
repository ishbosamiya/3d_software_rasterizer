#include "renderContext.h"

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
    int y_start = (int)minYVert.getY();
    int y_end = (int)maxYVert.getY();
    int x_start = (int)minYVert.getX();
    int x_end = (int)maxYVert.getX();

    //getting the x axis and y axis distance between the 2 vertices
    int y_dist = y_end - y_start;
    int x_dist = x_end - x_start;

    if(y_dist <= 0) {
        return;
    }

    //having a step to move along the x axis as the y coordinate changes
    float x_step = (float)x_dist/(float)y_dist;
    float cur_x = (float)x_start;

    //adding the starting and ending x coordinate for entire y axis (starting at y_start, ending at y_end)
    for(int i = y_start; i < y_end; i++) {
        m_scan_buffer[i * 2 + whichSide] = (int)cur_x;
        cur_x += x_step;
    }
}

//to convert the given 3 vertices in order into the algorithm ready scan buffer
void RenderContext::scanConvertTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert, int handiness) {
    scanConvertLine(minYVert, maxYVert, 0 + handiness);
    scanConvertLine(minYVert, midYVert, 1 - handiness);
    scanConvertLine(midYVert, maxYVert, 1 - handiness);
}

//creating a global triangle generation function that works with any ordering of the verticies
void RenderContext::fillTriangle(Vertex v1, Vertex v2, Vertex v3) {
    //randomly assigning values to the min, mid and max vertices
    //will later be sorted for the correct order
    Vertex minYVert = v1;
    Vertex midYVert = v2;
    Vertex maxYVert = v3;

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
    int handiness = area >= 0 ? 1 : 0;

    //Creating the triangle
    scanConvertTriangle(minYVert, midYVert, maxYVert, 0);
    fillShape((int)minYVert.getY(), (int)maxYVert.getY());
}

RenderContext::~RenderContext()
{
    //dtor
}
