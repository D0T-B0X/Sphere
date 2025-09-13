#version 430 core

layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 vWorldPos;
out vec3 vNormal;

void main() {
    vec4 worldPos = model * vec4(aPos, 1.0);
    vWorldPos = worldPos.xyz;

    vNormal = normalize(mat3(model) * aPos);

    gl_Position = projection * view * worldPos;
}