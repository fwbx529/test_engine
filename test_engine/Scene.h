#pragma once

#include "Objects.h"
#define GRAVITY 9.8f

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
    Scene(GLFWwindow*& window);
    void SetLight(Light& _light);
    void SetView();
    void SetView(glm::vec3 _eye, glm::vec3 _center, const float aspect);
    void Draw();
    void Simulation(const int total_iter = 10);
    void Bullet(const int total_iter = 10);
    void Free();

    void AddSphere(Sphere& sphere) { spheres.push_back(sphere); }
    void AddCube(Cube& cube) { cubes.push_back(cube); }
    void AddLogo(Logo& logo) { logos.push_back(logo); }
    float CalculateEnergy();
    glm::vec3 GetEye() { return eye; }
    glm::vec3 GetCenter() { return center; }
    glm::vec3 GetPosFront(const float dis) { return eye + glm::normalize(center - eye) * dis; }
    glm::vec3 GetDirFront() { return glm::normalize(center - eye); }

    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

    vector<Sphere> spheres;
    vector<Cube> cubes;
    vector<Logo> logos;

private:

    template <class Object>
    void SimulateForce(const float timestep, const glm::vec3 force, Object& object);
    template <class Object>
    void SimulateAcceleration(const float timestep, const glm::vec3 acceleration, Object& object);
    template <class Object>
    void CollisionRange(glm::vec3 edge, Object& object, const bool min_edge, const bool explode);
    void CollisionSphereInCube(Sphere& sphere, const Cube& cube, const bool explode = false);
    void CollisionSpheres(Sphere& sphere1, Sphere& sphere2, const bool explode = false);
    Sphere_renderer sphere_renderer;
    Cube_renderer cube_renderer;
    Cross_renderer cross_renderer;

    Light light;
    glm::vec3 eye;
    glm::vec3 center;

    GLuint prog_phong;
    GLuint prog_logo;

    int time;
};