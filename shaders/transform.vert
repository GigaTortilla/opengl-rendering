#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec3 vertColor;
out vec2 texCoord;

uniform mat4 transform;

void main()
{
    texCoord = aTexCoord;
    gl_Position = transform * vec4(aPos, 1.0);
}