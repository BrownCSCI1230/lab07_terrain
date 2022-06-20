#version 330 core
in vec4 vert;
in vec4 norm;
out vec4 fragColor;

void main(void)
{
    vec3 lightDir =  normalize(vec3(-1,1,1));
    vec3 objColor = vec3(1.0,1.0,1.0);

    fragColor = vec4(dot(norm.xyz,lightDir) * objColor,1.0);
    //fragColor = vec4(norm.xyz,1.0);
    //fragColor = vec4(1.0);
}
