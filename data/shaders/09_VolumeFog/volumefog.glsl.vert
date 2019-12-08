#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inUVW;
layout (location = 2) in vec3 inNormal;

layout (binding = 0) uniform UBO 
{
    mat4 projection;
    mat4 model;
    vec4 viewPos;
    float lodBias;
} ubo;

layout (binding = 1) uniform ModelView 
{
    mat4 model;
} volumeModel;

layout (location = 0) out vec3 outUV;
layout (location = 1) out mat4 outVolumeModel;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main() 
{
    outUV = vec3(inUVW);
    outVolumeModel=volumeModel.model;
    gl_Position = ubo.projection * ubo.model * vec4(inPos.xyz, 1.0);		
}
