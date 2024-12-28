#version 330 core
in vec3 vertPos;
out vec4 FragColor;

void main() {
    FragColor = vec4(vertPos.x, vertPos.y, 0.2f, 1.0f);
}
