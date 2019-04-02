#include "renderContext.h"

using namespace std;

RenderContext::RenderContext() {
    //ctor
}

RenderContext::RenderContext(unsigned int width, unsigned int height, unsigned int channels): Bitmap(width, height, channels) {
    m_z_buffer = new float[width*height];
}

void RenderContext::clearDepthBuffer() {
    for(int i = 0; i < m_width*m_height; i++) {
        m_z_buffer[i] = std::numeric_limits<float>::max();
    }
}

//same as the overloaded constructor
void RenderContext::initialize(unsigned int width, unsigned int height, unsigned int channels) {
    *this = RenderContext(width, height, channels);
}

void RenderContext::drawMesh(Mesh mesh, Matrix4f transform, Bitmap texture, bool wireframe, bool back_face_culling) {
    for(int i = 0; i < mesh.getNumIndices(); i+=3) {
        fillTriangle(mesh.getVertex(mesh.getIndex(i)).transform(transform),
                     mesh.getVertex(mesh.getIndex(i + 1)).transform(transform),
                     mesh.getVertex(mesh.getIndex(i + 2)).transform(transform),
                     texture, wireframe, back_face_culling);
    }
}

//while needing to render the triangles with wireframe around them
void RenderContext::fillTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap texture, bool wireframe, bool back_face_culling) {
    if(wireframe == true) {
        fillWireframe(v1, v2, v3, 255, 170, 64, 1, back_face_culling);
    }
    else {
        fillTriangle(v1, v2, v3, texture, back_face_culling);
    }
}

//creating a global triangle generation function that works with any ordering of the vertices
void RenderContext::fillTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap texture, bool back_face_culling) {
    //randomly assigning values to the min, mid and max vertices
    //will later be sorted for the correct order
    //adding the perspective transforms to the triangle vertices
    Matrix4f screen_space_transform;
    screen_space_transform.initScreenSpaceTransform(getWidth()/2.0, getHeight()/2.0);
    Vertex minYVert = v1.transform(screen_space_transform).perspectiveDivide();
    Vertex midYVert = v2.transform(screen_space_transform).perspectiveDivide();
    Vertex maxYVert = v3.transform(screen_space_transform).perspectiveDivide();

    if(back_face_culling == true) {
        if(minYVert.triangleArea(maxYVert, midYVert) >= 0) {
            return;
        }
    }

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
    scanTriangle(minYVert, midYVert, maxYVert, handedness, texture);
}

void RenderContext::scanTriangle(Vertex minYVert, Vertex midYVert, Vertex maxYVert, bool handedness, Bitmap texture) {
    Gradients gradients(minYVert, midYVert, maxYVert);

    Edge top_to_bottom(gradients, minYVert, maxYVert, 0);
    Edge top_to_middle(gradients, minYVert, midYVert, 0);
    Edge middle_to_bottom(gradients, midYVert, maxYVert, 1);

    scanEdges(gradients, top_to_bottom, top_to_middle, handedness, texture);
    scanEdges(gradients, top_to_bottom, middle_to_bottom, handedness, texture);
}

//getting information for (horizontal) lines from edge to edge
void RenderContext::scanEdges(Gradients gradients, Edge &a, Edge &b, bool handedness, Bitmap texture) {
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
        drawScanLine(gradients, left, right, i, texture);
        left.step();
        right.step();
    }
}

