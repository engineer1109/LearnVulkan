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

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main() 
{
    outUV = inUV.xyz;

    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPos.xyz, 1.0);	
}
