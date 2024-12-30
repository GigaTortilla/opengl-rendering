#version 330 core
out vec4 FragColor;

in vec3 vertColor;
in vec2 texCoord;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float blend;

void main() {
    // additional options to display the blended texture
    vec2 flippedTexCoordX = vec2(1.0 - texCoord.s, texCoord.t);
    vec2 multiTexCoord = texCoord * 2.0;
    FragColor = mix(texture(texture0, texCoord), texture(texture1, multiTexCoord), blend) * vec4(vertColor, 1.0);
}
