#version 330 core
in vec4 vert;
in vec4 norm;
in vec3 color;
out vec4 fragColor;

void main(void)
{
    vec3 lightDir =  normalize(vec3(-1,0,1));
    vec3 objColor = color;

    //fragColor = vec4((clamp(dot(norm.xyz,lightDir),0,1)* 0.7 +  0.3) * objColor,1.0);
    //fragColor = vec4(norm.xyz,1.0);
    //fragColor = vec4(vec3((vert.z + 1)/2.0),1.0);
    fragColor = vec4(0.8);
    //fragColor = vec4(color,1);
}
