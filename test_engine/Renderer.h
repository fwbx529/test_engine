#pragma once

#include <vector>
#include <string>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class Renderer 
{
public:
    Renderer();
    ~Renderer();

    void InitProgram(GLuint& prog, const string vs_shader, const string fs_shader);
    void SetData(vector<glm::vec3>& _vertex, vector<glm::ivec3>& _face);
    void SetData(vector<glm::vec3>& _vertex, vector<glm::ivec3>& _face, vector<glm::vec3>& _normal);
    void SetMVP(GLuint& prog, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
    void Draw(GLuint& prog, const bool draw_point = false);
private:
    void SetBuffer(const vector<glm::vec3>& vertex, const vector<glm::vec3>& normal);
    void CalNorm_SetBuffer(const vector<glm::vec3>& vertex);

    GLuint vao;
    GLuint vbo_array_buffer[3];
    unsigned int vertex_count;
    unsigned int face_count;

    bool data_seted;

    vector<glm::ivec3> face;
};