
// Vertex shader for multiple lights stays the same with all lighting
// done in the fragment shader.
#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
out vec4 Color;
out vec3 Normal;
out vec4 Position;
void main()
{
    Color = glm::vec4(0, 1, 1, 1);
    Normal = (model_matrix * vec4(normal, 0.0f)).xyz;
    Normal = normalize(Normal);
    Position = model_matrix * vec4(position, 1);
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
}