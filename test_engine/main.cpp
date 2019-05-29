
#include "Scene.h"
#include <time.h>

float rand1() { return (float)rand() / RAND_MAX; }
glm::vec3 rand_vec3() { return glm::vec3(rand1(), rand1(), rand1()); }

float aspect;
void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) { if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; std::cout << "---------------" << std::endl; std::cout << "Debug message (" << id << "): " << message << std::endl; switch (source) { case GL_DEBUG_SOURCE_API: std::cout << "Source: API"; break; case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "Source: Window System"; break; case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break; case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "Source: Third Party"; break; case GL_DEBUG_SOURCE_APPLICATION: std::cout << "Source: Application"; break; case GL_DEBUG_SOURCE_OTHER: std::cout << "Source: Other"; break; } std::cout << std::endl; switch (type) { case GL_DEBUG_TYPE_ERROR: std::cout << "Type: Error"; break; case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break; case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "Type: Undefined Behaviour"; break; case GL_DEBUG_TYPE_PORTABILITY: std::cout << "Type: Portability"; break; case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "Type: Performance"; break; case GL_DEBUG_TYPE_MARKER: std::cout << "Type: Marker"; break; case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "Type: Push Group"; break; case GL_DEBUG_TYPE_POP_GROUP: std::cout << "Type: Pop Group"; break; case GL_DEBUG_TYPE_OTHER: std::cout << "Type: Other"; break; } std::cout << std::endl; switch (severity) { case GL_DEBUG_SEVERITY_HIGH: std::cout << "Severity: high"; break; case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "Severity: medium"; break; case GL_DEBUG_SEVERITY_LOW: std::cout << "Severity: low"; break; case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break; } std::cout << std::endl; std::cout << std::endl; }

float theta_init = glm::radians(90.0f);
float phi_init = glm::radians(270.0f);
float angle_thres = glm::radians(20.0f);
float theta = theta_init;
float phi = phi_init;
double xpos_init = 400;
double ypos_init = 300;

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    float dx = (float)(xpos - xpos_init);
    float dy = (float)(ypos - ypos_init);
    theta += dy / 1000;
    theta = glm::clamp(theta, theta_init - angle_thres, theta_init + angle_thres);
    phi += dx / 1000;
    phi = glm::clamp(phi, phi_init - angle_thres, phi_init + angle_thres);
    glfwSetCursorPos(window, xpos_init, ypos_init);
}

int main()
{
    srand((int)time(0));

    const int width = 800;
    const int height = 600;
    aspect = float(height) / float(width);

    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(width, height, "test-opengl", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glfwSetCursorPos(window, xpos_init, ypos_init);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cursor_pos_callback);

#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(glDebugOutput, nullptr);
#endif // _DEBUG

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    Scene scene;

    glm::vec3 Ambient(0.5);
    glm::vec3 LightColor(0.5);
    glm::vec3 LightDirection(3, 5, 8);
    LightDirection = glm::normalize(LightDirection);
    float Shininess = 20.0f;
    float Strength = 1.5f;
    scene.SetLight(Light(Ambient, LightColor, LightDirection, Shininess, Strength));
    glm::vec3 eye(0, 0, 3);
    glm::vec3 center = eye + glm::vec3(0, 0, -1);
    
    Cube room(glm::vec3(0, 0, 0), glm::mat4(), glm::vec3(6, 6, 20), glm::vec3(1, 0.8f, 0.6f), true);
    vector<glm::vec3> room_color =
    {
        glm::vec3(0.5, 1, 1),
        glm::vec3(0.5, 1, 1),
        glm::vec3(0.5, 1, 0.5),
        glm::vec3(0.5, 1, 0.5),
        glm::vec3(1, 1, 0.5),
        glm::vec3(1, 1, 0.5)
    };
    room.SetColor(room_color);
    scene.AddCube(room);
    scene.AddSphere(Sphere(glm::vec3(0, -2.0f, -3), glm::vec3(0.5f), rand_vec3()));
    scene.SetSphereVelocity(glm::vec3(1, -1, -1), 0);
    //for (int i = 0; i < 100; i++)
    //{
    //    scene.AddSphere(Sphere(rand_vec3() * 2.0f - 1.0f, glm::vec3(0.2f * rand1()), rand_vec3()));
    //}

    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        static const float white[] = { 1.0f, 1.0f, 1.0f, 0.0f };
        glClearBufferfv(GL_COLOR, 0, white);

        scene.Bullet();
        center = eye + glm::vec3(sin(theta)*cos(phi), cos(theta), sin(theta)*sin(phi));
        //cout << scene.CalculateEnergy() << endl;
        scene.SetView(eye, center, aspect);
        scene.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    scene.Free();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}