#version 450

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inUVW;
layout (location = 2) in vec3 inNormal;

layout (binding = 0) uniform UBO 
{
    mat4 projection;
    mat4 model;
    mat4 view;
    mat4 normal;
    vec4 lightpos;
} ubo;

layout (binding = 1) uniform ModelView 
{
    mat4 model;
} volumeModel;

layout (location = 0) out vec3 outUV;
layout (location = 1) out mat4 outVolumeModel;
layout (location = 5) out mat4 outCubeProjModel;

out gl_PerVertex 
{
    vec4 gl_Position;   
};

void main() 
{
    vec3 pos = inPos * 3.f;
//    outUV = vec3(inUVW);
    outVolumeModel = volumeModel.model;
    outCubeProjModel = ubo.projection * ubo.view * ubo.model;

    float ratio = ubo.projection[0][0] / ubo.projection[1][1];
    outUV = vec3(pos.x / ratio * 0.5f + (0.5f), pos.y * 0.5f + 0.5f, 0.f);
    gl_Position = vec4(pos, 1.0);
}
