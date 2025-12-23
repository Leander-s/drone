#version 330 core

in vec3 fragColor;
in vec3 fNormal;
in vec3 fPos;

out vec4 FragColor;

vec3 lightpos = vec3(0.0f, 2.0f, 1.0f);
vec3 lightvec = lightpos - fPos;
float ambient = 0.1f;
float shininess = 32;

void main() {
    vec3 view = vec3(0.0f, 0.0f, -1.0f);
    float diff = max(0, dot(lightvec, fNormal));
    vec3 half = normalize(-normalize(view) + normalize(lightvec));
    float spec = pow(max(0, dot(half, fNormal)), shininess);
    FragColor = vec4((diff + spec + ambient) * fragColor, 1.0f);
}