void RenderContext::drawScanLine(Gradients gradients, Edge &left, Edge &right, int j, Bitmap texture) {
    //basic sweep render of all the pixels between the starting x coordinate and the ending x coordinate
    int x_min = ceil(left.getX());
    int x_max = ceil(right.getX());

    //for correct colors based on the position, otherwise there is a difference between the actual x and the ceil(x) which causes the colors to be slightly off
    float x_pre_step = (float)x_min - left.getX();

    float x_dist = right.getX() - left.getX();
    float texCoordX_XStep = (right.getTexCoordX() - left.getTexCoordX())/x_dist;
    float texCoordY_XStep = (right.getTexCoordY() - left.getTexCoordY())/x_dist;
    float one_over_z_XStep = (right.getOneOverZ() - left.getOneOverZ())/x_dist;
    float depth_x_step = (right.getDepth() - left.getDepth())/x_dist;

    //
    float texCoordX = left.getTexCoordX() + texCoordX_XStep * x_pre_step;
    float texCoordY = left.getTexCoordY() + texCoordY_XStep * x_pre_step;
    float one_over_z = left.getOneOverZ() + one_over_z_XStep * x_pre_step;
    float depth = left.getDepth() + depth_x_step * x_pre_step;

    for(int i = x_min; i < x_max; i++) {
        int index = i + j * getWidth();
        if(depth < m_z_buffer[index]) {
            m_z_buffer[index] = depth;
            float z = 1.0/one_over_z;
            int src_X = (int)((texCoordX * z) * (texture.getWidth() - 1) + 0.5f);
            int src_Y = (int)((texCoordY * z) * (texture.getHeight() - 1) + 0.5f);

            copyPixel(i, j, src_X, src_Y, texture);
        }

        one_over_z += one_over_z_XStep;
        texCoordX += texCoordX_XStep;
        texCoordY += texCoordY_XStep;
        depth += depth_x_step;
    }
}

void RenderContext::fillWireframe(Vertex v1, Vertex v2, Vertex v3, char r, char g, char b, int thickness, bool back_face_culling) {
    //performing the necessary transformations on the vertices to convert them from ratio space to the screen space
    Matrix4f screen_space_transform;
    screen_space_transform.initScreenSpaceTransform(getWidth()/2.0, getHeight()/2.0);
    Vertex minYVert = v1.transform(screen_space_transform).perspectiveDivide();
    Vertex midYVert = v2.transform(screen_space_transform).perspectiveDivide();
    Vertex maxYVert = v3.transform(screen_space_transform).perspectiveDivide();

    if(back_face_culling == true) {
        if(minYVert.triangleArea(maxYVert, midYVert) >= 0) {
            return;
        }
    }

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
    //prev_x as a buffer to be able to complete the line
    int y_start = edge.getYStart();
    int y_end = edge.getYEnd();
    int prev_x = ceil(edge.getXStart());

    char m_r, m_g, m_b;

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

void RenderContext::drawZBuffer() {
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
    float temp_max = std::numeric_limits<float>::max();
    for(int i = 0; i < m_width*m_height; i++) {
        if(min > m_z_buffer[i]) {
            min = m_z_buffer[i];
        }
        if(max < m_z_buffer[i] && m_z_buffer[i] != temp_max) {
            max = m_z_buffer[i];
        }
    }
    max += 0.001;
    //std::cout << "Min: " << min << std::endl;
    for(int i = 0; i < m_width; i++) {
        for(int j = 0; j < m_height; j++) {
            float value = m_z_buffer[i + j*m_width];
            float relative = 0;
            if(max != min && value != temp_max) {
                relative = (1.0 - ((value - min)/(max - min)));
            }
            if(value == temp_max) {
                relative = 0;
            }
            char colour = relative * 255;
            drawPixel(i, j, colour, colour, colour);
        }
    }
}

RenderContext RenderContext::getNormalizedZBuffer() {
    RenderContext result(m_width, m_height, m_channels);
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();
    float temp_max = std::numeric_limits<float>::max();
    for(int i = 0; i < m_width*m_height; i++) {
        if(min > m_z_buffer[i]) {
            min = m_z_buffer[i];
        }
        if(max < m_z_buffer[i] && m_z_buffer[i] != temp_max) {
            max = m_z_buffer[i];
        }
    }
    max += 0.001;
    //std::cout << "Min: " << min << std::endl;
    for(int i = 0; i < m_width; i++) {
        for(int j = 0; j < m_height; j++) {
            float value = m_z_buffer[i + j*m_width];
            float relative = 0;
            if(max != min && value != temp_max) {
                relative = (1.0 - ((value - min)/(max - min)));
            }
            if(value == temp_max) {
                relative = 0;
            }
            char colour = relative * 255;
            result.drawPixel(i, j, colour, colour, colour);
        }
    }
    return result;
}

RenderContext RenderContext::getResizedRenderContext(int width, int height) {
    RenderContext image(width, height, 3);
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            int source_x = x * getWidth() / width;
            int source_y = y * getHeight() / height;
            image.copyPixel(x, y, source_x, source_y, *this);
        }
    }
    return image;
}

RenderContext::~RenderContext() {
    //delete m_z_buffer;
}
