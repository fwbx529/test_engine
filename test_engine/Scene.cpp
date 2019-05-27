
#include "Scene.h"

Scene::Scene()
{
    Renderer::InitProgram(phong_prog, "vs_object.glsl", "fs_phong.glsl");
    eye = glm::vec3(0, 0, 0);
    center = glm::vec3(0, 0, -1);
}

void Scene::SetLight(Light& _light)
{
    light = _light;

    glUseProgram(phong_prog);

    GLuint Ambient_loc = glGetUniformLocation(phong_prog, "Ambient");
    GLuint LightColor_loc = glGetUniformLocation(phong_prog, "LightColor");
    GLuint LightDirection_loc = glGetUniformLocation(phong_prog, "LightDirection");
    GLuint HalfVector_loc = glGetUniformLocation(phong_prog, "HalfVector");
    GLuint Shininess_loc = glGetUniformLocation(phong_prog, "Shininess");
    GLuint Strength_loc = glGetUniformLocation(phong_prog, "Strength");

    glUniform3fv(Ambient_loc, 1, &light.Ambient[0]);
    glUniform3fv(LightColor_loc, 1, &light.LightColor[0]);
    glUniform3fv(LightDirection_loc, 1, &light.LightDirection[0]);
    glm::vec3 HalfVector = glm::normalize(glm::normalize(eye - center) + light.LightDirection);
    glUniform3fv(HalfVector_loc, 1, &HalfVector[0]);
    glUniform1f(Shininess_loc, light.Shininess);
    glUniform1f(Strength_loc, light.Strength);

    glUseProgram(0);
}

void Scene::SetView(glm::vec3 _eye, glm::vec3 _center, const float aspect)
{
    eye = _eye;
    center = _center;
    glm::mat4 model;
    glm::mat4 view = glm::lookAt(eye, center,
                                 glm::vec3(0, 1, 0));
    glm::mat4 projection(glm::perspective(glm::radians(60.0f), 1.0f / aspect, 0.1f, 100.0f));

    Renderer::SetMVP(phong_prog, model, view, projection);
    glUseProgram(phong_prog);
    GLuint HalfVector_loc = glGetUniformLocation(phong_prog, "HalfVector");
    glm::vec3 HalfVector = glm::normalize(glm::normalize(eye - center) + light.LightDirection);
    glUniform3fv(HalfVector_loc, 1, &HalfVector[0]);
    glUseProgram(0);
}

void Scene::Draw()
{
    glUseProgram(phong_prog);

    //Draw spheres
    sphere_renderer.BindVAO();
    for (int idx = 0; idx < spheres.size(); idx++)
    {
        glm::mat4 model = 
            glm::translate(glm::mat4(), spheres[idx].pos) *
            glm::scale(glm::mat4(), spheres[idx].radius);
        int use_color_pure = 1;
        GLuint model_matrix_loc = glGetUniformLocation(phong_prog, "model_matrix");
        GLuint color_pure_loc = glGetUniformLocation(phong_prog, "color_pure");
        GLuint use_color_pure_loc = glGetUniformLocation(phong_prog, "use_color_pure");
        glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, &model[0][0]);
        glUniform3fv(color_pure_loc, 1, &spheres[idx].color[0]);
        glUniform1i(use_color_pure_loc, use_color_pure);
        sphere_renderer.Draw();
    }

    //Draw cubes
    cube_renderer.BindVAO();
    for (int idx = 0; idx < cubes.size(); idx++)
    {
        glm::mat4 model =
            glm::translate(glm::mat4(), cubes[idx].pos) *
            cubes[idx].rotation * 
            glm::scale(glm::mat4(), cubes[idx].size);
        int use_color_pure = 1;
        GLuint model_matrix_loc = glGetUniformLocation(phong_prog, "model_matrix");
        GLuint color_pure_loc = glGetUniformLocation(phong_prog, "color_pure");
        GLuint use_color_pure_loc = glGetUniformLocation(phong_prog, "use_color_pure");
        glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, &model[0][0]);
        glUniform3fv(color_pure_loc, 1, &cubes[idx].color[0]);
        glUniform1i(use_color_pure_loc, use_color_pure);
        cube_renderer.Draw();
    }

    glUseProgram(0);
    glBindVertexArray(0);
}