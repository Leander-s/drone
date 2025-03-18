#version 330 core

in vec3 fragColor;
in vec3 fNormal;

out vec4 FragColor;

void main() {
    vec3 view = vec3(0.0f, 0.0f, -1.0f);
    float diff = dot(view, fNormal);
    FragColor = vec4((abs(diff) + 0.1f) * fragColor, 1.0f);
}
