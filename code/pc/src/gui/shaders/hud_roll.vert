#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;

uniform float uAngle;

out vec3 fragColor;

vec2 rotate(vec2 pos, float angle){
    return vec2(
        pos.x * cos(angle) - pos.y * sin(angle),
        pos.y * cos(angle) + pos.x * sin(angle)
    );
}

void main() {
    vec2 newPos = rotate(aPosition, uAngle);
    newPos = newPos + vec2(-0.7f, -0.7f);
    gl_Position = vec4(newPos, 0.0f, 1.0f);
    fragColor = aColor;
}
