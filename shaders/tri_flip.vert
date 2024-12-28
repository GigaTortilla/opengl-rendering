#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 vertPos;

uniform float offset;

void main()
{
    vertPos = vec3(aPos.x, -aPos.y + offset + 0.2, aPos.z);
    gl_Position = vec4(aPos.x, -aPos.y + offset, aPos.z, 1.0);
}