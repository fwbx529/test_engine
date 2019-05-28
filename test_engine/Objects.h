#pragma once

#include "Renderer.h"

struct Sphere
{
    glm::vec3 pos;
    glm::vec3 radius;
    glm::vec3 color;
    Sphere() {}
    Sphere(glm::vec3 _pos, glm::vec3 _radius, glm::vec3 _color) :
        pos(_pos), radius(_radius), color(_color) {}
};

struct Cube
{
    glm::vec3 pos;
    glm::mat4 rotation;
    glm::vec3 size;
    glm::vec3 color;
    bool inside;
    Cube() {}
    Cube(glm::vec3 _pos, glm::mat4 _rotation, glm::vec3 _size, glm::vec3 _color, bool _inside = false) :
        pos(_pos), rotation(_rotation), size(_size), color(_color), inside(_inside) {}
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