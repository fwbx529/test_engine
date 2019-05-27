#pragma once

#include "Renderer.h"

class Sphere:public Renderer
{
public:
    Sphere(const float radius, const int segments = 64);
    void SetTransform(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) { SetMVP(prog, m, v, p); }
    void SetPhong(const glm::vec3 Ambient, const glm::vec3 LightColor, const glm::vec3 LightDirection,
                  const glm::vec3 HalfVector, const float Shininess, const float Strength);
    void Draw(const bool draw_point = false) { Renderer::Draw(prog, draw_point); }
private:
    void SetSphereVertex(vector<glm::vec3>& vertex, const float radius, const int segments);
    void SetSphereFace(vector<glm::ivec3>& face, const float radius, const int segments);
    GLuint prog;
};