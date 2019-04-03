#include "renderContext.h"

using namespace std;

RenderContext::RenderContext() {
    //ctorz_buffer.getResizedBitmap(z_buffer_display.getWidth(), z_buffer_display.getHeight())
}

RenderContext::RenderContext(const RenderContext &obj) {
    m_z_buffer = new float[m_width * m_height];
    for(int i = 0; i < m_width * m_height; i++) {
        m_z_buffer[i] = obj.m_z_buffer[i];
    }
}

RenderContext::RenderContext(unsigned int width, unsigned int height) {
    initialize(width, height);
}

void RenderContext::clearDepthBuffer() {
    float max = std::numeric_limits<float>::max();
    fill_n(m_z_buffer, m_width * m_height, max);
}

//same as the overloaded constructor
void RenderContext::initialize(unsigned int width, unsigned int height) {
    Bitmap::initialize(width, height);
    m_z_buffer = new float[width*height];
}

void RenderContext::drawMesh(Mesh &mesh, Matrix4f transform, Bitmap &texture, bool wireframe, bool back_face_culling) {
    for(int i = 0; i < mesh.getNumOfFaces(); i++) {
        vector<Vertex> verts;
        int no_of_verts = mesh.getFace(i).getNumOfVerts();
        verts.reserve(no_of_verts);
        for(int j = 0; j < no_of_verts; j++) {
            Vector4f position = mesh.getPosition(mesh.getFace(i).getPosition(j));
            Vector4f texCoord = mesh.getTexCoord(mesh.getFace(i).getTexCoord(j));
            Vertex temp_vert(position, texCoord);
            verts.push_back(temp_vert.transform(transform));
        }
        //here I can make the wireframe not have the triangle, but for the rasterizer I need to make them into triangles
        for(int j = 0; j < verts.size() - 2; j++) {
            fillTriangle(verts[0], verts[j + 1], verts[j + 2], texture, wireframe, back_face_culling);
        }
        verts.clear();
    }
}

void RenderContext::drawWire(Vertex v1, Vertex v2, int thickness, char r, char g, char b) {
    float x1 = v1.getPosition().getX();
    float x2 = v2.getPosition().getX();
    float y1 = v1.getPosition().getY();
    float y2 = v2.getPosition().getY();

    //making y1 on top
    if(y1 > y2) {
        float temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    float m = (y2 - y1)/(x2 - x1);
    float c = x1 - (y1/m);

    float prev_x = x1;
    for(int y = y1; y <= y2; y++) {
        float x = c + y;
        if(prev_x > x) {
            float temp = prev_x;
            prev_x = x;
            x = temp;
        }
        for(int i = prev_x; i <= x; i++) {
            int m_x = ceil(i);
            if(m_x >= 0 && m_x < getWidth() && y >= 0 && y < getHeight()) {
                Colour temp_colour;
                temp_colour.fill(r, g, b, 255);
                drawPixel(m_x, y, temp_colour);
            }
        }
    }
}

bool RenderContext::clipPolygonAxis(vector<Vertex> &vertices, vector<Vertex> &auxillary_vector, int component_index) {
    clipPolygonComponent(vertices, component_index, 1.0, auxillary_vector);
    vertices.clear();

    if(auxillary_vector.empty()) {
        return false;
    }

    clipPolygonComponent(auxillary_vector, component_index, -1.0, vertices);
    auxillary_vector.clear();

    return !vertices.empty();
}

void RenderContext::clipPolygonComponent(vector<Vertex> &vertices, int component_index, float component_factor, vector<Vertex> &result) {
    Vertex previous_vertex = vertices[vertices.size() -1];
    float previous_component = previous_vertex.get(component_index) * component_factor;
    bool previous_inside = previous_component <= previous_vertex.getPosition().getW();

    result.reserve(vertices.size());

    for(int i = 0; i < vertices.size(); i++) {
        Vertex current_vertex = vertices[i];
        float current_component = current_vertex.get(component_index) * component_factor;
        bool current_inside = current_component <= current_vertex.getPosition().getW();

        if(current_inside ^ previous_inside) {
            float lerp_amount = (previous_vertex.getPosition().getW() - previous_component)/
                                ((previous_vertex.getPosition().getW() - previous_component) -
                                 (current_vertex.getPosition().getW() - current_component));

            result.push_back(previous_vertex.lerp(current_vertex, lerp_amount));
        }

        if(current_inside) {
            result.push_back(current_vertex);
        }

        previous_vertex = current_vertex;
        previous_component = current_component;
        previous_inside = current_inside;
    }
}

void RenderContext::drawTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap &texture, bool back_face_culling) {
    //if all inside then do not clip
    if(v1.isInsideViewFrustum() && v2.isInsideViewFrustum() & v3.isInsideViewFrustum()) {
        fillTriangle(v1, v2, v3, texture, back_face_culling);
        return;
    }
//    if(!v1.isInsideViewFrustum() && !v2.isInsideViewFrustum() && !v3.isInsideViewFrustum()) {
//        return;
//    }

    vector<Vertex> vertices;
    vertices.reserve(6);
    vector<Vertex> auxially_vector;
    auxially_vector.reserve(6);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    if(clipPolygonAxis(vertices, auxially_vector, 0) &&
       clipPolygonAxis(vertices, auxially_vector, 1) &&
       clipPolygonAxis(vertices, auxially_vector, 2)) {
        Vertex initial_vertex = vertices[0];
        for(int i = 1; i < vertices.size() - 1; i++) {
            fillTriangle(initial_vertex, vertices[i], vertices[i + 1], texture, back_face_culling);
        }
    }

    vertices.clear();
    auxially_vector.clear();
}

