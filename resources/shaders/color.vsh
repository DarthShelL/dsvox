#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat3 ti_model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = aNormal;
//    Normal = vec3(model * vec4(aNormal, 1.0));
    Normal = ti_model * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}