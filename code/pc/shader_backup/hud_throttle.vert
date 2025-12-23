#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec3 aColor;

uniform float uThrottle;

out vec3 fragColor;

void main() {
    float throttle = uThrottle * 0.8f;
    vec2 newPos = vec2(aPosition.x + 0.75f, aPosition.y * throttle - 0.8f);
    gl_Position = vec4(newPos, 0.0f, 1.0f);
    fragColor = aColor;
}
