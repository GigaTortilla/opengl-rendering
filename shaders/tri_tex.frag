#version 330 core
out vec4 FragColor;

in vec3 vertColor;
in vec2 texCoord;

uniform sampler2D myTex;

void main() {
    FragColor = texture(myTex, texCoord);
}
