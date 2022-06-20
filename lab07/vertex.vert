#version 330 core
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
out vec3 vert;
out vec3 norm;

void main()
{
    vert = vertex;
    norm = norm;
    gl_Position = vec4(vert.xyz,1.0);
}
