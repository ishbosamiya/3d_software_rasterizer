#include "shader.h"

Vector4f Shader::light_dir(cos(0), cos(0), cos(0));
int Shader::shader_type = 0;
int Shader::no_of_shaders = 5;

Vertex Shader::vertexShader(Vertex vertex, int nth_vertex, int width, int height) {
    Matrix4f screen_space_transform;
    screen_space_transform.initScreenSpaceTransform(width/2.0, height/2.0);
    Vertex result = vertex.transform(screen_space_transform).perspectiveDivide();

    if(shader_type > -1) {
        float lighting = vertex.getNormal().normalized().dot(light_dir.normalized());
        if(nth_vertex == 0) {
            normals[0] = vertex.getNormal();
            varying_intensity.setX(lighting);
        }
        else if(nth_vertex == 1) {
            normals[1] = vertex.getNormal();
            varying_intensity.setY(lighting);
        }
        else if(nth_vertex == 2) {
            normals[2] = vertex.getNormal();
            varying_intensity.setZ(lighting);
        }
    }
    return result;
}

bool Shader::fragmentShader(Vector4f bar, Colour &colour) {
    //Unlit
    if(shader_type == -1) {
        return true;
    }

    //Texture + Lighting Smooth
    if(shader_type % no_of_shaders == 0) {
        float intensity = varying_intensity.normalized().dot(bar.normalized());
        colour.multiply(max((float)(min(intensity * 1.2, 1.0)), (float)(0.1)));
    }
    //Lighting Smooth Normals
    else if(shader_type % no_of_shaders == 1) {
        float intensity = varying_intensity.normalized().dot(bar.normalized());
        colour.fill(max(intensity * 255.0f, 0.1f * 255.0f));
    }
    //Lighting Flat Normals
    else if(shader_type % no_of_shaders == 2) {
        colour.fill(255, 255, 255); //white
        //colour.fill(94, 219, 172); //greenish light blue
        float intensity = varying_intensity.getX() + varying_intensity.getY() + varying_intensity.getZ();
        intensity /= 3.0;
        intensity = max(intensity, 0.1f);
        colour.multiply(intensity);
    }
    //Texture + Lighting Flat
    else if(shader_type % no_of_shaders == 3) {
        float intensity = varying_intensity.getX() + varying_intensity.getY() + varying_intensity.getZ();
        intensity /= 3.0;
        intensity = max(intensity, 0.1f);
        colour.multiply(intensity);
    }
    //Normal Information Via Colour
    else if(shader_type % no_of_shaders == 4) {
        colour.fill(255);
        Vector4f sum = normals[0].add(normals[1].add(normals[2]));
        sum = sum.normalized();
        colour.multiply(sum);
    }

    return true;
}
