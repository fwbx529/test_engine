
#version 450 core
layout(location = 0) in vec2 position;
layout(location = 2) in vec3 color;
uniform int use_color_pure;
uniform vec3 color_pure;
out vec4 Color;
void main()
{
    if (use_color_pure != 0) Color = vec4(color_pure, 1.0f);
    else Color = vec4(color, 1.0f);
    gl_Position = vec4(position, 0.99, 1);
}