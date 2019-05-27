#pragma once

#include "Objects.h"

struct Light
{
    glm::vec3 Ambient;
    glm::vec3 LightColor;
    glm::vec3 LightDirection;
    float Shininess;
    float Strength;
    Light() {}
    Light(glm::vec3 _Ambient, glm::vec3 _LightColor, glm::vec3 _LightDirection, float _Shininess, float _Strength) :
        Ambient(_Ambient), LightColor(_LightColor), LightDirection(_LightDirection), Shininess(_Shininess), Strength(_Strength) {}
};

class Scene
{
public:
    Scene();
    void AddSphere(Sphere sphere) { spheres.push_back(sphere); }
    void SetLight(Light& _light);
    void SetView(glm::vec3 _eye, glm::vec3 _center, const float aspect);
    void Draw();
private:
    Sphere_renderer sphere_renderer;
    vector<Sphere> spheres;

    Light light;
    glm::vec3 eye;
    glm::vec3 center;

    GLuint phong_prog;
};