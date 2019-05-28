#pragma once

#include "Renderer.h"

class Sphere
{
public:
    glm::vec3 pos;
    glm::vec3 radius;
    glm::vec3 color;

    glm::vec3 velocity = glm::vec3(0, 0, 0);
    float mass;
    Sphere() {}
    Sphere(glm::vec3 _pos, glm::vec3 _radius, glm::vec3 _color, float density = 1.0f) :
        pos(_pos), radius(_radius), color(_color) 
    {
        mass = density*4.0f / 3 * (float)M_PI*radius.x*radius.y*radius.z;
    }

    void Free() {}
};

class Cube
{
public:
    glm::vec3 pos;
    glm::mat4 rotation;
    glm::vec3 size;
    glm::vec3 color;
    bool inside;

    glm::vec3 velocity = glm::vec3(0, 0, 0);
    float mass;
    Cube() {}
    Cube(glm::vec3 _pos, glm::mat4 _rotation, glm::vec3 _size, glm::vec3 _color, bool _inside = false, float density = 1.0f) :
        pos(_pos), rotation(_rotation), size(_size), color(_color), inside(_inside) 
    {
        mass = density*size.x*size.y*size.z;
    }
    void SetColor(vector<glm::vec3>& color);
    void BindColorVBO();
    void Free() { glDeleteBuffers(1, &vbo_color); }
    bool color_seted = false;
private:
    GLuint vbo_color = 0;
};

class Sphere_renderer:public Renderer
{
public:
    Sphere_renderer(const int segments = 64);
private:
    void SetSphereVertex(vector<glm::vec3>& vertex, const int segments);
    void SetSphereFace(vector<glm::ivec3>& face, const int segments);
};

class Cube_renderer:public Renderer 
{
public:
    Cube_renderer();
};