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
layout (location = 1) out float outLodBias;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out vec3 outViewVec;
layout (location = 4) out vec3 outLightVec;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main() 
{	
    outUV = inUV.xyz;

    vec3 worldPos = vec3(ubo.model * vec4(inPos, 1.0));

    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPos.xyz, 1.0);	

    vec4 pos = ubo.view * ubo.model * vec4(inPos, 1.0);
    outNormal = mat3(inverse(transpose(ubo.view * ubo.model))) * inNormal;
    vec3 lightPos = vec3(0.0);
    vec3 lPos = mat3(ubo.view * ubo.model) * lightPos.xyz;
    outLightVec = lPos - pos.xyz;
    
    vec3 viewPos = vec3(0.0);
    outViewVec = viewPos - pos.xyz;
}
