
#include "Objects.h"

void Sphere::SetSphereVertex(vector<glm::vec3>& vertex, const float radius, const int segments)
{
    int sphere_vertex_size = (segments / 2 - 1) * segments + 2;
    vertex.resize(sphere_vertex_size);
    for (int row = 1; row < segments / 2; row++)
    {
        for (int col = 0; col < segments; col++)
        {
            float angle_unit = 2 * (float)M_PI / segments;
            float theta = angle_unit * row;
            float phi = angle_unit * col;
            float x = sinf(theta) * cosf(phi);
            float y = cosf(theta);
            float z = sinf(theta) * sinf(phi);
            vertex[(row - 1) * segments + col] = radius * glm::vec3(x, y, z);
        }
    }
    vertex[sphere_vertex_size - 2] = glm::vec3(0, radius, 0);
    vertex[sphere_vertex_size - 1] = glm::vec3(0, -radius, 0);
}

void Sphere::SetSphereFace(vector<glm::ivec3>& face, const float radius, const int segments)
{
    int sphere_vertex_size = (segments / 2 - 1) * segments + 2;
    int sphere_face_size = (segments / 2 - 2) * (2 * segments) + 2 * segments;
    face.resize(sphere_face_size);
    for (int row = 1; row < segments / 2 - 1; row++)
    {
        for (int col = 0; col < segments; col++)
        {
            int vertex_index = (row - 1) * segments + col;
            int vertex_index_next = (row - 1) * segments + (col + 1) % segments;
            int face_index = (row - 1) * (2 * segments) + 2 * col;
            face[face_index] = glm::ivec3(vertex_index, vertex_index_next, vertex_index + segments);
            face[face_index + 1] = glm::ivec3(vertex_index + segments, vertex_index_next, vertex_index_next + segments);
        }
    }
    for (int col = 0; col < segments; col++)
    {
        int vertex_index = col;
        int vertex_index_next = (col + 1) % segments;
        face[sphere_face_size - 2 * segments + col] = glm::ivec3(vertex_index_next, vertex_index, sphere_vertex_size - 2);
    }
    for (int col = 0; col < segments; col++)
    {
        int row = segments / 2 - 1;
        int vertex_index = (row - 1) * segments + col;
        int vertex_index_next = (row - 1) * segments + (col + 1) % segments;
        face[sphere_face_size - segments + col] = glm::ivec3(vertex_index, vertex_index_next, sphere_vertex_size - 1);
    }
}

Sphere::Sphere(const float radius, const int segments)
{
    vector<glm::vec3> vertex;
    vector<glm::ivec3> face;
    vector<glm::vec3> normal;
    SetSphereVertex(vertex, radius, segments);
    SetSphereFace(face, radius, segments);
    SetSphereVertex(normal, 1.0f, segments);

    InitProgram(prog, "vs_object.glsl", "fs_phong.glsl");
    SetData(vertex, face, normal);
}

void Sphere::SetPhong(const glm::vec3 Ambient, const glm::vec3 LightColor, const glm::vec3 LightDirection,
                      const glm::vec3 HalfVector, const float Shininess, const float Strength)
{
    glUseProgram(prog);

    GLuint Ambient_loc = glGetUniformLocation(prog, "Ambient");
    GLuint LightColor_loc = glGetUniformLocation(prog, "LightColor");
    GLuint LightDirection_loc = glGetUniformLocation(prog, "LightDirection");
    GLuint HalfVector_loc = glGetUniformLocation(prog, "HalfVector");
    GLuint Shininess_loc = glGetUniformLocation(prog, "Shininess");
    GLuint Strength_loc = glGetUniformLocation(prog, "Strength");

    glUniform3fv(Ambient_loc, 1, &Ambient[0]);
    glUniform3fv(LightColor_loc, 1, &LightColor[0]);
    glUniform3fv(LightDirection_loc, 1, &LightDirection[0]);
    glUniform3fv(HalfVector_loc, 1, &HalfVector[0]);
    glUniform1f(Shininess_loc, Shininess);
    glUniform1f(Strength_loc, Strength);

    glUseProgram(0);
}