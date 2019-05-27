
#include "Renderer.h"

Renderer::Renderer()
{
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(3, vbo_array_buffer);
    vertex_count = face_count = 0;
    data_seted = false;
}

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(3, vbo_array_buffer);
}

void Renderer::SetBuffer(const vector<glm::vec3>& vertex, const vector<glm::vec3>& normal)
{
    vector<glm::vec3> vertex_face(3 * face_count);
    vector<glm::vec3> normal_face(3 * face_count);
    for (unsigned int f = 0; f < face_count; f++)
    {
        glm::ivec3 f_id(face[f]);
        vertex_face[3 * f] = vertex[f_id.x];
        vertex_face[3 * f + 1] = vertex[f_id.y];
        vertex_face[3 * f + 2] = vertex[f_id.z];
        normal_face[3 * f] = normal[f_id.x];
        normal_face[3 * f + 1] = normal[f_id.y];
        normal_face[3 * f + 2] = normal[f_id.z];
    }

    glBindVertexArray(vao);

    if (!data_seted)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo_array_buffer[0]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);
        glNamedBufferStorage(vbo_array_buffer[0], 3 * face_count * sizeof(glm::vec3), vertex_face.data(), GL_DYNAMIC_STORAGE_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, vbo_array_buffer[1]);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);
        glNamedBufferStorage(vbo_array_buffer[1], 3 * face_count * sizeof(glm::vec3), normal_face.data(), GL_DYNAMIC_STORAGE_BIT);
    }
    else
    {
        glNamedBufferSubData(vbo_array_buffer[0], 0, 3 * face_count * sizeof(glm::vec3), vertex_face.data());
        glNamedBufferSubData(vbo_array_buffer[1], 0, 3 * face_count * sizeof(glm::vec3), normal_face.data());
    }

    glBindVertexArray(0);

    data_seted = true;
}

void Renderer::CalNorm_SetBuffer(const vector<glm::vec3>& vertex)
{
    vector<glm::vec3> normal(vertex_count, glm::vec3(0, 0, 0));
    for (unsigned int f = 0; f < face_count; f++)
    {
        glm::ivec3 f_id(face[f]);
        glm::vec3 vx(vertex[f_id.x]);
        glm::vec3 vy(vertex[f_id.y]);
        glm::vec3 vz(vertex[f_id.z]);
        glm::vec3 n = glm::cross(vy - vx, vz - vx);
        normal[f_id.x] += n;
        normal[f_id.y] += n;
        normal[f_id.z] += n;
    }
    for (unsigned int v = 0; v < vertex_count; v++)
    {
        normal[v] = glm::normalize(normal[v]);
    }
    SetBuffer(vertex, normal);
}



void Renderer::SetData(vector<glm::vec3>& _vertex, vector<glm::ivec3>& _face)
{
    face = _face;
    vertex_count = (unsigned int)_vertex.size();
    face_count = (unsigned int)_face.size();
    CalNorm_SetBuffer(_vertex);
}

void Renderer::SetData(vector<glm::vec3>& _vertex, vector<glm::ivec3>& _face, vector<glm::vec3>& _normal)
{
    face = _face;
    vertex_count = (unsigned int)_vertex.size();
    face_count = (unsigned int)_face.size();
    SetBuffer(_vertex, _normal);
}


void Renderer::Draw(GLuint& prog, const bool draw_point)
{
    glUseProgram(prog);
    glBindVertexArray(vao);
    if (!draw_point)
    {
        glDrawArrays(GL_TRIANGLES, 0, 3 * face_count);
    }
    else
    {
        glEnable(GL_PROGRAM_POINT_SIZE);
        glPointSize(5.0f);
        glDrawArrays(GL_POINTS, 0, 3 * vertex_count);
    }
    glUseProgram(0);
    glBindVertexArray(0);
}

void Renderer::Draw(const bool draw_point)
{
    if (!draw_point)
    {
        glDrawArrays(GL_TRIANGLES, 0, 3 * face_count);
    }
    else
    {
        glEnable(GL_PROGRAM_POINT_SIZE);
        glPointSize(5.0f);
        glDrawArrays(GL_POINTS, 0, 3 * vertex_count);
    }
}

void Renderer::InitProgram(GLuint& prog, const string vs_shader, const string fs_shader)
{
    ShaderInfo shader_info[] =
    {
        { GL_VERTEX_SHADER, vs_shader.c_str() },
        { GL_FRAGMENT_SHADER, fs_shader.c_str() },
        { GL_NONE, NULL }
    };
    prog = LoadShaders(shader_info);
}

void Renderer::SetMVP(GLuint& prog, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    glUseProgram(prog);

    GLuint model_matrix_loc = glGetUniformLocation(prog, "model_matrix");
    GLuint view_matrix_loc = glGetUniformLocation(prog, "view_matrix");
    GLuint projection_matrix_loc = glGetUniformLocation(prog, "projection_matrix");

    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, &projection[0][0]);

    glUseProgram(0);
}