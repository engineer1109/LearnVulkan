#version 450

layout (location = 0) out vec4 outFragColor;

layout (location = 0) in vec4 inPos;
layout (location = 1) in vec3 inLightPos;

void main()
{
    // Store distance to light as 32 bit float value
    vec3 lightVec = inPos.xyz - inLightPos;
    outFragColor = vec4(vec3(length(lightVec)) , 1.f);
}