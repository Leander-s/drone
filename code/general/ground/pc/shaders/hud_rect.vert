#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;

uniform vec2 uTranslation;

out vec3 fragColor;

void main() {
    vec2 point = aPosition + uTranslation;
    gl_Position = vec4(point, 0.0f, 1.0f);
    fragColor = aColor;
}
