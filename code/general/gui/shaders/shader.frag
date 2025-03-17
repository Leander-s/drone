#version 330 core

in vec3 fragColor;
in vec3 fNormal;

out vec4 FragColor;

void main() {
    vec3 view = vec3(0.0f, -1.0f, 0.0f);
    float diff = dot(view, fNormal);
    FragColor = vec4(diff * fragColor, 1.0);
}
