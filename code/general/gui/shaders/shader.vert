#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;

uniform mat4 uProjection;
uniform vec4 uOrientation;

out vec3 fragColor;
out vec3 fNormal;

vec3 rotateByQuaternion(vec3 point, vec4 q) {
    vec3 u = q.xyz;
    float s = q.w;

    return 2.0 * dot(u, point) * u
         + (s*s - dot(u, u)) * point
         + 2.0f * s * cross(u, point);
}

void main() {
    vec3 point = rotateByQuaternion(aPosition, uOrientation);
    gl_Position = uProjection * vec4(point, 1.0f);
    fragColor = vec3(1.0f, 1.0f, 1.0f);
    fNormal = rotateByQuaternion(aNormal, uOrientation);
}
