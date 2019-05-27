#version 450

in vec4 world_normal;
out vec4 fcolor;


void main()
{
    float scale = world_normal.z;
    fcolor = vec4(scale, scale, world_normal.z, 1.0f);
}