#version 330 core

layout(location = 0) in vec3 position;

uniform mat4 uProjection;
uniform vec4 uOrientation;

out vec3 fragColor;

vec3 rotateByQuaternion(vec3 point, vec4 q) {
    vec3 u = q.xyz;
    float s = q.w;

    return 2.0 * dot(u, point) * u
         + (s*s - dot(u, u)) * point
         + 2.0f * s * cross(u, point);
}

void main() {
    vec3 point = rotateByQuaternion(position, uOrientation);
    gl_Position = uProjection * vec4(point, 1.0f);
    fragColor = vec3(0.5f, 0.5f, 0.5f, 1.0f);
}
