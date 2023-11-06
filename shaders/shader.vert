#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTex;

out vec3 fragPos;
out vec3 norm;
out vec2 texCoord;

void main()
{
    vec4 viewPos = view * model * vec4(aPos,1);
    gl_Position = projection * viewPos;
    fragPos = viewPos.xyz;

    texCoord = aTex;
    norm = normalize(mat3(transpose(inverse(view* model)))* aNorm);
}
