#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "LoadShaders.h"

using namespace std;

class Renderer 
{
public:
    Renderer();
    ~Renderer();

    void SetData(vector<glm::vec3>& _vertex, vector<glm::ivec3>& _face, const bool face_normal = false);
    void SetData(vector<glm::vec3>& _vertex, vector<glm::ivec3>& _face, vector<glm::vec3>& _normal);
    void SetData2D(vector<glm::vec2>& _vertex, vector<glm::ivec3>& _face);
    void SetData1D(vector<glm::vec2>& _vertex);
    void BindVAO() { glBindVertexArray(vao); }
    void Draw(GLuint& prog, const bool draw_point = false, const bool draw_line = false);
    void Draw(const bool draw_point = false, const bool draw_line = false);

    static void InitProgram(GLuint& prog, const string vs_shader, const string fs_shader);
    static void SetMVP(GLuint& prog, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);

private:
    void SetBuffer(const vector<glm::vec3>& vertex, const vector<glm::vec3>& normal, const bool face_normal = false);
    void SetBuffer2D(const vector<glm::vec2>& vertex);
    void CalNorm_SetBuffer(const vector<glm::vec3>& vertex, const bool face_normal = false);

    GLuint vao;
    GLuint vbo_array_buffer[3];
    unsigned int vertex_count;
    unsigned int face_count;

    bool data_seted;

    vector<glm::ivec3> face;
};