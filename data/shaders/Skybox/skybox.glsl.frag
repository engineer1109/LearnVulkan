#version 450

layout (binding = 3) uniform samplerCube samplerCubeMap;

layout (location = 0) in vec3 inUV;

layout (location = 0) out vec4 outFragColor;

void main() {
    outFragColor = texture(samplerCubeMap, inUV);
    gl_FragDepth = 1.f;
}
