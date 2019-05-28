
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
        glm::mat4 normal_matrix = model;
        int use_color_pure = 1;
        GLuint model_matrix_loc = glGetUniformLocation(phong_prog, "model_matrix");
        GLuint normal_matrix_loc = glGetUniformLocation(phong_prog, "normal_matrix");
        GLuint color_pure_loc = glGetUniformLocation(phong_prog, "color_pure");
        GLuint use_color_pure_loc = glGetUniformLocation(phong_prog, "use_color_pure");
        glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(normal_matrix_loc, 1, GL_FALSE, &normal_matrix[0][0]);
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
        glm::mat4 normal_matrix = model;
        if (cubes[idx].inside)
        {
            glm::mat4 inv(-1.0f);
            inv[3][3] = 1;
            normal_matrix = normal_matrix * inv;
        }
        int use_color_pure = (int)(cubes[idx].color_seted == false);
        GLuint model_matrix_loc = glGetUniformLocation(phong_prog, "model_matrix");
        GLuint normal_matrix_loc = glGetUniformLocation(phong_prog, "normal_matrix");
        GLuint color_pure_loc = glGetUniformLocation(phong_prog, "color_pure");
        GLuint use_color_pure_loc = glGetUniformLocation(phong_prog, "use_color_pure");
        glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(normal_matrix_loc, 1, GL_FALSE, &normal_matrix[0][0]);
        glUniform3fv(color_pure_loc, 1, &cubes[idx].color[0]);
        glUniform1i(use_color_pure_loc, use_color_pure);
        if (!use_color_pure) cubes[idx].BindColorVBO();
        cube_renderer.Draw();
    }

    glUseProgram(0);
    glBindVertexArray(0);
}

//4th order Runge-Kutta
template <class Object>
void Scene::SimulateForce(const float timestep, const glm::vec3 force, Object& object)
{
    object.velocity += force / object.mass * 0.5f * timestep;
    object.pos += object.velocity;
    object.velocity += force / object.mass * 0.5f * timestep;
}

//4th order Runge-Kutta
template <class Object>
void Scene::SimulateAcceleration(const float timestep, const glm::vec3 acceleration, Object& object)
{
    object.velocity += acceleration * 0.5f * timestep;
    object.pos += object.velocity * timestep;
    object.velocity += acceleration * 0.5f * timestep;
}

template <class Object>
void Scene::CollisionRange(glm::vec3 edge, Object& object, const bool min_edge)
{
    if (min_edge)
    {
        if (object.pos.x < edge.x) {
            object.velocity.x *= -1;
        }
        if (object.pos.y < edge.y) {
            object.velocity.y *= -1;
        }
        if (object.pos.z < edge.z) {
            object.velocity.z *= -1;
        }
    }
    else
    {
        if (object.pos.x > edge.x) {
            object.velocity.x *= -1;
        }
        if (object.pos.y > edge.y) {
            object.velocity.y *= -1;
        }
        if (object.pos.z > edge.z) {
            object.velocity.z *= -1;
        }
    }
}

void Scene::CollisionSphereInCube(Sphere& sphere, const Cube& cube)
{
    glm::vec3 cube_min = cube.pos - 0.5f * cube.size + sphere.radius;
    glm::vec3 cube_max = cube.pos + 0.5f * cube.size - sphere.radius;
    CollisionRange(cube_min, sphere, true);
    CollisionRange(cube_max, sphere, false);
}

//Attention: not for ellipse
void Scene::CollisionSpheres(Sphere& sphere1, Sphere& sphere2)
{
    float dis = glm::length(sphere1.pos - sphere2.pos);
    if (dis < sphere1.radius[0] + sphere2.radius[0])
    {
        float m1 = sphere1.mass, m2 = sphere2.mass;
        glm::vec3 v1 = sphere1.velocity, v2 = sphere2.velocity;
        glm::vec3 d = glm::normalize(sphere2.pos - sphere1.pos);
        glm::vec3 v1_ = sphere1.velocity - sphere2.velocity;
        glm::vec3 v2_ = 2.0f * d * m1 / (m1 + m2) * glm::dot(v1_, d);
        glm::vec3 u2 = v2 + v2_;
        glm::vec3 u1 = (m1*v1 + m2*v2 - m2*u2) / m1;
        sphere1.velocity = u1;
        sphere2.velocity = u2;
    }
}


void Scene::Simulation()
{
    static int time_prev = clock();
    time = clock();
    float timestep = (float)(time - time_prev) / 1000;
    glm::vec3 gravity = glm::vec3(0, -9.8f, 0);
    for (int idx = 0; idx < spheres.size(); idx++)
    {
        SimulateAcceleration(timestep, gravity, spheres[idx]);
        CollisionSphereInCube(spheres[idx], cubes[0]);
    }
    for (int idx = 0; idx < spheres.size(); idx++)
    {
        for (int idy = idx + 1; idy < spheres.size(); idy++)
        {
            CollisionSpheres(spheres[idx], spheres[idy]);
        }
    }
    time_prev = time;
}

void Scene::Free()
{
    for (int idx = 0; idx < spheres.size(); idx++) spheres[idx].Free();
    for (int idx = 0; idx < cubes.size(); idx++) cubes[idx].Free();
}