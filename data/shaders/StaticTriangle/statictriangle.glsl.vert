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
layout (location = 2) out vec3 outNormal;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main() 
{
    outUV = inUV.rgb;

    mat4 modelView = ubo.view * ubo.model;
    vec4 pos = modelView * vec4(inPos.xyz, 1.0);

    gl_Position = ubo.projection * pos;
}
