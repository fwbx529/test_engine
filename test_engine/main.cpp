
#include "Scene.h"
#include <time.h>

float rand1() { return (float)rand() / RAND_MAX; }
glm::vec3 rand_vec3() { return glm::vec3(rand1(), rand1(), rand1()); }

void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) { if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; std::cout << "---------------" << std::endl; std::cout << "Debug message (" << id << "): " << message << std::endl; switch (source) { case GL_DEBUG_SOURCE_API: std::cout << "Source: API"; break; case GL_DEBUG_SOURCE_WINDOW_SYSTEM: std::cout << "Source: Window System"; break; case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break; case GL_DEBUG_SOURCE_THIRD_PARTY: std::cout << "Source: Third Party"; break; case GL_DEBUG_SOURCE_APPLICATION: std::cout << "Source: Application"; break; case GL_DEBUG_SOURCE_OTHER: std::cout << "Source: Other"; break; } std::cout << std::endl; switch (type) { case GL_DEBUG_TYPE_ERROR: std::cout << "Type: Error"; break; case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break; case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: std::cout << "Type: Undefined Behaviour"; break; case GL_DEBUG_TYPE_PORTABILITY: std::cout << "Type: Portability"; break; case GL_DEBUG_TYPE_PERFORMANCE: std::cout << "Type: Performance"; break; case GL_DEBUG_TYPE_MARKER: std::cout << "Type: Marker"; break; case GL_DEBUG_TYPE_PUSH_GROUP: std::cout << "Type: Push Group"; break; case GL_DEBUG_TYPE_POP_GROUP: std::cout << "Type: Pop Group"; break; case GL_DEBUG_TYPE_OTHER: std::cout << "Type: Other"; break; } std::cout << std::endl; switch (severity) { case GL_DEBUG_SEVERITY_HIGH: std::cout << "Severity: high"; break; case GL_DEBUG_SEVERITY_MEDIUM: std::cout << "Severity: medium"; break; case GL_DEBUG_SEVERITY_LOW: std::cout << "Severity: low"; break; case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break; } std::cout << std::endl; std::cout << std::endl; }

bool shoot = false;
bool release = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && release == false)
        shoot = true;
}

int main()
{
    srand((int)time(0));

    const int width = 800;
    const int height = 600;

    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(width, height, "test-opengl", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    //glfwSetCursorPos(window, xpos_init, ypos_init);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(glDebugOutput, nullptr);
#endif // _DEBUG

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    Scene scene(window);
    
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
    scene.AddSphere(Sphere(scene.GetPosFront(0.5f), glm::vec3(0.01f), rand_vec3()));
    scene.spheres[0].exist = false;
    for (int i = -5; i <= 5; i++)
    {
        for (int j = -5; j <= 5; j++)
        {
            scene.AddSphere(Sphere(glm::vec3(i*0.3f, j*0.3f, -8.0f), glm::vec3(0.1f), rand_vec3()));
        }
    }
    scene.AddLogo(Logo(logo_cross, glm::vec3(0.0f)));

    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        static const float white[] = { 1.0f, 1.0f, 1.0f, 0.0f };
        glClearBufferfv(GL_COLOR, 0, white);

        scene.SetView();
        if (shoot)
        {
            shoot = false;
            release = true;
            scene.spheres[0].exist = true;
            scene.spheres[0].velocity = 20.0f * scene.GetDirFront();
        }
        if (!scene.spheres[0].exist)
        {
            release = false;
            scene.spheres[0].pos = scene.GetPosFront(0.5f);
            scene.spheres[0].velocity = glm::vec3(0, 0, 0);
        }
        scene.Bullet();
        scene.Draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    scene.Free();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}