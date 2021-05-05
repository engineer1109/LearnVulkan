#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inUV;
layout (location = 2) in vec3 inNormal;

layout (binding = 0) uniform UBO
{
    mat4 projection;
    mat4 model;
    mat4 view;
    mat4 normal;
    vec4 lightpos;
} ubo;

layout (location = 0) out vec3 outUV;

void main() {
    outUV = inPos;
    outUV.x *= -1.0;
    outUV.y *= -1.0;
    vec3 pos = inPos.xyz * 10.f;
    gl_Position = ubo.projection * ubo.model * vec4(pos, 1.0);
}
