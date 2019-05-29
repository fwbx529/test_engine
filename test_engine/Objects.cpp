
#include "Objects.h"

void Sphere_renderer::SetSphereVertex(vector<glm::vec3>& vertex, const int segments)
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
            vertex[(row - 1) * segments + col] = glm::vec3(x, y, z);
        }
    }
    vertex[sphere_vertex_size - 2] = glm::vec3(0, 1, 0);
    vertex[sphere_vertex_size - 1] = glm::vec3(0, -1, 0);
}

void Sphere_renderer::SetSphereFace(vector<glm::ivec3>& face, const int segments)
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

Sphere_renderer::Sphere_renderer(const int segments)
{
    vector<glm::vec3> vertex;
    vector<glm::ivec3> face;
    SetSphereVertex(vertex, segments);
    SetSphereFace(face, segments);

    SetData(vertex, face, vertex);
}

void Cube::SetColor(vector<glm::vec3>& color)
{
    assert(color.size() == 6);
    if (!color_seted)
    {
        glCreateBuffers(1, &vbo_color);
    }
    color_seted = true;
    vector<glm::vec3> color_face(36);
    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 6; j++)
            color_face[i * 6 + j] = color[i];
    glNamedBufferStorage(vbo_color, color_face.size() * sizeof(glm::vec3), color_face.data(), GL_DYNAMIC_STORAGE_BIT);
}

void Cube::BindColorVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_color);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(2);
}

Cube_renderer::Cube_renderer()
{
    vector<glm::vec3> vertex =
    {
        glm::vec3(-1, -1, -1),
        glm::vec3(-1, -1, 1),
        glm::vec3(-1, 1, -1),
        glm::vec3(-1, 1, 1),
        glm::vec3(1, -1, -1),
        glm::vec3(1, -1, 1),
        glm::vec3(1, 1, -1),
        glm::vec3(1, 1, 1)
    };
    vector<glm::ivec3> face =
    {
        glm::ivec3(1, 2, 0),
        glm::ivec3(1, 3, 2),
        glm::ivec3(5, 6, 7),
        glm::ivec3(5, 4, 6),
        glm::ivec3(0, 5, 1),
        glm::ivec3(0, 4, 5),
        glm::ivec3(2, 7, 6),
        glm::ivec3(2, 3, 7),
        glm::ivec3(7, 1, 5),
        glm::ivec3(7, 3, 1),
        glm::ivec3(6, 0, 2),
        glm::ivec3(6, 4, 0)
    };
    for (int v = 0; v < vertex.size(); v++) vertex[v] = vertex[v] * 0.5f;
    SetData(vertex, face, true);
}

Cross_renderer::Cross_renderer()
{
    vector<glm::vec2> vertex =
    {
        glm::vec2(-0.1, -0.1),
        glm::vec2(-0.1, 0.1),
        glm::vec2(0.1, 0.1),
        glm::vec2(0.1, 0.1),
    };
    vector<glm::ivec3> face =
    {
        glm::ivec3(1, 2, 0),
        glm::ivec3(1, 3, 2),
    };
    SetData2D(vertex, face);
}