void RenderContext::drawWireframe(Vertex v1, Vertex v2, Vertex v3, char r, char g, char b, int thickness, bool back_face_culling) {
//    //if all inside then do not clip
//    if(v1.isInsideViewFrustum() && v2.isInsideViewFrustum() && v3.isInsideViewFrustum()) {
//        fillWireframe(v1, v2, v3, r, g, b, thickness, back_face_culling);
//        return;
//    }
////    if(!v1.isInsideViewFrustum() && !v2.isInsideViewFrustum() && !v3.isInsideViewFrustum()) {
////        return;
////    }
//
//    vector<Vertex> vertices;
//    vertices.reserve(6);
//    vector<Vertex> auxially_vector;
//    auxially_vector.reserve(6);
//
//    vertices.push_back(v1);
//    vertices.push_back(v2);
//    vertices.push_back(v3);
//
//    if(clipPolygonAxis(vertices, auxially_vector, 0) &&
//       clipPolygonAxis(vertices, auxially_vector, 1) &&
//       clipPolygonAxis(vertices, auxially_vector, 2)) {
//        Vertex initial_vertex = vertices[0];
//        for(int i = 1; i < vertices.size() - 1; i++) {
//            fillWireframe(initial_vertex, vertices[i], vertices[i + 1], r, g, b, thickness, back_face_culling);
//        }
//    }

    if(v1.isInsideViewFrustum() && v2.isInsideViewFrustum() & v3.isInsideViewFrustum()) {
        fillWireframe(v1, v2, v3, r, g, b, thickness, back_face_culling);
        return;
    }
//    if(!v1.isInsideViewFrustum() && !v2.isInsideViewFrustum() && !v3.isInsideViewFrustum()) {
//        return;
//    }

    vector<Vertex> vertices;
    vertices.reserve(6);
    vector<Vertex> auxially_vector;
    auxially_vector.reserve(6);

    vertices.push_back(v1);
    vertices.push_back(v2);
    vertices.push_back(v3);

    if(clipPolygonAxis(vertices, auxially_vector, 0) &&
       clipPolygonAxis(vertices, auxially_vector, 1) &&
       clipPolygonAxis(vertices, auxially_vector, 2)) {
        Vertex initial_vertex = vertices[0];
        for(int i = 1; i < vertices.size() - 1; i++) {
            fillWireframe(v1, v2, v3, r, g, b, thickness, back_face_culling);
        }
    }

    vertices.clear();
    auxially_vector.clear();
}

