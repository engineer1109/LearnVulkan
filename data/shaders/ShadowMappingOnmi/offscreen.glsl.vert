#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec4 inUV;
layout (location = 2) in vec3 inNormal;

layout (binding = 0) uniform UBO2
{
    mat4 projection;
    mat4 model;
    mat4 view;
    mat4 normal;
    vec4 lightpos;
} ubo_origin;

layout (binding = 10) uniform UBO
{
    mat4 projection;
    mat4 model;
    mat4 view;
    mat4 normal;
    vec4 lightpos;
} ubo;

layout(push_constant) uniform PushConsts
{
    mat4 view;
} pushConsts;

out gl_PerVertex
{
    vec4 gl_Position;
};

layout (location = 1) out vec4 outPos;
layout (location = 2) out vec3 outLightPos;

void main()
{
    mat4 model = mat4(1.0f);
    gl_Position = ubo.projection * pushConsts.view * ubo.model  * vec4(inPos, 1.0);

    outPos = vec4(inPos, 1.0);
    outLightPos = (ubo.lightpos).xyz;
}