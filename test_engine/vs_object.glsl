
// Vertex shader for multiple lights stays the same with all lighting
// done in the fragment shader.
#version 450 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform int use_color_pure;
uniform vec3 color_pure;
out vec4 Color;
out vec3 Normal;
out vec4 Position;
void main()
{
    if (use_color_pure != 0) Color = vec4(color_pure, 1.0f);
    else Color = vec4(color, 1.0f);
    Normal = (normal_matrix * vec4(normal, 0.0f)).xyz;
    Normal = normalize(Normal);
    Position = model_matrix * vec4(position, 1);
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
}