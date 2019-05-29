
#include "Scene.h"

float theta = glm::radians(90.0f), phi = glm::radians(270.0f);
double xpos_init, ypos_init;

void Scene::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    static float theta_init = theta;
    static float phi_init = phi;
    static float angle_thres = glm::radians(20.0f);
    float dx = (float)(xpos - xpos_init);
    float dy = (float)(ypos - ypos_init);
    theta += dy / 1000;
    theta = glm::clamp(theta, theta_init - angle_thres, theta_init + angle_thres);
    phi += dx / 1000;
    phi = glm::clamp(phi, phi_init - angle_thres, phi_init + angle_thres);
    glfwSetCursorPos(window, xpos_init, ypos_init);
}

Scene::Scene(GLFWwindow*& window)
{
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    xpos_init = viewport[2] / 2; ypos_init = viewport[3] / 2;

    Renderer::InitProgram(prog_phong, "vs_object.glsl", "fs_phong.glsl");
    Renderer::InitProgram(prog_logo, "vs_logo.glsl", "fs_logo.glsl");
    eye = glm::vec3(0, 0, 0);
    center = glm::vec3(0, 0, -1);

    glm::vec3 Ambient(0.5);
    glm::vec3 LightColor(0.5);
    glm::vec3 LightDirection(3, 5, 8);
    LightDirection = glm::normalize(LightDirection);
    float Shininess = 20.0f;
    float Strength = 1.5f;
    SetLight(Light(Ambient, LightColor, LightDirection, Shininess, Strength));
}

void Scene::SetLight(Light& _light)
{
    light = _light;

    glUseProgram(prog_phong);

    GLuint Ambient_loc = glGetUniformLocation(prog_phong, "Ambient");
    GLuint LightColor_loc = glGetUniformLocation(prog_phong, "LightColor");
    GLuint LightDirection_loc = glGetUniformLocation(prog_phong, "LightDirection");
    GLuint HalfVector_loc = glGetUniformLocation(prog_phong, "HalfVector");
    GLuint Shininess_loc = glGetUniformLocation(prog_phong, "Shininess");
    GLuint Strength_loc = glGetUniformLocation(prog_phong, "Strength");

    glUniform3fv(Ambient_loc, 1, &light.Ambient[0]);
    glUniform3fv(LightColor_loc, 1, &light.LightColor[0]);
    glUniform3fv(LightDirection_loc, 1, &light.LightDirection[0]);
    glm::vec3 HalfVector = glm::normalize(glm::normalize(eye - center) + light.LightDirection);
    glUniform3fv(HalfVector_loc, 1, &HalfVector[0]);
    glUniform1f(Shininess_loc, light.Shininess);
    glUniform1f(Strength_loc, light.Strength);

    glUseProgram(0);
}

void Scene::SetView()
{
    center = eye + glm::vec3(sin(theta)*cos(phi), cos(theta), sin(theta)*sin(phi));

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float width = (float)viewport[2]; float height = (float)viewport[3];
    float aspect = height / width;

    SetView(eye, center, aspect);
}

void Scene::SetView(glm::vec3 _eye, glm::vec3 _center, const float aspect)
{
    eye = _eye;
    center = _center;
    glm::mat4 model;
    glm::mat4 view = glm::lookAt(eye, center,
                                 glm::vec3(0, 1, 0));
    glm::mat4 projection(glm::perspective(glm::radians(60.0f), 1.0f / aspect, 0.1f, 100.0f));

    Renderer::SetMVP(prog_phong, model, view, projection);
    glUseProgram(prog_phong);
    GLuint HalfVector_loc = glGetUniformLocation(prog_phong, "HalfVector");
    glm::vec3 HalfVector = glm::normalize(glm::normalize(eye - center) + light.LightDirection);
    glUniform3fv(HalfVector_loc, 1, &HalfVector[0]);
    glUseProgram(0);
}

void Scene::Draw()
{
    //Draw objects
    glUseProgram(prog_phong);
    //Draw spheres
    sphere_renderer.BindVAO();
    for (int idx = 0; idx < spheres.size(); idx++)
    {
        if (!spheres[idx].exist) continue;
        glm::mat4 model = 
            glm::translate(glm::mat4(), spheres[idx].pos) *
            glm::scale(glm::mat4(), spheres[idx].radius);
        glm::mat4 normal_matrix = model;
        int use_color_pure = 1;
        GLuint model_matrix_loc = glGetUniformLocation(prog_phong, "model_matrix");
        GLuint normal_matrix_loc = glGetUniformLocation(prog_phong, "normal_matrix");
        GLuint color_pure_loc = glGetUniformLocation(prog_phong, "color_pure");
        GLuint use_color_pure_loc = glGetUniformLocation(prog_phong, "use_color_pure");
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
        if (!cubes[idx].exist) continue;
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
        GLuint model_matrix_loc = glGetUniformLocation(prog_phong, "model_matrix");
        GLuint normal_matrix_loc = glGetUniformLocation(prog_phong, "normal_matrix");
        GLuint color_pure_loc = glGetUniformLocation(prog_phong, "color_pure");
        GLuint use_color_pure_loc = glGetUniformLocation(prog_phong, "use_color_pure");
        glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(normal_matrix_loc, 1, GL_FALSE, &normal_matrix[0][0]);
        glUniform3fv(color_pure_loc, 1, &cubes[idx].color[0]);
        glUniform1i(use_color_pure_loc, use_color_pure);
        if (!use_color_pure) cubes[idx].BindColorVBO();
        cube_renderer.Draw();
    }

    //Draw logos
    glUseProgram(prog_logo);
    //Draw cross
    cross_renderer.BindVAO();
    for (int idx = 0; idx < logos.size(); idx++)
    {
        if (logos[idx].type != logo_cross) continue;
        int use_color_pure = 1;
        GLuint color_pure_loc = glGetUniformLocation(prog_logo, "color_pure");
        GLuint use_color_pure_loc = glGetUniformLocation(prog_logo, "use_color_pure");
        glUniform3fv(color_pure_loc, 1, &logos[idx].color[0]);
        glUniform1i(use_color_pure_loc, use_color_pure);
        cross_renderer.Draw(false, true);
    }

    glUseProgram(0);
    glBindVertexArray(0);
}

