
#include "Objects.h"
#include <time.h>

float aspect;
void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) { if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; std::cout << "---------------" << std::endl; std::cout << "Debug message (" << id << "): " << message << std::endl; switch (source) { case GL_DEBUG_SOURCE_API: std::cout << "Source: API"; break; case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "Source: Window System"; break; case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break; case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "Source: Third Party"; break; case GL_DEBUG_SOURCE_APPLICATION: std::cout << "Source: Application"; break; case GL_DEBUG_SOURCE_OTHER: std::cout << "Source: Other"; break; } std::cout << std::endl; switch (type) { case GL_DEBUG_TYPE_ERROR: std::cout << "Type: Error"; break; case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break; case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "Type: Undefined Behaviour"; break; case GL_DEBUG_TYPE_PORTABILITY: std::cout << "Type: Portability"; break; case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "Type: Performance"; break; case GL_DEBUG_TYPE_MARKER: std::cout << "Type: Marker"; break; case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "Type: Push Group"; break; case GL_DEBUG_TYPE_POP_GROUP: std::cout << "Type: Pop Group"; break; case GL_DEBUG_TYPE_OTHER: std::cout << "Type: Other"; break; } std::cout << std::endl; switch (severity) { case GL_DEBUG_SEVERITY_HIGH: std::cout << "Severity: high"; break; case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "Severity: medium"; break; case GL_DEBUG_SEVERITY_LOW: std::cout << "Severity: low"; break; case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break; } std::cout << std::endl; std::cout << std::endl; }

void Display(Sphere& s)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glClearBufferfv(GL_COLOR, 0, black);

    glm::mat4 model = glm::translate(glm::mat4(), glm::vec3(0, 0, -3));
    int time = clock();
    glm::vec3 center(0, 0, -3);
    glm::vec3 eye;// = center + glm::vec3(cosf((float)time / 1000), 0, sinf((float)time / 1000));
    glm::mat4 view = glm::lookAt(eye, center,
                                 glm::vec3(0, 1, 0));    
    glm::mat4 projection(glm::perspective(glm::radians(60.0f), 1.0f / aspect, 0.1f, 100.0f));
    s.SetTransform(model, view, projection);

    glm::vec3 Ambient(0.5, 0.5, 0.5);
    glm::vec3 LightColor(1, 1, 1);
    glm::vec3 LightDirection(1, 0, 0);
    LightDirection = glm::normalize(LightDirection);
    glm::vec3 HalfVector = glm::normalize(glm::normalize(eye - center) + LightDirection);
    float Shininess = 20.0f; 
    float Strength = 1.0f;
    s.SetPhong(Ambient, LightColor, LightDirection, HalfVector, Shininess, Strength);

    s.Draw();
}

int main()
{
    const int width = 800;
    const int height = 600;
    aspect = float(height) / float(width);

    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(width, height, "test-opengl", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(glDebugOutput, nullptr);
#endif // _DEBUG

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    Sphere s(0.5);

    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window))
    {
        Display(s);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}