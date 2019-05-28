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
    void SetLight(Light& _light);
    void SetView(glm::vec3 _eye, glm::vec3 _center, const float aspect);
    void Draw();
    void Simulation();
    void Free();

    void AddSphere(Sphere& sphere) { spheres.push_back(sphere); }
    void AddCube(Cube& cube) { cubes.push_back(cube); }
private:
    template <class Object>
    void SimulateForce(const float timestep, const glm::vec3 force, Object& object);
    template <class Object>
    void SimulateAcceleration(const float timestep, const glm::vec3 acceleration, Object& object);
    Sphere_renderer sphere_renderer;
    Cube_renderer cube_renderer;
    vector<Sphere> spheres;
    vector<Cube> cubes;

    Light light;
    glm::vec3 eye;
    glm::vec3 center;

    GLuint phong_prog;

    int time;
};