//4th order Runge-Kutta
template <class Object>
void Scene::SimulateForce(const float timestep, const glm::vec3 force, Object& object)
{
    if (!object.exist) return;
    object.velocity += force / object.mass * 0.5f * timestep;
    object.pos += object.velocity;
    object.velocity += force / object.mass * 0.5f * timestep;
}

//4th order Runge-Kutta
template <class Object>
void Scene::SimulateAcceleration(const float timestep, const glm::vec3 acceleration, Object& object)
{
    if (!object.exist) return;
    object.velocity += acceleration * 0.5f * timestep;
    object.pos += object.velocity * timestep;
    object.velocity += acceleration * 0.5f * timestep;
}

template <class Object>
void Scene::CollisionRange(glm::vec3 edge, Object& object, const bool min_edge, const bool explode)
{
    if (!explode)
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
    else
    {
        if (min_edge)
        {
            if (object.pos.x < edge.x || object.pos.y < edge.y || object.pos.z < edge.z) object.exist = false;
        }
        else
        {
            if (object.pos.x > edge.x || object.pos.y > edge.y || object.pos.z > edge.z) object.exist = false;
        }
    }
}

void Scene::CollisionSphereInCube(Sphere& sphere, const Cube& cube, const bool explode)
{
    if (!sphere.exist || !cube.exist) return;
    glm::vec3 cube_min = cube.pos - 0.5f * cube.size + sphere.radius;
    glm::vec3 cube_max = cube.pos + 0.5f * cube.size - sphere.radius;
    CollisionRange(cube_min, sphere, true, explode);
    CollisionRange(cube_max, sphere, false, explode);
}

//Attention: not for ellipse
void Scene::CollisionSpheres(Sphere& sphere1, Sphere& sphere2, const bool explode)
{
    if (!sphere1.exist || !sphere2.exist) return;
    float dis = glm::length(sphere1.pos - sphere2.pos);
    if (dis < sphere1.radius[0] + sphere2.radius[0])
    {
        if (!explode)
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
        else
        {
            sphere1.exist = false;
            sphere2.exist = false;
        }
    }
}

void Scene::Simulation(const int total_iter)
{
    static int time_prev = clock();
    time = clock();
    float timestep = (float)(time - time_prev) / 1000;
    glm::vec3 gravity = glm::vec3(0, -GRAVITY, 0);
    for (int iter = 0; iter < total_iter; iter++)
    {
        for (int idx = 0; idx < spheres.size(); idx++)
        {
            SimulateAcceleration(timestep / total_iter, gravity, spheres[idx]);
            CollisionSphereInCube(spheres[idx], cubes[0]);
        }
        for (int idx = 0; idx < spheres.size(); idx++)
        {
            for (int idy = idx + 1; idy < spheres.size(); idy++)
            {
                CollisionSpheres(spheres[idx], spheres[idy]);
            }
        }
    }
    time_prev = time;
}

void Scene::Bullet(const int total_iter)
{
    static int time_prev = clock();
    time = clock();
    float timestep = (float)(time - time_prev) / 1000;

    for (int iter = 0; iter < total_iter; iter++)
    {
        for (int idx = 0; idx < spheres.size(); idx++)
        {
            SimulateAcceleration(timestep / total_iter, glm::vec3(0), spheres[idx]);
            CollisionSphereInCube(spheres[idx], cubes[0], true);
        }
        for (int idx = 0; idx < spheres.size(); idx++)
        {
            for (int idy = idx + 1; idy < spheres.size(); idy++)
            {
                CollisionSpheres(spheres[idx], spheres[idy], true);
            }
        }
    }
    time_prev = time;
}

float Scene::CalculateEnergy()
{
    float energy = 0;
    for (int idx = 0; idx < spheres.size(); idx++)
    {
        float m = spheres[idx].mass;
        float v = glm::length(spheres[idx].velocity);

        energy += (0.5f * m * v * v + m * GRAVITY * spheres[idx].pos.y);
    }
    return energy;
}

void Scene::Free()
{
    for (int idx = 0; idx < spheres.size(); idx++) spheres[idx].Free();
    for (int idx = 0; idx < cubes.size(); idx++) cubes[idx].Free();
}