//while needing to render the triangles with wireframe around them
void RenderContext::fillTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap &texture, bool wireframe, bool back_face_culling) {
    if(wireframe == true) {
        drawWireframe(v1, v2, v3, 255, 170, 64, 1, back_face_culling);
    }
    else {
        //changed here
        drawTriangle(v1, v2, v3, texture, back_face_culling);
    }
}

//creating a global triangle generation function that works with any ordering of the vertices
void RenderContext::fillTriangle(Vertex v1, Vertex v2, Vertex v3, Bitmap &texture, bool back_face_culling) {
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

void RenderContext::scanTriangle(Vertex &minYVert, Vertex &midYVert, Vertex &maxYVert, bool handedness, Bitmap &texture) {
    Gradients gradients(minYVert, midYVert, maxYVert);

    Edge top_to_bottom(gradients, minYVert, maxYVert, 0);
    Edge top_to_middle(gradients, minYVert, midYVert, 0);
    Edge middle_to_bottom(gradients, midYVert, maxYVert, 1);

    scanEdges(gradients, top_to_bottom, top_to_middle, handedness, texture);
    scanEdges(gradients, top_to_bottom, middle_to_bottom, handedness, texture);
}

//getting information for (horizontal) lines from edge to edge
void RenderContext::scanEdges(Gradients &gradients, Edge &a, Edge &b, bool handedness, Bitmap &texture) {
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

void RenderContext::drawScanLine(Gradients &gradients, Edge &left, Edge &right, int j, Bitmap &texture) {
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

void RenderContext::drawWire(Edge &edge, int thickness, char r, char g, char b) {
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
                    Colour temp_colour;
                    temp_colour.fill(r, g, b, 255);
                    drawPixel(x + j, y_start + k, temp_colour);
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
                        Colour temp_colour;
                        temp_colour.fill(r, g, b, 255);
                        drawPixel(x + j, i + k, temp_colour);
                    }
                }
                else {
                    for(int x = curr_x; x <= prev_x; x++) {
                        Colour temp_colour;
                        temp_colour.fill(r, g, b, 255);
                        drawPixel(x + j, i + k, temp_colour);
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
                Colour temp_colour;
                temp_colour.fill(r, g, b, 255);
                drawPixel(x + j, y_end + k, temp_colour);
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
            Colour temp_colour;
            temp_colour.fill(colour);
            drawPixel(i, j, temp_colour);
        }
    }
}

Bitmap RenderContext::getNormalizedZBuffer() {
    Bitmap result(m_width, m_height);
    getNormalizedZBuffer(result);
    return result;
}

void RenderContext::getNormalizedZBuffer(Bitmap &image) {
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
            Colour temp_colour;
            temp_colour.fill(colour);
            image.drawPixel(i, j, temp_colour);
        }
    }
}

RenderContext RenderContext::getResizedRenderContext(int width, int height) {
    RenderContext image(width, height);
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            int source_x = x * getWidth() / width;
            int source_y = y * getHeight() / height;
            image.copyPixel(x, y, source_x, source_y, *this);
        }
    }
    return image;
}

void RenderContext::getResizedRenderContext(RenderContext &image) {
    int width = image.getWidth();
    int height = image.getHeight();
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++) {
            int source_x = x * getWidth() / width;
            int source_y = y * getHeight() / height;
            image.copyPixel(x, y, source_x, source_y, *this);
        }
    }
}

RenderContext::~RenderContext() {
    delete [] m_z_buffer;
}
